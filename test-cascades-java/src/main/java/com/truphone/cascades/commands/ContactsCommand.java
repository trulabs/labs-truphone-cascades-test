package com.truphone.cascades.commands;

import java.util.List;
import java.util.Map;

/**
 * Manage the native address book.
 * @author STruscott
 *
 */
public class ContactsCommand extends DefaultCommand {

	private final String _payload;

	/**
	 * Supported attributes for contacts.
	 * @author STruscott
	 *
	 */
	public enum Attribute {
		/**
		 * The contact's forename.
		 */
		FORENAME,
		/**
		 *  The contact's surname.
		 */
		SURNAME,
		/**
		 * The mobile number of the contact.
		 */
		MOBILE,
		/**
		 * The work number of the contact.
		 */
		WORK,
		/**
		 * The home number of the contact.
		 */
		HOME,
		/**
		 * Other number.
		 */
		OTHER,
		/**
		 * Email address of the contact.
		 */
		EMAIL;

		/**
		 * Convert the attribute to a string.
		 * @return The name of the attribute
		 */
		public String asString() {
			String enumName;
			switch(this) {
			case FORENAME:
				enumName = "forename";
				break;
			case SURNAME:
				enumName = "surname";
				break;
			case MOBILE:
				enumName = "mobile";
				break;
			case WORK:
				enumName = "work";
				break;
			case HOME:
				enumName = "home";
				break;
			case OTHER:
				enumName = "other";
				break;
			case EMAIL:
				enumName = "email";
				break;
			default:
				enumName = "unsupported";
				break;
			}
			return enumName;
		}
	}

	/**
	 * Create a new contact.
	 *
	 * @param attributes A Map of attributes to create for the new contact
	 */
	public ContactsCommand(final Map<Attribute, String> attributes) {
		super("");
		final StringBuilder builder = new StringBuilder("contacts create ");
		for (final Map.Entry<Attribute, String> entry : attributes.entrySet()) {
			builder.append(entry.getKey().asString());
			builder.append('=');
			builder.append(entry.getValue());
			builder.append(',');
		}
		builder.append("\r\n");
		this._payload = builder.toString();
	}

	/**
	 * Modify a contact by removing a list of attributes.
	 *
	 * @param name The name of the contact to change
	 * @param attributes A list of attributes to remove
	 */
	public ContactsCommand(final String name, final List<Attribute> attributes) {
		super("");
		final StringBuilder builder = new StringBuilder("contacts modify ");
		builder.append(name);
		builder.append(' ');
		for (final Attribute item : attributes) {
			builder.append(item.asString());
			builder.append(',');
		}
		builder.append("\r\n");
		this._payload = builder.toString();
	}

	/**
	 * Modify a contact by changing a map of attributes.
	 *
	 * @param name The name of the contact to change
	 * @param attributes A map of attributes to modify
	 */
	public ContactsCommand(final String name, final Map<Attribute, String> attributes) {
		super("");
		final StringBuilder builder = new StringBuilder("contacts modify ");
		builder.append(name);
		builder.append(' ');
		for (final Map.Entry<Attribute, String> entry : attributes.entrySet()) {
			builder.append(entry.getKey().asString());
			builder.append('=');
			builder.append(entry.getValue());
			builder.append(',');
		}
		builder.append("\r\n");
		this._payload = builder.toString();
	}

	/**
	 * Delete an existing contact.
	 *
	 * @param name The name of the contact to delete
	 */
	public ContactsCommand(final String name) {
		super("");
		this._payload = "contacts delete " + name + "\r\n";
	}

	@Override
    public final String getPayload() {
        return this._payload;
    }
}
