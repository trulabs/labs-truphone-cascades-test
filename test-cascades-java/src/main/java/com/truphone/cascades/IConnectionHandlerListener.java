package com.truphone.cascades;

import org.jboss.netty.buffer.ChannelBuffer;
import org.jboss.netty.channel.Channel;

/**
 * Event handling for the channels.
 * @author STruscott
 *
 */
interface IConnectionHandlerListener {
	/**
	 * Event that fires when a connection occurs.
	 * @param channel The new channel
	 */
	void connected(final Channel channel);

	/**
	 * Event that occurs when a new buffer is received.
	 * @param channel The channel that receives the data
	 * @param data The data that's received
	 */
    void received(final Channel channel, final ChannelBuffer data);
}
