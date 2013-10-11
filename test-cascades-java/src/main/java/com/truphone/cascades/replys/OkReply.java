package com.truphone.cascades.replys;

/**
 * Success reply from the server.
 *
 * @author struscott
 *
 */
public class OkReply extends DefaultReply {
    /**
     * @param message_ The ok message received from the server
     */
    public OkReply(final String message_) {
        super(true, message_);
    }
}
