package com.truphone.cascades.replys;

/**
 * Recording message from the server.
 *
 * @author struscott
 *
 */
public class RecordReply extends DefaultReply {
    /**
     * @param recording_ The recorded command
     */
    public RecordReply(final String recording_) {
        super(true, recording_, true);
    }
}
