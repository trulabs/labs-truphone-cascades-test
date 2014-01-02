package com.truphone.cascades;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.Charset;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.Semaphore;

import org.junit.Assert;

final class FakeDevice {

	protected static final String OK_MESSAGE = "OK";
	private static final String FAIL_MESSAGE = "Unexpected exception - ";
	private final ServerSocket server;
	private final Semaphore lock;
	private boolean isListening;
	private FakeDeviceProcess fakeDeviceProcess;
	private Thread serverThread;

	private static final int DEFAULT_TEST_PORT = 15000;
	protected static final int DEFAULT_TIMEOUT = 1000;
	private static final int STARTUP_TIMEOUT = 10000;
	protected static final SynchronousConnection CONN;
	protected static final FakeDevice DEVICE = new FakeDevice(DEFAULT_TEST_PORT);

	static {
		DEVICE.listen();
		DEVICE.getProcess().addListener(new FakeDeviceListener() {
			@Override
			public void messageReceived(final String message, final PrintStream replyStream) {
				if ("record".equals(message)) {
					replyStream.println(FakeDevice.OK_MESSAGE);
				}
			}
		});
		CONN = new SynchronousConnection("localhost", DEFAULT_TEST_PORT);
		try {
			CONN.connect(STARTUP_TIMEOUT);
		} catch (TimeoutException exception) {
			Assert.fail(exception.getLocalizedMessage());
		}
	}

	protected interface FakeDeviceListener {
		void messageReceived(final String message, final PrintStream replyStream);
	}

	protected static final class FakeDeviceProcess implements Runnable {

		private final ServerSocket server;
		private Socket client;
		private boolean running;
		private final List<FakeDeviceListener> listeners;

		public FakeDeviceProcess(final ServerSocket socket) {
			this.listeners = new LinkedList<FakeDeviceListener>();
			this.server = socket;
			this.running = true;
		}

		public void addListener(final FakeDeviceListener listener) {
			this.listeners.add(listener);
		}

		public void removeListener(final FakeDeviceListener listener) {
			this.listeners.remove(listener);
		}

		@Override
		public void run() {
			try {
				while (this.running) {
					this.client = this.server.accept();
					this.client.getOutputStream().write(OK_MESSAGE.getBytes(Charset.defaultCharset()));
					final BufferedReader reader = new BufferedReader(
							new InputStreamReader(
									this.client.getInputStream(), Charset.defaultCharset()));
					String line = reader.readLine();
					final PrintStream replyStream = new PrintStream(this.client.getOutputStream(), false, Charset.defaultCharset().name());
					while (line != null) {
						for (final FakeDeviceListener listener : this.listeners) {
							try {
								listener.messageReceived(line, replyStream);
							} catch (Throwable t) {
								Assert.fail(FAIL_MESSAGE + t);
							}
						}
						line = reader.readLine();
					}
				}
			} catch (Throwable t) {
				if (this.running) {
					Assert.fail(FAIL_MESSAGE + t);
				}
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
				this.serverThread.setDaemon(true);
				this.serverThread.start();
				this.isListening = true;
			}
			this.lock.release();
		} catch (Throwable t) {
			Assert.fail(FAIL_MESSAGE + t);
		}
		return listenOk;
	}

	public FakeDeviceProcess getProcess() {
		return this.fakeDeviceProcess;
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
