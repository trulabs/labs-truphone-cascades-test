/**
 * Copyright 2014 Truphone
 */
#include "XmppDisconnectCommand.h"

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
    const QString XMPPDisconnectCommand::CMD_NAME = "xmppDisconnect";

    XMPPDisconnectCommand::XMPPDisconnectCommand(Connection * const socket,
                                             QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    XMPPDisconnectCommand::~XMPPDisconnectCommand()
    {
    }

    bool XMPPDisconnectCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->length() not_eq 1)
        {
            this->client->write("ERROR: xmppDisconnect <resource>\r\n");
        }
        else
        {
            const QString resource = arguments->first();
            QXmppClient * const client =
                    XMPPResourceStore::instance()->getFromStore(resource);
            arguments->removeFirst();
            if (client)
            {
                client->disconnectFromServer();
                XMPPResourceStore::instance()->removeFromStore(resource);
                ret = true;
            }
            else
            {
                this->client->write("ERROR: Unknown resource\r\n");
            }
        }
        return ret;
    }

    void XMPPDisconnectCommand::showHelp()
    {
        this->client->write("> xmppDisconnect <resource>\r\n");
        this->client->write("Disconnect a resource's connection.\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
