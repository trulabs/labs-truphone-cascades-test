/**
 * Copyright 2014 Truphone
 */
#include "XmppResourceStore.h"

#include <QXmppClient.h>
#include <QXmppPubSubIq.h>
#include <QXmppMessageCarbonsIq.h>

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
          pubSub(NULL),
          carbon(NULL)
    {
    }

    void XMPPResourceStoreItem::setStanza(const QXmppMessage& stanza)
    {
        QSharedPointer<QXmppMessage> newMsg(new QXmppMessage(stanza));
        message.swap(newMsg);
        presence.clear();
        pubSub.clear();
        carbon.clear();
    }
    void XMPPResourceStoreItem::setStanza(const QXmppPresence& stanza)
    {
        QSharedPointer<QXmppPresence> newMsg(new QXmppPresence(stanza));
        message.clear();
        presence.swap(newMsg);
        pubSub.clear();
        carbon.clear();
    }
    void XMPPResourceStoreItem::setStanza(const QXmppPubSubIq& stanza)
    {
        QSharedPointer<QXmppPubSubIq> newMsg(new QXmppPubSubIq(stanza));
        message.clear();
        presence.clear();
        pubSub.swap(newMsg);
        carbon.clear();
    }
    void XMPPResourceStoreItem::setStanza(const QXmppMessageCarbonsIq& stanza)
    {
        QSharedPointer<QXmppMessageCarbonsIq> newMsg(new QXmppMessageCarbonsIq(stanza));
        message.clear();
        presence.clear();
        pubSub.clear();
        carbon.swap(newMsg);
    }

    const QSharedPointer<QXmppStanza> XMPPResourceStoreItem::getStanza() const
    {
        if (not message.isNull()) { return QSharedPointer<QXmppMessage>(message); }
        if (not presence.isNull()) { return QSharedPointer<QXmppPresence>(presence); }
        if (not pubSub.isNull()) { return QSharedPointer<QXmppPubSubIq>(pubSub); }
        if (not carbon.isNull()) { return QSharedPointer<QXmppMessageCarbonsIq>(carbon); }
        return QSharedPointer<QXmppStanza>();
    }

    XMPPResourceStore::XMPPResourceStore(QObject * const parent)
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
        this->lastMsgReceivedMap[client] = new XMPPResourceStoreItem(client);
        this->lastMsgSentMap[client] = new XMPPResourceStoreItem(client);
    }

    QXmppClient * XMPPResourceStore::getFromStore(const QString &resource) const
    {
        return this->map.value(resource, NULL);
    }

    void XMPPResourceStore::removeFromStore(const QString &resource)
    {
        this->map.remove(resource);
    }

    const QSharedPointer<QXmppStanza> XMPPResourceStore::getLastMessageReceived(
            QXmppClient * const client) const
    {
        QSharedPointer<QXmppStanza> ret(NULL);
        Q_ASSERT(client);
        if (this->lastMsgReceivedMap.contains(client))
        {
            ret = this->lastMsgReceivedMap[client]->getStanza();
        }
        return ret;
    }

    const QSharedPointer<QXmppStanza> XMPPResourceStore::getLastMessageSent(
            QXmppClient * const client) const
    {
        QSharedPointer<QXmppStanza> ret(NULL);
        Q_ASSERT(client);
        if (this->lastMsgSentMap.contains(client))
        {
            ret = this->lastMsgSentMap[client]->getStanza();
        }
        return ret;
    }

    void XMPPResourceStore::messageReceived(const QXmppMessage &message)
    {
        QXmppClient * const client = qobject_cast<QXmppClient*>(sender());
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgReceivedMap[client]->setStanza(message);
            if (XMPPDebugCommand::isDebugEnabled())
            {
                XMPPPrintCommand::printMessage(
                            false,
                            &message);
            }
            if (message.hasMessageCarbon())
            {
                this->lastMsgSentMap[client]->setStanza(message);
                if (XMPPDebugCommand::isDebugEnabled())
                {
                    XMPPPrintCommand::printMessage(
                                true,
                                &message);
                }
            }
        }
    }

    void XMPPResourceStore::setLastMessageSent(
            QXmppClient * const client,
            const QXmppMessage& message)
    {
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgSentMap[client]->setStanza(message);
        }
    }

    void XMPPResourceStore::setLastMessageSent(
            QXmppClient * const client,
            const QXmppPubSubIq& pubsub)
    {
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgSentMap[client]->setStanza(pubsub);
        }
    }

    void XMPPResourceStore::setLastMessageSent(
            QXmppClient * const client,
            const QXmppPresence& presence)
    {
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgSentMap[client]->setStanza(presence);
        }
    }

    void XMPPResourceStore::setLastMessageSent(
            QXmppClient * const client,
            const QXmppMessageCarbonsIq& carbon)
    {
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgSentMap[client]->setStanza(carbon);
        }
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
