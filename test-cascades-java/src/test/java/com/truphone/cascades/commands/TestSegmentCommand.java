package com.truphone.cascades.commands;

import java.io.PrintStream;

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
public final class TestSegmentCommand {

	/**
	 * Test the Segment command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testSegmentIndexCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("segment mySegment index 1".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new SegmentCommand(
				"mySegment", 1), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the Segment command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testSegmentOptionCommand() throws TimeoutException {
		final FakeDeviceListener response = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("segment mySegment option myOption".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				} else {
					Assert.fail(message);
				}
			}
		};
		FakeDevice.DEVICE.getProcess().addListener(response);
		final IReply reply = FakeDevice.CONN.transmit(new SegmentCommand(
				"mySegment", "myOption"), FakeDevice.DEFAULT_TIMEOUT);
		FakeDevice.DEVICE.getProcess().removeListener(response);
		Assert.assertTrue(reply.isSuccess());
	}
}

