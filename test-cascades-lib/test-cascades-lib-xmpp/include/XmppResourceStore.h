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
    class XMPPResourceStore : public QObject
    {
    public:
        static void initialiseStore(QObject * parent = 0);
        static XMPPResourceStore * instance();

        void addToStore(const QString& resource, QXmppClient * const client);
        QXmppClient * getFromStore(const QString& resource);
        void removeFromStore(const QString& resource);
    private:
        static XMPPResourceStore * INSTANCE;
        explicit XMPPResourceStore(QObject * parent = 0);
        virtual ~XMPPResourceStore();
        QMap<QString, QXmppClient*> * const map;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // XMPPRESOURCESTORE_H_
