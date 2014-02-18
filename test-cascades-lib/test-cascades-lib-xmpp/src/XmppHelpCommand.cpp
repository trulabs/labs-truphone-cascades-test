/**
 * Copyright 2014 Truphone
 */
#include "XmppHelpCommand.h"

#include <QString>
#include <QObject>

#include "Connection.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString XMPPHelpCommand::CMD_NAME = "xmppHelp";

    XMPPHelpCommand::XMPPHelpCommand(Connection * const socket,
                                     QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    XMPPHelpCommand::~XMPPHelpCommand()
    {
    }

    bool XMPPHelpCommand::executeCommand(QStringList * const arguments)
    {
        Q_UNUSED(arguments);
        this->showHelp();
        return true;
    }

    void XMPPHelpCommand::showHelp()
    {
        this->client->write("> xmppHelp\r\n");
        this->client->write("The XMPP addin can be used to control an XMPP\r\n");
        this->client->write("connection and used to send messages, statues updates\r\n");
        this->client->write("and other XEPs to fully test messaging clients.\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
