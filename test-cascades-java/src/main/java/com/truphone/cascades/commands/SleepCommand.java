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
     * @param periodInMs_ The period (in milliseconds) to delay
     */
    public SleepCommand(final int periodInMs_) {
        super("sleep " + periodInMs_);
        this._periodInMs = periodInMs_;
    }

    @Override
    public final int getTimeoutOffset() {
        return this._periodInMs;
    }
}
