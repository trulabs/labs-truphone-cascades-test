/**
 * Copyright 2013 Truphone
 */
#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Menu>
#include <bb/cascades/TabbedPane>

#include "ActionCommand.h"
#include "Connection.h"

using bb::cascades::AbstractPane;
using bb::cascades::AbstractActionItem;
using bb::cascades::Application;
using bb::cascades::NavigationPane;
using bb::cascades::Menu;
using bb::cascades::Page;
using bb::cascades::TabbedPane;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString ActionCommand::CMD_NAME = "action";

    ActionCommand::ActionCommand(Connection * const socket,
                                 QObject * parent)
        : Command(parent),
          client(socket)
    {
    }

    ActionCommand::~ActionCommand()
    {
    }

    bool ActionCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;

        if (arguments->size() >= 1)
        {
            const QString name = arguments->first();
            arguments->removeFirst();
            if (name.isEmpty())
            {
                this->client->write("ERROR: The name of the action is empty\r\n");
            }
            else
            {
                const AbstractPane * const pane = Application::instance()->scene();
                if (name == "menu")
                {
                    if (arguments->size() == 1)
                    {
                        const Menu * const menu = Application::instance()->menu();
                        const QString settingsOrIndex = arguments->first();
                        arguments->removeFirst();

                        if (settingsOrIndex == "settings")
                        {
                            if (menu->settingsAction())
                            {
                                ret = executeAction(reinterpret_cast<AbstractActionItem*>(
                                                        menu->settingsAction()));
                                if (not ret)
                                {
                                    this->client->write("ERROR: Failed to execute the menu " \
                                                        "settings action\r\n");
                                }
                            }
                            else
                            {
                                this->client->write("ERROR: There is no settings action " \
                                                    "in the menu\r\n");
                            }
                        }
                        else
                        {
                            bool ok = false;
                            const int index = settingsOrIndex.toInt(&ok);
                            if (ok)
                            {
                                if (menu->actionAt(index))
                                {
                                    ret = executeAction(reinterpret_cast<AbstractActionItem*>(
                                                            menu->actionAt(index)));
                                    if (not ret)
                                    {
                                        this->client->write("ERROR: Failed to execute the " \
                                                            "indexed menu action\r\n");
                                    }
                                }
                                else
                                {
                                    this->client->write("ERROR: That index is an invalid " \
                                                        "action\r\n");
                                }
                            }
                            else
                            {
                                this->client->write("ERROR: Menu index couldn't be converted"\
                                                    " to an integer\r\n");
                            }
                        }
                    }
                    else
                    {
                        this->client->write("ERROR: action menu <settings|index>\r\n");
                    }
                }
                else if (name == "page")
                {
                    if (arguments->size() == 1)
                    {
                        const Page * const currentPage = findCurrentPage(pane);
                        if (currentPage)
                        {
                            bool ok = false;
                            const QString indexString = arguments->first();
                            arguments->removeFirst();
                            const int index = indexString.toInt(&ok);
                            if (ok)
                            {
                                if (currentPage->actionAt(index))
                                {
                                    ret = executeAction(currentPage->actionAt(index));
                                    if (not ret)
                                    {
                                        this->client->write("ERROR: Failed to execute the " \
                                                            "indexed page action\r\n");
                                    }
                                }
                                else
                                {
                                    this->client->write("ERROR: Failed to find an action " \
                                                        "at that index\r\n");
                                }
                            }
                            else
                            {
                                this->client->write("ERROR: Page index couldn't be converted"\
                                                    "to an integer\r\n");
                            }
                        }
                        else
                        {
                            this->client->write("ERROR: Can't find the current page to "\
                                                "run the action\r\n");
                        }
                    }
                    else
                    {
                        this->client->write("ERROR: action page <index>\r\n");
                    }
                }
                else if (pane)
                {
                    AbstractActionItem * action = pane->findChild<AbstractActionItem*>(name);
                    if (action)
                    {
                        ret = executeAction(action);
                    }
                    else
                    {
                        action = Application::instance()->findChild<AbstractActionItem*>(name);
                        if (action)
                        {
                            ret = executeAction(action);
                        }
                        else
                        {
                            action = findAction(pane, name);
                            if (action)
                            {
                                ret = executeAction(action);
                                if (not ret)
                                {
                                    this->client->write("ERROR: Failed to execute the"\
                                                        " named action\r\n");
                                }
                            }
                            else
                            {
                                this->client->write("ERROR: Unable to find the named action\r\n");
                            }
                        }
                    }
                }
                else
                {
                    this->client->write("ERROR: Unable to obtain the root pane\r\n");
                }
            }
        }
        else
        {
            this->client->write("ERROR: You need to specify which action to run\r\n");
        }

        return ret;
    }

    bool ActionCommand::executeAction(AbstractActionItem * const action)
    {
        bool ret = false;
        if (action)
        {
            ret = QMetaObject::invokeMethod(action, "triggered");
        }
        return ret;
    }

    AbstractActionItem * ActionCommand::findAction(
            const QObject * const obj,
            const QString& name,
            const size_t call_depth,
            const size_t max_call_depth)
    {
        AbstractActionItem *ret = NULL, *tmp = NULL;

        // check the call depth to avoid stack thrash
        if (call_depth < max_call_depth)
        {
            tmp = qobject_cast<AbstractActionItem*>(const_cast<QObject*>(obj));
            if (tmp)
            {
                // check this object
                const QString title = tmp->title();
                if (not title.isEmpty())
                {
                    if (title == name)
                    {
                        ret = tmp;
                    }
                }
            }
            // isn't this object, check the children
            if (not ret)
            {
                Q_FOREACH(const QObject * const child, obj->children())
                {
                    ret = findAction(child, name, call_depth+1, max_call_depth);
                    if (ret)
                    {
                        break;
                    }
                }
            }
        }

        return ret;
    }

    Page * ActionCommand::findCurrentPage(
            const AbstractPane * const pane,
            const size_t callLevel,
            const size_t maxCallLevel)
    {
        Page * ret = NULL;

        if (callLevel < maxCallLevel)
        {
            // the root scene has to be either be a tabbed pane, navigation pane
            // or a page. a page can't pop itself so we must be dealing with either
            // a navigation pane OR a tabbed pane
            const NavigationPane * const navPane = qobject_cast<const NavigationPane*>(pane);
            if (navPane)
            {
                ret = navPane->top();
            }
            else
            {
                const TabbedPane * const tabbedPane = qobject_cast<const TabbedPane*>(pane);
                if (tabbedPane)
                {
                    // we don't know what the tabbed pane is looked at so we need
                    // to recurse down and find out
                    ret = findCurrentPage(tabbedPane->activePane(),
                                          callLevel + 1,
                                          maxCallLevel);
                }
            }
        }

        return ret;
    }

    void ActionCommand::showHelp()
    {
        this->client->write("> action <objectName/title>\r\n");
        this->client->write("> action menu <index>\r\n");
        this->client->write("> action page <index>\r\n");
        this->client->write("Execute an Action Item\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
