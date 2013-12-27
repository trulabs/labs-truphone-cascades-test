package com.truphone.cascades.commands;


/**
 * Simulate a touch event.
 *
 * @author struscott
 *
 */
public class TouchCommand extends DefaultCommand {

    /**
     * @author struscott
     *
     */
    public enum Type {
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
        UP,
        /**
         *
         */
        CANCEL
    }

    /**
     * @author struscott
     *
     */
    public static class TouchLocation {
        private final int _screenX;
        private final int _screenY;
        private final int _windowX;
        private final int _windowY;
        private final int _localX;
        private final int _localY;

        /**
     * @param screenX The position of the touch relative to the screen (x)
     * @param screenY The position of the touch relative to the screen (y)
     * @param windowX The position of the touch relative to the window (x)
     * @param windowY The position of the touch relative to the window (y)
     * @param localX The position of the touch relative to the object (x)
     * @param localY The position of the touch relative to the object (y)
         */
        public TouchLocation(
                final int screenX,
                final int screenY,
                final int windowX,
                final int windowY,
                final int localX,
                final int localY) {
            this._screenX = screenX;
            this._screenY = screenY;
            this._windowX = windowX;
            this._windowY = windowY;
            this._localX = localX;
            this._localY = localY;
        }

        protected final int getScreenX() {
            return this._screenX;
        }

        protected final int getScreenY() {
            return this._screenY;
        }

        protected final int getWindowX() {
            return this._windowX;
        }

        protected final int getWindowY() {
            return this._windowY;
        }

        protected final int getLocalX() {
            return this._localX;
        }

        protected final int getLocalY() {
            return this._localY;
        }
    }

    /**
     * @param location The location of the touch
     * @param type The type of touch
     * @param receiverObject The receiving object
     * @param targetObject The target object
     */
    public TouchCommand(
            final TouchLocation location,
            final Type type,
            final String receiverObject,
            final String targetObject) {
        super("touch " + location.getScreenX() + " " + location.getScreenY() + " "
            + location.getWindowX() + " " + location.getWindowY() + " " + location.getLocalX() + " "
            + location.getLocalY() + " " + type.ordinal() + " " + receiverObject + " " + targetObject);
    }
}
