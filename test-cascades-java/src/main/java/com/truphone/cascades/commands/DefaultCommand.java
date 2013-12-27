package com.truphone.cascades.commands;

abstract class DefaultCommand implements ICommand {

    private final String payload;
    private static final int DEFAULT_TIMEOUT = 0;

    /**
     * @param aPayload The payload of the message
     */
    protected DefaultCommand(final String aPayload) {
        this.payload = aPayload + "\r\n";
    }

    @Override
    public String getPayload() {
        return this.payload;
    }

    @Override
    public int getTimeoutOffset() {
        return DEFAULT_TIMEOUT;
    }
}
