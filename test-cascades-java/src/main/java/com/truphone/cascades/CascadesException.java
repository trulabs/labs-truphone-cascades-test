package com.truphone.cascades;

/**
 * Exception thrown if there's a connection error.
 * @author STruscott
 *
 */
class CascadesException extends Exception {

	/**
	 * Create a new exception.
	 * @param msg The message associated with the exception
	 * @param cause The cause of the exception
	 */
	public CascadesException(final String msg, final Throwable cause) {
		super(msg, cause);
	}
}
