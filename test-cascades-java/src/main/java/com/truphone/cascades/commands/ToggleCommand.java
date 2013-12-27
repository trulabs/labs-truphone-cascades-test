package com.truphone.cascades.commands;

/**
 * Toggle the state of a toggle button.
 *
 * @author struscott
 *
 */
public class ToggleCommand extends DefaultCommand {

    /**
     * @param toggle The name of the toggle object
     * @param state The new state of the toggle button
     */
    public ToggleCommand(final String toggle, final boolean state) {
        super("toggle " + toggle + (state ? " true" : " false"));
    }
}
