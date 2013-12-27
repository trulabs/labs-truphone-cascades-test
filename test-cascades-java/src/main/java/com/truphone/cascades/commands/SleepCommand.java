package com.truphone.cascades.commands;

/**
 * Sleep.
 *
 * @author struscott
 *
 */
public class SleepCommand extends DefaultCommand {

    private final int _periodInMs;

    /**
     * @param periodInMs The period (in milliseconds) to delay
     */
    public SleepCommand(final int periodInMs) {
        super("sleep " + periodInMs);
        this._periodInMs = periodInMs;
    }

    @Override
    public final int getTimeoutOffset() {
        return this._periodInMs;
    }
}
