/**
 * Copyright 2014 Truphone
 */
#include "XmppChatMarkerCommand.h"

#include <QString>
#include <QObject>
#include <QXmppMessage.h>
#include <QXmppClient.h>

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
    const QString XMPPChatMarkerCommand::CMD_NAME = "xmppChatMarker";

    XMPPChatMarkerCommand::XMPPChatMarkerCommand(Connection * const socket,
                                             QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    XMPPChatMarkerCommand::~XMPPChatMarkerCommand()
    {
    }

    bool XMPPChatMarkerCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->length() not_eq 2)
        {
            this->client->write(tr("ERROR: xmppChatMarker <resource> <marker>") + "\r\n");
        }
        else
        {
            const QString resource = arguments->first();
            QXmppClient * const client =
                    XMPPResourceStore::instance()->getFromStore(resource);
            arguments->removeFirst();
            if (client)
            {
                QXmppMessage lastMsg;
                const bool lastMsgOk = XMPPResourceStore::instance()->getLastMessageReceived(
                            client,
                            lastMsg);
                if (lastMsgOk)
                {
                    if (lastMsg.isMarkable())
                    {
                        bool markerOk = true;
                        QXmppMessage::Marker msgMarker;
                        const QString marker = arguments->first();
                        arguments->removeFirst();

                        if (marker == "received")
                        {
                            msgMarker = QXmppMessage::Received;
                        }
                        else if (marker == "displayed")
                        {
                            msgMarker = QXmppMessage::Displayed;
                        }
                        else if (marker == "acknowledged")
                        {
                            msgMarker = QXmppMessage::Acknowledged;
                        }
                        else
                        {
                            markerOk = false;
                        }

                        if (markerOk)
                        {
                            QXmppMessage markerMessage;
                            markerMessage.setId(QUuid::createUuid().toString());
                            markerMessage.setThread("");
                            markerMessage.setMarkable(false);
                            markerMessage.setTo(lastMsg.from());
                            markerMessage.setMarker(msgMarker, lastMsg.id());
                            markerMessage.addHint(QXmppMessage::AllowPermantStorage);
                            if (XMPPDebugCommand::isDebugEnabled())
                            {
                                XMPPPrintCommand::printMessage(
                                            this->client,
                                            markerMessage);
                            }
                            ret = client->sendPacket(markerMessage);
                            if (not ret)
                            {
                                this->client->write(tr("ERROR: Failed to send packet") + "\r\n");
                            }
                        }
                        else
                        {
                            this->client->write(tr("ERROR: Invalid marker") + "\r\n");
                        }
                    }
                    else
                    {
                        this->client->write(tr("ERROR: The last message isn't Markable") + "\r\n");
                    }
                }
                else
                {
                    this->client->write(tr("ERROR: Can't find the last message received to mark") + "\r\n");
                }
            }
            else
            {
                this->client->write(tr("ERROR: Unknown resource") + "\r\n");
            }
        }
        return ret;
    }

    void XMPPChatMarkerCommand::showHelp()
    {
        this->client->write(tr("> xmppChatMarker <resource> <marker>") + "\r\n");
        this->client->write(tr("Send a chat marker back for the last message") + "\r\n");
        this->client->write(tr("received from that resource") + "\r\n");
        this->client->write(tr("Valid markers: received, displayed, acknowledged") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
