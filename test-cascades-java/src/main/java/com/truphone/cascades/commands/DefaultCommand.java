package com.truphone.cascades.commands;

abstract class DefaultCommand implements ICommand {

    private final String _payload;

    /**
     * @param aPayload_ The payload of the message
     */
    protected DefaultCommand(final String aPayload_) {
        this._payload = aPayload_ + "\r\n";
    }

    @Override
    public String getPayload() {
        return this._payload;
    }

    @Override
    public int getTimeoutOffset() {
        return 0;
    }
}
