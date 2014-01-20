/**
 * Copyright 2013 Truphone
 */
#include "Utils.h"

#include <string.h>
#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/QmlDocument>

using bb::cascades::Application;
using bb::cascades::QmlDocument;

namespace truphone
{
namespace test
{
namespace cascades
{
    bool Utils::isDelim(const QString& delim,
                        const char c)
    {
        const int delim_count = delim.length();
        bool is_delim = false;

        for (int p = 0 ; (p < delim_count) and (not is_delim) ; p++)
        {
            if (c == delim.at(p))
            {
                is_delim = true;
            }
        }

        return is_delim;
    }

    QStringList Utils::tokenise(const QString& delim,
                                const QString& buffer,
                                const bool includeDelim)
    {
        QStringList list;
        QString tmp;

        for (int p = 0; p < buffer.length(); p++)
        {
            if (buffer.at(p) == static_cast<char>(0xC2u))  // ASCII/SHIFT etc
            {
                // ignore me
            }
            else if (isDelim(delim, buffer.at(p).toAscii()))
            {
                list.append(QString(tmp));
                // ensure tokens are added
                // Q: why are spaces a special case?
                // we can use isDelim (which they are)
                // so actually this is always true.
                //
                // A: There may be many spaces, we don't
                // want to add them multiple times
                // so we may have to peek at the top
                // of the list to see if a space already
                // exists.
                if (buffer.at(p) not_eq ' ')
                {
                    if (includeDelim)
                    {
                        list.append(QString(buffer.at(p)));
                    }
                }
                tmp.clear();
            }
            else
            {
                tmp.append(buffer.at(p));
            }
        }
        if (not tmp.isEmpty())
        {
            list.append(tmp);
        }

        return list;
    }

    // -------------------------------------------------
    // functions from hooq to get the unique object name
    // -------------------------------------------------

    const QString Utils::rawObjectName(const QObject * const obj)
    {
        // Grab the object name
        if (not obj->objectName().isEmpty())
        {
            return obj->objectName();
        }

        // If it's got no parent, classname:0
        if (not obj->parent())
        {
            return QString("%1-0").arg(obj->metaObject()->className());
        }

        // It does - classname:Index
        const QList<QObject*> siblings = obj->parent()->children();
        int index = 1;
        Q_FOREACH(const QObject * const sibling, siblings)
        {
            if (sibling == obj)
            {
                break;
            }
            // QML workaround
            if (strcmp(sibling->metaObject()->className(),
                       obj->metaObject()->className()) == 0)
            {
                ++index;
            }
        }

        return QString(
            "%1-%2").arg(
                obj->metaObject()->className()).arg(
                    index);
    }

    const QString Utils::objectName(const QObject * const obj)
    {
        QString name = rawObjectName(obj);
        name.replace("::", ":_:");  // hooq uses :: as path separators
        return name;
    }

    const QString Utils::objectPath(const QObject * const obj)
    {
        QStringList pathElements;
        const QObject * e = obj;
        while (e)
        {
            pathElements.prepend(objectName(e));
            e = e->parent();
        }
        return pathElements.join("::");
    }

    QObject * Utils::findQmlDocumentVaraible(
            QObject * const obj,
            const QString varName,
            const int level,
            const int maxLevel)
    {
        QObject * result = NULL;
        // too deep
        if (level <= maxLevel)
        {
            // check this object
            QmlDocument * const doc = qobject_cast<QmlDocument*>(obj);
            if (doc)
            {
                QVariant var = doc->documentContext()->contextProperty(varName);
                if (not var.isNull() and var.isValid())
                {
                    QObject * const varPropertyObj = var.value<QObject*>();
                    if (varPropertyObj)
                    {
                        result = varPropertyObj;
                    }
                }
            }

            if (not result)
            {
                // no luck, check all the children
                Q_FOREACH(QObject * const child, obj->children())
                {
                    QObject * const o = findQmlDocumentVaraible(
                                child,
                                varName,
                                level + 1,
                                maxLevel);
                    if (o)
                    {
                        result = o;
                        break;
                    }
                }
            }
        }
        return result;
    }

    QObject* Utils::findObject(const QString& path, const bool scanQmlContent)
    {
        QObject * result = Application::instance()->scene()->findChild<QObject*>(path);
        if (not result)
        {
            result = Application::instance()->findChild<QObject*>(path);
        }
        if (not result && path == "Application")
        {
            result = Application::instance();
        }
        if (not result and scanQmlContent)
        {
            result = findQmlDocumentVaraible(Application::instance(), path);
        }
        if (not result)
        {
            const QString separator("::");
            QStringList parts = path.split(separator);
            if (parts.isEmpty())
            {
                return NULL;
            }
            const QString name = parts.takeLast();
            const QObject * parent = NULL;
            if (parts.isEmpty())
            {
                const QString childObjectName = Utils::objectName(Application::instance());
                if (childObjectName == name)
                {
                    result = Application::instance();
                }
                else
                {
                    // Top level widget
                    Q_FOREACH(QObject * const object, Application::instance()->children())
                    {
                        const QString childObjectName = Utils::objectName(object);
                        if (childObjectName == name)
                        {
                            result = object;
                            break;
                        }
                    }
                }
            }
            else
            {
                parent = findObject(parts.join(separator), false);
            }

            if (parent)
            {
                Q_FOREACH(QObject * const child, parent->children())
                {
                    if (objectName(child) == name)
                    {
                        result = child;
                        break;
                    }
                }
            }
        }

        return result;
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
