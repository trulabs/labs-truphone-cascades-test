package com.truphone.cascades.commands;

/**
 * Execute a QML statement.
 *
 * @author struscott
 *
 */
public class QmlCommand extends DefaultCommand {

    /**
     * @param qml_ The QML command to execute
     */
    public QmlCommand(final String qml_) {
        super("qml " + qml_);
    }
}
