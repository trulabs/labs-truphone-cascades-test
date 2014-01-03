package com.truphone.cascades;

import java.util.LinkedList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelStateEvent;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;

class ConnectionHandler extends SimpleChannelUpstreamHandler {

    private static final Logger LOGGER = Logger.getLogger(ConnectionHandler.class.getName());

    private final List<IConnectionHandlerListener> listeners;

    /**
     * Creates a client-side handler.
     */
    public ConnectionHandler() {
    	super();
        this.listeners = new LinkedList<IConnectionHandlerListener>();
    }

    public void addListener(final IConnectionHandlerListener listener) {
        synchronized (this.listeners) {
            this.listeners.add(listener);
        }
    }

    @Override
    public void channelConnected(final ChannelHandlerContext ctx, final ChannelStateEvent event) {
        synchronized (this.listeners) {
            for (final IConnectionHandlerListener listener : this.listeners) {
                if (listener != null) {
                    try {
                        listener.connected(event.getChannel());
                    } catch (Throwable t) {
                        LOGGER.log(Level.SEVERE, "channelConnected failed to inform listener", t);
                    }
                }
            }
        }
    }

    @Override
    public void messageReceived(final ChannelHandlerContext ctx, final MessageEvent event) {
        final ChannelBuffer buffer = (ChannelBuffer) event.getMessage();
        synchronized (this.listeners) {
            for (final IConnectionHandlerListener listener : this.listeners) {
                if (listener != null) {
                    try {
                        listener.received(event.getChannel(), buffer);
                    } catch (Throwable t) {
                    	LOGGER.log(Level.SEVERE, "messageReceived failed to inform listener", t);
                    }
                }
            }
        }
    }

    @Override
    public void exceptionCaught(final ChannelHandlerContext ctx, final ExceptionEvent event) {
        // Close the connection when an exception is raised.
        LOGGER.log(Level.WARNING, "Unexpected exception from downstream.", event.getCause());
        event.getChannel().close();
    }
}
