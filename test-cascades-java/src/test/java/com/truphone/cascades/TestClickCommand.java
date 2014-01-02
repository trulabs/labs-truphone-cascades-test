package com.truphone.cascades;

import java.io.PrintStream;

import org.junit.Assert;
import org.junit.Test;

import com.truphone.cascades.FakeDevice.FakeDeviceListener;
import com.truphone.cascades.commands.ClickCommand;
import com.truphone.cascades.replys.IReply;

/**
 * Test class.
 * @author STruscott
 *
 */
public final class TestClickCommand {
	/**
	 * Test the click command.
	 *
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testClickCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("click theLoginButton".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new ClickCommand(
				"theLoginButton"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}
}
