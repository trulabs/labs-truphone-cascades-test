/**
 * Copyright 2014 Truphone
 */
#include "XmppCarbonsCommand.h"

#include <QString>
#include <QObject>
#include <QXmppClient.h>
#include <QXmppMessageCarbonsIq.h>

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
    const QString XMPPCarbonsCommand::CMD_NAME = "xmppCarbons";

    XMPPCarbonsCommand::XMPPCarbonsCommand(Connection * const socket,
                                             QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    XMPPCarbonsCommand::~XMPPCarbonsCommand()
    {
    }

    bool XMPPCarbonsCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->length() not_eq 1)
        {
            this->client->write(tr("ERROR: xmppCarbons <resource>")
                                + "\r\n");
        }
        else
        {
            QXmppClient * const client =
                    XMPPResourceStore::instance()->getFromStore(arguments->first());
            arguments->removeFirst();
            if (client)
            {
                QXmppMessageCarbonsIq carbIq;
                ret = client->sendPacket(carbIq);
                if (not ret)
                {
                    this->client->write(tr("ERROR: Failed to send packet") + "\r\n");
                }
                else
                {
                    XMPPResourceStore::instance()->setLastMessageSent(client, carbIq);
                }
            }
            else
            {
                this->client->write(tr("ERROR: Unknown resource") + "\r\n");
            }
        }
        return ret;
    }

    void XMPPCarbonsCommand::showHelp()
    {
        this->client->write(tr("> xmppCarbons <resource>") + "\r\n");
        this->client->write(tr("Enables the receipt of carbons from the server")
                            + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
