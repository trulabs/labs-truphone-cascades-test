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
	 */
	void connected();

	/**
	 * Event that occurs when a reply is received from a connection.
	 * @param reply The reply that's received
	 */
    void received(final IReply reply);
}
