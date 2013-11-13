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

import com.truphone.cascades.SynchronousConnection.TimeoutException;
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
	private static final int DEFAULT_TIMEOUT_PERIOD = 1000;
	private static final int DEFAULT_SLEEP_PERIOD = 3000;
	private SynchronousConnection conn;
	
	@Before
	public void testInit() {
		device = new FakeDevice(15000);
		device.listen();
        conn = new SynchronousConnection("localhost", 15000);
        try {
        	conn.connect(DEFAULT_TIMEOUT_PERIOD);
        } catch(TimeoutException exception) {
        	Assert.fail(exception.getLocalizedMessage());
        }
	}
	
	@Test
	public void testTextCommand() throws TimeoutException {
		final IReply reply = 
				conn.transmit(new TextCommand(
						"myUsernameField",
						"my.user-name"),
						DEFAULT_TIMEOUT_PERIOD);
		Assert.assertTrue(reply.isSuccess());
	}
	
	 
	 @Test
	 public void testClickCommand() throws TimeoutException {
		 final IReply reply = 
				 conn.transmit(new ClickCommand(
						 "theLoginButton"),
						 DEFAULT_TIMEOUT_PERIOD);
		 Assert.assertTrue(reply.isSuccess());
	 }
	 
	 @Test
	 public void testToastCommand() throws TimeoutException {
		 final IReply reply = 
				 conn.transmit(new ToastCommand(
						 "You didn't enter a password"),
						 DEFAULT_TIMEOUT_PERIOD);
		 Assert.assertTrue(reply.isSuccess());
	 }
	 
	 @Test
	 public void testSleepCommand() throws TimeoutException {
		 final IReply reply = 
				 conn.transmit(new SleepCommand(
						 DEFAULT_SLEEP_PERIOD),
						 DEFAULT_TIMEOUT_PERIOD);
		 Assert.assertTrue(reply.isSuccess());
	 }
	 
	 @After
	 public void testExit() {
		 device.stop();
	 }
	 
	 private static final class FakeDevice {
		 
		 private static final class FakeDeviceProcess implements Runnable {
			 
			 private final ServerSocket server;
			 private Socket client;
			 private boolean running;
			 
			 public FakeDeviceProcess(final ServerSocket socket_) {
				 this.server = socket_;
				 this.running = true;
			 }
			 
			 public void run() {
				 try {
					 while(running) {
						 client = server.accept();
						 client.getOutputStream().write("OK\n".getBytes());
						 final BufferedReader reader = 
								 new BufferedReader(
										 new InputStreamReader(client.getInputStream()));
						 String line = reader.readLine();
						 while (line != null) {
							 System.out.println(line);
							 if (line.equals("record")) {
								 client.getOutputStream().write("OK\n".getBytes());
							 }
						 }
						 client = null;
					 }
				 } catch (Throwable t) {
				 }
			 }
			 
			 public void shutdown() {
				 running = false;
				 try {
					 this.server.close();
				 } catch(Throwable t) {}
				 if (client != null) {
					 try {
						 client.close();
					 }catch (Throwable t) {}
				 }
			 }
		 }
		 
		 private final ServerSocket server;
		 private final Semaphore lock;
		 private boolean isListening;
		 private FakeDeviceProcess fakeDeviceProcess;
		 private Thread serverThread;
		 
		 public FakeDevice(final int port) {
			 this.lock = new Semaphore(1);
			 this.isListening = false;
			 try {
				 this.server = new ServerSocket(port);
			 } catch(IOException ioe) {
				 throw new RuntimeException("Can't create the fake device port - fail");
			 }
		 }
		 
		 public boolean listen() {
			 boolean r = false;
			 try {
				 this.lock.acquire();
				 if (!isListening) {
					 serverThread = new Thread(
							 fakeDeviceProcess = new FakeDeviceProcess(this.server));
					 r = true;
					 serverThread.start();
					 isListening = true;
				 }
				 this.lock.release();
			 } catch(Throwable t) {
			 }
			 return r;
		 }
		 
		 public void stop() {
			 try {
				 this.lock.acquire();
				 if (isListening) {
					 this.fakeDeviceProcess.shutdown();
					 this.serverThread.join();
					 this.isListening = false;
				 }
				 this.lock.release();
			 } catch(Throwable t) {
			 }
		 }
	 }
}
