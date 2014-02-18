/**
 * Copyright 2014 Truphone
 */
#include "XmppResourceStore.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    XMPPResourceStore * XMPPResourceStore::INSTANCE = 0;

    XMPPResourceStore::XMPPResourceStore(QObject *parent)
        : QObject(parent),
          map(new QMap<QString, QXmppClient*>())
    {
    }
    XMPPResourceStore::~XMPPResourceStore()
    {
        delete map;
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
        this->map->insert(resource, client);
    }
    QXmppClient * XMPPResourceStore::getFromStore(const QString &resource)
    {
        return this->map->value(resource);
    }
    void XMPPResourceStore::removeFromStore(const QString &resource)
    {
        this->map->remove(resource);
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
