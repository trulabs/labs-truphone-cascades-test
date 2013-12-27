package com.truphone.cascades.commands;

/**
 * Check the status and text of toasts.
 *
 * @author struscott
 *
 */
public class ToastCommand extends DefaultCommand {

    /**
     * @param visible Test if a toast is visible <code>true</code>
     * or not visible <code>false</code>
     */
    public ToastCommand(final boolean visible) {
        super("toast " + (visible ? "true" : "false"));
    }

    /**
     * @param text Test that a toast is displayed and its value is <code>text_</code>
     */
    public ToastCommand(final String text) {
        super("toast " + text);
    }
}
