package com.truphone.cascades.replys;

/**
 * Reply for a failure. The message will contain the reason.
 *
 * @author struscott
 *
 */
public class FailReply extends DefaultReply {
    /**
     * @param message The message received from the library
     */
    public FailReply(final String message) {
        super(false, message);
    }
}
