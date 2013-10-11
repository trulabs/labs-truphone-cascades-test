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
     * @param screenX_ The position of the touch relative to the screen (x)
     * @param screenY_ The position of the touch relative to the screen (y)
     * @param windowX_ The position of the touch relative to the window (x)
     * @param windowY_ The position of the touch relative to the window (y)
     * @param localX_ The position of the touch relative to the object (x)
     * @param localY_ The position of the touch relative to the object (y)
         */
        public TouchLocation(
                final int screenX_,
                final int screenY_,
                final int windowX_,
                final int windowY_,
                final int localX_,
                final int localY_) {
            this._screenX = screenX_;
            this._screenY = screenY_;
            this._windowX = windowX_;
            this._windowY = windowY_;
            this._localX = localX_;
            this._localY = localY_;
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
     * @param location_ The location of the touch
     * @param type_ The type of touch
     * @param receiverObject_ The receiving object
     * @param targetObject_ The target object
     */
    public TouchCommand(
            final TouchLocation location_,
            final Type type_,
            final String receiverObject_,
            final String targetObject_) {
        super("touch " + location_.getScreenX() + " " + location_.getScreenY() + " "
            + location_.getWindowX() + " " + location_.getWindowY() + " " + location_.getLocalX() + " "
            + location_.getLocalY() + " " + type_.ordinal() + " " + receiverObject_ + " " + targetObject_);
    }
}
