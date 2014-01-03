package com.truphone.cascades;

import org.jboss.netty.channel.ChannelFuture;

import com.truphone.cascades.commands.ICommand;

/**
 * Interface for a connection.
 * @author STruscott
 *
 */
public interface IConnection {
	/**
	 * Add a connection listener.
	 * @param listener The new listener
	 */
	void addListener(final IConnectionListener listener);

	/**
	 * Remove a connection listener.
	 * @param listener The connection listener to remove
	 */
	void removeListener(final IConnectionListener listener);

	/**
	 * Is the connection connected to the server?
	 * @return True if connected.
	 */
	boolean isConnected();

	/**
	 * Connect to the server.
	 * @return A future for the connection
	 */
	ChannelFuture connectNow();

	/**
	 * Close the connection.
	 */
	void close();

	/**
	 * Write a command on the connection.
	 * @param command The command to write
	 * @return True if the write completes ok
	 * @throws CascadesException Thrown if an error occurs in the write
	 */
	boolean write(final ICommand command) throws CascadesException;
}
