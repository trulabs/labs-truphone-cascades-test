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
          scenePane(bb::cascades::Application::instance()->scene()),
          namedPathEnd("^"),
          namedPathSep("~"),
          assignSep("=")
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
                // check the count/size of the list
                if (command == "count" && not arguments->isEmpty())
                {
                    bool ok = false;
                    const int expected = arguments->first().toInt(&ok);
                    if (ok)
                    {
                        arguments->removeFirst();
                        const int actual =
                                listView->dataModel()->childCount(listView->rootIndexPath());
                        ret = (actual == expected);
                        if (not ret)
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
                // check an element by index
                else if (command == "index")
                {
                    const QVariant element = findElementByIndex(listView, arguments->first());
                    arguments->removeFirst();
                    if (arguments->isEmpty())
                    {
                        ret = checkElement(element, NULL);
                    }
                    else
                    {
                        ret = checkElement(element, arguments->join(" "));
                    }
                }
                // check an element by named index
                else if (command == "name")
                {
                    QStringList path;
                    while(not arguments->isEmpty())
                    {
                        QString arg = arguments->first();
                        path.push_back(arg);
                        arguments->removeFirst();
                        if (arg.endsWith("^"))
                        {
                            break;
                        }
                    }

                    QString tmp = path.join(" ").trimmed();
                    normaliseValue(tmp);
                    const QString namedIndex = tmp.left(tmp.lastIndexOf(namedPathEnd.cdata()));
                    const QVariant element = findElementByName(listView, namedIndex);
                    ret = checkElement(element, arguments->join(" "));
                }
                else if (command == "select")
                {
                    this->client->write("ERROR: Not supported yet\r\n");
                }
                else if (command == "clear")
                {
                    this->client->write("ERROR: Not supported yet\r\n");
                }
                else if (command == "unselect")
                {
                    this->client->write("ERROR: Not supported yet\r\n");
                }
                else if (command == "scroll")
                {
                    this->client->write("ERROR: Not supported yet\r\n");
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

    void ListCommand::normaliseValue(QString& value)
    {
        if (value.endsWith("^"))
        {
            value.chop(1);
        }
    }

    QVariant ListCommand::findElementByIndex(
            ListView * const list,
            const QString& index) const
    {
        QVariant result;
        Buffer indexString (index.toUtf8().constData());
        DataModel * const model = list->dataModel();
        QStringList indexes = Utils::tokenise(&namedPathSep, &indexString, false);

        bool failed = false;
        QVariantList indexList;
        Q_FOREACH(QString sIndex, indexes)
        {
            bool ok = false;
            const int iIndex = sIndex.toInt(&ok);
            if (ok)
            {
                 indexList.push_back(iIndex);
            }
            else
            {
                failed = true;
                break;
            }
        }

        if (not failed)
        {
            result = model->data(indexList);
        }
        return result;
    }

    QVariant ListCommand::findElementByName(
            bb::cascades::ListView * const list,
            const QString& index) const
    {
        QVariant result;
        const Buffer indexString (index.toUtf8().constData());
        DataModel * const model = list->dataModel();
        QStringList indexes = Utils::tokenise(&namedPathSep, &indexString, false);

        bool failed = false;
        QVariantList indexList;
        while(not indexes.isEmpty())
        {
            const QString elementName = indexes.first();
            bool found = false;
            for (int i = 0 ; i < model->childCount(indexList) ; i++)
            {
                QVariantList tmp(indexList);
                tmp.push_back(i);
                const QVariant v = model->data(tmp);
                const QString type = v.typeName();
                // just compare the value
                if (type == "QString")
                {
                    const QString value = v.toString();
                    if (value == elementName)
                    {
                        indexList.push_back(i);
                        found = true;
                        break;
                    }
                }
                // look up the name/value pair
                else if (type == "QVariantMap")
                {
                    QStringList keyValuePair = elementName.split(assignSep.cdata());
                    if (keyValuePair.size() == 2)
                    {
                        const QString key = keyValuePair.first().trimmed();
                        keyValuePair.removeFirst();
                        const QString value = keyValuePair.first().trimmed();

                        if (not key.isNull() && not key.isEmpty()
                                && not value.isNull() && not value.isEmpty())
                        {
                            const QVariantMap elementMap(v.toMap());
                            const QString actual = elementMap[key].toString();
                            if (actual == value)
                            {
                                indexList.push_back(i);
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }
            if (not found)
            {
            	failed = true;
            	break;
            }
            indexes.removeFirst();
        }

        if (not failed)
        {
             result = model->data(indexList);
        }
        return result;
    }

    bool ListCommand::checkElement(
                    const QVariant element,
                    const QString& check) const
    {
        bool ret = false;

        if (check == NULL)
        {
            ret = ((check == NULL) == element.isNull());
            if (not ret)
            {
                this->client->write("ERROR: Value is {not null} expected {null}}\r\n");
            }
        }
        else
        {
            if (not element.isNull() && element.isValid())
            {
                const QString elementType = QString(element.typeName());
                if (elementType == "QString")
                {
                    ret = (check == element.toString());
                    if (not ret)
                    {
                        this->client->write("ERROR: Value is {");
                        this->client->write(check.toUtf8().constData());
                        this->client->write("} expected {");
                        this->client->write(element.toString().toUtf8().constData());
                        this->client->write("}\r\n");
                    }
                }
                else if (elementType == "QVariantMap")
                {
                    QString normalisedCheck(check);
                    normaliseValue(normalisedCheck);
                    QStringList keyValuePair = normalisedCheck.split(assignSep.cdata());
                    if (keyValuePair.size() == 2)
                    {
                        const QString key = keyValuePair.first().trimmed();
                        keyValuePair.removeFirst();
                        const QString value = keyValuePair.first().trimmed();

                        if (not key.isNull() && not key.isEmpty()
                                && not value.isNull() && not value.isEmpty())
                        {
                            const QVariantMap elementMap(element.toMap());
                            const QString actual = elementMap[key].toString();
                            ret = (actual == value);
                            if (not ret)
                            {
                                this->client->write("ERROR: Value is {");
                                this->client->write(actual.toUtf8().constData());
                                this->client->write("} expected {");
                                this->client->write(value.toUtf8().constData());
                                this->client->write("}\r\n");
                            }
                        }
                        else
                        {
                            this->client->write("ERROR: You didn't enter a key=value pair\r\n");
                        }
                    }
                    else
                    {
                        this->client->write("ERROR: You didn't enter a key=value pair\r\n");
                    }
                }
                else
                {
                    this->client->write("ERROR: Unsupported list element type\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Element is null or non-valid type\r\n");
            }
        }

        return ret;
    }

    void ListCommand::showHelp()
    {
        this->client->write("> list <list> count <expectedSize>\r\n");
        this->client->write("> list <list> <index> <expected value> - check string values\r\n");
        this->client->write("e.g. list someList 0~1~2 /etc/files/file\r\n");
        this->client->write("> list <list> <index> <key>=<expected value> - check QVarientMap values\r\n");
        this->client->write("e.g. list someList 0~1~2 filename=/etc/files/file\r\n");
        this->client->write("> list <list> <name> <expected value> - check string values\r\n");
        this->client->write("e.g. list someList /~etc~files^ /etc/files/file\r\n");
        this->client->write("> list <list> <name> <key>=<expected value> - check QVarientMap values\r\n");
        this->client->write("e.g. list someList /~etc~files^ filename=/etc/files/file\r\n");
        this->client->write("> list <list> select index <index> - select an index\r\n");
        this->client->write("> list <list> select name <name> - select a named index\r\n");
        this->client->write("> list <list> unselect index <index> - unselect an index\r\n");
        this->client->write("> list <list> unselect name <name> - unselect a named index\r\n");
        this->client->write("> list <list> scroll index <index> - scroll to an index\r\n");
        this->client->write("> list <list> scroll name <name> - scroll to a named index\r\n");
        this->client->write("> list <list> clear\r\n");
        this->client->write(">\r\n");
        this->client->write("> <index> should be numerical and separated by ~ (i.e. 0~1~2)\r\n");
        this->client->write("> <name> should be text and separated by ~ and terminated by ^\r\n");
        this->client->write("\t level 1~level 2~level 3^");

    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
