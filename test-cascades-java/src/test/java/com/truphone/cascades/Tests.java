package com.truphone.cascades;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.Semaphore;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import com.truphone.cascades.commands.ClickCommand;
import com.truphone.cascades.commands.SleepCommand;
import com.truphone.cascades.commands.TextCommand;
import com.truphone.cascades.commands.ToastCommand;
import com.truphone.cascades.replys.IReply;

/**
 * @author struscott
 */
public final class Tests {

	private FakeDevice device;
	private static final String OK_MESSAGE = "OK\n";
	private static final String FAIL_MESSAGE = "Unexpected exception - ";
	private static final int DEFAULT_TEST_PORT = 15000;
	private static final int DEFAULT_TIMEOUT = 1000;
	private static final int DEFAULT_SLEEP = 3000;
	private SynchronousConnection conn;

	/**
	 * Initialisation to perform prior to tests.
	 */
	@Before
	public void testInit() {
		this.device = new FakeDevice(DEFAULT_TEST_PORT);
		this.device.listen();
		this.conn = new SynchronousConnection("localhost", DEFAULT_TEST_PORT);
		try {
			this.conn.connect(DEFAULT_TIMEOUT);
		} catch (TimeoutException exception) {
			Assert.fail(exception.getLocalizedMessage());
		}
	}

	/**
	 * Test the text command.
	 *
	 * @throws TimeoutException Thrown if the command times out.
	 */
	@Test
	public void testTextCommand() throws TimeoutException {
		final IReply reply = this.conn.transmit(new TextCommand(
				"myUsernameField", "my.user-name"), DEFAULT_TIMEOUT);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the click command.
	 *
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testClickCommand() throws TimeoutException {
		final IReply reply = this.conn.transmit(new ClickCommand(
				"theLoginButton"), DEFAULT_TIMEOUT);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the toast command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testToastCommand() throws TimeoutException {
		final IReply reply = this.conn.transmit(new ToastCommand(
				"You didn't enter a password"), DEFAULT_TIMEOUT);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Test the sleep command.
	 * @throws TimeoutException Thrown if the command times out
	 */
	@Test
	public void testSleepCommand() throws TimeoutException {
		final IReply reply = this.conn.transmit(new SleepCommand(
				DEFAULT_SLEEP), DEFAULT_TIMEOUT);
		Assert.assertTrue(reply.isSuccess());
	}

	/**
	 * Steps to execute after the tests.
	 */
	@After
	public void testExit() {
		this.device.stop();
	}

	private static final class FakeDevice {

		private final ServerSocket server;
		private final Semaphore lock;
		private boolean isListening;
		private FakeDeviceProcess fakeDeviceProcess;
		private Thread serverThread;

		private static final class FakeDeviceProcess implements Runnable {

			private final ServerSocket server;
			private Socket client;
			private boolean running;

			public FakeDeviceProcess(final ServerSocket socket) {
				this.server = socket;
				this.running = true;
			}

			@Override
			public void run() {
				try {
					while (this.running) {
						this.client = this.server.accept();
						this.client.getOutputStream().write(OK_MESSAGE.getBytes());
						final BufferedReader reader = new BufferedReader(
								new InputStreamReader(
										this.client.getInputStream()));
						String line = reader.readLine();
						while (line != null) {
							if ("record".equals(line)) {
								this.client.getOutputStream().write(
										OK_MESSAGE.getBytes());
							} else if ("click theLoginButton".equals(line)) {
								this.client.getOutputStream().write(
										OK_MESSAGE.getBytes());
							} else if ("text myUsernameField my.user-name"
									.equals(line)) {
								this.client.getOutputStream().write(
										OK_MESSAGE.getBytes());
							} else if ("toast You didn't enter a password"
									.equals(line)) {
								this.client.getOutputStream().write(
										OK_MESSAGE.getBytes());
							} else if ("sleep 3000".equals(line)) {
								this.client.getOutputStream().write(
										OK_MESSAGE.getBytes());
							}
							line = reader.readLine();
						}
					}
				} catch (Throwable t) {
					Assert.fail(FAIL_MESSAGE + t);
				}
			}

			public void shutdown() {
				this.running = false;
				try {
					this.server.close();
				} catch (Throwable t) {
					Assert.fail(FAIL_MESSAGE + t);
				}
				if (this.client != null) {
					try {
						this.client.close();
					} catch (Throwable t) {
						Assert.fail(FAIL_MESSAGE + t);
					}
				}
			}
		}

		public FakeDevice(final int port) {
			this.lock = new Semaphore(1);
			this.isListening = false;
			try {
				this.server = new ServerSocket(port);
			} catch (IOException ioe) {
				Assert.fail(FAIL_MESSAGE + ioe);
				throw null;
			}
		}

		public boolean listen() {
			boolean listenOk = false;
			try {
				this.lock.acquire();
				if (!this.isListening) {
					this.serverThread = new Thread(
							this.fakeDeviceProcess = new FakeDeviceProcess(
									this.server));
					listenOk = true;
					this.serverThread.start();
					this.isListening = true;
				}
				this.lock.release();
			} catch (Throwable t) {
				Assert.fail(FAIL_MESSAGE + t);
			}
			return listenOk;
		}

		public void stop() {
			try {
				this.lock.acquire();
				if (this.isListening) {
					this.fakeDeviceProcess.shutdown();
					this.serverThread.join();
					this.isListening = false;
				}
				this.lock.release();
			} catch (Throwable t) {
				Assert.fail(FAIL_MESSAGE + t);
			}
		}
	}
}
