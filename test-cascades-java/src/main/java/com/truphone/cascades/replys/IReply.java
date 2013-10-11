package com.truphone.cascades.replys;

/**
 * Interface for a reply from the target library on the device.
 *
 * @author struscott
 *
 */
public interface IReply {
    /**
     * @return The success of state of the command
     */
    boolean isSuccess();

    /**
     * @return <code>true</code> if the message is from a recording session
     */
    boolean isRecording();

    /**
     * @return The message payload received
     */
    String getMessage();
}
