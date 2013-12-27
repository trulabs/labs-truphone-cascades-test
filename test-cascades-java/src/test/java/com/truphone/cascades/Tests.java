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
		this.device = new FakeDevice(15000);
		this.device.listen();
		this.conn = new SynchronousConnection("localhost", 15000);
        try {
        	this.conn.connect(DEFAULT_TIMEOUT_PERIOD);
        } catch(TimeoutException exception) {
        	Assert.fail(exception.getLocalizedMessage());
        }
	}
	
	@Test
	public void testTextCommand() throws TimeoutException {
		final IReply reply = 
				this.conn.transmit(new TextCommand(
						"myUsernameField",
						"my.user-name"),
						DEFAULT_TIMEOUT_PERIOD);
		Assert.assertTrue(reply.isSuccess());
	}
	
	 
	 @Test
	 public void testClickCommand() throws TimeoutException {
		 final IReply reply = 
				 this.conn.transmit(new ClickCommand(
						 "theLoginButton"),
						 DEFAULT_TIMEOUT_PERIOD);
		 Assert.assertTrue(reply.isSuccess());
	 }
	 
	 @Test
	 public void testToastCommand() throws TimeoutException {
		 final IReply reply = 
				 this.conn.transmit(new ToastCommand(
						 "You didn't enter a password"),
						 DEFAULT_TIMEOUT_PERIOD);
		 Assert.assertTrue(reply.isSuccess());
	 }
	 
	 @Test
	 public void testSleepCommand() throws TimeoutException {
		 final IReply reply = 
				 this.conn.transmit(new SleepCommand(
						 DEFAULT_SLEEP_PERIOD),
						 DEFAULT_TIMEOUT_PERIOD);
		 Assert.assertTrue(reply.isSuccess());
	 }
	 
	 @After
	 public void testExit() {
		 this.device.stop();
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
			 
			 @Override
			 public void run() {
				 try {
					 while(this.running) {
						 this.client = this.server.accept();
						 this.client.getOutputStream().write("OK\n".getBytes());
						 final BufferedReader reader = 
								 new BufferedReader(
										 new InputStreamReader(this.client.getInputStream()));
						 String line = reader.readLine();
						 while (line != null) {
							 if (line.equals("record")) {
								 this.client.getOutputStream().write("OK\n".getBytes());
							 } else if (line.equals("click theLoginButton")) {
								 this.client.getOutputStream().write("OK\n".getBytes());
							 } else if (line.equals("text myUsernameField my.user-name")) {
								 this.client.getOutputStream().write("OK\n".getBytes());
							 } else if (line.equals("toast You didn't enter a password")) {
								 this.client.getOutputStream().write("OK\n".getBytes());
							 } else if (line.equals("sleep 3000")) {
								 this.client.getOutputStream().write("OK\n".getBytes());
							 }
							 line = reader.readLine();
						 }
						 this.client = null;
					 }
				 } catch (Throwable t) {
					 throw new RuntimeException(t);
				 }
			 }
			 
			 public void shutdown() {
				 this.running = false;
				 try {
					 this.server.close();
				 } catch(Throwable t) {
					 throw new RuntimeException(t);
				 }
				 if (this.client != null) {
					 try {
						 this.client.close();
					 }catch (Throwable t) {
						 throw new RuntimeException(t);
					 }
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
				 if (!this.isListening) {
					 this.serverThread = new Thread(
							 this.fakeDeviceProcess = new FakeDeviceProcess(this.server));
					 r = true;
					 this.serverThread.start();
					 this.isListening = true;
				 }
				 this.lock.release();
			 } catch(Throwable t) {
				 throw new RuntimeException(t);
			 }
			 return r;
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
			 } catch(Throwable t) {
				 throw new RuntimeException(t);
			 }
		 }
	 }
}
