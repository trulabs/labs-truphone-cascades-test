/**
 * Copyright 2013 Truphone
 */
#include "include/RecordCommand.h"

#include <string.h>
#include <stdio.h>
#include <QString>
#include <bb/cascades/AbstractActionItem>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/AbstractToggleButton>
#include <bb/cascades/ActionItem>
#include <bb/cascades/Control>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Menu>
#include <bb/cascades/Page>
#include <bb/cascades/SettingsActionItem>
#include <bb/cascades/Tab>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/TapEvent>
#include <bb/cascades/UIObject>
#include <bb/cascades/VisualNode>

#include "RecordCommandTouchHandler.h"
#include "RecordCommandKeyHandler.h"
#include "RecordCommandActionHandler.h"
#include "RecordCommandToggleHandler.h"
#include "RecordCommandDropDownHandler.h"
#include "Buffer.h"
#include "Utils.h"
#include "Connection.h"

using bb::cascades::AbstractPane;
using bb::cascades::AbstractActionItem;
using bb::cascades::AbstractToggleButton;
using bb::cascades::ActionItem;
using bb::cascades::Application;
using bb::cascades::BaseObject;
using bb::cascades::Control;
using bb::cascades::DropDown;
using bb::cascades::NavigationPane;
using bb::cascades::Menu;
using bb::cascades::Option;
using bb::cascades::Page;
using bb::cascades::SettingsActionItem;
using bb::cascades::Tab;
using bb::cascades::TabbedPane;
using bb::cascades::VisualNode;
using bb::system::SystemToast;
using truphone::test::cascades::Utils;
using truphone::test::cascades::Buffer;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString RecordCommand::CMD_NAME = "record";

    RecordCommand * RecordCommand::instance = NULL;

    #define SLEEP_GRANULARITY 5

    RecordCommand::RecordCommand(Connection * const socket,
                                 QObject* parent)
        : Command(parent),
          client(socket),
          ignoreEvents(false),
          ctrlAndShiftPressed(false)
    {
        this->ignoreEvents = true;
        Application::instance()->installEventFilter(this);
        addListenersToUiObjects(Application::instance(), true);
        this->ignoreEvents = false;
    }

    RecordCommand::~RecordCommand()
    {
        this->ignoreEvents = true;
        Application::instance()->removeEventFilter(this);
        foreach(RecordCommandKeyHandler * const listener, this->keyListeners)
        {
            if (listener)
            {
                delete listener;
            }
        }
        this->keyListeners.clear();
        foreach(RecordCommandTouchHandler * const listener, this->touchListeners)
        {
            if (listener)
            {
                delete listener;
            }
        }
        this->touchListeners.clear();
        foreach(RecordCommandActionHandler * const listener, this->actionListeners)
        {
            if (listener)
            {
                delete listener;
            }
        }
        this->actionListeners.clear();
        foreach(RecordCommandToggleHandler * const listener, this->toggleListeners)
        {
            if (listener)
            {
                delete listener;
            }
        }
        this->toggleListeners.clear();
        foreach(RecordCommandDropDownHandler * const listener, this->dropDownListeners)
        {
            if (listener)
            {
                delete listener;
            }
        }
        this->dropDownListeners.clear();
    }

    bool RecordCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (not arguments->isEmpty())
        {
            this->client->write("ERROR: record doesn't take arguments\r\n");
        }
        else
        {
            this->lastEventTime.restart();
            ret = true;
        }
        return ret;
    }

    // cppcheck-suppress unusedFunction
    bool RecordCommand::eventFilter(QObject * const q, QEvent * const e)
    {
        if (not this->ignoreEvents)
        {
            const BaseObject * const bo = qobject_cast<BaseObject*>(q);

            switch (e->type())
            {
                case QEvent::ChildAdded:
                    if (bo)
                    {
                        this->ignoreEvents = true;
                        addListenersToUiObjects(q, true);
                        this->ignoreEvents = false;
                    }
                    break;
                case QEvent::ChildRemoved:
                    {
                        if (this->keyListeners.contains(q))
                        {
                            if (this->keyListeners[q])
                            {
                                delete this->keyListeners[q];
                            }
                            this->keyListeners.remove(q);
                        }
                        if (this->touchListeners.contains(q))
                        {
                            if (this->touchListeners[q])
                            {
                                delete this->touchListeners[q];
                            }
                            this->touchListeners.remove(q);
                        }
                        if (this->actionListeners.contains(q))
                        {
                            if (this->actionListeners[q])
                            {
                                delete this->actionListeners[q];
                            }
                            this->actionListeners.remove(q);
                        }
                        if (this->toggleListeners.contains(q))
                        {
                            if (this->toggleListeners[q])
                            {
                                delete this->toggleListeners[q];
                            }
                            this->toggleListeners.remove(q);
                        }
                        if (this->dropDownListeners.contains(q))
                        {
                            if (this->dropDownListeners[q])
                            {
                                delete this->dropDownListeners[q];
                            }
                            this->dropDownListeners.remove(q);
                        }
                        const SystemToast * const toast = qobject_cast<const SystemToast*>(bo);
                        if (toast)
                        {
                            if (this->connectedToasts.contains(toast))
                            {
                                this->connectedToasts.remove(toast);
                            }
                        }
                        const AbstractPane * const pane = qobject_cast<const AbstractPane*>(bo);
                        if (pane)
                        {
                            if (this->connectedPanes.contains(pane))
                            {
                                this->connectedPanes.remove(pane);
                            }
                        }
                    }
                    break;
                default:
                    /* don't care */
                    break;
            }
        }
        return false;
    }

    void RecordCommand::addListenersToUiObjects(
            QObject * const obj,
            const bool childrenToo,
            const size_t callLevel,
            const size_t maxCallLevel)
    {
        if (callLevel < maxCallLevel)
        {
            VisualNode * const vs = qobject_cast<VisualNode*>(obj);
            if (vs)
            {
                if (not this->touchListeners.contains(obj))
                {
                    this->touchListeners.insert(obj, new RecordCommandTouchHandler(this,
                                                                                   vs,
                                                                                   this));
                }
            }

            Control * const ctrl = qobject_cast<Control*>(obj);
            if (ctrl)
            {
                if (not this->keyListeners.contains(ctrl))
                {
                    this->keyListeners.insert(obj, new RecordCommandKeyHandler(this,
                                                                               ctrl,
                                                                               this));
                }
            }
            else
            {
                AbstractPane * const pane = qobject_cast<AbstractPane*>(obj);
                if (pane)
                {
                    if (not this->keyListeners.contains(pane))
                    {
                        this->keyListeners.insert(pane, new RecordCommandKeyHandler(this,
                                                                                    pane,
                                                                                    this));
                    }
                }
            }

            TabbedPane * const tabbedPane = qobject_cast<TabbedPane*>(obj);
            if (tabbedPane)
            {
                if (not this->connectedPanes.contains(tabbedPane))
                {
                    connect(tabbedPane,
                            SIGNAL(activeTabChanged(bb::cascades::Tab*)),
                            SLOT(tabChanged(bb::cascades::Tab*)));
                    this->connectedPanes.insert(tabbedPane, QBool(true));
                }
            }

            NavigationPane * const navPane = qobject_cast<NavigationPane*>(obj);
            if (navPane)
            {
                if (not this->connectedPanes.contains(navPane))
                {
                    connect(navPane,
                            SIGNAL(popTransitionEnded(bb::cascades::Page*)),
                            SLOT(onPopFinished(bb::cascades::Page*)));
                    this->connectedPanes.insert(navPane, QBool(true));
                }
            }

            AbstractActionItem * const actionItem = qobject_cast<AbstractActionItem*>(obj);
            // don't duplicate listening on tabs
            if (actionItem and not qobject_cast<Tab*>(obj))
            {
                if (not this->actionListeners.contains(obj))
                {
                    this->actionListeners.insert(
                                obj,
                                new RecordCommandActionHandler(this,
                                                               actionItem,
                                                               this));
                }
            }

            SystemToast * const toast = qobject_cast<SystemToast*>(obj);
            if (toast)
            {
                if (not this->connectedToasts.contains(toast))
                {
                    this->connectedToasts.insert(toast, QBool(true));
                    connect(toast,
                            SIGNAL(bodyChanged(QString)),
                            SLOT(toastStarted(QString)));
                    connect(toast,
                            SIGNAL(finished(bb::system::SystemUiResult::Type)),
                            SLOT(toastEnded(bb::system::SystemUiResult::Type)));
                }
            }

            AbstractToggleButton * const toggleButton = qobject_cast<AbstractToggleButton*>(obj);
            if (toggleButton)
            {
                if (not this->toggleListeners.contains(obj))
                {
                    this->toggleListeners.insert(
                                obj,
                                new RecordCommandToggleHandler(this,
                                                               toggleButton,
                                                               this));
                }
            }

            DropDown * const dropDown = qobject_cast<DropDown*>(obj);
            if (dropDown)
            {
                if (not this->dropDownListeners.contains(obj))
                {
                    this->dropDownListeners.insert(
                                obj,
                                new RecordCommandDropDownHandler(this,
                                                                 dropDown,
                                                                 this));
                }
            }

            // finally, do all the children
            if (childrenToo)
            {
                foreach(QObject * const child, obj->children())
                {
                    addListenersToUiObjects(child, childrenToo, callLevel + 1, maxCallLevel);
                }
            }
        }
    }

    int RecordCommand::updateSleepValue()
    {
        const Buffer buffer;
        const int msSinceLastTx = this->lastEventTime.restart();

        // check to see how far in the future we can
        // to see if we need to dump out some sleep statements
        if ( msSinceLastTx > SLEEP_GRANULARITY)
        {
            snprintf(buffer.data(), buffer.length(), "sleep %d\r\n", msSinceLastTx);
            this->client->write(buffer.data());
            this->client->flush();
            buffer.data()[0] = '\0';
        }
        return msSinceLastTx;
    }

    void RecordCommand::tabChanged(bb::cascades::Tab* tab)
    {
        TabbedPane * const pane =
            qobject_cast<TabbedPane*>(tab->parent());
        if (pane)
        {
            const Buffer tmp;

            updateSleepValue();

            const QString title = tab->title();
            if (not title.isNull() and not title.isEmpty())
            {
                snprintf(tmp.data(), tmp.length(), "tab %s\r\n", title.toUtf8().constData());
            }
            else
            {
                const int indexOfTab = pane->indexOf(tab);
                snprintf(tmp.data(), tmp.length(), "tab %d\r\n", indexOfTab);
            }
            this->client->write(tmp.data());
        }
    }

    void RecordCommand::touched(
            const QObject * const receiver,
            const bb::cascades::TouchEvent * const event)
    {
        const int msSinceLastTx = updateSleepValue();

        const Buffer buffer;

        if ( not (msSinceLastTx < SLEEP_GRANULARITY and
             this->lastReceiver == receiver and
             this->lastTarget == event->target() and
             this->lastTouchType == event->touchType()))
        {
            snprintf(buffer.data(), buffer.length(),
                     "touch %f %f %f %f %f %f %d %s %s\r\n",
                     event->screenX(),
                     event->screenY(),
                     event->windowX(),
                     event->windowY(),
                     event->localX(),
                     event->localY(),
                     event->touchType(),
                     Utils::objectPath(receiver).toUtf8().constData(),
                     Utils::objectPath(event->target()).toUtf8().constData());
            this->client->write(buffer.data());
        }
        if (this->ctrlAndShiftPressed and event->target() == receiver)
        {
            this->testObjectProperties(receiver);
        }

        // update the timespace of the last operation
        this->lastReceiver = receiver;
        this->lastTarget = event->target();
        this->lastTouchType = event->touchType();
    }

    void RecordCommand::keyed(
            const QObject * const receiver,
            const bb::cascades::KeyEvent * const event)
    {
        if (event->isCtrlPressed()
            and event->isShiftPressed())
        {
            this->ctrlAndShiftPressed = true;
        }
        else
        {
            const Buffer buffer;
            updateSleepValue();

            this->ctrlAndShiftPressed = false;

            snprintf(buffer.data(), buffer.length(), "key %d %d %d %d %d %s\r\n",
                     event->key(),
                     event->isPressed()?1:0,
                     event->isAltPressed()?1:0,
                     event->isShiftPressed()?1:0,
                     event->isCtrlPressed()?1:0,
                     Utils::objectPath(receiver).toUtf8().constData());
            this->client->write(buffer.data());
        }
    }

    void RecordCommand::onPopFinished(bb::cascades::Page* page)
    {
        Q_UNUSED(page);
        this->updateSleepValue();
        this->client->write("pop\r\n");
    }

    void RecordCommand::toastStarted(const QString& value)
    {
        this->updateSleepValue();
        this->client->write("toast ");
        if (value.isNull() or value.isEmpty())
        {
            this->client->write("false");
        }
        else
        {
            this->client->write(value.toUtf8().constData());
        }
        this->client->write("\r\n");
    }

    void RecordCommand::toastEnded(bb::system::SystemUiResult::Type result)
    {
        Q_UNUSED(result);
        this->updateSleepValue();
        this->client->write("toast false\r\n");
    }

    void RecordCommand::toggled(const bb::cascades::AbstractToggleButton * const button,
                                const bool newState)
    {
        this->updateSleepValue();
        this->client->write("toggle ");
        if (not button->objectName().isNull() and not button->objectName().isEmpty())
        {
            this->client->write(button->objectName().toUtf8().constData());
        }
        else
        {
            this->client->write(Utils::objectPath(button).toUtf8().constData());
        }
        if (newState)
        {
            this->client->write(" true\r\n");
        }
        else
        {
            this->client->write(" false\r\n");
        }
    }

    void RecordCommand::dropDownChanged(
                    const DropDown * const dropDown,
                    const Option* const option)
    {
        this->updateSleepValue();
        this->client->write("dropdown ");
        if (not dropDown->objectName().isNull() and not dropDown->objectName().isEmpty())
        {
            this->client->write(dropDown->objectName().toUtf8().constData());
        }
        else
        {
            this->client->write(Utils::objectPath(dropDown).toUtf8().constData());
        }
        this->client->write(" ");
        this->client->write(option->text().toUtf8().constData());
        this->client->write("\r\n");
    }

    void RecordCommand::testObjectProperties(const QObject * const obj)
    {
        QString objName = obj->objectName();
        if (objName.isNull() or objName.isEmpty())
        {
            objName = Utils::objectPath(obj);
        }
        const QMetaObject * const meta = obj->metaObject();
        if (meta)
        {
            const int properties = meta->propertyCount();
            for (int i = 0 ; i < properties ; i++)
            {
                const QMetaProperty metaProperty = meta->property(i);
                const char * const propertyName = metaProperty.name();
                if (propertyName
                        and metaProperty.isStored()
                        and metaProperty.isValid()
                        and metaProperty.isReadable())
                {
                    const size_t propertyNameLength = strlen(propertyName);
                    // limit the tests we record
                    if (strncmp(propertyName, "text", propertyNameLength) == 0
                     or strncmp(propertyName, "visible", propertyNameLength) == 0)
                    {
                        const QVariant var = obj->property(propertyName);
                        const QString varString = var.toString();
                        if (var.isValid() and not varString.isNull() and not varString.isEmpty())
                        {
                                this->client->write("test ");
                                this->client->write(objName.toUtf8().constData());
                                this->client->write(" ");
                                this->client->write(propertyName);
                                this->client->write(" ");
                                this->client->write(varString.toUtf8().constData());
                                this->client->write("\r\n");
                        }
                    }
                }
            }
        }
    }

    void RecordCommand::actionExecuted(AbstractActionItem * action)
    {
        updateSleepValue();
        if (action->title().isEmpty())
        {
            // work out the index and the parent
            QObject * const parent = action->parent();
            if (parent)
            {
                const Menu * const menu = qobject_cast<Menu*>(parent);
                if (menu)
                {
                    if (menu->settingsAction() == qobject_cast<SettingsActionItem*>(action))
                    {
                        this->client->write("action menu settings\r\n");
                    }
                    else
                    {
                        const int actions = menu->actionCount();
                        for (int i = 0 ; i < actions ; i++)
                        {
                            if (menu->actionAt(i) == qobject_cast<ActionItem*>(action))
                            {
                                const Buffer tmp;

                                snprintf(tmp.data(), tmp.length(), "%d", i);
                                this->client->write("action menu ");
                                this->client->write(tmp.cdata());
                                this->client->write("\r\n");
                                break;
                            }
                        }
                    }
                }
                else
                {
                    const Page * const page = qobject_cast<Page*>(parent);
                    if (page)
                    {
                        const int actions = page->actionCount();
                        for (int i = 0 ; i < actions ; i++)
                        {
                            if (page->actionAt(i) == action)
                            {
                                const Buffer tmp;

                                snprintf(tmp.data(), tmp.length(), "%d", i);
                                this->client->write("action page ");
                                this->client->write(tmp.cdata());
                                this->client->write("\r\n");
                                break;
                            }
                        }
                    }
                    else
                    {
                        this->client->write("ERROR: ");
                        this->client->write(parent->metaObject()->className());
                        this->client->write(" is not a supported action parent\r\n");
                    }
                }
            }
        }
        else
        {
            this->client->write("action ");
            this->client->write(action->title().toUtf8().constData());
            this->client->write("\r\n");
        }
    }

    void RecordCommand::showHelp()
    {
        this->client->write("> record\r\n");
        this->client->write("Record events - to stop listening you will have to " \
                            "terminate the connection\r\n");
        this->client->write("- it's really for debugging rather than for use in scripts\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
