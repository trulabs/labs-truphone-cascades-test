/**
 * Copyright 2013 Truphone
 */
#include "TouchCommand.h"

#include <bb/cascades/AbstractTextControl>
#include <bb/cascades/Application>
#include <bb/cascades/Control>
#include <bb/cascades/TouchEvent>
#include <bb/cascades/TouchType>
#include <bb/cascades/VisualNode>

#include "Utils.h"
#include "TouchCommandLPHandler.h"
#include "Connection.h"

using bb::cascades::AbstractTextControl;
using bb::cascades::Application;
using bb::cascades::Control;
using bb::cascades::TouchEvent;
using bb::cascades::TouchType;
using bb::cascades::VisualNode;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString TouchCommand::CMD_NAME = "touch";

    QHash<QObject*, TouchCommandLPHandler*> TouchCommand::longPressHandlers;

    TouchCommand::TouchCommand(Connection * const socket,
                           QObject * parent)
        : Command(parent),
          client(socket)
    {
    }

    TouchCommand::~TouchCommand()
    {
    }

    bool TouchCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;

        float sx = NAN, sy = NAN, wx = NAN, wy = NAN, lx = NAN, ly = NAN;
        int touchType = -1, processed;
        QString targetBuffer;
        QString receiverBuffer;

        for (int param = 0 ; param < 8 ; param++)
        {
            bool ok = false;
            switch(param)
            {
            case 0:
                sx = QString(arguments->first()).toInt(&ok);
                break;
            case 1:
                sy = QString(arguments->first()).toInt(&ok);
                break;
            case 2:
                wx = QString(arguments->first()).toInt(&ok);
                break;
            case 3:
                wy = QString(arguments->first()).toInt(&ok);
                break;
            case 4:
                lx = QString(arguments->first()).toInt(&ok);
                break;
            case 5:
                ly = QString(arguments->first()).toInt(&ok);
                break;
            case 6:
                receiverBuffer = arguments->first();
                ok = not receiverBuffer.isNull() and not receiverBuffer.isEmpty();
                break;
            case 7:
                targetBuffer = arguments->first();
                ok = not targetBuffer.isNull() and not targetBuffer.isEmpty();
                break;
            default:
                break;
            }
            arguments->removeFirst();
            if (ok)
            {
                processed++;
            }
        }

        if (processed == 8)
        {
            if (targetBuffer.isNull() || targetBuffer.isEmpty())
            {
                // there's no receiver and the receiver has been replaced
                // with the target so we need to watch them over
                receiverBuffer = targetBuffer;
                processed++;
            }
        }
        if (processed not_eq 9)  // we're testing for 9 elements
        {
            this->client->write("ERROR: Play touch command failed "\
                                "because not all arguments were parsed\r\n");
        }
        else
        {
            QObject * const targetObject = Utils::findObject(targetBuffer);
            if (not targetObject)
            {
                this->client->write("ERROR: Could not find the object pointed " \
                                    "to by the play touch command\r\n");
            }
            else
            {
                QObject * const receiverObject = Utils::findObject(receiverBuffer);
                if (not receiverObject)
                {
                    this->client->write("ERROR: Could not find the object that " \
                                        "receives the play touch event\r\n");
                }
                else
                {
                    VisualNode * const visualObject = qobject_cast<VisualNode*>(targetObject);
                    if (not visualObject)
                    {
                        this->client->write("ERROR: The object target of the play "\
                                            "touch command isn't a Visual Object\r\n");
                    }
                    else
                    {
                        Control * const ctrl = qobject_cast<Control*>(visualObject);
                        if (ctrl and not ctrl->isFocused())
                        {
                            ctrl->requestFocus();
                        }

                        TouchEvent * const te = new TouchEvent(
                                    TouchType::Type(touchType),
                                    sx,
                                    sy,
                                    wx,
                                    wy,
                                    lx,
                                    ly,
                                    visualObject);
                        // the receiver object isn't needed here but really we should
                        // be using it to correctly propogate the event from
                        // the parent (receiver) to the target
                        //
                        // that doesn't seem to work, possibly because the event's
                        // PropogationPhase (which is private data) seems to have
                        // been initialised incorrectly (I get entirely random
                        // numbers when querying it)
                        bool fired = false;
                        bool isLong = false;

                        // check if there was an existing touch occurin'
                        // DON'T cancel a long press if we're moving the cursor
                        if (te->touchType() not_eq TouchType::Move and
                                longPressHandlers[visualObject])
                        {
                            isLong = longPressHandlers[visualObject]->hasExpired();
                            longPressHandlers[visualObject]->cancel();
                            longPressHandlers.remove(visualObject);
                            delete longPressHandlers[visualObject];
                        }
                        // start of a touch which may be long - start a timer
                        if (te->touchType() == TouchType::Down)
                        {
                            TouchCommandLPHandler * const longPressHandler =
                                    new TouchCommandLPHandler(visualObject,
                                                             Application::instance());
                            longPressHandlers[visualObject] = longPressHandler;
                        }
                        // are we releasing the press?
                        else if (te->touchType() == TouchType::Up)
                        {
                            if (isLong)
                            {
                                // cancel the touch down event
                                fired = QMetaObject::invokeMethod(
                                            receiverObject,
                                            "touch",
                                            Q_ARG(bb::cascades::TouchEvent*, new TouchEvent(
                                                      TouchType::Cancel,
                                                      sx, sy, wx, wy,
                                                      lx, ly, visualObject)));
                            }
                            else
                            {
                                // up & the long press was cancelled
                                fired = QMetaObject::invokeMethod(
                                        visualObject,
                                        "clicked");
                            }
                        }
                        if (not fired)
                        {
                            // normal event
                            fired = QMetaObject::invokeMethod(
                                        receiverObject,
                                        "touch",
                                        Q_ARG(bb::cascades::TouchEvent*, te));
                            if (not fired)
                            {
                                this->client->write("ERROR: Failed to call touch " \
                                                    "on receiver\r\n");
                            }
                        }
                        ret = fired;
                    }
                }
            }
        }
        return ret;
    }

    void TouchCommand::showHelp()
    {
        this->client->write("> touch <sx> <sy> <wx> <wy> <lx> <ly> <type> <receiver> <target>\r\n");
        this->client->write("<type> should be 0=Down,1=Move,2=Up,3=Cancel\r\n");
        this->client->write("<target> can be omitted if <receiver> is the target\r\n");
        this->client->write("Simulate a touch event on a UI object\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
