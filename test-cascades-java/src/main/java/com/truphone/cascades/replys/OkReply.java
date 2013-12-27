package com.truphone.cascades.replys;

/**
 * Success reply from the server.
 *
 * @author struscott
 *
 */
public class OkReply extends DefaultReply {
    /**
     * @param message The ok message received from the server
     */
    public OkReply(final String message) {
        super(true, message);
    }
}
