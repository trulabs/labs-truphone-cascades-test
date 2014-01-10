package com.truphone.cascades.commands;

import java.io.PrintStream;
import java.util.LinkedList;
import java.util.List;
import java.util.SortedMap;
import java.util.TreeMap;

import org.junit.Assert;
import org.junit.Test;

import com.truphone.cascades.TimeoutException;
import com.truphone.cascades.replys.IReply;
import com.truphone.cascades.testutils.FakeDevice;
import com.truphone.cascades.testutils.FakeDevice.FakeDeviceListener;

/**
 * Test class.
 * @author STruscott
 *
 */
public final class TestContactsCommand {
	/**
	 * Test the Contacts (Add) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testContactsAddCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("contacts create forename=Some,surname=Person,mobile=+44123,".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final SortedMap<ContactsCommand.Attribute, String> data = new TreeMap<ContactsCommand.Attribute, String>();
		data.put(ContactsCommand.Attribute.FORENAME, "Some");
		data.put(ContactsCommand.Attribute.SURNAME, "Person");
		data.put(ContactsCommand.Attribute.MOBILE, "+44123");
		final IReply reply = FakeDevice.CONN.transmit(new ContactsCommand(data), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the Contacts (Modify-Remove) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testContactsModifyRemoveCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("contacts modify Some Person forename,surname,mobile,".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final List<ContactsCommand.Attribute> data = new LinkedList<ContactsCommand.Attribute>();
		data.add(ContactsCommand.Attribute.FORENAME);
		data.add(ContactsCommand.Attribute.SURNAME);
		data.add(ContactsCommand.Attribute.MOBILE);
		final IReply reply = FakeDevice.CONN.transmit(new ContactsCommand("Some Person", data), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the Contacts (Modify-Edit) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testContactsModifyEditCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("contacts modify Some Person forename=Somee,surname=Personn,mobile=+441234,".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final SortedMap<ContactsCommand.Attribute, String> data = new TreeMap<ContactsCommand.Attribute, String>();
		data.put(ContactsCommand.Attribute.FORENAME, "Somee");
		data.put(ContactsCommand.Attribute.SURNAME, "Personn");
		data.put(ContactsCommand.Attribute.MOBILE, "+441234");
		final IReply reply = FakeDevice.CONN.transmit(new ContactsCommand("Some Person", data), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the Contacts (Delete) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testContactsDeleteCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("contacts delete Some Person".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new ContactsCommand("Some Person"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the Contacts (RemoveAll) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testContactsRemoveAllYesCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("contacts removeAll yesImSure".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(
				new ContactsCommand(ContactsCommand.AreYouSure.SURE_GO), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the Contacts (RemoveAll) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testContactsRemoveAllNoCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("contacts removeAll notSure".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(
				new ContactsCommand(ContactsCommand.AreYouSure.NOT_SURE), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}
}
