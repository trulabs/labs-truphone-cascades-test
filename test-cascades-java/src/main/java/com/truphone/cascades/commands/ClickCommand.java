package com.truphone.cascades.commands;

/**
 * Click commands are used to trigger onClick events.
 *
 * @author struscott
 *
 */
public class ClickCommand extends DefaultCommand {

    /**
     * @param object_ The click command
     */
    public ClickCommand(final String object_) {
        super("click " + object_);
    }
}
