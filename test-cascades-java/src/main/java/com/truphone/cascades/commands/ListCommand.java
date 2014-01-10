package com.truphone.cascades.commands;


/**
 * Check and control ListViews.
 * @author STruscott
 *
 */
public class ListCommand extends DefaultCommand {

	private static final String COMMAND_NAME = "list ";
	private final String payload;

	/**
	 * Supported indexing modes.
	 * @author STruscott
	 *
	 */
	public enum IndexingMode {
		/**
		 * A numeric index, separated by ~'s, i.e. 1~2~3
		 */
		INDEX,
		/**
		 * A named index, seperated by ~'s, terminated by ^. Key values allowed.
		 * i.e. Names~S~Name=Sam Truscott^
		 */
		NAME;

		/**
		 * Convert the attribute to a string.
		 * @return The name of the attribute
		 */
		public String asString() {
			String enumName;
			switch(this) {
			case INDEX:
				enumName = "index";
				break;
			case NAME:
				enumName = "name";
				break;
			default:
				enumName = "unsupported";
				break;
			}
			return enumName;
		}
	}

	/**
	 * Create a command to check the size of the list.
	 * @param listView The list view to check
	 * @param expectedCount The expected count
	 */
	public ListCommand(final String listView, final int expectedCount) {
		super("");
		this.payload = COMMAND_NAME + listView + " count " + expectedCount + "\r\n";
	}

	/**
	 * Create a check for an expected value.
	 * @param listView The listview to check
	 * @param indexMode The index mode (numeric, name)
	 * @param indexPath The path (0~1~2, S~Sam Truscott^)
	 * @param expectedValue The expected value (Susan Boyle)
	 */
	public ListCommand(
			final String listView,
			final IndexingMode indexMode,
			final String indexPath,
			final String expectedValue) {
		super("");
		final StringBuilder builder = new StringBuilder();
		builder.append(COMMAND_NAME);
		builder.append(listView);
		builder.append(' ');
		builder.append(indexMode.asString());
		builder.append(' ');
		builder.append(indexPath);
		builder.append(' ');
		builder.append(expectedValue);
		builder.append("\r\n");
		this.payload = builder.toString();
	}

	/**
	 * Create a check for an expected value.
	 * @param listView The listview to check
	 * @param indexMode The index mode (numeric, name)
	 * @param indexPath The path (0~1~2, S~Sam Truscott^)
	 * @param expectedKey The expected value (Name)
	 * @param expectedValue The expected value (Susan Boyle^)
	 */
	public ListCommand(
			final String listView,
			final IndexingMode indexMode,
			final String indexPath,
			final String expectedKey,
			final String expectedValue) {
		super("");
		final StringBuilder builder = new StringBuilder();
		builder.append(COMMAND_NAME);
		builder.append(listView);
		builder.append(' ');
		builder.append(indexMode.asString());
		builder.append(' ');
		builder.append(indexPath);
		builder.append(' ');
		builder.append(expectedKey);
		builder.append('=');
		builder.append(expectedValue);
		builder.append("\r\n");
		this.payload = builder.toString();
	}

	@Override
    public final String getPayload() {
        return this.payload;
    }

    /**
     * Command to perform a select/unselect on a list.
     * @author STruscott
     *
     */
	public static final class ListSelectCommand extends DefaultCommand {

		private final String _payload;
		/**
		 * Select an index on a list.
		 * @param listView The listview
		 * @param indexMode The index mode (numeric, name)
		 * @param indexPath The path (0~1~2, S~Sam Truscott^)
		 * @param select True if it's a selection and false if a deselection
		 */
		public ListSelectCommand(
				final String listView,
				final IndexingMode indexMode,
				final String indexPath,
				final boolean select) {
			super("");
			final StringBuilder builder = new StringBuilder();
			builder.append(COMMAND_NAME);
			builder.append(listView);
			if (select) {
				builder.append(" select ");
			} else {
				builder.append(" unselect ");
			}
			builder.append(indexMode.asString());
			builder.append(' ');
			builder.append(indexPath);
			builder.append("\r\n");
			this._payload = builder.toString();
		}

		@Override
	    public String getPayload() {
	        return this._payload;
	    }
	}

	/**
	 * Command to scroll to an item in a list.
	 * @author STruscott
	 *
	 */
	public static final class ListScrollCommand extends DefaultCommand {

		private final String _payload;
		/**
		 * Scroll to an index on a list.
		 * @param listView The listview
		 * @param indexMode The index mode (numeric, name)
		 * @param indexPath The path (0~1~2, S~Sam Truscott^)
		 */
		public ListScrollCommand(
				final String listView,
				final IndexingMode indexMode,
				final String indexPath) {
			super("");
			final StringBuilder builder = new StringBuilder();
			builder.append(COMMAND_NAME);
			builder.append(listView);
			builder.append(" scroll ");
			builder.append(indexMode.asString());
			builder.append(' ');
			builder.append(indexPath);
			builder.append("\r\n");
			this._payload = builder.toString();
		}

		@Override
	    public String getPayload() {
	        return this._payload;
	    }
	}

	/**
	 * Command to tap on an item in a list.
	 * @author STruscott
	 *
	 */
	public static final class ListTapCommand extends DefaultCommand {

		private final String _payload;
		/**
		 * Tap on an index on a list.
		 * @param listView The listview
		 * @param indexMode The index mode (numeric, name)
		 * @param indexPath The path (0~1~2, S~Sam Truscott^)
		 */
		public ListTapCommand(
				final String listView,
				final IndexingMode indexMode,
				final String indexPath) {
			super("");
			final StringBuilder builder = new StringBuilder();
			builder.append(COMMAND_NAME);
			builder.append(listView);
			builder.append(" tap ");
			builder.append(indexMode.asString());
			builder.append(' ');
			builder.append(indexPath);
			builder.append("\r\n");
			this._payload = builder.toString();
		}

		@Override
	    public String getPayload() {
	        return this._payload;
	    }
	}
}
