package com.truphone.cascades.commands;

public class PageCommand extends DefaultCommand {
	
	/**
     * @param page_ The object name of the page we're expecting to be on
     */
    public PageCommand(final String page_) {
        super("page " + page_);
    }
}
