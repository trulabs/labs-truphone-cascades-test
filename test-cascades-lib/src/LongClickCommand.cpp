/**
 * Copyright 2013 Truphone
 */
#include "include/LongClickCommand.h"

#include <bb/cascades/Application>
#include <bb/cascades/UIObject>
#include <bb/cascades/AbstractActionItem>
#include <QCoreApplication>

#include "include/Connection.h"

using bb::cascades::Application;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString LongClickCommand::CMD_NAME = "longClick";

    LongClickCommand::LongClickCommand(Connection * const socket,
                                       QObject * parent)
        : Command(parent),
          scenePane(bb::cascades::Application::instance()->scene()),
          client(socket)
    {
    }

    LongClickCommand::~LongClickCommand()
    {
    }

    bool LongClickCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if ((not arguments->isEmpty()) and (arguments->size() >= 1))
        {
            QObject * const obj = this->scenePane->findChild<QObject*>(arguments->first());
            if (obj)
            {
                const bool invoked = QMetaObject::invokeMethod(obj, "longClicked");
                if (invoked)
                {
    #if defined(__DEBUG)
                    this->client->write("DEBUG: Invoked\r\n");
    #endif
                    ret = true;
                }
                else
                {
                    this->client->write("ERROR: Failed to invoke function\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Unknown object\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: Not enough arguments, longClick <object>\r\n");
        }
        return ret;
    }

    void LongClickCommand::showHelp()
    {
        this->client->write("> longClick <object>\r\n");
        this->client->write("Simulate a long clicked event on a UI object " \
                            "and trigger the onLongClicked method\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
