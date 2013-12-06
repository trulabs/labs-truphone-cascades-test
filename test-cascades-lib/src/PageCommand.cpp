/**
 * Copyright 2013 Truphone
 */
#include "include/PageCommand.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/PaneProperties>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/TitleBar>
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
using bb::cascades::TitleBar;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString truphone::test::cascades::PageCommand::CMD_NAME = "page";

    truphone::test::cascades::PageCommand::PageCommand(Connection * const socket,
                                                     QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    truphone::test::cascades::PageCommand::~PageCommand()
    {
    }

    bool PageCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->size() == 1)
        {
            Page * const page = findCurrentPage(Application::instance()->scene());
            if (page)
            {
                const QString expectedPage = arguments->first();
                if (page->objectName() == expectedPage)
                {
                    ret = true;
                }
                else
                {
                    this->client->write("ERROR: Page is named {");
                    this->client->write(page->objectName().toUtf8().constData());
                    this->client->write("} which wasn't expected {");
                    this->client->write(expectedPage.toUtf8().constData());
                    this->client->write("}\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Couldn't find a page\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: No parameters for page\r\n");
        }
        return ret;
    }

    Page * PageCommand::findCurrentPage(
            AbstractPane * const pane,
            const size_t callLevel,
            const size_t maxCallLevel)
    {
        Page * page = NULL;

        if (callLevel < maxCallLevel)
        {
            // the root scene has to be either be a tabbed pane, navigation pane
            // or a page. a page can't pop itself so we must be dealing with either
            // a navigation pane OR a tabbed pane
            NavigationPane * const navPane = qobject_cast<NavigationPane*>(pane);
            if (navPane)
            {
                if (navPane->count() > 0)
                {
                    page = navPane->top();
                }
            }
            else
            {
                TabbedPane * const tabbedPane = qobject_cast<TabbedPane*>(pane);
                if (tabbedPane)
                {
                    // we don't know what the tabbed pane is looked at so we need
                    // to recurse down and find out
                    page = findCurrentPage(tabbedPane->activePane(),
                                           callLevel + 1,
                                           maxCallLevel);
                }
                else
                {
                    this->client->write("ERROR: Application root must be a " \
                                        "Nav Pane or a Tabbed Pane\r\n");
                }
            }
        }
        else
        {
            this->client->write("ERROR: The depth of Panes is too deep!\r\n");
        }

        return page;
    }

    void PageCommand::showHelp()
    {
        this->client->write("> page <name>\r\n");
        this->client->write("Ok if the current page objectName is right\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
