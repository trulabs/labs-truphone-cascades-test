package com.truphone.cascades.commands;

/**
 * The Spy class can be used to create a SignalSpy object
 * and query the signal count for the number of times signals
 * have fired.
 * @author struscott
 *
 */
public class SpyCommand extends DefaultCommand {

    /**
     * Create a new spy.
     *
     * @param name_ The new name for the spy
     * @param object_ The object to spy on
     * @param signal_ The signal that object will emit to spy on
     */
    public SpyCommand(final String name_, final String object_, final String signal_) {
        super("spy create " + name_ + " " + object_ + " " + signal_);
    }

    /**
     * Get the signal count of a spy.
     *
     * @param name_ The name of the spy to check
     * @param expectedCount_ The expected signal count
     */
    public SpyCommand(final String name_, final int expectedCount_) {
        super("spy count " + name_ + " " + expectedCount_);
    }

    /**
     * Kill a spy.
     *
     * @param name_ The name of the spy to kill
     */
    public SpyCommand(final String name_) {
        super("spy kill " + name_);
    }
}
