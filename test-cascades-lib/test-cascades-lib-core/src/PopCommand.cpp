/**
 * Copyright 2013 Truphone
 */
#include "include/PopCommand.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/PaneProperties>
#include <bb/cascades/TabbedPane>
#include <QList>
#include <QString>
#include <QObject>

#include "Connection.h"

using bb::cascades::Application;
using bb::cascades::AbstractPane;
using bb::cascades::Page;
using bb::cascades::NavigationPane;
using bb::cascades::PaneProperties;
using bb::cascades::TabbedPane;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString truphone::test::cascades::PopCommand::CMD_NAME = "pop";

    truphone::test::cascades::PopCommand::PopCommand(Connection * const socket,
                                                     QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    truphone::test::cascades::PopCommand::~PopCommand()
    {
    }

    bool PopCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->empty())
        {
            ret = popFromAbstractPane(Application::instance()->scene());
        }
        else
        {
            this->client->write(tr("ERROR: No parameters for pop") + "\r\n");
        }
        return ret;
    }

    bool PopCommand::popFromAbstractPane(
            AbstractPane * const pane,
            const size_t callLevel,
            const size_t maxCallLevel)
    {
        bool ret = false;

        if (callLevel < maxCallLevel)
        {
            // the root scene has to be either be a tabbed pane, navigation pane
            // or a page. a page can't pop itself so we must be dealing with either
            // a navigation pane OR a tabbed pane
            NavigationPane * const navPane = qobject_cast<NavigationPane*>(pane);
            if (navPane)
            {
                const int stackSize = navPane->count();
                if (stackSize > 1)
                {
                    bb::cascades::Application::processEvents();
                    navPane->pop();
                    bb::cascades::Application::processEvents();
                    ret = true;
                }
                else
                {
                    this->client->write(tr("ERROR: You can't pop now, only 1 thing left") + "\r\n");
                }
            }
            else
            {
                TabbedPane * const tabbedPane = qobject_cast<TabbedPane*>(pane);
                if (tabbedPane)
                {
                    // we don't know what the tabbed pane is looked at so we need
                    // to recurse down and find out
                    ret = popFromAbstractPane(tabbedPane->activePane(),
                                              callLevel + 1,
                                              maxCallLevel);
                }
                else
                {
                    this->client->write(tr("ERROR: Application root must be a " \
                                        "Nav Pane or a Tabbed Pane") + "\r\n");
                }
            }
        }
        else
        {
            this->client->write(tr("ERROR: The depth of Panes is too deep") + "\r\n");
        }

        return ret;
    }

    void PopCommand::showHelp()
    {
        this->client->write(tr("> pop") + "\r\n");
        this->client->write(tr("Pop a page from the Navigation stack") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
