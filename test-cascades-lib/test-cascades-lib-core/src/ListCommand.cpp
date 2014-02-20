/**
 * Copyright 2013 Truphone
 */
#include "ListCommand.h"

#include <QString>
#include <QList>
#include <QObject>

#include <bb/cascades/ListView>
#include <bb/cascades/DataModel>
#include <bb/cascades/MultiSelectHandler>
#include <bb/cascades/MultiSelectActionItem>

#include "Connection.h"
#include "Utils.h"

using bb::cascades::ListView;
using bb::cascades::MultiSelectHandler;
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
            ListView * const listView =
                    qobject_cast<ListView*>(Utils::findObject(arguments->first()));
            arguments->removeFirst();
            if (listView)
            {
                const QString command = arguments->first();
                arguments->removeFirst();
                // check the count/size of the list
                if (command == "count" and not arguments->isEmpty())
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
                            QString data("ERROR: List size is {");
                            data += QString::number(actual);
                            data += "} was expecting {";
                            data += QString::number(expected);
                            data += "}\r\n";
                            this->client->write(data.trimmed().toUtf8());
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
                    if (findElementByIndex(listView, arguments->first(), indexPath))
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
                    const QString namedIndex = extractNamedPath(arguments, namedPathEnd);
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
                else if (command == "hold")
                {
                    ret = holdPath(arguments, listView, true);
                }
                else if (command == "release")
                {
                    ret = holdPath(arguments, listView, false);
                }
                else if (command == "tap")
                {
                    ret = tapPath(arguments, listView);
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
        QVariantList indexPath;
        bool ret = convertPathToIndex(arguments, listView, indexPath);
        if (ret)
        {
            QVariant element = listView->dataModel()->data(indexPath);
            ret = not element.isNull() and element.isValid();
            if (ret)
            {
                bb::cascades::Application::processEvents();
                listView->scrollToItem(indexPath);
                bb::cascades::Application::processEvents();
            }
            else
            {
                this->client->write("ERROR: Tried to scroll to invalid item\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: failed to convert path to to index\r\n");
        }
        return ret;
    }

    bool ListCommand::showKeysOnPath(
            QStringList * const arguments,
            ListView * const listView)
    {
        QVariantList indexPath;
        bool ret = convertPathToIndex(arguments, listView, indexPath);
        if (ret)
        {
            const QVariant element = listView->dataModel()->data(indexPath);
            ret = not element.isNull() and element.isValid();
            if (ret)
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
                    ret = false;  // yep, false. Stops OK getting reported twice.
                }
                else
                {
                    this->client->write("ERROR: Can only show keys for QVariantMaps\r\n");
                    ret = false;
                }
            }
            else
            {
                this->client->write("ERROR: Tried to key to invalid item\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: failed to convert path to to index\r\n");
        }
        return ret;
    }

    bool ListCommand::selectUnselectPath(
            QStringList * const arguments,
            ListView * const listView,
            const bool select)
    {
        QVariantList indexPath;
        bool ret = convertPathToIndex(arguments, listView, indexPath);
        if (ret)
        {
            QVariant element = listView->dataModel()->data(indexPath);
            ret = not element.isNull() and element.isValid();
            if (ret)
            {
                bb::cascades::Application::processEvents();
                listView->select(indexPath, select);
                bb::cascades::Application::processEvents();
            }
            else
            {
                this->client->write("ERROR: Tried to select invalid item\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: failed to convert path to to index\r\n");
        }
        return ret;
    }

    bool ListCommand::tapPath(
            QStringList * const arguments,
            bb::cascades::ListView * const listView)
    {
        QVariantList indexPath;
        bool ret = convertPathToIndex(arguments, listView, indexPath);
        if (ret)
        {
            QVariant element = listView->dataModel()->data(indexPath);
            if (not element.isNull() and element.isValid())
            {
                bb::cascades::Application::processEvents();
                ret = QMetaObject::invokeMethod(
                            listView,
                            "triggered",
                            Q_ARG(QVariantList, indexPath));
                bb::cascades::Application::processEvents();
                if (not ret)
                {
                    this->client->write("ERROR: Failed to run triggered on object\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Tried to tap invalid item\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: failed to convert path to to index\r\n");
        }
        return ret;
    }

    bool ListCommand::holdPath(
            QStringList * const arguments,
            bb::cascades::ListView * const listView,
            const bool select)
    {
        bool ret = false;
        if (select)
        {
            QVariantList indexPath;
            ret = convertPathToIndex(arguments, listView, indexPath);
            if (ret)
            {
                QVariant element = listView->dataModel()->data(indexPath);
                ret = not element.isNull() and element.isValid();
                if (ret)
                {
                    ret = select not_eq listView->multiSelectHandler()->isActive();
                    if (ret)
                    {
                        bb::cascades::Application::processEvents();
                        listView->select(indexPath, select);
                        bb::cascades::Application::processEvents();
                        listView->multiSelectHandler()->setActive(select);
                        bb::cascades::Application::processEvents();
                    }
                }
                else
                {
                    this->client->write("ERROR: Tried to hold invalid item\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: failed to convert path to to index\r\n");
            }
        }
        else
        {
            bb::cascades::Application::processEvents();
            listView->multiSelectHandler()->setActive(select);
            bb::cascades::Application::processEvents();
            ret = true;
        }
        return ret;
    }

    bool ListCommand::convertPathToIndex(
            QStringList * const arguments,
            bb::cascades::ListView * const listView,
            QVariantList &index)
    {
        bool ret = false;
        if (arguments->size() > 1)
        {
            bool indexPathOk = true;
            const QString selectType = arguments->first();
            arguments->removeFirst();
            if (selectType == "index")
            {
                if (not findElementByIndex(listView, arguments->first(), index))
                {
                    indexPathOk = false;
                }
                arguments->removeFirst();
            }
            else if (selectType == "name")
            {
                const QString namedIndex = extractNamedPath(arguments, namedPathEnd);
                if (not findElementByName(listView, namedIndex, index))
                {
                    indexPathOk = false;
                }
            }
            else
            {
                indexPathOk = false;
            }
            ret = indexPathOk;
        }
        return ret;
    }

    QString ListCommand::extractNamedPath(
            QStringList * const arguments,
            const QString& endOfPath)
    {
        QStringList path;
        while (not arguments->isEmpty())
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
        normalisePath(&tmp, endOfPath);
        return tmp.left(tmp.lastIndexOf(endOfPath));
    }

    void ListCommand::normalisePath(
            QString * const value,
            const QString& endOfPath)
    {
        if (value->endsWith(endOfPath))
        {
            value->chop(1);
        }
    }

    bool ListCommand::findElementByIndex(
            bb::cascades::ListView * const list,
            const QString& index,
            QVariantList& elementIndexPath) const
    {
        QStringList indexes = Utils::tokenise(namedPathSep, index, false);

        bool failed = false;
        Q_FOREACH(QString sIndex, indexes)
        {
            bool ok = false;
            const int iIndex = sIndex.toInt(&ok);
            if (ok)
            {
                 elementIndexPath.push_back(iIndex);
            }
            else if (sIndex == "last")
            {
                elementIndexPath.push_back(
                            list->dataModel()->childCount(elementIndexPath)-1);
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
        DataModel * const model = list->dataModel();
        QStringList indexes = Utils::tokenise(namedPathSep, index, false);

        bool failed = false;
        while (not indexes.isEmpty())
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
                    QStringList keyValuePair = elementName.split(assignSep);
                    if (keyValuePair.size() == 2)
                    {
                        const QString key = keyValuePair.first().trimmed();
                        keyValuePair.removeFirst();
                        const QString value = keyValuePair.first().trimmed();

                        if (not key.isNull() and not key.isEmpty()
                                and not value.isNull() and not value.isEmpty())
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
            if (not element.isNull() and element.isValid())
            {
                const QString elementType = QString(element.typeName());
                if (elementType == "QString")
                {
                    ret = (check == element.toString());
                    if (not ret)
                    {
                        QString data("ERROR: Value is {");
                        data += element.toString();
                        data += "} expected {";
                        data += check;
                        data += "}\r\n";
                        this->client->write(data.trimmed().toUtf8());
                    }
                }
                else if (elementType == "QVariantMap")
                {
                    QString normalisedCheck(check);
                    normalisePath(&normalisedCheck, namedPathEnd);
                    QStringList keyValuePair = normalisedCheck.split(assignSep);
                    if (keyValuePair.size() == 2)
                    {
                        const QString key = keyValuePair.first().trimmed();
                        keyValuePair.removeFirst();
                        const QString value = keyValuePair.first().trimmed();

                        if (not key.isNull() and not key.isEmpty()
                                and not value.isNull() and not value.isEmpty())
                        {
                            const QVariantMap elementMap(element.toMap());
                            const QString actual = elementMap[key].toString();
                            ret = (actual == value);
                            if (not ret)
                            {
                                QString data("ERROR: Value is {");
                                data += actual;
                                data += "} expected {";
                                data += value;
                                data += "}\r\n";
                                this->client->write(data.trimmed().toUtf8());
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
        this->client->write("> list <list> index <index> <expected value> - " \
                            "check string values\r\n");
        this->client->write("e.g. list someList index 0~1~2 /etc/files/file\r\n");
        this->client->write("> list <list> index <index> <key>=<expected value>" \
                            " - check QVarientMap values\r\n");
        this->client->write("e.g. list someList 0~1~2 filename=/etc/files/file\r\n");
        this->client->write("> list <list> name <name> <expected value> - check string values\r\n");
        this->client->write("e.g. list someList /~etc~files^ /etc/files/file\r\n");
        this->client->write("> list <list> name <name> <key>=<expected value>"
                            " - check QVarientMap values\r\n");
        this->client->write("e.g. list someList /~etc~files^ filename=/etc/files/file\r\n");
        this->client->write("> list <list> select index <index> - select an index\r\n");
        this->client->write("> list <list> select name <name> - select a named index\r\n");
        this->client->write("> list <list> unselect index <index> - unselect an index\r\n");
        this->client->write("> list <list> unselect name <name> - unselect a named index\r\n");
        this->client->write("> list <list> scroll index <index> - scroll to an index\r\n");
        this->client->write("> list <list> scroll name <name> - scroll to a named index\r\n");
        this->client->write("> list <list> key index <index> - list the keys in Map for index\r\n");
        this->client->write("> list <list> key name <name> - list the keys in Map " \
                            "for named index\r\n");
        this->client->write("> list <list> hold index <index> - open multiselect\r\n");
        this->client->write("> list <list> hold name <name> - open multiselect\r\n");
        this->client->write("> list <list> release - close multiselect\r\n");
        this->client->write("> list <list> release - close multiselect\r\n");
        this->client->write("> list <list> tap index <index> - tap and open a list item\r\n");
        this->client->write("> list <list> tap name <index> - tap and open a list item\r\n");
        this->client->write("e.g. list contacts scroll name J~Name=John Smith^\r\n");
        this->client->write("e.g. list contacts select index 0~1\r\n");
        this->client->write("> list <list> clear\r\n");
        this->client->write(">\r\n");
        this->client->write("> <index> should be numerical and separated by ~ (i.e. 0~1~2)\r\n");
        this->client->write("> <index> can also support \"last\" i.e. 0~1~last\r\n");
        this->client->write("> <name> should be text and separated by ~ and terminated by ^\r\n");
        this->client->write("\t level 1~level 2~level 3^");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
