package com.truphone.cascades;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.jboss.netty.channel.ChannelFuture;

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

	private static final Logger LOGGER = Logger.getLogger(SynchronousConnection.class.getName());

    private static final int MAX_RECENT_RECS = 1;

    private final IConnection           connection;
    private final PrivateHandler        handler;
    private final BlockingQueue<IReply> replyQueue;
    private ChannelFuture               connectFuture;

    static {
    	LOGGER.setLevel(Level.ALL);
    }

    private static final class PrivateHandler implements IConnectionListener {

    	private static final Logger LOGGER = Logger.getLogger(PrivateHandler.class.getName());

        private final BlockingQueue<RecordReply> recentRecordings;
        private final BlockingQueue<IReply>      replyQueue;
        private final List<IRecordingListener>    recordListeners;
        private final Object                     lock = new Object();
        private boolean                          connectionOk;

        public PrivateHandler(final BlockingQueue<IReply> queue) {
            this.connectionOk = false;
            this.replyQueue = queue;
            this.recentRecordings = new ArrayBlockingQueue<RecordReply>(MAX_RECENT_RECS);
            this.recordListeners = new LinkedList<IRecordingListener>();
        }

        public void addRecordingListener(final IRecordingListener listener) {
            synchronized (this.recordListeners) {
                this.recordListeners.add(listener);
            }
        }

        public void removeRecordingListener(final IRecordingListener listener) {
            synchronized (this.recordListeners) {
                this.recordListeners.remove(listener);
            }
        }

        public BlockingQueue<RecordReply> getRecentRecordings() {
            return this.recentRecordings;
        }

        @Override
        public void connected() {
            synchronized (this.lock) {
                this.connectionOk = true;
                this.lock.notifyAll();
            }
        }

        public boolean waitForConnection(final int timeout) throws CascadesException {
            boolean acquired = false;

            synchronized (this.lock) {
                try {
                    this.lock.wait(timeout);
                    acquired = this.connectionOk;
                } catch (Throwable t) {
                    throw new CascadesException("Failed to wait for inbound connection", t);
                }
            }

            return acquired;
        }

        public boolean isConnected() {
            synchronized (this.lock) {
                return this.connectionOk;
            }
        }

        @Override
        public void received(final IReply reply) {
            if (reply.isRecording()) {
                // don't record sleep commands
                if (!reply.getMessage().startsWith("sleep")) {
                    if (this.recentRecordings.size() >= MAX_RECENT_RECS) {
                        this.recentRecordings.remove();
                    }
                    this.recentRecordings.add((RecordReply) reply);
                    synchronized (this.recordListeners) {
                        for (final IRecordingListener listener : this.recordListeners) {
                            if (listener != null) {
                                try {
                                    listener.recordedCommand((RecordReply) reply);
                                } catch (Throwable t) {
                                    LOGGER.log(Level.SEVERE, "received failed to inform listener", t);
                                }
                            }
                        }
                    }
                }
            } else {
                this.replyQueue.add(reply);
            }
        }
    }

    /**
     * Create a new connection.
     *
     * @param host The hostname
     * @param port The port
     */
    public SynchronousConnection(final String host, final int port) {
    	LOGGER.log(Level.FINE, "SynchronousConnection()");
        this.replyQueue = new LinkedBlockingQueue<IReply>();
        this.handler = new PrivateHandler(this.replyQueue);
        this.connection = new Connection(host, port);
        this.connection.addListener(this.handler);
    }

    /**
     * Connect to the server.
     *
     * @param timeout The timeout for the connection
     * @throws TimeoutException Thrown if the connection times out
     */
    public final void connect(final int timeout) throws TimeoutException {
    	LOGGER.log(Level.FINE, "connect() - starting connection");
        this.connectFuture = this.connection.connectNow();

        IReply reply = null;
        try {
            reply = this.replyQueue.poll(timeout, TimeUnit.MILLISECONDS);
        } catch (Throwable t) {
        	throw new TimeoutException(timeout, t);
        }
        if (reply == null) {
            throw new TimeoutException(timeout);
        }

    	LOGGER.log(Level.FINE, "connect() - starting transmit of record command");
        this.transmit(new RecordCommand(), timeout);
    }

    protected final void waitForDisconnect() {
        this.connectFuture.awaitUninterruptibly();
    }

    /**
     * Close the connection.
     */
    public final void close() {
        this.connection.close();
        this.waitForDisconnect();
    }

    /**
     * Transmit a command and wait for a reply or a timeout.
     *
     * @param command The command to execute
     * @param timeout The timeout to apply to the command
     * @return A reply or possibly <code>null</code> if the timeout expired
     * @throws TimeoutException Thrown if the timeout occurs
     */
    public final IReply transmit(
            final ICommand command,
            final int timeout) throws TimeoutException {

        if (!this.handler.isConnected()) {
            try {
	        	if (!this.handler.waitForConnection(timeout)) {
	                throw new TimeoutException(command, timeout);
	            }
            } catch (CascadesException ce) {
            	throw new TimeoutException(command, timeout, ce);
            }
        }

        try {
        	this.connection.write(command);
        } catch (CascadesException ce) {
        	throw new TimeoutException(command, timeout, ce);
        }
        IReply reply = null;
        try {
            reply = this.replyQueue.poll(timeout + command.getTimeoutOffset(), TimeUnit.MILLISECONDS);
        } catch (Throwable t) {
        	throw new TimeoutException(command, timeout, t);
        }
        if (reply == null) {
            throw new TimeoutException(command, timeout);
        }

        return reply;
    }

    /**
     * Wait for a reply (synchronise on an asynchronous event).
     *
     * @param command The command to wait for
     * @param timeout The timeout for the command to occur
     * @return The recorded command or <code>null</code> if the timeout expired
     * @throws TimeoutException Thrown if the timeout occurs
     */
    public final RecordReply waitForCommand(
            final ICommand command,
            final int timeout) throws TimeoutException {
        RecordReply reply = null;

        final BlockingQueue<RecordReply> recentRecordings = this.handler.getRecentRecordings();
        for (final RecordReply aReply : recentRecordings) {
            if (command.getPayload().trim().equals(aReply.getMessage())) {
                reply = aReply;
                break;
            }
        }

        // it wasn't received recently, wait for it
        if (reply == null) {
            final BlockingQueue<RecordReply> recordings = new LinkedBlockingQueue<RecordReply>();
            final IRecordingListener recordingListener = new IRecordingListener() {
                @Override
                public void recordedCommand(final RecordReply aReply) {
                    if (command.getPayload().trim().equals(aReply.getMessage())) {
                        recordings.add(aReply);
                    }
                }
            };

            this.handler.addRecordingListener(recordingListener);
            try {
                reply = recordings.poll(timeout, TimeUnit.MILLISECONDS);
            } catch (Throwable t) {
            	throw new TimeoutException(command, timeout, t);
            }
            this.handler.removeRecordingListener(recordingListener);
        }
        if (reply == null) {
            throw new TimeoutException(command, timeout);
        }
        return reply;
    }

}
