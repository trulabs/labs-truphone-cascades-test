package com.truphone.cascades.commands;

/**
 * Interface for a command that can be executed by the library on the target.
 *
 * @author struscott
 *
 */
public interface ICommand {
    /**
     * @return The payload of the command
     */
    String getPayload();

    /**
     * @return The timeout offset (i.e. the modified to apply to any timeout
     * associated with this command. For example the sleep command timeout is
     * offset by the sleep period).
     */
    int getTimeoutOffset();
}
