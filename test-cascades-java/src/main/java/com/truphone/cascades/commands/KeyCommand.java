package com.truphone.cascades.commands;

/**
 * Simulate a key press.
 *
 * @author struscott
 *
 */
public class KeyCommand extends DefaultCommand {

    /**
     * @param code_ The key code
     * @param pressed_ Is the key pressed
     * @param ctrl_ Is ctrl pressed
     * @param alt_ Is alt pressed
     * @param shift_ Is shift pressed
     */
    public KeyCommand(
            final int code_,
            final boolean pressed_,
            final boolean ctrl_,
            final boolean alt_,
            final boolean shift_) {
        super("key " + code_ + " " + pressed_ + " " + alt_ + " " + shift_ + " " + ctrl_);
    }
}
