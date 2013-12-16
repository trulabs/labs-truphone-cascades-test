/**
 * Copyright 2013 Truphone
 */
#include "ListCommand.h"

#include <QString>
#include <QList>
#include <QObject>

#include <bb/cascades/ListView>
#include <bb/cascades/DataModel>

#include "Connection.h"
#include "Utils.h"

using bb::cascades::ListView;
using bb::cascades::DataModel;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString ListCommand::CMD_NAME = "list";

    ListCommand::ListCommand(Connection * const socket,
                             QObject* parent)
        : Command(parent),
          client(socket),
          scenePane(bb::cascades::Application::instance()->scene())
    {
    }

    ListCommand::~ListCommand()
    {
    }

    bool ListCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->size() > 1)
        {
            const QString listViewName = arguments->first();
            arguments->removeFirst();
            ListView * const listView = scenePane->findChild<ListView*>(listViewName);
            if (listView)
            {
                const QString command = arguments->first();
                arguments->removeFirst();
                //
                // check the count/size of the list
                //
                if (command == "count" && !arguments->isEmpty())
                {
                    bool ok = false;
                    const int expected = arguments->first().toInt(&ok);
                    if (ok)
                    {
                        arguments->removeFirst();
                        const int actual =
                                listView->dataModel()->childCount(listView->rootIndexPath());
                        ret = (actual == expected);
                        if (!ret)
                        {
                            this->client->write("ERROR: List size is {");
                            this->client->write(QString::number(actual).toUtf8().constData());
                            this->client->write("} was expecting {");
                            this->client->write(QString::number(expected).toUtf8().constData());
                            this->client->write("}\r\n");
                        }
                    }
                    else
                    {
                        this->client->write("ERROR: Expected list size wasn't an integer\r\n");
                    }
                }
                //
                // check an element by index
                //
                else if (command == "index")
                {
                    const QVariant element = findElementByIndex(listView, arguments->first());
                    qDebug() << "got" << element.toString();
                    if (!element.isNull() && element.isValid())
                    {
                        const QString elementType = QString(element.typeName());
                        if (elementType == "QString")
                        {
                            qDebug() << "Result" << element.toString();
                        }
                        else if (elementType == "QVariantMap")
                        {
                            QVariantMap elementMap(element.toMap());
                            Q_FOREACH(QString key, elementMap.keys())
                            {
                                qDebug() << "Key" << key << "=" << elementMap.value(key).toString();
                            }
                        }
                        else
                        {
                            this->client->write("ERROR: Unsupported list element type\r\n");
                        }
                    }
                    else
                    {
                        this->client->write("ERROR: Couldn't find the index in the list\r\n");
                    }
                }
                //
                // check an element by named index
                //
                else if (command == "name")
                {
                    const QString tmp = arguments->join(" ").trimmed();
                    const QString namedIndex = tmp.left(tmp.lastIndexOf("."));
                    const QVariant element = findElementByName(listView, namedIndex);
                    if (!element.isNull() && element.isValid())
                    {
                        const QString elementType = QString(element.typeName());
                        if (elementType == "QString")
                        {
                            qDebug() << "Result" << element.toString();
                        }
                        else if (elementType == "QVariantMap")
                        {
                            QVariantMap elementMap(element.toMap());
                            Q_FOREACH(QString key, elementMap.keys())
                            {
                                qDebug() << "Key" << key << "=" << elementMap.value(key).toString();
                            }
                        }
                        else
                        {
                            this->client->write("ERROR: Unsupported list element type\r\n");
                        }
                    }
                    else
                    {
                        this->client->write("ERROR: Couldn't find the index in the list\r\n");
                    }
                }
                else
                {
                    this->client->write("ERROR: Unknown list command\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Couldn't find the listview\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: Not enough arguments, sleep <timeInMs>\r\n");
        }
        return ret;
    }

    QVariant ListCommand::findElementByIndex(
            ListView * const list,
            const QString& index)
    {
        QVariant result;
        Buffer delim(".");
        Buffer indexString (index.toUtf8().constData());
        DataModel * const model = list->dataModel();
        QStringList indexes = Utils::tokenise(&delim, &indexString, false);

        bool failed = false;
        QVariantList indexList;
        Q_FOREACH(QString sIndex, indexes)
        {
            bool ok = false;
            qDebug() << "findElementByIndex" << sIndex;
            const int iIndex = sIndex.toInt(&ok);
            if (ok)
            {
                qDebug() << "findElementByIndex converted to" << iIndex;
                indexList.push_back(iIndex);
            }
            else
            {
                failed = true;
                break;
            }
        }

        if (!failed)
        {
            qDebug() << "findElementByIndex index is" << indexList;
            result = model->data(indexList);
        }
        return result;
    }

    QVariant ListCommand::findElementByName(
            bb::cascades::ListView * const list,
            const QString& index)
    {
        QVariant result;
        Buffer delim(".");
        Buffer indexString (index.toUtf8().constData());
        DataModel * const model = list->dataModel();
        QStringList indexes = Utils::tokenise(&delim, &indexString, false);

        bool failed = false;
        QVariantList indexList;
        while(!indexes.isEmpty())
        {
            const QString elementName = indexes.first();
            bool found = false;
            for (int i = 0 ; i < model->childCount(indexList) ; i++)
            {
                QVariantList tmp(indexList);
                tmp.push_back(i);
                const QVariant v = model->data(tmp);
                const QString value = v.toString();
                const QString type = v.typeName();
                if (value == elementName)
                {
                    qDebug() << "findElementByName " << value << "<" << type << ")" << "=" << elementName;
                    qDebug() << "findElementByName converted to" << i;
                    indexList.push_back(i);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
            	failed = true;
            	break;
            }
            indexes.removeFirst();
        }

        if (!failed)
        {
            qDebug() << "findElementByName index is" << indexList;
            result = model->data(indexList);
        }
        return result;
    }

    void ListCommand::showHelp()
    {
        this->client->write("> list <list> count <expectedSize>\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
