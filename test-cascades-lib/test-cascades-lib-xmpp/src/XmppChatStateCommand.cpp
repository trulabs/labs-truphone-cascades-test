/**
 * Copyright 2014 Truphone
 */
#include "XmppChatStateCommand.h"

#include <QString>
#include <QObject>
#include <QXmppMessage.h>
#include <QXmppClient.h>

#include "Connection.h"
#include "XmppResourceStore.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString XMPPChatStateCommand::CMD_NAME = "xmppChatState";

    XMPPChatStateCommand::XMPPChatStateCommand(Connection * const socket,
                                             QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    XMPPChatStateCommand::~XMPPChatStateCommand()
    {
    }

    bool XMPPChatStateCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->length() not_eq 3)
        {
            this->client->write("ERROR: xmppChatState <resource> <toJID> <state>\r\n");
        }
        else
        {
            const QString resource = arguments->first();
            QXmppClient * const client =
                    XMPPResourceStore::instance()->getFromStore(resource);
            arguments->removeFirst();
            if (client)
            {
                const QString toJid = arguments->first();
                arguments->removeFirst();

                bool stateOk = true;
                QXmppMessage::State msgState;
                const QString state = arguments->first();
                arguments->removeFirst();

                if (state == "active")
                {
                    msgState = QXmppMessage::Active;
                }
                else if (state == "inactive")
                {
                    msgState = QXmppMessage::Inactive;
                }
                else if (state == "gone")
                {
                    msgState = QXmppMessage::Gone;
                }
                else if (state == "composing")
                {
                    msgState = QXmppMessage::Composing;
                }
                else if (state == "paused")
                {
                    msgState = QXmppMessage::Paused;
                }
                else
                {
                    stateOk = false;
                }

                if (stateOk)
                {
                    QXmppMessage chatMessage;
                    chatMessage.setId(QUuid::createUuid().toString());
                    chatMessage.setThread("");
                    chatMessage.setMarkable(false);
                    chatMessage.setTo(toJid);
                    chatMessage.setState(msgState);
                    ret = client->sendPacket(chatMessage);
                }
                else
                {
                    this->client->write("ERROR: Invalid state\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Unknown resource\r\n");
            }
        }
        return ret;
    }

    void XMPPChatStateCommand::showHelp()
    {
        this->client->write("> xmppChatState <resource> <toJID> <state>\r\n");
        this->client->write("Update the chat state for a user.\r\n");
        this->client->write("Valid states: active, inactive, gone, composing, paused\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
