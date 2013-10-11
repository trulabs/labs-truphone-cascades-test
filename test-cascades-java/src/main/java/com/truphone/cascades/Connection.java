package com.truphone.cascades;

import java.net.InetSocketAddress;
import java.util.LinkedList;
import java.util.List;
import java.util.StringTokenizer;
import java.util.concurrent.Executors;

import org.jboss.netty.bootstrap.ClientBootstrap;
import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.channel.socket.nio.NioClientSocketChannelFactory;

import com.truphone.cascades.ConnectionHandler.IConnectionHandlerListener;
import com.truphone.cascades.commands.ICommand;
import com.truphone.cascades.replys.FailReply;
import com.truphone.cascades.replys.IReply;
import com.truphone.cascades.replys.OkReply;
import com.truphone.cascades.replys.RecordReply;

class Connection implements IConnectionHandlerListener {

    private final List<IConnectionListener> _listeners;
    private final ConnectionHandler         _handler;
    private final ClientBootstrap           _bootstrap;
    private final String                    _hostName;
    private final int                       _hostPort;
    private Channel                         _channel;

    public interface IConnectionListener {
        void connected(Connection connection_);

        void received(Connection connection_, IReply reply_);
    }

    public Connection(final String host_, final int port_) {
        this._channel = null;
        this._hostName = host_;
        this._hostPort = port_;
        this._listeners = new LinkedList<Connection.IConnectionListener>();
        this._handler = new ConnectionHandler();
        this._handler.addListener(this);

        this._bootstrap = new ClientBootstrap(new NioClientSocketChannelFactory(Executors.newCachedThreadPool(),
                Executors.newCachedThreadPool()));

        this._bootstrap.setPipelineFactory(new ChannelPipelineFactory() {
            @Override
            public ChannelPipeline getPipeline() throws Exception {
                return Channels.pipeline(Connection.this.getHandler());
            }
        });
    }

    protected ConnectionHandler getHandler() {
        return this._handler;
    }

    protected ChannelFuture connectNow() {
        return this._bootstrap.connect(new InetSocketAddress(this._hostName, this._hostPort));
    }

    public void connect() {
        // Start the connection attempt.
        final ChannelFuture future = connectNow();

        // Wait until the connection is closed or the connection attempt fails.
        future.getChannel().getCloseFuture().awaitUninterruptibly();

        // Shut down thread pools to exit.
        this._bootstrap.releaseExternalResources();
    }

    public void close() {
        if (this._channel != null) {
            if (this._channel.isConnected()) {
                this._channel.close();
                this._bootstrap.releaseExternalResources();
            }
        }
    }

    public void addListener(final IConnectionListener listener_) {
        synchronized (this._listeners) {
            this._listeners.add(listener_);
        }
    }

    @Override
    public void connected(final Channel theChannel_) {
        this._channel = theChannel_;
        synchronized (this._listeners) {
            for (IConnectionListener listener : this._listeners) {
                try {
                    listener.connected(this);
                } catch (Throwable t_) {
                    // TODO
                    System.err.println(t_);
                }
            }
        }
    }

    @Override
    public void received(final Channel theChannel_, final ChannelBuffer data_) {

        final String packet = new String(data_.array()).trim();
        final StringTokenizer messages = new StringTokenizer(packet, "\r\n");
        while (messages.hasMoreElements()) {
            final String message = messages.nextToken();
            final IReply reply;
            if (message.equals("OK")) {
                reply = new OkReply(message);
            } else if (message.startsWith("ERROR")) {
                reply = new FailReply(message);
            } else {
                reply = new RecordReply(message);
            }
            synchronized (this._listeners) {
                for (IConnectionListener listener : this._listeners) {
                    try {
                        listener.received(this, reply);
                    } catch (Throwable t_) {
                        // TODO
                        System.err.println(t_);
                    }
                }
            }
        }
    }

    public boolean write(final ICommand command_) {
        boolean written = false;

        if (this._channel != null) {
            final String payload = command_.getPayload();
            final ChannelBuffer buffer = ChannelBuffers.buffer(payload.length());
            buffer.writeBytes(command_.getPayload().getBytes());

            final ChannelFuture future = this._channel.write(buffer);
            try {
                future.await();
                written = future.isSuccess();
            } catch (InterruptedException ie_) {
                /* failed */
                System.err.println(ie_);
            }
        }

        return written;
    }
}
