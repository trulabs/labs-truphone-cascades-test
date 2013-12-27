package com.truphone.cascades.commands;

/**
 * Test the properties of an object.
 *
 * @author struscott
 *
 */
public class TestCommand extends DefaultCommand {

    /**
     * @param object The object to test
     * @param property The property to test
     * @param value The expected value of the property
     */
    public TestCommand(final String object, final String property, final String value) {
        super("test " + object + " " + property + " " + value);
    }

}
