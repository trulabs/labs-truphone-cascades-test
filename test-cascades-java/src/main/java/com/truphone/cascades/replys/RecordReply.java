package com.truphone.cascades.replys;

/**
 * Recording message from the server.
 *
 * @author struscott
 *
 */
public class RecordReply extends DefaultReply {
    /**
     * @param recording The recorded command
     */
    public RecordReply(final String recording) {
        super(true, recording, true);
    }
}
