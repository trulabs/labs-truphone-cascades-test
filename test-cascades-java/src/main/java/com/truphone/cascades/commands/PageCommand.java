package com.truphone.cascades.commands;

/**
 * Check that a named page is being shown.
 * @author STruscott
 *
 */
public class PageCommand extends DefaultCommand {

	/**
     * @param page The object name of the page we're expecting to be on
     */
    public PageCommand(final String page) {
        super("page " + page);
    }
}
