/**
 * Copyright 2013 Truphone
 */
#include "include/DropDownCommand.h"

#include <bb/cascades/Application>
#include <QList>
#include <QString>
#include <QObject>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/DropDown>
#include <bb/cascades/Option>

#include "Utils.h"
#include "Connection.h"

using bb::cascades::Application;
using bb::cascades::AbstractPane;
using bb::cascades::DropDown;
using bb::cascades::Option;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString DropDownCommand::CMD_NAME = "dropdown";

    DropDownCommand::DropDownCommand(Connection * const socket,
                 QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    DropDownCommand::~DropDownCommand()
    {
    }

    bool DropDownCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->size() >= 2)
        {
            const AbstractPane * const pane = Application::instance()->scene();
            DropDown * dropDown =
                    pane->findChild<DropDown*>(arguments->first());
            if (not dropDown)
            {
                dropDown = qobject_cast<DropDown*>(Utils::findObject(arguments->first()));
            }
            arguments->removeFirst();  // drop down name
            if (dropDown)
            {
                bool validIndex = false;
                const int index = arguments->first().toInt(&validIndex);
                if (validIndex)
                {
                    validIndex = index < dropDown->count();
                    if (validIndex)
                    {
                        dropDown->setSelectedIndex(index);
                        ret = true;
                    }
                    else
                    {
                        this->client->write("ERROR: The index is out of bounds\r\n");
                    }
                }
                else
                {
                    const QString optionText = arguments->join(" ");
                    const int options = dropDown->count();
                    const Option * option = NULL;
                    for (int i = 0 ; (i < options) and (not option) ; i++)
                    {
                        const Option * const anOption = dropDown->at(i);
                        if (anOption->text() == optionText)
                        {
                            option = anOption;
                        }
                    }
                    if (option)
                    {
                        dropDown->setSelectedOption(const_cast<Option*>(option));
                        ret = true;
                    }
                    else
                    {
                        this->client->write("ERROR: The name of the selected entry"\
                                            "wasn't found\r\n");
                    }
                }
            }
            else
            {
                this->client->write("ERROR: Can't find the drop down\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: Not enough arguments, " \
                                "toggle <button> <true|false>\r\n");
        }
        return ret;
    }

    void DropDownCommand::showHelp()
    {
        this->client->write("> dropdown <list> <index|value>\r\n");
        this->client->write("Selects either an index in the menu or\r\n");
        this->client->write("selections the option with the given text.\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
