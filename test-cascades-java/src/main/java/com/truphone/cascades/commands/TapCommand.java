package com.truphone.cascades.commands;


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
        CANCEL;

        /**
         * Get the name of the enumeration.
         * @return String name of the enumeration
         */
        public String asString() {
        	String enumName;
        	switch (this) {
        		case UP:
        			enumName = "up";
        			break;
        		case DOWN:
        			enumName = "down";
        			break;
        		case MOVE:
        			enumName = "move";
        			break;
        		case CANCEL:
        			enumName = "cancel";
        			break;
        		default:
        			enumName = "unsupported";
        	}
        	return enumName;
        }
    }

    /**
     * @param type The type of the tap
     * @param object The object to tap
     */
    public TapCommand(final TapType type, final String object) {
        super("tap " + type.asString() + " " + object);
    }
}
