/**
 * Copyright 2014 Truphone
 */
#include "XmppMessageCommand.h"

#include <QString>
#include <QObject>
#include <QXmppClient.h>
#include <QXmppMessage.h>

#include "Connection.h"
#include "XmppResourceStore.h"
#include "XmppDebugCommand.h"
#include "XmppPrintCommand.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString XMPPMessageCommand::CMD_NAME = "xmppMessage";

    XMPPMessageCommand::XMPPMessageCommand(Connection * const socket,
                                             QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    XMPPMessageCommand::~XMPPMessageCommand()
    {
    }

    bool XMPPMessageCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->length() < 3)
        {
            this->client->write(tr("ERROR: xmppMessage <resource> <to> <message>") + "\r\n");
        }
        else
        {
            QXmppClient * const client =
                    XMPPResourceStore::instance()->getFromStore(arguments->first());
            arguments->removeFirst();
            if (client)
            {
                const QString toJid = arguments->first();
                arguments->removeFirst();

                QXmppMessage chatMessage;
                chatMessage.setId(QUuid::createUuid().toString());
                chatMessage.setThread("");
                chatMessage.setBody(arguments->join(" "));
                chatMessage.setMarkable(true);
                chatMessage.setTo(toJid);
                chatMessage.setState(QXmppMessage::Active);
                chatMessage.addHint(QXmppMessage::AllowPermantStorage);
                if (XMPPDebugCommand::isDebugEnabled())
                {
                    XMPPPrintCommand::printMessage(
                        this->client,
                        chatMessage);
                }
                ret = client->sendPacket(chatMessage);
                if (not ret)
                {
                    this->client->write(tr("ERROR: Failed to send packet") + "\r\n");
                }
                else
                {
                    XMPPResourceStore::instance()->setLastMessageSent(client, chatMessage);
                }
            }
            else
            {
                this->client->write(tr("ERROR: Unknown resource") + "\r\n");
            }
        }
        return ret;
    }

    void XMPPMessageCommand::showHelp()
    {
        this->client->write(tr("> xmppMessage <resource> <to> <message>") + "\r\n");
        this->client->write(tr("Send a message to a user") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
