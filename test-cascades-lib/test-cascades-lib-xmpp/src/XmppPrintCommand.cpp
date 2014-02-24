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
                bool ok = false;
                QXmppMessage message;
                if (direction == "rx")
                {
                    ok = XMPPResourceStore::instance()->getLastMessageReceived(client, message);
                }
                else if (direction == "tx")
                {
                    ok = XMPPResourceStore::instance()->getLastMessageSent(client, message);
                }
                else
                {
                    this->client->write(tr("ERROR: Unknown direction") + "\r\n");
                }
                if (ok)
                {
                    printMessage(this->client, message);
                    ret = ok;
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
            Connection * const connection,
            const QXmppStanza& message)
    {
        QBuffer buffer;
        buffer.open(QIODevice::ReadWrite);
        QXmlStreamWriter writer(&buffer);
        message.toXml(&writer);
        connection->write(QString(buffer.data()));
    }

    void XMPPPrintCommand::showHelp()
    {
        this->client->write(tr("> xmppPrint <resource> <tx | rx>") + "\r\n");
        this->client->write(tr("Print the last message sent (tx) or received (rx)") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
