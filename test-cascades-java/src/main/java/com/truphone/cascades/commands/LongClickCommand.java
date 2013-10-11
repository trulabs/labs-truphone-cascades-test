package com.truphone.cascades.commands;

/**
 * Simulate a long click command triggering the longPress event.
 *
 * @author struscott
 *
 */
public class LongClickCommand extends DefaultCommand {

    /**
     * @param object_ The object to long click
     */
    public LongClickCommand(final String object_) {
        super("longClick " + object_);
    }

}
