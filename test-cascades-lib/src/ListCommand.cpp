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
                    QVariantList indexPath;
                    if (findElementByIndex(arguments->first(), indexPath))
                    {
                        const QVariant element = listView->dataModel()->data(indexPath);
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
                    else
                    {
                        this->client->write("ERROR: Failed to convert index to indexPath\n");
                    }
                }
                // check an element by named index
                else if (command == "name")
                {
                    const QString namedIndex = extractNamedPath(arguments,namedPathEnd.cdata());
                    QVariantList indexPath;
                    if (findElementByName(listView, namedIndex, indexPath))
                    {
                        const QVariant element = listView->dataModel()->data(indexPath);
                        ret = checkElement(element, arguments->join(" "));
                    }
                    else
                    {
                        this->client->write("ERROR: Failed to convert named index to indexPath\n");
                    }
                }
                else if (command == "select")
                {
                    ret = selectUnselectPath(arguments, listView, true);
                }
                else if (command == "clear")
                {
                    listView->clearSelection();
                    ret = true;
                }
                else if (command == "unselect")
                {
                    ret = selectUnselectPath(arguments, listView, false);
                }
                else if (command == "scroll")
                {
                    ret = scrollToPath(arguments, listView);
                }
                else if (command == "key")
                {
                    ret = showKeysOnPath(arguments, listView);
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

    bool ListCommand::scrollToPath(
            QStringList * const arguments,
            ListView * const listView)
    {
        bool ret = false;
        if (arguments->size() > 1)
        {
            bool indexPathOk = true;
            QVariantList indexPath;
            const QString selectType = arguments->first();
            arguments->removeFirst();
            if (selectType == "index")
            {
                if (not findElementByIndex(arguments->first(), indexPath))
                {
                    this->client->write("ERROR: Failed to convert index to indexPath\n");
                    indexPathOk = false;
                }
                arguments->removeFirst();

            }
            else if (selectType == "name")
            {
                const QString namedIndex = extractNamedPath(arguments,namedPathEnd.cdata());
                if (not findElementByName(listView, namedIndex, indexPath))
                {
                    this->client->write("ERROR: Failed to convert named index to indexPath\n");
                    indexPathOk = false;
                }
            }
            else
            {
                indexPathOk = false;
                this->client->write("ERROR: Invalid lookup method, use index or name\r\n");
            }
            if (indexPathOk)
            {
                QVariant element = listView->dataModel()->data(indexPath);
                if (not element.isNull() && element.isValid())
                {
                    listView->scrollToItem(indexPath);
                    ret = true;
                }
                else
                {
                    this->client->write("ERROR: Tried to scroll to invalid item\r\n");
                }
            }
        }
        else
        {
            this->client->write("ERROR: list select command needs more parameters\r\n");
        }
        return ret;
    }

    bool ListCommand::showKeysOnPath(
            QStringList * const arguments,
            ListView * const listView)
    {
        bool ret = false;
        if (arguments->size() > 1)
        {
            bool indexPathOk = true;
            QVariantList indexPath;
            const QString selectType = arguments->first();
            arguments->removeFirst();
            if (selectType == "index")
            {
                if (not findElementByIndex(arguments->first(), indexPath))
                {
                    this->client->write("ERROR: Failed to convert index to indexPath\n");
                    indexPathOk = false;
                }
                arguments->removeFirst();

            }
            else if (selectType == "name")
            {
                const QString namedIndex = extractNamedPath(arguments,namedPathEnd.cdata());
                if (not findElementByName(listView, namedIndex, indexPath))
                {
                    this->client->write("ERROR: Failed to convert named index to indexPath\n");
                    indexPathOk = false;
                }
            }
            else
            {
                indexPathOk = false;
                this->client->write("ERROR: Invalid lookup method, use index or name\r\n");
            }
            if (indexPathOk)
            {
                QVariant element = listView->dataModel()->data(indexPath);
                if (not element.isNull() && element.isValid())
                {
                    if (QString(element.typeName()) == "QVariantMap")
                    {
                        QStringList keys;
                        Q_FOREACH(QString key, element.toMap().keys())
                        {
                            keys.push_back(key);
                        }
                        this->client->write("OK (");
                        this->client->write(keys.join(", ").toUtf8().constData());
                        this->client->write(")\r\n");
                        ret = false; // yep, false. Stops OK getting reported twice.
                    }
                    else
                    {
                        this->client->write("ERROR: Can only show keys for QVariantMaps\r\n");
                    }
                }
                else
                {
                    this->client->write("ERROR: Tried to key to invalid item\r\n");
                }
            }
        }
        else
        {
            this->client->write("ERROR: list select command needs more parameters\r\n");
        }
        return ret;
    }

    bool ListCommand::selectUnselectPath(
            QStringList * const arguments,
            ListView * const listView,
            const bool select)
    {
        bool ret = false;
        if (arguments->size() > 1)
        {
            bool indexPathOk = true;
            QVariantList indexPath;
            const QString selectType = arguments->first();
            arguments->removeFirst();
            if (selectType == "index")
            {
                if (not findElementByIndex(arguments->first(), indexPath))
                {
                    this->client->write("ERROR: Failed to convert index to indexPath\n");
                    indexPathOk = false;
                }
                arguments->removeFirst();

            }
            else if (selectType == "name")
            {
                const QString namedIndex = extractNamedPath(arguments,namedPathEnd.cdata());
                if (not findElementByName(listView, namedIndex, indexPath))
                {
                    this->client->write("ERROR: Failed to convert named index to indexPath\n");
                    indexPathOk = false;
                }
            }
            else
            {
                indexPathOk = false;
                this->client->write("ERROR: Invalid lookup method, use index or name\r\n");
            }
            if (indexPathOk)
            {
                QVariant element = listView->dataModel()->data(indexPath);
                if (not element.isNull() && element.isValid())
                {
                    listView->select(indexPath, select);
                    ret = true;
                }
                else
                {
                    this->client->write("ERROR: Tried to select invalid item\r\n");
                }
            }
        }
        else
        {
            this->client->write("ERROR: list select command needs more parameters\r\n");
        }
        return ret;
    }

    QString ListCommand::extractNamedPath(
            QStringList * const arguments,
            const char * const endOfPath)
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
        normalisePath(tmp);
        return tmp.left(tmp.lastIndexOf(endOfPath));
    }

    void ListCommand::normalisePath(QString& value)
    {
        if (value.endsWith("^"))
        {
            value.chop(1);
        }
    }

    bool ListCommand::findElementByIndex(
            const QString& index,
            QVariantList& elementIndexPath) const
    {
        Buffer indexString (index.toUtf8().constData());
        QStringList indexes = Utils::tokenise(&namedPathSep, &indexString, false);

        bool failed = false;
        Q_FOREACH(QString sIndex, indexes)
        {
            bool ok = false;
            const int iIndex = sIndex.toInt(&ok);
            if (ok)
            {
                 elementIndexPath.push_back(iIndex);
            }
            else
            {
                failed = true;
                break;
            }
        }
        return not failed;
    }

    bool ListCommand::findElementByName(
            bb::cascades::ListView * const list,
            const QString& index,
            QVariantList& elementIndexPath) const
    {
        const Buffer indexString (index.toUtf8().constData());
        DataModel * const model = list->dataModel();
        QStringList indexes = Utils::tokenise(&namedPathSep, &indexString, false);

        bool failed = false;
        while(not indexes.isEmpty())
        {
            const QString elementName = indexes.first();
            bool found = false;
            for (int i = 0 ; i < model->childCount(elementIndexPath) ; i++)
            {
                QVariantList tmp(elementIndexPath);
                tmp.push_back(i);
                const QVariant v = model->data(tmp);
                const QString type = v.typeName();
                // just compare the value
                if (type == "QString")
                {
                    const QString value = v.toString();
                    if (value == elementName)
                    {
                        elementIndexPath.push_back(i);
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
                                elementIndexPath.push_back(i);
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

        return not failed;
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
                    normalisePath(normalisedCheck);
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
        this->client->write("> list <list> key index <index> - list the keys in Map for index\r\n");
        this->client->write("> list <list> key name <name> - list the keys in Map for named index\r\n");
        this->client->write("e.g. list contacts scroll name J~Name=John Smith^\r\n");
        this->client->write("e.g. list contacts select index 0~1\r\n");
        this->client->write("> list <list> clear\r\n");
        this->client->write(">\r\n");
        this->client->write("> <index> should be numerical and separated by ~ (i.e. 0~1~2)\r\n");
        this->client->write("> <name> should be text and separated by ~ and terminated by ^\r\n");
        this->client->write("\t level 1~level 2~level 3^");

    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
