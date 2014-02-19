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
            this->client->write("ERROR: xmppTest <resource> <field> <optional: value>\r\n");
        }
        else
        {
            QXmppClient * const client =
                    XMPPResourceStore::instance()->getFromStore(arguments->first());
            arguments->removeFirst();
            if (client)
            {
                QXmppMessage lastMessageRecieved;
                const bool lastMsgOk = XMPPResourceStore::instance()->getLastMessageReceived(
                            client, lastMessageRecieved);
                if (lastMsgOk)
                {
                    bool propertyOk = true;
                    const QString property = arguments->first();
                    arguments->removeFirst();
                    QVariant value;
                    // I tried to make this use reflection.
                    // It didn't work - might use reflection on function calls later
                    if (property == "body")
                    {
                        value = lastMessageRecieved.body();
                    }
                    else if (property == "attentionRequired")
                    {
                        value = lastMessageRecieved.isAttentionRequested();
                    }
                    else if (property == "mucInvitationJid")
                    {
                        value = lastMessageRecieved.mucInvitationJid();
                    }
                    else if (property == "mucInvitationPassword")
                    {
                        value = lastMessageRecieved.mucInvitationPassword();
                    }
                    else if (property == "mucInvitationReason")
                    {
                        value = lastMessageRecieved.mucInvitationReason();
                    }
                    else if (property == "receiptId")
                    {
                        value = lastMessageRecieved.receiptId();
                    }
                    else if (property == "stamp")
                    {
                        value = lastMessageRecieved.stamp();
                    }
                    else if (property == "state")
                    {
                        value = lastMessageRecieved.state();
                    }
                    else if (property == "subject")
                    {
                        value = lastMessageRecieved.subject();
                    }
                    else if (property == "thread")
                    {
                        value = lastMessageRecieved.thread();
                    }
                    else if (property == "type")
                    {
                        value = lastMessageRecieved.type();
                    }
                    else if (property == "hasForward")
                    {
                        value = lastMessageRecieved.hasForwarded();
                    }
                    else if (property == "forwardBody")
                    {
                        value = lastMessageRecieved.forwarded().body();
                    }
                    else if (property == "hasCarbon")
                    {
                        value = lastMessageRecieved.hasMessageCarbon();
                    }
                    else if (property == "carbonBody")
                    {
                        value = lastMessageRecieved.carbonMessage().body();
                    }
                    else if (property == "markable")
                    {
                        value = lastMessageRecieved.isMarkable();
                    }
                    else if (property == "markedId")
                    {
                        value = lastMessageRecieved.markedId();
                    }
                    else if (property == "markedThread")
                    {
                        value = lastMessageRecieved.markedThread();
                    }
                    else if (property == "isReplace")
                    {
                        value = lastMessageRecieved.isReplace();
                    }
                    else if (property == "replaceId")
                    {
                        value = lastMessageRecieved.replaceId();
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
                            const QString expected = arguments->first();
                            arguments->removeFirst();
                            ret = (value.toString() == expected);
                            expectedValue = expected;
                        }
                        if (not ret)
                        {
                            QString buffer = "ERROR: Value is {";
                            buffer += value.toString();
                            buffer += "} but expected {";
                            buffer += expectedValue.toString();
                            buffer += "}\r\n";
                            this->client->write(buffer.toUtf8());
                        }
                    }
                    else
                    {
                        this->client->write("ERROR: Unknown property\r\n");
                    }
                }
                else
                {
                    this->client->write("ERROR: Couldn't find the last received message\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Unknown resource\r\n");
            }
        }
        return ret;
    }

    void XMPPTestCommand::showHelp()
    {
        this->client->write("> xmppTest <resource> <field> <optional: value>\r\n");
        this->client->write("Test a field in the last received message\r\n");
        this->client->write("If <value> is missing it'll test for null/empty\r\n");
        this->client->write("Supported <value> options:\r\n");
        this->client->write("\tbody\r\n");
        this->client->write("\tattentionRequired\r\n");
        this->client->write("\tmucInvitationJid\r\n");
        this->client->write("\tmucInvitationPassword\r\n");
        this->client->write("\tmucInvitationReason\r\n");
        this->client->write("\treceiptId\r\n");
        this->client->write("\tstamp\r\n");
        this->client->write("\tstate\r\n");
        this->client->write("\tsubject\r\n");
        this->client->write("\tthread\r\n");
        this->client->write("\ttype\r\n");
        this->client->write("\thasForward\r\n");
        this->client->write("\tfordwardBody\r\n");
        this->client->write("\thasCarbon\r\n");
        this->client->write("\tcarbonBody\r\n");
        this->client->write("\tmarkable\r\n");
        this->client->write("\tmarkedId\r\n");
        this->client->write("\tmarkedThread\r\n");
        this->client->write("\tisReplace\r\n");
        this->client->write("\treplaceId\r\n");


    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
