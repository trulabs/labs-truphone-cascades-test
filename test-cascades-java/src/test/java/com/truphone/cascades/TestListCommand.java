package com.truphone.cascades;

import java.io.PrintStream;

import org.junit.Assert;
import org.junit.Test;

import com.truphone.cascades.FakeDevice.FakeDeviceListener;
import com.truphone.cascades.commands.ListCommand;
import com.truphone.cascades.replys.IReply;

/**
 * Test class.
 * @author STruscott
 *
 */
public final class TestListCommand {

	/**
	 * Test the List (count) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testListCountCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("list list1 count 65".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new ListCommand("list1", 65), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the List (check, numerical index) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testListCheckIndexCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("list list1 index 0~1~2 value".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new ListCommand("list1", ListCommand.IndexingMode.INDEX, "0~1~2", "value"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the List (check, named index) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testListCheckNamedIndexCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("list list1 name A~B~C^ value".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new ListCommand("list1", ListCommand.IndexingMode.NAME, "A~B~C^", "value"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the List (check, named index) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testListCheckKeyIndexCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("list list1 index 0~1~2 Name=Susan Boyle".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new ListCommand("list1", ListCommand.IndexingMode.INDEX, "0~1~2", "Name", "Susan Boyle"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the List (check, named index) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testListCheckKeyNamedIndexCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("list list1 name S~Susan^ Name=Susan Boyle".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(
				new ListCommand("list1", ListCommand.IndexingMode.NAME, "S~Susan^", "Name", "Susan Boyle"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the List (select, index) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testListSelectIndexCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("list list1 select index 0~1~2".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else if ("list list1 unselect index 0~1~2".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply1 = FakeDevice.CONN.transmit(
				new ListCommand.ListSelectCommand("list1", ListCommand.IndexingMode.INDEX, "0~1~2", true), FakeDevice.DEFAULT_TIMEOUT);
		Assert.assertTrue(reply1.isSuccess());
		final IReply reply2 = FakeDevice.CONN.transmit(
				new ListCommand.ListSelectCommand("list1", ListCommand.IndexingMode.INDEX, "0~1~2", false), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply2.isSuccess());
	}

	/**
	 * Test the List (select, named index) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testListSelectNamedIndexCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("list list1 select name S~Susan^".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else if ("list list1 unselect name S~Susan^".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply1 = FakeDevice.CONN.transmit(
				new ListCommand.ListSelectCommand("list1", ListCommand.IndexingMode.NAME, "S~Susan^", true), FakeDevice.DEFAULT_TIMEOUT);
		Assert.assertTrue(reply1.isSuccess());
		final IReply reply2 = FakeDevice.CONN.transmit(
				new ListCommand.ListSelectCommand("list1", ListCommand.IndexingMode.NAME, "S~Susan^", false), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply2.isSuccess());
	}

	/**
	 * Test the List (scroll, index) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testListScrollIndexCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("list list1 scroll index 0~1~2".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(
				new ListCommand.ListScrollCommand("list1", ListCommand.IndexingMode.INDEX, "0~1~2"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the List (select, named index) command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testListScrollNamedIndexCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("list list1 scroll name S~Susan^".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(
				new ListCommand.ListScrollCommand("list1", ListCommand.IndexingMode.NAME, "S~Susan^"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}
}
