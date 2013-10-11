package com.truphone.cascades.commands;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Simulate a tap event.
 *
 * @author struscott
 *
 */
public class TapCommand extends DefaultCommand {

    /**
     * @author struscott
     *
     */
    public enum TapType {
        /**
         *
         */
        UP,
        /**
         *
         */
        DOWN,
        /**
         *
         */
        MOVE,
        /**
         *
         */
        CANCEL
    }

    private static final Map<TapType, String> _TYPE_LOOKUP = new ConcurrentHashMap<TapCommand.TapType, String>();

    static {
        _TYPE_LOOKUP.put(TapType.UP, "up");
        _TYPE_LOOKUP.put(TapType.DOWN, "down");
        _TYPE_LOOKUP.put(TapType.MOVE, "move");
        _TYPE_LOOKUP.put(TapType.CANCEL, "cancel");
    }

    /**
     * @param type_ The type of the tap
     * @param object_ The object to tap
     */
    public TapCommand(final TapType type_, final String object_) {
        super("tap " + _TYPE_LOOKUP.get(type_) + " " + object_);
    }
}
