/**
 * Copyright 2014 Truphone
 */
#include "XmppPrintCommand.h"

#include <QString>
#include <QObject>
#include <QXmppClient.h>
#include <QXmppPresence.h>

#include "Connection.h"
#include "XmppResourceStore.h"
#include "XmppDebugCommand.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString XMPPPrintCommand::CMD_NAME = "xmppPrint";

    XMPPPrintCommand::XMPPPrintCommand(Connection * const socket,
                                             QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    XMPPPrintCommand::~XMPPPrintCommand()
    {
    }

    bool XMPPPrintCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->length() not_eq 2)
        {
            this->client->write(tr("ERROR: xmppPrint <resource> <tx | rx>") + "\r\n");
        }
        else
        {
            QXmppClient * const client =
                    XMPPResourceStore::instance()->getFromStore(arguments->first());
            arguments->removeFirst();
            if (client)
            {
                const QString direction = arguments->first();
                QSharedPointer<QXmppStanza> message(NULL);
                if (direction == "rx")
                {
                    message = XMPPResourceStore::instance()->getLastMessageReceived(
                                client);
                }
                else if (direction == "tx")
                {
                    message = XMPPResourceStore::instance()->getLastMessageSent(
                                client);
                }
                else
                {
                    this->client->write(tr("ERROR: Unknown direction") + "\r\n");
                }
                if (message)
                {
                    printMessage(direction=="tx", message.data(), this->client);
                    ret = true;
                }
                else
                {
                    this->client->write(tr("ERROR: Failed to get the last message") + "\r\n");
                }
            }
            else
            {
                this->client->write(tr("ERROR: Unknown resource") + "\r\n");
            }
        }
        return ret;
    }

    void XMPPPrintCommand::printMessage(
            const bool tx,
            const QXmppStanza * const message,
            Connection * const connection)
    {
        QBuffer buffer;
        buffer.open(QIODevice::ReadWrite);
        QXmlStreamWriter writer(&buffer);
        message->toXml(&writer);

        const QString xmlMessage(tr("----XMPP ") + ((tx) ? tr("TX") : tr("RX")) + "----\r\n"
                           + buffer.data()
                           + "\r\n----\r\n");
        if (connection)
        {
            connection->write(xmlMessage);
        }
        else
        {
            Q_FOREACH(Connection * const conn, XMPPDebugCommand::debugClients())
            {
                conn->write(xmlMessage);
            }
        }
    }

    void XMPPPrintCommand::showHelp()
    {
        this->client->write(tr("> xmppPrint <resource> <tx | rx>") + "\r\n");
        this->client->write(tr("Print the last message sent (tx) or received (rx)") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
