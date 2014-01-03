package com.truphone.cascades;

import java.io.PrintStream;

import org.junit.Assert;
import org.junit.Test;

import com.truphone.cascades.commands.ICommand;
import com.truphone.cascades.replys.IReply;
import com.truphone.cascades.testutils.FakeDevice;
import com.truphone.cascades.testutils.FakeDevice.FakeDeviceListener;

/**
 * Test class.
 * @author STruscott
 *
 */
public final class TestSynchronousConnection {
	private static final int CONNECT_TIMEOUT = 10000;
	private static final int MESSAGE_TIMEOUT = 10000;
	private static final int ERR_CONNECT_TIMEOUT = 1000;
	private static final int ERR_MESSAGE_TIMEOUT = 1000;

	/**
	 * Tests the action command.
	 */
	@Test
	public void testSynchronousConnection() {

		final FakeDeviceListener listener = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("message".equals(message)) {
					replyStream.println("OK\r\n");
				}
			}
		};

		FakeDevice.DEVICE.getProcess().addListener(listener);

		final SynchronousConnection conn = new SynchronousConnection("localhost", FakeDevice.DEFAULT_TEST_PORT);
		try {
			conn.connect(CONNECT_TIMEOUT);
		} catch (TimeoutException te) {
			Assert.fail(te.getLocalizedMessage());
		}
		Assert.assertTrue(conn.isConnected());

		IReply reply;
		try {
			reply = conn.transmit(new ICommand() {
				@Override
				public int getTimeoutOffset() {
					return 0;
				}
				@Override
				public String getPayload() {
					return "message\r\n";
				}
			}, MESSAGE_TIMEOUT);
			Assert.assertEquals("OK", reply.getMessage());
		} catch (TimeoutException te) {
			Assert.fail(te.getLocalizedMessage());
		}
		conn.close();
		Assert.assertFalse(conn.isConnected());

		FakeDevice.DEVICE.getProcess().removeListener(listener);
	}

	/**
	 * Tests the action command.
	 */
	@Test
	public void testSynchronousConnectionTimeout() {

		final FakeDeviceListener listener = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("message".equals(message)) {
					replyStream.println("OK\r\n");
				}
			}
		};

		FakeDevice.DEVICE.getProcess().addListener(listener);

		final SynchronousConnection conn = new SynchronousConnection("aReallyLongAndInvalidHostName", 65093);
		try {
			conn.connect(ERR_CONNECT_TIMEOUT);
		} catch (TimeoutException te) {
			Assert.assertEquals(ERR_CONNECT_TIMEOUT, te.getTimeout());
		}
		Assert.assertFalse(conn.isConnected());

		try {
			conn.transmit(new ICommand() {
				@Override
				public int getTimeoutOffset() {
					return 0;
				}
				@Override
				public String getPayload() {
					return "message\r\n";
				}
			}, ERR_MESSAGE_TIMEOUT);
			Assert.fail("Expected a timeout");
		} catch (TimeoutException te) {
			Assert.assertEquals(ERR_MESSAGE_TIMEOUT, te.getTimeout());
		}
		Assert.assertFalse(conn.isConnected());
		conn.close();
		Assert.assertFalse(conn.isConnected());

		FakeDevice.DEVICE.getProcess().removeListener(listener);
	}
}
