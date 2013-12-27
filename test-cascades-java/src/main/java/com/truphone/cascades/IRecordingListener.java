package com.truphone.cascades;

import com.truphone.cascades.replys.RecordReply;

/**
 * Interface for a listener to recorded events.
 * @author STruscott
 *
 */
interface IRecordingListener {
	/**
	 * Event fired when a new record command is received.
	 * @param reply The reply (command) that's received.
	 */
	void recordedCommand(final RecordReply reply);
}
