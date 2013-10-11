package com.truphone.cascades;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import com.truphone.cascades.SynchronousConnection.TimeoutException;
import com.truphone.cascades.commands.TextCommand;
import com.truphone.cascades.commands.ClickCommand;
import com.truphone.cascades.commands.SleepCommand;
import com.truphone.cascades.commands.ToastCommand;
import com.truphone.cascades.replys.IReply;

/**
 * @author struscott
 */
public final class Tests {

	private String deviceIpAddress;
	private int deviceIpPort;
	private final int defaultTimeoutPeriod = 1000;
	private final int defaultSleepPeriod = 3000;
	private SynchronousConnection conn;
	
	@Before
    public void setUp() {
        deviceIpAddress = System.getProperty("deviceIpAddress", "192.168.70.130");
        deviceIpPort = Integer.parseInt(System.getProperty("deviceIpPort", "15000"));
        conn = new SynchronousConnection(deviceIpAddress, deviceIpPort);
        try {
        	conn.connect(defaultTimeoutPeriod);
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
						defaultTimeoutPeriod);
		Assert.assertTrue(reply.isSuccess());
	}
	
	 
	 @Test
	 public void testClickCommand() throws TimeoutException {
		 final IReply reply = 
				 conn.transmit(new ClickCommand(
						 "theLoginButton"),
						 defaultTimeoutPeriod);
		 Assert.assertTrue(reply.isSuccess());
	 }
	 
	 @Test
	 public void testToastCommand() throws TimeoutException {
		 final IReply reply = 
				 conn.transmit(new ToastCommand(
						 "You didn't enter a password"),
						 defaultTimeoutPeriod);
		 Assert.assertTrue(reply.isSuccess());
	 }
	 
	 @Test
	 public void testSleepCommand() throws TimeoutException {
		 final IReply reply = 
				 conn.transmit(new SleepCommand(
						 defaultSleepPeriod),
						 defaultTimeoutPeriod);
		 Assert.assertTrue(reply.isSuccess());
	 }
}
