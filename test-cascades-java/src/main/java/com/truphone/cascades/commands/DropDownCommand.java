package com.truphone.cascades.commands;

/**
 * Change the selected entry on a drop down list.
 *
 * @author struscott
 *
 */
public class DropDownCommand extends DefaultCommand {

    /**
     * @param list The list to change
     * @param index The new index for the list
     */
    public DropDownCommand(final String list, final int index) {
        super("dropdown " + list + " " + index);
    }

    /**
     * @param list The list to change
     * @param optionText The new option to select
     */
    public DropDownCommand(final String list, final String optionText) {
        super("dropdown " + list + " " + optionText);
    }
}
