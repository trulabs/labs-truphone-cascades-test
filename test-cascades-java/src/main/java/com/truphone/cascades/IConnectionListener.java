package com.truphone.cascades;

import com.truphone.cascades.replys.IReply;
/**
 * Listener for connections.
 * @author STruscott
 *
 */
public interface IConnectionListener {
	/**
	 * Event that occurs when new connections are received.
	 * @param connection The new connections
	 */
	void connected(final IConnection connection);

	/**
	 * Event that occurs when a reply is received from a connection.
	 * @param connection The connection that receives the reply
	 * @param reply The reply that's received
	 */
    void received(final IConnection connection, final IReply reply);
}
