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
     * @param name The new name for the spy
     * @param object The object to spy on
     * @param signal The signal that object will emit to spy on
     */
    public SpyCommand(final String name, final String object, final String signal) {
        super("spy create " + name + " " + object + " " + signal);
    }

    /**
     * Get the signal count of a spy.
     *
     * @param name The name of the spy to check
     * @param expectedCount The expected signal count
     */
    public SpyCommand(final String name, final int expectedCount) {
        super("spy count " + name + " " + expectedCount);
    }

    /**
     * Kill a spy.
     *
     * @param name The name of the spy to kill
     */
    public SpyCommand(final String name) {
        super("spy kill " + name);
    }
}
