package com.truphone.cascades.commands;

/**
 * Test the properties of an object.
 *
 * @author struscott
 *
 */
public class TestCommand extends DefaultCommand {

    /**
     * @param object_ The object to test
     * @param property_ The property to test
     * @param value_ The expected value of the property
     */
    public TestCommand(final String object_, final String property_, final String value_) {
        super("test " + object_ + " " + property_ + " " + value_);
    }

}
