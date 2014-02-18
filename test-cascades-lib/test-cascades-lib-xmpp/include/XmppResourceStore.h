/**
 * Copyright 2014 Truphone
 */
#ifndef XMPPRESOURCESTORE_H_
#define XMPPRESOURCESTORE_H_

#include <QObject>
#include <QXmppClient.h>
#include <QMap>

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The XMPPResourceStore class is used to store resources/connections as
     * a map so that all commands can access the connections.
     *
     * @since test-cascades 1.1.0
     */
    class XMPPResourceStore : public QObject
    {
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
        QXmppClient * getFromStore(const QString& resource);
        /*!
         * \brief removeFromStore Remove a resource from the store.
         *
         * \param resource The resource to remove.
         *
         * @since test-cascades 1.1.0
         */
        void removeFromStore(const QString& resource);
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
        explicit XMPPResourceStore(QObject * parent = 0);
        /*!
         * \brief ~XMPPResourceStore Destructor
         *
         * @since test-cascades 1.1.0
         */
        virtual ~XMPPResourceStore();
        /*!
         * \brief map Map of resources/to/connections.
         */
        QMap<QString, QXmppClient*> * const map;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // XMPPRESOURCESTORE_H_
