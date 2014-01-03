package com.truphone.cascades;

import java.io.PrintStream;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

import org.jboss.netty.channel.ChannelFuture;
import org.junit.Assert;
import org.junit.Test;

import com.truphone.cascades.commands.ICommand;
import com.truphone.cascades.replys.IReply;
import com.truphone.cascades.replys.OkReply;
import com.truphone.cascades.testutils.FakeDevice;
import com.truphone.cascades.testutils.FakeDevice.FakeDeviceListener;

/**
 * Test class.
 * @author STruscott
 *
 */
public final class TestConnection {

	private static final int CONNECT_TIMEOUT = 10000;
	private static final int MESSAGE_TIMEOUT = 10000;

	private String incomingMessage;

	private void setMessage(final String msg) {
		this.incomingMessage = msg;
	}

	/**
	 * Tests the action command.
	 */
	@Test
	public void testConnection() {

		final Semaphore messageSema = new Semaphore(0);
		final Semaphore connectSema = new Semaphore(0);

		final IConnectionListener listener = new IConnectionListener() {
			@Override
			public void connected() {
				/* nothing needed */
				connectSema.release();
			}
			@SuppressWarnings("synthetic-access")
			@Override
		    public void received(final IReply reply) {
				if (reply instanceof OkReply && ((OkReply) reply).isSuccess()) {
					connectSema.release();
				}
				if ("message".equals(reply.getMessage())) {
					TestConnection.this.setMessage(reply.getMessage());
					messageSema.release();
				}
			}
		};

		final FakeDeviceListener fakeDevListener = new FakeDeviceListener() {
			@Override
			public void messageReceived(String message, PrintStream replyStream) {
				if ("message".equals(message)) {
					replyStream.println(message);
				}
			}
		};

		FakeDevice.DEVICE.getProcess().addListener(fakeDevListener);

		final Connection conn = new Connection("localhost", FakeDevice.DEFAULT_TEST_PORT);
		Assert.assertEquals(conn.getHostName(), "localhost");
		Assert.assertEquals(conn.getHostPort(), FakeDevice.DEFAULT_TEST_PORT);
		conn.addListener(listener);
		final ChannelFuture connectionFuture = conn.connectNow();
		try {
			final boolean connectedOk = connectionFuture.await(CONNECT_TIMEOUT);
			Assert.assertTrue(connectedOk);
		} catch (InterruptedException ie) {
			Assert.fail(ie.getLocalizedMessage());
			conn.close();
		}

		try {
			final boolean connectedOk = connectSema.tryAcquire(CONNECT_TIMEOUT, TimeUnit.MILLISECONDS);
			Assert.assertTrue(connectedOk);
		} catch (InterruptedException ie) {
			Assert.fail(ie.getLocalizedMessage());
			conn.close();
		}

		try {
			final boolean writtenOk = conn.write(new ICommand() {
				@Override
				public int getTimeoutOffset() {
					return 0;
				}
				@Override
				public String getPayload() {
					return "message\r\n";
				}
			});
			Assert.assertTrue(writtenOk);
		} catch (CascadesException ce) {
			Assert.fail(ce.getLocalizedMessage());
			conn.close();
		}
		try {
			final boolean msgRxOk = messageSema.tryAcquire(MESSAGE_TIMEOUT, TimeUnit.MILLISECONDS);
			Assert.assertTrue(msgRxOk);
		} catch (InterruptedException ie) {
			Assert.fail(ie.getLocalizedMessage());
			conn.close();
		}
		Assert.assertEquals("message", this.incomingMessage);
		conn.close();
		try {
			conn.getChannel().getCloseFuture().await(CONNECT_TIMEOUT);
		} catch (InterruptedException ie) {
			Assert.fail(ie.getLocalizedMessage());
		}
		Assert.assertFalse(conn.getChannel().isConnected());

		conn.removeListener(listener);
		FakeDevice.DEVICE.getProcess().removeListener(fakeDevListener);
	}
}
