/**
 * Copyright 2013 Truphone
 */
#include "TapCommand.h"

#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/TouchEvent>
#include <bb/cascades/VisualNode>

#include "Utils.h"
#include "Connection.h"

using truphone::test::cascades::Utils;
using bb::cascades::Application;
using bb::cascades::VisualNode;
using bb::cascades::TouchEvent;
using bb::cascades::TouchType;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString TapCommand::CMD_NAME = "tap";

    TapCommand::TapCommand(Connection * const socket,
                           QObject * parent)
        : Command(parent),
          client(socket)
    {
    }

    TapCommand::~TapCommand()
    {
    }

    bool TapCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if ((not arguments->isEmpty()) and (arguments->size() >= 2))
        {
            const QString typeName = arguments->first();
            arguments->removeFirst();
            QObject * const obj = Utils::findObject(arguments->first());
            arguments->removeFirst();
            if (obj)
            {
                TouchType::Type type = TouchType::Move;
                if (typeName == "up")
                {
                    type = TouchType::Up;
                }
                else if (typeName == "down")
                {
                    type = TouchType::Down;
                }
                else if (typeName == "move")
                {
                    type = TouchType::Move;
                }
                else if (typeName == "cancel")
                {
                    type = TouchType::Cancel;
                }
                else
                {
                    QString data(typeName);
                    data += " is not a known event type for touch events, " \
                            "assuming 'move'\r\n";
                    this->client->write(data.trimmed().toUtf8());
                }
                VisualNode * const visual_node = qobject_cast<VisualNode*>(obj);
                if (visual_node)
                {
                    TouchEvent * const te = new TouchEvent(type,
                                                           0.0,
                                                           0.0,
                                                           0.0,
                                                           0.0,
                                                           0.0,
                                                           0.0,
                                                           visual_node);
                    bb::cascades::Application::processEvents();
                    const bool invoked = QMetaObject::invokeMethod(
                                obj,
                                "touch",
                                Q_ARG(bb::cascades::TouchEvent*, te));
                    if (invoked)
                    {
                        bb::cascades::Application::processEvents();
                        ret = true;
                    }
                    else
                    {
                        this->client->write("ERROR: Failed to invoke function\r\n");
                    }
                }
                else
                {
                    this->client->write("ERROR: The object in question isn't a visual element\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Unknown object\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: Not enough arguments, " \
                                "tap <up/down/move/cancel> <object>\r\n");
        }
        return ret;
    }
    void TapCommand::showHelp()
    {
        this->client->write("> tap <up/down/move/cancel> <object>\r\n");
        this->client->write("Simulate a touch event on a UI object and " \
                            "trigger the onTouched method.\r\n");
        this->client->write("You can specify the type of touch as up/down/move/cancel " \
                            "which will be\r\n");
        this->client->write(" accessibleto the event handler\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
