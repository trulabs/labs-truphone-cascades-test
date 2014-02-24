/**
 * Copyright 2014 Truphone
 */
#include "XmppPresenceCommand.h"

#include <QString>
#include <QObject>
#include <QXmppClient.h>
#include <QXmppPresence.h>

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
    const QString XMPPPresenceCommand::CMD_NAME = "xmppPresence";

    XMPPPresenceCommand::XMPPPresenceCommand(Connection * const socket,
                                             QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    XMPPPresenceCommand::~XMPPPresenceCommand()
    {
    }

    bool XMPPPresenceCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->length() < 4)
        {
            this->client->write(tr("ERROR: xmppPresence <resource> <priority> <status> <message>") + "\r\n");
        }
        else
        {
            QXmppClient * const client =
                    XMPPResourceStore::instance()->getFromStore(arguments->first());
            arguments->removeFirst();
            if (client)
            {
                bool intOk = false;
                const int priority = arguments->first().toInt(&intOk);
                arguments->removeFirst();
                if (not intOk)
                {
                    this->client->write(tr("ERROR: Priority must be an integer") + "\r\n");
                }
                else
                {
                    QXmppPresence presence;
                    presence.setPriority(priority);

                    const QString type = arguments->first();
                    arguments->removeFirst();
                    bool sOk = true;
                    presence.setType(QXmppPresence::Available);
                    if (type == "online")
                    {
                        presence.setAvailableStatusType(QXmppPresence::Online);
                    }
                    else if (type == "away")
                    {
                        presence.setAvailableStatusType(QXmppPresence::Away);
                    }
                    else if (type == "xa")
                    {
                        presence.setAvailableStatusType(QXmppPresence::XA);
                    }
                    else if (type == "dnd")
                    {
                        presence.setAvailableStatusType(QXmppPresence::DND);
                    }
                    else if (type == "chat")
                    {
                        presence.setAvailableStatusType(QXmppPresence::Chat);
                    }
                    else if (type == "invisible")
                    {
                        presence.setAvailableStatusType(QXmppPresence::Invisible);
                    }
                    else
                    {
                        sOk = false;
                        this->client->write(tr("ERROR: Unknown presence status type") + "\r\n");
                    }
                    if (sOk)
                    {
                        const QString message = arguments->join(" ");
                        presence.setStatusText(message);
                        if (XMPPDebugCommand::isDebugEnabled())
                        {
                            XMPPPrintCommand::printMessage(
                                presence);
                        }
                        ret = client->sendPacket(presence);
                        if (not ret)
                        {
                            this->client->write(tr("ERROR: Failed to send packet") + "\r\n");
                        }
                    }
                }
            }
            else
            {
                this->client->write(tr("ERROR: Unknown resource") + "\r\n");
            }
        }
        return ret;
    }

    void XMPPPresenceCommand::showHelp()
    {
        this->client->write(tr("> xmppPresence <resource> <priority> <status> <message>") + "\r\n");
        this->client->write(tr("Set your XMPP status (online, away, xa, dnd, chat, invisible)") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
