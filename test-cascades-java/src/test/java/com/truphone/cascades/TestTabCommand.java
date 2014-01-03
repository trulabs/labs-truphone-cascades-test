package com.truphone.cascades;

import java.io.PrintStream;

import org.junit.Assert;
import org.junit.Test;

import com.truphone.cascades.FakeDevice.FakeDeviceListener;
import com.truphone.cascades.commands.TabCommand;
import com.truphone.cascades.replys.IReply;

/**
 * Test class.
 * @author STruscott
 *
 */
public final class TestTabCommand {
	/**
	 * Test the Tab command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testTabIndexCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("tab 5".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new TabCommand(5), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the Tab command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testTabNameCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("tab someTab".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new TabCommand("someTab"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}
}
