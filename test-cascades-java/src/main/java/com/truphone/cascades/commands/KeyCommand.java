package com.truphone.cascades.commands;

/**
 * Simulate a key press.
 *
 * @author struscott
 *
 */
public class KeyCommand extends DefaultCommand {

    /**
     * @param code The key code
     * @param pressed Is the key pressed
     * @param ctrl Is ctrl pressed
     * @param alt Is alt pressed
     * @param shift Is shift pressed
     */
    public KeyCommand(
            final int code,
            final boolean pressed,
            final boolean ctrl,
            final boolean alt,
            final boolean shift) {
        super("key " + code + " " + pressed + " " + alt + " " + shift + " " + ctrl);
    }
}
