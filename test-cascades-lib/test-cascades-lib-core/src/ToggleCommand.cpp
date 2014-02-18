/**
 * Copyright 2013 Truphone
 */
#include "ToggleCommand.h"

#include <QList>
#include <QString>
#include <QObject>
#include <bb/cascades/AbstractToggleButton>

#include "Utils.h"
#include "Connection.h"

using bb::cascades::AbstractToggleButton;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString ToggleCommand::CMD_NAME = "toggle";

    ToggleCommand::ToggleCommand(Connection * const socket,
                                 QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    ToggleCommand::~ToggleCommand()
    {
    }

    bool ToggleCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->size() >= 2)
        {
            AbstractToggleButton * const button =
                    qobject_cast<AbstractToggleButton*>(Utils::findObject(arguments->first()));
            arguments->removeFirst();  // button name
            if (button)
            {
                const bool state = (arguments->first() == "true");
                arguments->removeFirst();
                button->setChecked(state);
                bb::cascades::Application::processEvents();
                ret = true;
            }
            else
            {
                this->client->write("ERROR: Can't find the button\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: Not enough arguments, " \
                                "toggle <button> <true|false>\r\n");
        }
        return ret;
    }

    void ToggleCommand::showHelp()
    {
        this->client->write("> toggle <button> <true|false>\r\n");
        this->client->write("toggle a toggle button to a new state\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
