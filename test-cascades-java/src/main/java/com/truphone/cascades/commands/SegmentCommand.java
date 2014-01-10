package com.truphone.cascades.commands;

/**
 * Select an option on a SegmentControl.
 *
 * @author struscott
 *
 */
public class SegmentCommand extends DefaultCommand {

	/**
	 * Select a segment based on its index.
	 * @param segment The segment to select
	 * @param index The index to select
	 */
    public SegmentCommand(final String segment, final int index) {
        super("segment " + segment + " index " + index);
    }

    /**
     * Select a segment based on its option name.
     * @param segment The segment to select
     * @param option The name of the option to select
     */
    public SegmentCommand(final String segment, final String option) {
        super("segment " + segment + " option " + option);
    }
}
