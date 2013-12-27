package com.truphone.cascades.commands;

/**
 * Change the tab index of a tabbed pane.
 *
 * @author struscott
 *
 */
public class TabCommand extends DefaultCommand {

    /**
     * @param index The index of the tab on the current pane
     */
    public TabCommand(final int index) {
        super("tab " + index);
    }

    /**
     * @param tabName The name of the tab on the current pane
     */
    public TabCommand(final String tabName) {
        super("tab " + tabName);
    }
}
