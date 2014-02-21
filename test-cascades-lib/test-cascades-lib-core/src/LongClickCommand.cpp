/**
 * Copyright 2013 Truphone
 */
#include "include/LongClickCommand.h"

#include "Connection.h"
#include "Utils.h"

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
            QObject * const obj = Utils::findObject(arguments->first());
            if (obj)
            {
                bb::cascades::Application::processEvents();
                const bool invoked = QMetaObject::invokeMethod(obj, "longClicked");
                if (invoked)
                {
                    bb::cascades::Application::processEvents();
                    ret = true;
                }
                else
                {
                    // may not be a normal thing, recursivly search for something
                    // that we can click on
                    ret = clickOnChildren(obj);
                    if (not ret)
                    {
                        this->client->write(tr("ERROR: Failed to invoke function") + "\r\n");
                    }
                }
            }
            else
            {
                this->client->write(tr("ERROR: Unknown object") + "\r\n");
            }
        }
        else
        {
            this->client->write(tr("ERROR: Not enough arguments, longClick <object>") + "\r\n");
        }
        return ret;
    }

    bool LongClickCommand::clickOnChildren(
            QObject * const parent,
            const int level,
            const int maxLevel)
    {
        bool found = false;
        if (level <= maxLevel)
        {
            Q_FOREACH(QObject * object, parent->children())
            {
                found = QMetaObject::invokeMethod(object, "longClicked");
                if (not found)
                {
                    found = clickOnChildren(object, level + 1, maxLevel);
                    if (found)
                    {
                        bb::cascades::Application::processEvents();
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        return found;
    }

    void LongClickCommand::showHelp()
    {
        this->client->write(tr("> longClick <object>") + "\r\n");
        this->client->write(tr("Simulate a long clicked event on a UI object " \
                            "and trigger the onLongClicked method") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
