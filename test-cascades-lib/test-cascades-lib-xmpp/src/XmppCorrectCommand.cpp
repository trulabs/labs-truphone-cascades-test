/**
 * Copyright 2014 Truphone
 */
#include "XmppCorrectCommand.h"

#include <QString>
#include <QObject>
#include <QXmppClient.h>
#include <QXmppMessage.h>

#include "Connection.h"
#include "XmppResourceStore.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString XMPPCorrectCommand::CMD_NAME = "xmppCorrect";

    XMPPCorrectCommand::XMPPCorrectCommand(Connection * const socket,
                                             QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    XMPPCorrectCommand::~XMPPCorrectCommand()
    {
    }

    bool XMPPCorrectCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->length() < 1)
        {
            this->client->write("ERROR: xmppCorrect <resource> <optional: message>\r\n");
        }
        else
        {
            QXmppClient * const client =
                    XMPPResourceStore::instance()->getFromStore(arguments->first());
            arguments->removeFirst();
            if (client)
            {
                QXmppMessage lastSentMessage;
                const bool lastMsgOk = XMPPResourceStore::instance()->getLastMessageSent(
                            client, lastSentMessage);
                if (lastMsgOk)
                {
                    if (not (lastSentMessage.body().isNull()
                          or lastSentMessage.body().isEmpty()))
                    {
                        QXmppMessage correctedMessage;
                        // correction fields
                        correctedMessage.setReceiptId(lastSentMessage.id());
                        correctedMessage.setReplace(lastSentMessage.id());
                        correctedMessage.setBody(arguments->join(" "));
                        correctedMessage.setTo(lastSentMessage.to());
                        // normal messaging
                        correctedMessage.setId(QUuid::createUuid().toString());
                        correctedMessage.setThread("");
                        correctedMessage.setState(QXmppMessage::Active);
                        correctedMessage.addHint(QXmppMessage::AllowPermantStorage);
                        ret = client->sendPacket(correctedMessage);
                        if (not ret)
                        {
                            this->client->write("ERROR: Failed to send packet\r\n");
                        }
                        else
                        {
                            XMPPResourceStore::instance()->setLastMessageSent(
                                        client, correctedMessage);
                        }
                    }
                    else
                    {
                        this->client->write(
                                    "ERROR: The previous message has no body to correct\r\n");
                    }
                }
                else
                {
                    this->client->write("ERROR: No previous sent message to correct\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Unknown resource\r\n");
            }
        }
        return ret;
    }

    void XMPPCorrectCommand::showHelp()
    {
        this->client->write("> xmppCorrect <resource> <optional: message>\r\n");
        this->client->write("Corrects the last message sent. If <message> is missing\r\n");
        this->client->write("then it's treated as a deletion of the previous message.\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
