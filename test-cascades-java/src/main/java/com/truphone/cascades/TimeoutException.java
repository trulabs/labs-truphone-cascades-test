package com.truphone.cascades;

import com.truphone.cascades.commands.ICommand;

/**
 * Exception for communication timeout events.
 *
 * @author struscott
 *
 */
public class TimeoutException extends Exception {

	private static final String MESSAGE = "Timedout (";
	private ICommand    		command;
    private final int         	timeout;

    /**
     * Create a new timeout exception for a given time.
     *
     * @param aTimeout The timeout
     */
    public TimeoutException(final int aTimeout) {
        super(MESSAGE + aTimeout + "ms)");
        this.timeout = aTimeout;
    }

    /**
     * Create a new timeout exception for a given time.
     *
     * @param aTimeout The timeout
     * @param cause The cause of the exception
     */
    public TimeoutException(final int aTimeout, final Throwable cause) {
        super(MESSAGE + aTimeout + "ms)", cause);
        this.timeout = aTimeout;
    }

    /**
     * Create a new timeout exception for a command timeout.
     *
     * @param aCommand The command that caused an exception
     * @param aTimeout The timeout
     */
    public TimeoutException(final ICommand aCommand, final int aTimeout) {
        super(MESSAGE + aTimeout + "ms) waiting for a response to " + aCommand.getPayload());
        this.command = aCommand;
        this.timeout = aTimeout;
    }

    /**
     * Create a new timeout exception for a command timeout.
     *
     * @param aCommand The command that caused an exception
     * @param aTimeout The timeout
     * @param cause The cause of the timeout
     */
    public TimeoutException(final ICommand aCommand, final int aTimeout, final Throwable cause) {
        super(MESSAGE + aTimeout + "ms) waiting for a response to " + aCommand.getPayload(), cause);
        this.command = aCommand;
        this.timeout = aTimeout;
    }

    /**
     * Get the command for the timeout.
     *
     * @return The command
     */
    public final ICommand getCommand() {
        return this.command;
    }

    /**
     * Get the timeout period for the exception.
     *
     * @return The timeout
     */
    public final int getTimeout() {
        return this.timeout;
    }
}
