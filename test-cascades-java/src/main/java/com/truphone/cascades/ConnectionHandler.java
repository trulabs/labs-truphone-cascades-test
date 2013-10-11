package com.truphone.cascades;

import java.util.LinkedList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.channel.Channel;
import org.jboss.netty.channel.ChannelHandlerContext;
import org.jboss.netty.channel.ChannelStateEvent;
import org.jboss.netty.channel.ExceptionEvent;
import org.jboss.netty.channel.MessageEvent;
import org.jboss.netty.channel.SimpleChannelUpstreamHandler;

class ConnectionHandler extends SimpleChannelUpstreamHandler {

    private static final Logger _LOGGER = Logger.getLogger(ConnectionHandler.class.getName());

    public interface IConnectionHandlerListener {
        void connected(Channel channel_);

        void received(Channel channel_, ChannelBuffer data_);
    }

    private final List<IConnectionHandlerListener> _listeners;

    /**
     * Creates a client-side handler.
     */
    public ConnectionHandler() {
        this._listeners = new LinkedList<ConnectionHandler.IConnectionHandlerListener>();
    }

    public void addListener(final IConnectionHandlerListener listener_) {
        synchronized (this._listeners) {
            this._listeners.add(listener_);
        }
    }

    @Override
    public void channelConnected(final ChannelHandlerContext ctx_, final ChannelStateEvent e_) {
        synchronized (this._listeners) {
            for (final IConnectionHandlerListener listener : this._listeners) {
                if (listener != null) {
                    try {
                        listener.connected(e_.getChannel());
                    } catch (Throwable t_) {
                        // TODO
                        System.err.println(t_);
                    }
                }
            }
        }
    }

    @Override
    public void messageReceived(final ChannelHandlerContext ctx_, final MessageEvent e_) {
        final ChannelBuffer buffer = (ChannelBuffer) e_.getMessage();
        synchronized (this._listeners) {
            for (final IConnectionHandlerListener listener : this._listeners) {
                if (listener != null) {
                    try {
                        listener.received(e_.getChannel(), buffer);
                    } catch (Throwable t_) {
                        // TODO
                        System.err.println(t_);
                    }
                }
            }
        }
    }

    @Override
    public void exceptionCaught(final ChannelHandlerContext ctx_, final ExceptionEvent e_) {
        // Close the connection when an exception is raised.
        _LOGGER.log(Level.WARNING, "Unexpected exception from downstream.", e_.getCause());
        e_.getChannel().close();
    }
}
