package com.truphone.cascades.commands;

/**
 * Request an abstract action be executed.
 *
 * @author struscott
 *
 */
public class ActionCommand extends DefaultCommand {

    /**
     * @param action_ The action
     */
    public ActionCommand(final String action_) {
        super("action " + action_);
    }
}
