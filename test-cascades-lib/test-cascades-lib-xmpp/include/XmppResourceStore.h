/**
 * Copyright 2014 Truphone
 */
#ifndef XMPPRESOURCESTORE_H_
#define XMPPRESOURCESTORE_H_

#include <QObject>
#include <QMap>

class QXmppClient;
#include <QXmppMessage.h>
class QXmppPresence;
class QXmppPubSubIq;
class QXmppMessageCarbonsIq;

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The XMPPResourceStoreItem class is used to store
     * previously sent and received messages.
     *
     * @since test-cascades 1.1.0
     */
    class XMPPResourceStoreItem : public QObject
    {
    public:
        /*!
         * \brief XMPPResourceStoreItem Empty constructor
         *
         * \param parent
         *
         * @since test-cascades 1.1.0
         */
        XMPPResourceStoreItem(QObject * const parent = 0);
        /*!
         * \brief XMPPResourceStoreItem Create storage for a message
         *
         * \param stanza The message
         *
         * @since test-cascades 1.1.0
         */
        void setStanza(const QXmppMessage& stanza);
        /*!
         * \brief XMPPResourceStoreItem Create storage for a presence
         *
         * \param stanza The message
         *
         * @since test-cascades 1.1.0
         */
        void setStanza(const QXmppPresence& stanza);
        /*!
         * \brief XMPPResourceStoreItem Create storage for a pubsub iq
         *
         * \param stanza The message
         *
         * @since test-cascades 1.1.0
         */
        void setStanza(const QXmppPubSubIq& stanza);
        /*!
         * \brief XMPPResourceStoreItem Create storage for a pubsub iq
         *
         * \param stanza The message
         *
         * @since test-cascades 1.1.0
         */
        void setStanza(const QXmppMessageCarbonsIq& stanza);
        /*!
         * \brief getStanza Get a pointer to the stanza
         *
         * \return The stanza stored in this message
         *
         * @since test-cascades 1.1.0
         */
        const QSharedPointer<QXmppStanza> getStanza() const;
    private:
        Q_DISABLE_COPY(XMPPResourceStoreItem)
        /*!
         * \brief message Stored message
         */
        QSharedPointer<QXmppMessage> message;
        /*!
         * \brief presence Stored presence
         */
        QSharedPointer<QXmppPresence> presence;
        /*!
         * \brief pubSub Stored pubsub
         */
        QSharedPointer<QXmppPubSubIq> pubSub;
        /*!
         * \brief pubSub Stored carbon
         */
        QSharedPointer<QXmppMessageCarbonsIq> carbon;
    };

    /*!
     * \brief The XMPPResourceStore class is used to store resources/connections as
     * a map so that all commands can access the connections.
     *
     * @since test-cascades 1.1.0
     */
    class XMPPResourceStore : public QObject
    {
    Q_OBJECT
    public:
        /*!
         * \brief initialiseStore Initialise the singleton.
         * \param parent The parent object
         *
         * @since test-cascades 1.1.0
         */
        static void initialiseStore(QObject * parent = 0);
        /*!
         * \brief instance Get the current instance.
         *
         * \return The current instance.
         *
         * @since test-cascades 1.1.0
         */
        static XMPPResourceStore * instance();

        /*!
         * \brief addToStore Add a resource to the store.
         *
         * \param resource The resource id.
         * \param client The connection for the resource.
         *
         * @since test-cascades 1.1.0
         */
        void addToStore(const QString& resource, QXmppClient * const client);
        /*!
         * \brief getFromStore Get a connection from the store.
         *
         * \param resource The resource ID to get
         *
         * \return The XMPP connection for that resource or NULL if there
         * isn't one
         *
         * @since test-cascades 1.1.0
         */
        QXmppClient * getFromStore(const QString& resource) const;
        /*!
         * \brief removeFromStore Remove a resource from the store.
         *
         * \param resource The resource to remove.
         *
         * @since test-cascades 1.1.0
         */
        void removeFromStore(const QString& resource);
        /*!
         * \brief getLastMessageReceived Get the latest message that was
         * received from the client.
         *
         * \param client The client to query.
         *
         * \return @c The message - you need to handle the memory
         *
         * @since test-cascades 1.1.0
         */
        const QSharedPointer<QXmppStanza> getLastMessageReceived(
                QXmppClient * const client) const;
        /*!
         * \brief getLastMessageSent Gets the latest message that was
         * sent by the client.
         *
         * \param client The client to query.
         *
         * \return @c The message - you need to handle the memory
         *
         * @since test-cascades 1.1.0
         */
        const QSharedPointer<QXmppStanza> getLastMessageSent(
                QXmppClient * const client) const;
        /*!
         * \brief setLastMessageSent Sets the latest message that was
         * sent by the client.
         *
         * \param client The client that sent the message.
         * \param message A reference to the message
         *
         * @since test-cascades 1.1.0
         */
        void setLastMessageSent(QXmppClient * const client, const QXmppMessage& message);
        /*!
         * \brief setLastMessageSent Sets the latest message that was
         * sent by the client.
         *
         * \param client The client that sent the message.
         * \param presence A reference to the presence
         *
         * @since test-cascades 1.1.0
         */
        void setLastMessageSent(QXmppClient * const client, const QXmppPresence& presence);
        /*!
         * \brief setLastMessageSent Sets the latest message that was
         * sent by the client.
         *
         * \param client The client that sent the message.
         * \param pubsub A reference to the pubsub
         *
         * @since test-cascades 1.1.0
         */
        void setLastMessageSent(QXmppClient * const client, const QXmppPubSubIq& pubsub);
        /*!
         * \brief setLastMessageSent Sets the latest message that was
         * sent by the client.
         *
         * \param client The client that sent the message.
         * \param carbon A reference to the carbon
         *
         * @since test-cascades 1.1.1
         */
        void setLastMessageSent(QXmppClient * const client, const QXmppMessageCarbonsIq& carbon);
    private slots:
        /*!
         * \brief messageReceived Event that occurs when we receive a message.
         *
         * \param message The message that we receive.
         *
         * @since test-cascades 1.1.0
         */
        void messageReceived(const QXmppMessage &message);
    private:
        /*!
         * \brief INSTANCE Our singleton
         */
        static XMPPResourceStore * INSTANCE;
        /*!
         * \brief XMPPResourceStore Hidden constructor
         *
         * \param parent The parent object
         *
         * @since test-cascades 1.1.0
         */
        explicit XMPPResourceStore(QObject * const parent = 0);
        /*!
         * \brief ~XMPPResourceStore Destructor
         *
         * @since test-cascades 1.1.0
         */
        virtual ~XMPPResourceStore();
        /*!
         * \brief map Map of resources/to/connections.
         */
        QMap<QString, QXmppClient*> map;
        /*!
         * \brief lastMsgMap Contains the last message received
         * from each client.
         */
        QMap<QXmppClient*, XMPPResourceStoreItem*> lastMsgReceivedMap;
        /*!
         * \brief lastMsgMap Contains the last message sent
         * from each client.
         */
        QMap<QXmppClient*, XMPPResourceStoreItem*> lastMsgSentMap;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // XMPPRESOURCESTORE_H_
