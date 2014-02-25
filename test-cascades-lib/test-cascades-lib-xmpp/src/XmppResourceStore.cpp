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

    class XMPPResourceStorePrivate
    {
    public:
        XMPPResourceStorePrivate()
            : message(NULL),
              presence(NULL),
              pubSub(NULL)
        {
        }

        XMPPResourceStorePrivate(const QXmppMessage& stanza)
            : message(new QXmppMessage(stanza)),
              presence(NULL),
              pubSub(NULL)
        {
        }
        XMPPResourceStorePrivate(const QXmppPresence& stanza)
            : message(NULL),
              presence(new QXmppPresence(stanza)),
              pubSub(NULL)
        {
        }
        XMPPResourceStorePrivate(const QXmppPubSubIq& stanza)
            : message(NULL),
              presence(NULL),
              pubSub(new QXmppPubSubIq(stanza))
        {
        }
        ~XMPPResourceStorePrivate()
        {
            if (message) delete message;
            if (presence) delete presence;
            if (pubSub) delete pubSub;
        }
        const QXmppStanza* getStanza()
        {
            if (message) return message;
            if (presence) return presence;
            if (pubSub) return pubSub;
            return NULL;
        }
    private:
        const QXmppMessage * message;
        const QXmppPresence * presence;
        const QXmppPubSubIq * pubSub;
    };

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

    bool XMPPResourceStore::getLastMessageReceived(
            QXmppClient * const client,
            const QXmppStanza ** message)
    {
        bool ret = false;
        Q_ASSERT(client);
        if (this->lastMsgReceivedMap.contains(client) and message)
        {
            *message = (const QXmppStanza*)(this->lastMsgReceivedMap[client].getStanza());
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
            this->lastMsgReceivedMap[client] = XMPPResourceStorePrivate(
                        message);
            if (XMPPDebugCommand::isDebugEnabled())
            {
                XMPPPrintCommand::printMessage(
                            false,
                            &message);
            }
        }
    }

    bool XMPPResourceStore::getLastMessageSent(
            QXmppClient * const client,
            const QXmppStanza ** message)
    {
        bool ret = false;
        if (this->lastMsgSentMap.contains(client) and message)
        {
            *message = this->lastMsgSentMap[client].getStanza();
            ret = true;
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
            this->lastMsgSentMap[client] = XMPPResourceStorePrivate(message);
        }
    }

    void XMPPResourceStore::setLastMessageSent(
            QXmppClient * const client,
            const QXmppPubSubIq& pubsub)
    {
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgSentMap[client] = XMPPResourceStorePrivate(pubsub);
        }
    }

    void XMPPResourceStore::setLastMessageSent(
            QXmppClient * const client,
            const QXmppPresence& presence)
    {
        Q_ASSERT(client);
        if (client)
        {
            this->lastMsgSentMap[client] = XMPPResourceStorePrivate(presence);
        }
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
