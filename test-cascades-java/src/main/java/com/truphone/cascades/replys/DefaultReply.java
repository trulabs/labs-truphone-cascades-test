package com.truphone.cascades.replys;

abstract class DefaultReply implements IReply {
    private final boolean _success;
    private final String  _message;
    private final boolean _recording;

    protected DefaultReply(final boolean successful_, final String aMessage_, final boolean recording_) {
        this._success = successful_;
        this._message = aMessage_;
        this._recording = recording_;
    }

    protected DefaultReply(final boolean successful_, final String aMessage_) {
        this(successful_, aMessage_, false);
    }

    @Override
    public boolean isSuccess() {
        return this._success;
    }

    @Override
    public String getMessage() {
        return this._message;
    }

    @Override
    public boolean isRecording() {
        return this._recording;
    }
}
