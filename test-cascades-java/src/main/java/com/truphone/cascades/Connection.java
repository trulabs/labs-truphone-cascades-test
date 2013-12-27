package com.truphone.cascades;

import java.net.InetSocketAddress;
import java.nio.charset.Charset;
import java.util.LinkedList;
import java.util.List;
import java.util.StringTokenizer;
import java.util.concurrent.Executors;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.jboss.netty.bootstrap.ClientBootstrap;
import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.buffer.ChannelBuffers;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelFuture;
import org.jboss.netty.channel.ChannelPipeline;
import org.jboss.netty.channel.ChannelPipelineFactory;
import org.jboss.netty.channel.Channels;
import org.jboss.netty.channel.socket.nio.NioClientSocketChannelFactory;

import com.truphone.cascades.commands.ICommand;
import com.truphone.cascades.replys.FailReply;
import com.truphone.cascades.replys.IReply;
import com.truphone.cascades.replys.OkReply;
import com.truphone.cascades.replys.RecordReply;

class Connection implements IConnection, IConnectionHandlerListener {

	private static final Logger LOGGER = Logger.getLogger(Connection.class.getName());

    private final List<IConnectionListener> listeners;
    private final ConnectionHandler			handler;
    private final ClientBootstrap 			bootstrap;
    private final String                    hostName;
    private final int                       hostPort;
    private Channel                         channel;

    public Connection(final String host, final int port) {
        this.hostName = host;
        this.hostPort = port;
        this.listeners = new LinkedList<IConnectionListener>();
        this.handler = new ConnectionHandler();
        this.handler.addListener(this);

        this.bootstrap = new ClientBootstrap(new NioClientSocketChannelFactory(Executors.newCachedThreadPool(),
                Executors.newCachedThreadPool()));

        this.bootstrap.setPipelineFactory(new ChannelPipelineFactory() {
            @Override
            public ChannelPipeline getPipeline() throws Exception {
                return Channels.pipeline(Connection.this.getHandler());
            }
        });
    }

    protected ConnectionHandler getHandler() {
        return this.handler;
    }

    @Override
    public ChannelFuture connectNow() {
        return this.bootstrap.connect(new InetSocketAddress(this.hostName, this.hostPort));
    }

    public void connect() {
        // Start the connection attempt.
        final ChannelFuture future = connectNow();

        // Wait until the connection is closed or the connection attempt fails.
        future.getChannel().getCloseFuture().awaitUninterruptibly();

        // Shut down thread pools to exit.
        this.bootstrap.releaseExternalResources();
    }

    @Override
    public void close() {
        if (this.channel != null && this.channel.isConnected()) {
            this.channel.close();
            this.bootstrap.releaseExternalResources();
        }
    }

    @Override
    public void addListener(final IConnectionListener listener) {
        synchronized (this.listeners) {
            this.listeners.add(listener);
        }
    }

    @Override
    public void connected(final Channel theChannel) {
        this.channel = theChannel;
        synchronized (this.listeners) {
            for (final IConnectionListener listener : this.listeners) {
                try {
                    listener.connected();
                } catch (Throwable t) {
                	LOGGER.log(Level.SEVERE, "connected failed to tell listener", t);
                }
            }
        }
    }

    @Override
    public void received(final Channel theChannel, final ChannelBuffer data) {

        final String packet = new String(data.array(), Charset.defaultCharset()).trim();
        final StringTokenizer messages = new StringTokenizer(packet, "\r\n");
        while (messages.hasMoreElements()) {
            final String message = messages.nextToken();
            IReply reply;
            if (message.startsWith("OK")) {
                reply = new OkReply(message);
            } else if (message.startsWith("ERROR")) {
                reply = new FailReply(message);
            } else {
                reply = new RecordReply(message);
            }
            synchronized (this.listeners) {
                for (final IConnectionListener listener : this.listeners) {
                    try {
                        listener.received(reply);
                    } catch (Throwable t) {
                    	LOGGER.log(Level.SEVERE, "received failed to tell listener", t);
                    }
                }
            }
        }
    }

    @Override
    public boolean write(final ICommand command) throws CascadesException {
        boolean written = false;

        if (this.channel != null) {
            final String payload = command.getPayload();
            final ChannelBuffer buffer = ChannelBuffers.buffer(payload.length());
            buffer.writeBytes(command.getPayload().getBytes(Charset.defaultCharset()));

            final ChannelFuture future = this.channel.write(buffer);
            try {
                future.await();
                written = future.isSuccess();
            } catch (InterruptedException ie) {
                throw new CascadesException("Failed to write to connection", ie);
            }
        }

        return written;
    }

	public List<IConnectionListener> getListeners() {
		return this.listeners;
	}

	public String getHostName() {
		return this.hostName;
	}

	public int getHostPort() {
		return this.hostPort;
	}

	public Channel getChannel() {
		return this.channel;
	}
}
