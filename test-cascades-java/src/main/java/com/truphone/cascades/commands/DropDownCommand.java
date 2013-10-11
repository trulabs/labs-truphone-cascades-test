package com.truphone.cascades.commands;

/**
 * Change the selected entry on a drop down list.
 *
 * @author struscott
 *
 */
public class DropDownCommand extends DefaultCommand {

    /**
     * @param list_ The list to change
     * @param index_ The new index for the list
     */
    public DropDownCommand(final String list_, final int index_) {
        super("dropdown " + list_ + " " + index_);
    }

    /**
     * @param list_ The list to change
     * @param optionText_ The new option to select
     */
    public DropDownCommand(final String list_, final String optionText_) {
        super("dropdown " + list_ + " " + optionText_);
    }
}
