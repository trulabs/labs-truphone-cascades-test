/**
 * Copyright 2014 Truphone
 */
#include "XmppMessageCommand.h"

#include <QString>
#include <QObject>
#include <QXmppMessage.h>

#include "Connection.h"
#include "XmppResourceStore.h"

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
        if (arguments->length() < 3)
        {
            this->client->write("ERROR: xmppMessage <resource> <to> <message>\r\n");
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
                client->sendPacket(chatMessage);
            }
            else
            {
                this->client->write("ERROR: Unknown resource\r\n");
            }
        }
        // we always return false and wait for a signal
        // back from qxmpp about when we've connected at which point
        // we'll write out OK to let us continue.
        return false;
    }

    void XMPPMessageCommand::showHelp()
    {
        this->client->write("> xmppMessage <resource> <to> <message>\r\n");
        this->client->write("Send a message to a user.\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone