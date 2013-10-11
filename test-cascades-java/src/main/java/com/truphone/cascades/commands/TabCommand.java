package com.truphone.cascades.commands;

/**
 * Change the tab index of a tabbed pane.
 *
 * @author struscott
 *
 */
public class TabCommand extends DefaultCommand {

    /**
     * @param index_ The index of the tab on the current pane
     */
    public TabCommand(final int index_) {
        super("tab " + index_);
    }

    /**
     * @param tabName_ The name of the tab on the current pane
     */
    public TabCommand(final String tabName_) {
        super("tab " + tabName_);
    }
}
