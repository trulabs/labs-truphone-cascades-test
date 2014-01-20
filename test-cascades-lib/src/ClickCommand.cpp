/**
 * Copyright 2013 Truphone
 */
#include "include/ClickCommand.h"

#include "Utils.h"
#include "Connection.h"

using truphone::test::cascades::Utils;

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
            QObject * const obj = Utils::findObject(arguments->first());
            if (obj)
            {
                // if it's a standard button, try clicking as normal
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
                    // may not be a normal thing, recursivly search for something
                    // that we can click on
                    ret = clickOnChildren(obj);
                    if (not ret)
                    {
                        this->client->write("ERROR: Failed to invoke function\r\n");
                    }
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

    bool ClickCommand::clickOnChildren(QObject * const parent)
    {
        bool found = false;
        Q_FOREACH(QObject * object, parent->children())
        {
            found = QMetaObject::invokeMethod(object, "clicked");
            if (not found)
            {
                found = clickOnChildren(object);
                if (found)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        return found;
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
