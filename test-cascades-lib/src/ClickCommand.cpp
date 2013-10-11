/**
 * Copyright 2013 Truphone
 */
#include "include/ClickCommand.h"

#include <bb/cascades/Application>
#include <bb/cascades/UIObject>
#include <bb/cascades/AbstractActionItem>
#include <QCoreApplication>

#include "include/Utils.h"
#include "include/Connection.h"

using truphone::test::cascades::Utils;
using bb::cascades::Application;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString ClickCommand::CMD_NAME = "click";

    ClickCommand::ClickCommand(Connection * const socket,
                               QObject * parent)
        : Command(parent),
          scenePane(bb::cascades::Application::instance()->scene()),
          client(socket)

    {
    }

    ClickCommand::~ClickCommand()
    {
    }

    bool ClickCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if ((not arguments->isEmpty()) and (arguments->size() >= 1))
        {
            QObject * const obj = this->scenePane->findChild<QObject*>(arguments->first());
            if (obj)
            {
                const bool invoked = QMetaObject::invokeMethod(obj, "clicked");
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
            this->client->write("ERROR: Not enough arguments, click <object>\r\n");
        }
        return ret;
    }

    void ClickCommand::showHelp()
    {
        this->client->write("> click <object>\r\n");
        this->client->write("Simulate a clicked event on a UI object " \
                            "and trigger the onClicked method\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
