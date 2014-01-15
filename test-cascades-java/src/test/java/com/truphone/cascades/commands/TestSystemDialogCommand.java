package com.truphone.cascades.commands;

import java.io.PrintStream;

import org.junit.Assert;
import org.junit.Test;

import com.truphone.cascades.TimeoutException;
import com.truphone.cascades.replys.IReply;
import com.truphone.cascades.testutils.FakeDevice;
import com.truphone.cascades.testutils.FakeDevice.FakeDeviceListener;

public class TestSystemDialogCommand {
	/**
	 * Test the System dialog command command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testSystemDialogNoneCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("sysdialog isSusanBoyleAmazing".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new SystemDialogCommand(
				"isSusanBoyleAmazing",
				SystemDialogCommand.DialogResult.NONE), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}
	
	/**
	 * Test the System dialog command command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testSystemDialogConfirmCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("sysdialog isSusanBoyleAmazing confirm".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new SystemDialogCommand(
				"isSusanBoyleAmazing",
				SystemDialogCommand.DialogResult.CONFIRM), // obviously
				FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}
	
	/**
	 * Test the System dialog command command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testSystemDialogCancelCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("sysdialog isSusanBoyleAmazing cancel".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new SystemDialogCommand(
				"isSusanBoyleAmazing",
				SystemDialogCommand.DialogResult.CANCEL), // no no no
				FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}
	
	/**
	 * Test the System dialog command command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testSystemDialogCustomCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("sysdialog isSusanBoyleAmazing custom".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new SystemDialogCommand(
				"isSusanBoyleAmazing",
				SystemDialogCommand.DialogResult.CUSTOM), // hmm?
				FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}
}
