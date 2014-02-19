/**
 * Copyright 2014 Truphone
 */
#include "XmppReachabilityCommand.h"

#include <QString>
#include <QObject>
#include <QXmppClient.h>
#include <QXmppReachAddress.h>
#include <QXmppPubSubIq.h>

#include "Connection.h"
#include "XmppResourceStore.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString XMPPReachabilityCommand::CMD_NAME = "xmppReachability";

    XMPPReachabilityCommand::XMPPReachabilityCommand(Connection * const socket,
                                             QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    XMPPReachabilityCommand::~XMPPReachabilityCommand()
    {
    }

    bool XMPPReachabilityCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->length() < 2)
        {
            this->client->write("ERROR: xmppReachability <resource> <address>\r\n");
        }
        else
        {
            QXmppClient * const client =
                    XMPPResourceStore::instance()->getFromStore(arguments->first());
            arguments->removeFirst();
            if (client)
            {
                QXmppAddress addr;
                addr.setAddress(arguments->first());
                arguments->removeFirst();

                QXmppReachAddress reachAddr;
                reachAddr.addAddress(addr);

                QXmppPubSubIq publish;
                QXmppPubSubItem item;
                QList <QXmppPubSubItem> listItems;

                publish.setType(QXmppIq::Set);
                publish.setQueryType(QXmppPubSubIq::PublishQuery);
                publish.setId(QUuid::createUuid().toString());
                publish.setQueryNode("urn:xmpp:reach:0");
                item.setId(QUuid::createUuid().toString());
                item.setContents(reachAddr.toQXmppElement());
                listItems.append(item);
                publish.setItems(listItems);
                ret = client->sendPacket(publish);
                if (not ret)
                {
                    this->client->write("ERROR: Failed to send packet\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Unknown resource\r\n");
            }
        }
        return ret;
    }

    void XMPPReachabilityCommand::showHelp()
    {
        this->client->write("> xmppReachability <resource> <address>\r\n");
        this->client->write("Send a reachability address to a user\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
