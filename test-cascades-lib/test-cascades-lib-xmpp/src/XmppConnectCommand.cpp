/**
 * Copyright 2014 Truphone
 */
#include "XmppConnectCommand.h"

#include <QString>
#include <QObject>

#include "Connection.h"
#include "XmppResourceStore.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString XMPPConnectCommand::CMD_NAME = "xmppConnect";

    XMPPConnectCommand::XMPPConnectCommand(Connection * const socket,
                                           QObject* parent)
        : Command(parent),
          client(socket),
          xmppClient(new QXmppClient(XMPPResourceStore::instance()))
    {
        bool ok = connect(xmppClient, SIGNAL(connected()), SLOT(connected()));
        Q_ASSERT(ok); Q_UNUSED(ok);
        ok = connect(xmppClient, SIGNAL(disconnected()), SLOT(disconnected()));
        Q_ASSERT(ok); Q_UNUSED(ok);
        ok = connect(xmppClient,
                SIGNAL(error(QXmppClient::Error)),
                SLOT(error(QXmppClient::Error)));
        Q_ASSERT(ok); Q_UNUSED(ok);
    }

    XMPPConnectCommand::~XMPPConnectCommand()
    {
    }

    bool XMPPConnectCommand::executeCommand(QStringList * const arguments)
    {
        if (arguments->length() not_eq 3)
        {
            this->client->write("ERROR: xmppConnect <user> <pass> <resource>\r\n");
        }
        else
        {
            const QString username = arguments->at(0);
            const QString password = arguments->at(1);
            this->resourceName = arguments->at(2);
            this->xmppClient->connectToServer(username, password);
            this->xmppClient->configuration().setAutoAcceptSubscriptions(true);
            this->xmppClient->configuration().setAutoReconnectionEnabled(false);
            this->xmppClient->configuration().setIgnoreSslErrors(true);
            this->xmppClient->configuration().setResource(this->resourceName);
        }
        // we always return false and wait for a signal
        // back from qxmpp about when we've connected at which point
        // we'll write out OK to let us continue.
        return false;
    }

    void XMPPConnectCommand::showHelp()
    {
        this->client->write("> xmppConnect <user> <pass> <resourceId>\r\n");
        this->client->write("Connect to an XMPP server.\r\n");
    }

    void XMPPConnectCommand::connected()
    {
        XMPPResourceStore::instance()->addToStore(this->resourceName, this->xmppClient);
        this->client->write("OK\r\n");
        this->deleteLater();
    }

    void XMPPConnectCommand::disconnected()
    {
        this->xmppClient->deleteLater();
        this->client->write("ERROR: Disconnected\r\n");
        this->deleteLater();
    }

    void XMPPConnectCommand::error(QXmppClient::Error)
    {
        this->xmppClient->deleteLater();
        this->client->write("ERROR: QXmppError\r\n");
        this->deleteLater();
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
