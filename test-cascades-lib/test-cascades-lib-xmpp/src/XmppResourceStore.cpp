/**
 * Copyright 2014 Truphone
 */
#include "XmppResourceStore.h"

#include <QXmppClient.h>
#include <QXmppPubSubIq.h>

#include "XmppDebugCommand.h"
#include "XmppPrintCommand.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    XMPPResourceStore * XMPPResourceStore::INSTANCE = 0;

    XMPPResourceStoreItem::XMPPResourceStoreItem(QObject * const parent)
        : QObject(parent),
          message(NULL),
          presence(NULL),
          pubSub(NULL)
    {
    }

    XMPPResourceStoreItem::XMPPResourceStoreItem(const XMPPResourceStoreItem &other,
                                                 QObject * const parent)
        : QObject(parent),
          message(other.message not_eq NULL ? new QXmppMessage(*other.message) : NULL),
          presence(other.presence not_eq NULL ? new QXmppPresence(*other.presence) : NULL),
          pubSub(other.pubSub not_eq NULL ? new QXmppPubSubIq(*other.pubSub) : NULL)
    {
    }

    XMPPResourceStoreItem::XMPPResourceStoreItem(const QXmppMessage& stanza,
                                                 QObject * const parent)
        : QObject(parent),
          message(new QXmppMessage(stanza)),
          presence(NULL),
          pubSub(NULL)
    {
    }
    XMPPResourceStoreItem::XMPPResourceStoreItem(const QXmppPresence& stanza,
                                                 QObject * const parent)
        : QObject(parent),
          message(NULL),
          presence(new QXmppPresence(stanza)),
          pubSub(NULL)
    {
    }
    XMPPResourceStoreItem::XMPPResourceStoreItem(const QXmppPubSubIq& stanza,
                                                 QObject * const parent)
        : QObject(parent),
          message(NULL),
          presence(NULL),
          pubSub(new QXmppPubSubIq(stanza))
    {
    }
    XMPPResourceStoreItem::~XMPPResourceStoreItem()
    {
        if (message) { delete message; }
        if (presence) { delete presence; }
        if (pubSub) { delete pubSub; }
    }
    const QXmppStanza* XMPPResourceStoreItem::getStanza() const
    {
        if (message) { return message; }
        if (presence) { return presence; }
        if (pubSub) { return pubSub; }
        return NULL;
    }

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

    const XMPPResourceStoreItem* XMPPResourceStore::getLastMessageReceived(
            QXmppClient * const client)
    {
        XMPPResourceStoreItem * ret = NULL;
        Q_ASSERT(client);
        if (this->lastMsgReceivedMap.contains(client))
        {
            ret = new XMPPResourceStoreItem(*this->lastMsgReceivedMap[client].data());
        }
        return ret;
    }

    void XMPPResourceStore::messageReceived(const QXmppMessage &message)
    {
        QXmppClient * const client = qobject_cast<QXmppClient*>(sender());
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgReceivedMap[client] = QSharedPointer<XMPPResourceStoreItem>
                    (new XMPPResourceStoreItem(
                        message));
            if (XMPPDebugCommand::isDebugEnabled())
            {
                XMPPPrintCommand::printMessage(
                            false,
                            &message);
            }
        }
    }

    const XMPPResourceStoreItem* XMPPResourceStore::getLastMessageSent(
            QXmppClient * const client)
    {
        XMPPResourceStoreItem * ret = NULL;
        if (this->lastMsgSentMap.contains(client))
        {
            ret = new XMPPResourceStoreItem(this->lastMsgSentMap[client].data());
        }
        return ret;
    }

    void XMPPResourceStore::setLastMessageSent(
            QXmppClient * const client,
            const QXmppMessage& message)
    {
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgSentMap[client] = QSharedPointer<XMPPResourceStoreItem>
                    (new XMPPResourceStoreItem(message));
        }
    }

    void XMPPResourceStore::setLastMessageSent(
            QXmppClient * const client,
            const QXmppPubSubIq& pubsub)
    {
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgSentMap[client] = QSharedPointer<XMPPResourceStoreItem>
                    (new XMPPResourceStoreItem(pubsub));
        }
    }

    void XMPPResourceStore::setLastMessageSent(
            QXmppClient * const client,
            const QXmppPresence& presence)
    {
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgSentMap[client] = QSharedPointer<XMPPResourceStoreItem>
                    (new XMPPResourceStoreItem(presence));
        }
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
