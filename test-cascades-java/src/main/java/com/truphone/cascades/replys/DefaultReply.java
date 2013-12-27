package com.truphone.cascades.replys;

abstract class DefaultReply implements IReply {
    private final boolean success;
    private final String  message;
    private final boolean recording;

    protected DefaultReply(final boolean successful, final String aMessage, final boolean isRecording) {
        this.success = successful;
        this.message = aMessage;
        this.recording = isRecording;
    }

    protected DefaultReply(final boolean successful, final String aMessage) {
        this(successful, aMessage, false);
    }

    @Override
    public boolean isSuccess() {
        return this.success;
    }

    @Override
    public String getMessage() {
        return this.message;
    }

    @Override
    public boolean isRecording() {
        return this.recording;
    }
}
