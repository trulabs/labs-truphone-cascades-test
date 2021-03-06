/**
 * Copyright 2014 Truphone
 */
#include "XmppTestCommand.h"

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
    const QString XMPPTestCommand::CMD_NAME = "xmppTest";

    XMPPTestCommand::XMPPTestCommand(Connection * const socket,
                                     QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    XMPPTestCommand::~XMPPTestCommand()
    {
    }

    bool XMPPTestCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->length() < 2)
        {
            this->client->write(tr("ERROR: xmppTest <resource> <field> <optional: value>")
                                + "\r\n");
        }
        else
        {
            QXmppClient * const client =
                    XMPPResourceStore::instance()->getFromStore(arguments->first());
            arguments->removeFirst();
            if (client)
            {
                QSharedPointer<QXmppStanza> lastStanza(NULL);
                if (arguments->first() == "tx")
                {
                    arguments->removeFirst();
                    lastStanza = XMPPResourceStore::instance()->getLastMessageSent(client);
                }
                else
                {
                    lastStanza = XMPPResourceStore::instance()->getLastMessageReceived(client);
                }
                QSharedPointer<QXmppMessage> lastMessage(NULL);
                if (lastStanza)
                {
                    lastMessage
                            = lastStanza.dynamicCast<QXmppMessage>();
                }
                if (lastMessage)
                {
                    bool propertyOk = true;
                    const QString property = arguments->first();
                    arguments->removeFirst();
                    QVariant value;
                    // I tried to make this use reflection.
                    // It didn't work - might use reflection on function calls later
                    if (property == "body")
                    {
                        value = lastMessage->body();
                    }
                    else if (property == "attentionRequired")
                    {
                        value = lastMessage->isAttentionRequested();
                    }
                    else if (property == "mucInvitationJid")
                    {
                        value = lastMessage->mucInvitationJid();
                    }
                    else if (property == "mucInvitationPassword")
                    {
                        value = lastMessage->mucInvitationPassword();
                    }
                    else if (property == "mucInvitationReason")
                    {
                        value = lastMessage->mucInvitationReason();
                    }
                    else if (property == "receiptId")
                    {
                        value = lastMessage->receiptId();
                    }
                    else if (property == "stamp")
                    {
                        value = lastMessage->stamp();
                    }
                    else if (property == "state")
                    {
                        value = lastMessage->state();
                    }
                    else if (property == "subject")
                    {
                        value = lastMessage->subject();
                    }
                    else if (property == "thread")
                    {
                        value = lastMessage->thread();
                    }
                    else if (property == "type")
                    {
                        value = lastMessage->type();
                    }
                    else if (property == "hasForward")
                    {
                        value = lastMessage->hasForwarded();
                    }
                    else if (property == "forwardBody")
                    {
                        value = lastMessage->forwarded().body();
                    }
                    else if (property == "hasCarbon")
                    {
                        value = lastMessage->hasMessageCarbon();
                    }
                    else if (property == "carbonBody")
                    {
                        value = lastMessage->carbonMessage().body();
                    }
                    else if (property == "markable")
                    {
                        value = lastMessage->isMarkable();
                    }
                    else if (property == "marker")
                    {
                        value = lastMessage->marker();
                    }
                    else if (property == "markedId")
                    {
                        value = lastMessage->markedId();
                    }
                    else if (property == "markedThread")
                    {
                        value = lastMessage->markedThread();
                    }
                    else if (property == "isReplace")
                    {
                        value = lastMessage->isReplace();
                    }
                    else if (property == "replaceId")
                    {
                        value = lastMessage->replaceId();
                    }
                    else if (property == "from")
                    {
                        value = lastMessage->from();
                    }
                    else if (property == "to")
                    {
                        value = lastMessage->to();
                    }
                    else
                    {
                        propertyOk = false;
                    }
                    if (propertyOk)
                    {
                        QVariant expectedValue;
                        if (arguments->isEmpty())
                        {
                            ret = value.isNull() or not value.isValid();
                        }
                        else
                        {
                            const QString expected = arguments->join(" ");
                            ret = (value.toString() == expected);
                            expectedValue = expected;
                        }
                        if (not ret)
                        {
                            QString buffer = tr("ERROR: Value is {");
                            buffer += value.toString();
                            buffer += tr("} but expected {");
                            buffer += expectedValue.toString();
                            buffer += tr("}") + "\r\n";
                            this->client->write(buffer);
                        }
                    }
                    else
                    {
                        this->client->write(tr("ERROR: Unknown property") + "\r\n");
                    }
                }
                else
                {
                    this->client->write(tr("ERROR: Couldn't find the last received message")
                                        + "\r\n");
                }
            }
            else
            {
                this->client->write(tr("ERROR: Unknown resource") + "\r\n");
            }
        }
        return ret;
    }

    void XMPPTestCommand::showHelp()
    {
        this->client->write(tr("> xmppTest <resource> <opt: tx> <field> <optional: value>") + "\r\n");
        this->client->write(tr("Test a field in the last received message") + "\r\n");
        this->client->write(tr("If <tx> is specified, the last sent message is used."));
        this->client->write(tr("If <value> is missing it'll test for null/empty") + "\r\n");
        this->client->write(tr("Supported <value> options:") + "\r\n");
        this->client->write("\t" + tr("body") + "\r\n");
        this->client->write("\t" + tr("attentionRequired") + "\r\n");
        this->client->write("\t" + tr("mucInvitationJid") + "\r\n");
        this->client->write("\t" + tr("mucInvitationPassword") + "\r\n");
        this->client->write("\t" + tr("mucInvitationReason") + "\r\n");
        this->client->write("\t" + tr("receiptId") + "\r\n");
        this->client->write("\t" + tr("stamp") + "\r\n");
        this->client->write("\t" + tr("state") + "\r\n");
        this->client->write("\t" + tr("subject") + "\r\n");
        this->client->write("\t" + tr("thread") + "\r\n");
        this->client->write("\t" + tr("type") + "\r\n");
        this->client->write("\t" + tr("hasForward") + "\r\n");
        this->client->write("\t" + tr("fordwardBody") + "\r\n");
        this->client->write("\t" + tr("hasCarbon") + "\r\n");
        this->client->write("\t" + tr("carbonBody") + "\r\n");
        this->client->write("\t" + tr("markable") + "\r\n");
        this->client->write("\t" + tr("markedId") + "\r\n");
        this->client->write("\t" + tr("marker") + "\r\n");
        this->client->write("\t" + tr("markedThread") + "\r\n");
        this->client->write("\t" + tr("isReplace") + "\r\n");
        this->client->write("\t" + tr("replaceId") + "\r\n");
        this->client->write("\t" + tr("from") + "\r\n");
        this->client->write("\t" + tr("to") + "\r\n");


    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
