package com.truphone.cascades.commands;

import java.util.List;
import java.util.Map;

/**
 * Manage the native address book
 * @author STruscott
 *
 */
public class ContactsCommand extends DefaultCommand {

	private final String _payload;
	
	/**
	 * Supported attributes for contacts
	 * @author STruscott
	 *
	 */
	public enum Attribute {
		FORENAME,
		SURNAME,
		MOBILE,
		WORK,
		HOME,
		OTHER,
		EMAIL;
		
		/**
		 * Convert the attribute to a string
		 * @return The name of the attribute
		 */
		public String asString() {
			final String v;
			switch(this) {
			case FORENAME:
				v = "forename";
				break;
			case SURNAME:
				v = "surname";
				break;
			case MOBILE:
				v = "mobile";
				break;
			case WORK:
				v = "work";
				break;
			case HOME:
				v = "home";
				break;
			case OTHER:
				v = "other";
				break;
			case EMAIL:
				v = "email";
				break;
			default:
				v = "unsupported";
				break;
			}
			return v;
		}
	}
	
	/**
	 * Create a new contact
	 * 
	 * @param variables_ A Map of attributes to create for the new contact
	 */
	public ContactsCommand(final Map<Attribute, String> attributes_) {
		super("");
		final StringBuilder builder = new StringBuilder("contacts create ");
		for(final Attribute item : attributes_.keySet()) {
			builder.append(item.asString());
			builder.append("=");
			builder.append(attributes_.get(item));
			builder.append(',');
		}
		this._payload = builder.toString();
	}
	
	/**
	 * Modify a contact by removing a list of attributes
	 * 
	 * @param name_ The name of the contact to change
	 * @param variables_ A list of attributes to remove
	 */
	public ContactsCommand(final String name_, final List<Attribute> attributes_) {
		super("");
		final StringBuilder builder = new StringBuilder("contacts modify ");
		for(final Attribute item : attributes_) {
			builder.append(item.asString());
			builder.append(',');
		}
		this._payload = builder.toString();
	}
	
	/**
	 * Modify a contact by changing a map of attributes
	 * 
	 * @param name_ The name of the contact to change
	 * @param attributes_ A map of attributes to modify
	 */
	public ContactsCommand(final String name_, final Map<Attribute, String> attributes_) {
		super("");
		final StringBuilder builder = new StringBuilder("contacts modify ");
		for(final Attribute item : attributes_.keySet()) {
			builder.append(item.asString());
			builder.append("=");
			builder.append(attributes_.get(item));
			builder.append(',');
		}
		this._payload = builder.toString();
	}
	
	/**
	 * Delete an existing contact
	 * 
	 * @param name_ The name of the contact to delete
	 */
	public ContactsCommand(final String name_) {
		super("");
		this._payload = "contacts delete " + name_;
	}
	
	@Override
    public String getPayload() {
        return this._payload;
    }
}
