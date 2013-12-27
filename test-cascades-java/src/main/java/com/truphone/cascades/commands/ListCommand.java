package com.truphone.cascades.commands;


/**
 * Check and control ListViews
 * @author STruscott
 *
 */
public class ListCommand extends DefaultCommand {
	
	private final String _payload;
	
	/**
	 * Supported indexing modes
	 * @author STruscott
	 *
	 */
	public enum IndexingMode {
		INDEX,
		NAME;
		
		/**
		 * Convert the attribute to a string
		 * @return The name of the attribute
		 */
		public String asString() {
			final String v;
			switch(this) {
			case INDEX:
				v = "index";
				break;
			case NAME:
				v = "name";
				break;
			default:
				v = "unsupported";
				break;
			}
			return v;
		}
	}
	
	/**
	 * Create a command to check the size of the list
	 * @param listView_ The list view to check
	 * @param expectedCount_ The expected count
	 */
	public ListCommand(final String listView_, final int expectedCount_) {
		super("");
		this._payload = "list " + listView_ + " count " + expectedCount_;
	}
	
	/**
	 * Create a check for an expected value
	 * @param listView_ The listview to check
	 * @param indexMode The index mode (numeric, name)
	 * @param indexPath The path (0~1~2, S~Sam Truscott^)
	 * @param expectedValue The expected value (Susan Boyle)
	 */
	public ListCommand(
			final String listView_,
			final IndexingMode indexMode,
			final String indexPath,
			final String expectedValue) {
		super("");
		final StringBuilder builder = new StringBuilder();
		builder.append("list ");
		builder.append(listView_);
		builder.append(' ');
		builder.append(indexMode.asString());
		builder.append(' ');
		builder.append(indexPath);
		builder.append(' ');
		builder.append(expectedValue);
		
		this._payload = builder.toString();
	}
	
	/**
	 * Create a check for an expected value
	 * @param listView_ The listview to check
	 * @param indexMode The index mode (numeric, name)
	 * @param indexPath The path (0~1~2, S~Sam Truscott^)
	 * @param expectedKey_ The expected value (Name)
	 * @param expectedValue The expected value (Susan Boyle^)
	 */
	public ListCommand(
			final String listView_,
			final IndexingMode indexMode_,
			final String indexPath_,
			final String expectedKey_,
			final String expectedValue_) {
		super("");
		final StringBuilder builder = new StringBuilder();
		builder.append("list ");
		builder.append(listView_);
		builder.append(' ');
		builder.append(indexMode_.asString());
		builder.append(' ');
		builder.append(indexPath_);
		builder.append(' ');
		builder.append(expectedKey_);
		builder.append('=');
		builder.append(expectedValue_);
		
		this._payload = builder.toString();
	}
	
	@Override
    public String getPayload() {
        return this._payload;
    }
	
	public static final class ListSelectCommand extends DefaultCommand {
		
		private final String _payload;
		/**
		 * Select an index on a list
		 * @param listView_ The listview to check
		 * @param indexMode_ The index mode (numeric, name)
		 * @param indexPath_ The path (0~1~2, S~Sam Truscott^)
		 * @param select_ True if it's a selection and false if a deselection
		 */
		public ListSelectCommand(
				final String listView_,
				final IndexingMode indexMode_,
				final String indexPath_,
				final boolean select_) {
			super("");
			final StringBuilder builder = new StringBuilder();
			builder.append("list ");
			builder.append(listView_);
			if (select_) {
				builder.append(" select ");
			} else {
				builder.append(" unselect ");
			}
			builder.append(indexMode_.asString());
			builder.append(' ');
			builder.append(indexPath_);
			
			this._payload = builder.toString();
		}
		
		@Override
	    public String getPayload() {
	        return this._payload;
	    }
	}
	
	public static final class ListScrollCommand extends DefaultCommand {
		
		private final String _payload;
		/**
		 * Scroll to an index on a list
		 * @param listView_ The listview to check
		 * @param indexMode_ The index mode (numeric, name)
		 * @param indexPath_ The path (0~1~2, S~Sam Truscott^)
		 */
		public ListScrollCommand(
				final String listView_,
				final IndexingMode indexMode_,
				final String indexPath_) {
			super("");
			final StringBuilder builder = new StringBuilder();
			builder.append("list ");
			builder.append(listView_);
			builder.append(" scroll ");
			builder.append(indexMode_.asString());
			builder.append(' ');
			builder.append(indexPath_);
			
			this._payload = builder.toString();
		}
		
		@Override
	    public String getPayload() {
	        return this._payload;
	    }
	}
}
