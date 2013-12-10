/**
 * Copyright 2013 Truphone
 */
#include "ListCommand.h"

#include <QString>
#include <QList>
#include <QObject>

#include <bb/cascades/ListView>

#include "Connection.h"

using bb::cascades::ListView;

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
        if (arguments->size() > 1)
        {
            const QString listViewName = arguments->first();
            arguments->removeFirst();
            ListView * const listView = scenePane->findChild<ListView*>(listViewName);
            if (listView)
            {
                const QString command = arguments->first();
                arguments->removeFirst();
                if (command == "count")
                {
                    const int s = listView->dataModel()->childCount(listView->rootIndexPath());
                    this->client->write(QString::number(s).toUtf8().constData());
                    this->client->write("\r\n");
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
        return false;
    }

    void ListCommand::showHelp()
    {
        this->client->write("> list....?\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
