/**
 * Copyright 2013 Truphone
 */
#include "include/TabCommand.h"

#include <QString>
#include <QList>
#include <QObject>
#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>

#include "include/Connection.h"

using bb::cascades::Application;
using bb::cascades::AbstractPane;
using bb::cascades::Tab;
using bb::cascades::TabbedPane;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString TabCommand::CMD_NAME = "tab";

    TabCommand::TabCommand(Connection * const socket,
                           QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    TabCommand::~TabCommand()
    {
    }

    bool TabCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->size() >= 1)
        {
            const QString tabName = arguments->first();
            arguments->removeFirst();
            if ((not tabName.isNull()) and (not tabName.isEmpty()))
            {
                TabbedPane * const tabs = qobject_cast<TabbedPane*>(
                            bb::cascades::Application::instance()->scene());
                if (tabs)
                {
                    Tab * const tab = tabs->findChild<Tab*>(tabName);
                    if (tab)
                    {
                        selectTab(tabs, tab);
                        ret = true;
                    }
                    else
                    {
                        bool isNumber = false;
                        const int tabIndex = tabName.toInt(&isNumber);
                        if (isNumber and tabIndex < tabs->count())
                        {
                            selectTab(tabs, tabs->at(tabIndex));
                            ret = true;
                        }
                        else
                        {
                            for (int i = 0 ; i < tabs->count() and not ret ; i++)
                            {
                                Tab * aTab = tabs->at(i);
                                if (aTab)
                                {
                                    if (aTab->title() == tabName)
                                    {
                                        selectTab(tabs, aTab);
                                        ret = true;
                                    }
                                }
                            }
                            if (not ret)
                            {
                                this->client->write("ERROR: Failed to find the tab\r\n");
                            }
                        }
                    }
                }
                else
                {
                    this->client->write("ERROR: There is no tabbed pane in the scene\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Tab name/number is empty!\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: Not enough arguments, sleep <timeInMs>\r\n");
        }
        return ret;
    }

    void TabCommand::selectTab(TabbedPane * const pane, Tab * const tab)
    {
        pane->setActiveTab(tab);
    }

    void TabCommand::showHelp()
    {
        this->client->write("> tab <index/name>\r\n");
        this->client->write("Select a tab on a UI. You can use a zero-based index or " \
                            "if the tab is named by\r\n");
        this->client->write("title or objectName then you can use that too.\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
