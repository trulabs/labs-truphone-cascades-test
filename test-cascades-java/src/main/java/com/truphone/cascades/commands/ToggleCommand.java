package com.truphone.cascades.commands;

/**
 * Toggle the state of a toggle button.
 *
 * @author struscott
 *
 */
public class ToggleCommand extends DefaultCommand {

    /**
     * @param toggle_ The name of the toggle object
     * @param state_ The new state of the toggle button
     */
    public ToggleCommand(final String toggle_, final boolean state_) {
        super("toggle " + toggle_ + (state_ ? " true" : " false"));
    }
}
