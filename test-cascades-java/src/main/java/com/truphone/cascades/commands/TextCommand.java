package com.truphone.cascades.commands;

/**
 * Set the text field of a textbox.
 *
 * @author struscott
 *
 */
public class TextCommand extends DefaultCommand {

    /**
     * @param object The object to set the text of
     * @param value The new text value
     */
    public TextCommand(final String object, final String value) {
        super("text " + object + " " + value);
    }
}
