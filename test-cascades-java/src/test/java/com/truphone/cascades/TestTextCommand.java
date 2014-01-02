package com.truphone.cascades;

import java.io.PrintStream;

import org.junit.Assert;
import org.junit.Test;

import com.truphone.cascades.FakeDevice.FakeDeviceListener;
import com.truphone.cascades.commands.TextCommand;
import com.truphone.cascades.replys.IReply;

/**
 * Test class.
 * @author STruscott
 *
 */
public final class TestTextCommand {
	/**
	 * Test the text command.
	 *
	 * @throws TimeoutException Thrown if the command times out.
	 */
	@Test
	public void testTextCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("text myUsernameField my.user-name".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new TextCommand(
				"myUsernameField", "my.user-name"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}
}
