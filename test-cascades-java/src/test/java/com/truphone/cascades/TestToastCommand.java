package com.truphone.cascades;

import java.io.PrintStream;

import org.junit.Assert;
import org.junit.Test;

import com.truphone.cascades.FakeDevice.FakeDeviceListener;
import com.truphone.cascades.commands.ToastCommand;
import com.truphone.cascades.replys.IReply;
/**
 * Test class.
 * @author STruscott
 *
 */
public final class TestToastCommand {
	/**
	 * Test the toast command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testToastCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("toast You didn't enter a password".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new ToastCommand(
				"You didn't enter a password"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}
}
