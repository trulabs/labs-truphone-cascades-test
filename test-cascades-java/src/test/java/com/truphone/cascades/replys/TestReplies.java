package com.truphone.cascades.replys;

import org.junit.Assert;
import org.junit.Test;

/**
 * Test class.
 * @author STruscott
 *
 */
public final class TestReplies {

	/**
	 * Test the OK reply.
	 */
	@Test
	public void testOkReply() {
		final IReply reply = new OkReply("OK");
		Assert.assertEquals(reply.getMessage(), "OK");
		Assert.assertEquals(reply.isRecording(), false);
		Assert.assertEquals(reply.isSuccess(), true);
	}

	/**
	 * Test the Fail reply.
	 */
	@Test
	public void testFailReply() {
		final IReply reply = new FailReply("ERROR: Failed to do something I enjoy doing");
		Assert.assertEquals(reply.getMessage(), "ERROR: Failed to do something I enjoy doing");
		Assert.assertEquals(reply.isRecording(), false);
		Assert.assertEquals(reply.isSuccess(), false);
	}

	/**
	 * Test the Recording reply.
	 */
	@Test
	public void testRecordReply() {
		final IReply reply = new RecordReply("click myButton");
		Assert.assertEquals(reply.getMessage(), "click myButton");
		Assert.assertEquals(reply.isRecording(), true);
		Assert.assertEquals(reply.isSuccess(), true);
	}
}
