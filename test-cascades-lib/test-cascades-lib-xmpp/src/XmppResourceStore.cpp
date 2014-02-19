/**
 * Copyright 2014 Truphone
 */
#include "XmppResourceStore.h"

#include <QXmppClient.h>

namespace truphone
{
namespace test
{
namespace cascades
{
    XMPPResourceStore * XMPPResourceStore::INSTANCE = 0;

    XMPPResourceStore::XMPPResourceStore(QObject *parent)
        : QObject(parent)
    {
    }
    XMPPResourceStore::~XMPPResourceStore()
    {
    }
    void XMPPResourceStore::initialiseStore(QObject *parent)
    {
        if (not INSTANCE)
        {
            INSTANCE = new XMPPResourceStore(parent);
        }
    }
    XMPPResourceStore * XMPPResourceStore::instance()
    {
        return INSTANCE;
    }
    void XMPPResourceStore::addToStore(const QString &resource, QXmppClient * const client)
    {
        this->map[resource] = client;
        const bool ok = connect(
                    client,
                    SIGNAL(messageReceived(const QXmppMessage&)),
                    SLOT(messageReceived(const QXmppMessage&)));
        Q_ASSERT(ok); Q_UNUSED(ok);
    }
    QXmppClient * XMPPResourceStore::getFromStore(const QString &resource)
    {
        return this->map.value(resource, NULL);
    }
    void XMPPResourceStore::removeFromStore(const QString &resource)
    {
        this->map.remove(resource);
    }
    bool XMPPResourceStore::getLastMessage(QXmppClient * const client, QXmppMessage& message)
    {
        bool ret = false;
        if (this->lastMsgMap.contains(client))
        {
            message = this->lastMsgMap[client];
            ret = true;
        }
        return ret;
    }
    void XMPPResourceStore::messageReceived(const QXmppMessage &message)
    {
        QXmppClient * const client = qobject_cast<QXmppClient*>(sender());
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgMap[client] = message;
        }
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
