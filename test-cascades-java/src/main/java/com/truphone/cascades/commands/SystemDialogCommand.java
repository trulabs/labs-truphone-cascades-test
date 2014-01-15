package com.truphone.cascades.commands;

/**
 * Close a System Dialog.
 *
 * @author struscott
 *
 */
public final class SystemDialogCommand extends DefaultCommand {

    /**
	 * Enumeration of all the dialog result types.
	 * @author STruscott
	 *
	 */
	public enum DialogResult {
		/**
		 * No result - closed dialog.
		 */
		NONE,
		/**
		 * Confirm result.
		 */
		CONFIRM,
		/**
		 * Dialog was cancelled.
		 */
		CANCEL,
		/**
		 * An additional (added) button was used.
		 */
		BUTTON,
		/**
		 * Custom button was pressed.
		 */
		CUSTOM;

		/**
		 * Convert the dialog result to a string.
		 * @return String value the library understands
		 */
		protected String asString() {
			String name;
			switch(this) {
			case NONE:
				name = "";
				break;
			case CONFIRM:
				name = "confirm";
				break;
			case CANCEL:
				name = "cancel";
				break;
			case BUTTON:
				name = "button";
				break;
			case CUSTOM:
				name = "custom";
				break;
			default:
				name = "unknown result";
			}
			return name;
		}
	}

	/**
	 * Create a new SystemDialog command.
	 * @param dialogObjectName The name of the dialog
	 * @param result The result to use
	 */
	public SystemDialogCommand(final String dialogObjectName, final DialogResult result) {
		super(("sysdialog " + dialogObjectName + " " + result.asString()).trim() + "\r\n");
	}
}
