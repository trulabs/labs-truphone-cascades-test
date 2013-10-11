package com.truphone.cascades.commands;

/**
 * Set the text field of a textbox.
 *
 * @author struscott
 *
 */
public class TextCommand extends DefaultCommand {

    /**
     * @param object_ The object to set the text of
     * @param value_ The new text value
     */
    public TextCommand(final String object_, final String value_) {
        super("text " + object_ + " " + value_);
    }
}
