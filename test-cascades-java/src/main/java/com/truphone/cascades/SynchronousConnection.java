package com.truphone.cascades;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

import org.jboss.netty.channel.ChannelFuture;

import com.truphone.cascades.Connection.IConnectionListener;
import com.truphone.cascades.commands.ICommand;
import com.truphone.cascades.commands.RecordCommand;
import com.truphone.cascades.replys.IReply;
import com.truphone.cascades.replys.RecordReply;

/**
 * Class to handle synchronous communication with the library.
 *
 * @author struscott
 *
 */
public class SynchronousConnection {

    private static final int _MAX_RECENT_RECORDINGS = 1;

    private static class PrivateHandler implements IConnectionListener {

        private final BlockingQueue<RecordReply> _recentRecordings;
        private final BlockingQueue<IReply>      _replyQueue;
        private final List<RecordingListener>    _recordListeners;
        private final Object                     _lock = new Object();
        private boolean                          _connected;

        public PrivateHandler(final BlockingQueue<IReply> queue_) {
            this._connected = false;
            this._replyQueue = queue_;
            this._recentRecordings = new ArrayBlockingQueue<RecordReply>(_MAX_RECENT_RECORDINGS);
            this._recordListeners = new LinkedList<SynchronousConnection.RecordingListener>();
        }

        public void addRecordingListener(final RecordingListener listener_) {
            synchronized (this._recordListeners) {
                this._recordListeners.add(listener_);
            }
        }

        public void removeRecordingListener(final RecordingListener listener_) {
            synchronized (this._recordListeners) {
                this._recordListeners.remove(listener_);
            }
        }

        public BlockingQueue<RecordReply> getRecentRecordings() {
            return this._recentRecordings;
        }

        @Override
        public void connected(final Connection connection_) {
            synchronized (this._lock) {
                this._connected = true;
                this._lock.notifyAll();
            }
        }

        public boolean waitForConnection(final int timeout_) {
            boolean acquired = false;

            synchronized (this._lock) {
                try {
                    this._lock.wait(timeout_);
                    acquired = this._connected;
                } catch (Throwable t_) {
                    /* don't care */
                    System.err.println(t_);
                }
            }

            return acquired;
        }

        public boolean isConnected() {
            synchronized (this._lock) {
                return this._connected;
            }
        }

        @Override
        public void received(final Connection connection_, final IReply reply_) {
            if (reply_.isRecording()) {
                // don't record sleep commands
                if (!reply_.getMessage().startsWith("sleep")) {
                    if (this._recentRecordings.size() >= _MAX_RECENT_RECORDINGS) {
                        this._recentRecordings.remove();
                    }
                    this._recentRecordings.add((RecordReply) reply_);
                    synchronized (this._recordListeners) {
                        for (final RecordingListener listener : this._recordListeners) {
                            if (listener != null) {
                                try {
                                    listener.recordedCommand((RecordReply) reply_);
                                } catch (Throwable t_) {
                                    /* don't care */
                                    System.err.println(t_);
                                }
                            }
                        }
                    }
                }
            } else {
                this._replyQueue.add(reply_);
            }
        }
    }

    private interface RecordingListener {
        void recordedCommand(RecordReply reply_);
    }

    /**
     * Exception for communication timeout events.
     *
     * @author struscott
     *
     */
    public static class TimeoutException extends Exception {
        private static final long serialVersionUID = 1;

        private final ICommand    _command;
        private final int         _timeout;

        /**
         * Create a new timeout exception for a given time.
         *
         * @param timeout_ The timeout
         */
        public TimeoutException(final int timeout_) {
            super("Timedout (" + timeout_ + "ms)");
            this._command = null;
            this._timeout = timeout_;
        }

        /**
         * Create a new timeout exception for a command timeout.
         *
         * @param command_ The command that caused an exception
         * @param timeout_ The timeout
         */
        public TimeoutException(final ICommand command_, final int timeout_) {
            super("Timedout (" + timeout_ + "ms) waiting for a response to " + command_.getPayload());
            this._command = command_;
            this._timeout = timeout_;
        }

        /**
         * Get the command for the timeout.
         *
         * @return The command
         */
        public final ICommand getCommand() {
            return this._command;
        }

        /**
         * Get the timeout period for the exception.
         *
         * @return The timeout
         */
        public final int getTimeout() {
            return this._timeout;
        }
    }

    private final Connection            _connection;
    private final PrivateHandler        _handler;
    private final BlockingQueue<IReply> _replyQueue;
    private ChannelFuture               _connectFuture;

    /**
     * Create a new connection.
     *
     * @param host_ The hostname
     * @param port_ The port
     */
    public SynchronousConnection(final String host_, final int port_) {
        this._connectFuture = null;
        this._replyQueue = new LinkedBlockingQueue<IReply>();
        this._handler = new PrivateHandler(this._replyQueue);
        this._connection = new Connection(host_, port_);
        this._connection.addListener(this._handler);
    }

    /**
     * Connect to the server.
     *
     * @param timeout_ The timeout for the connection
     * @throws TimeoutException Thrown if the connection times out
     */
    public final void connect(final int timeout_) throws TimeoutException {
        this._connectFuture = this._connection.connectNow();

        IReply reply = null;
        try {
            reply = this._replyQueue.poll(timeout_, TimeUnit.MILLISECONDS);
        } catch (Throwable t_) {
            /* don't care */
            System.err.println(t_);
        }
        if (reply == null) {
            throw new TimeoutException(timeout_);
        }

        this.transmit(new RecordCommand(), timeout_);
    }

    protected final void waitForDisconnect() {
        this._connectFuture.awaitUninterruptibly();
    }

    /**
     * Close the connection.
     */
    public final void close() {
        this._connection.close();
        this.waitForDisconnect();
    }

    /**
     * Transmit a command and wait for a reply or a timeout.
     *
     * @param command_ The command to execute
     * @param timeout_ The timeout to apply to the command
     * @return A reply or possibilty <code>null</code> if the timeout expired
     * @throws TimeoutException Thrown if the timeout occurs
     */
    public final IReply transmit(
            final ICommand command_,
            final int timeout_) throws TimeoutException {

        if (!this._handler.isConnected()) {
            if (!this._handler.waitForConnection(timeout_)) {
                throw new TimeoutException(command_, timeout_);
            }
        }

        IReply reply = null;
        this._connection.write(command_);
        try {
            reply = this._replyQueue.poll(timeout_ + command_.getTimeoutOffset(), TimeUnit.MILLISECONDS);
        } catch (Throwable t_) {
            // don't care
            System.err.println(t_);
        }
        if (reply == null) {
            throw new TimeoutException(command_, timeout_);
        }

        return reply;
    }

    /**
     * Wait for a reply (synchronise on an asynchronous event).
     *
     * @param command_ The command to wait for
     * @param timeout_ The timeout for the command to occur
     * @return The recorded command or <code>null</code> if the timeout expired
     * @throws TimeoutException Thrown if the timeout occurs
     */
    public final RecordReply waitForCommand(
            final ICommand command_,
            final int timeout_) throws TimeoutException {
        RecordReply reply = null;

        final BlockingQueue<RecordReply> recentRecordings = this._handler.getRecentRecordings();
        for (final RecordReply aReply : recentRecordings) {
            if (command_.getPayload().trim().equals(aReply.getMessage())) {
                reply = aReply;
                break;
            }
        }

        // it wasn't received recently, wait for it
        if (reply == null) {
            final BlockingQueue<RecordReply> recordings = new LinkedBlockingQueue<RecordReply>();
            final RecordingListener recordingListener = new RecordingListener() {
                @Override
                public void recordedCommand(final RecordReply aReply_) {
                    if (command_.getPayload().trim().equals(aReply_.getMessage())) {
                        recordings.add(aReply_);
                    }
                }
            };

            this._handler.addRecordingListener(recordingListener);
            try {
                reply = recordings.poll(timeout_, TimeUnit.MILLISECONDS);
            } catch (Throwable t_) {
                /* don't care */
                System.err.println(t_);
            }
            this._handler.removeRecordingListener(recordingListener);
        }
        if (reply == null) {
            throw new TimeoutException(command_, timeout_);
        }
        return reply;
    }

}
