/**
 * Copyright 2013 Truphone
 */
#include "TextCommand.h"

#include <bb/cascades/TextField>
#include <bb/cascades/TextArea>
#include <QList>
#include <QString>
#include <QObject>

#include "Connection.h"
#include "Utils.h"

using bb::cascades::Application;
using bb::cascades::TextField;
using bb::cascades::TextArea;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString TextCommand::CMD_NAME = "text";

    TextCommand::TextCommand(class Connection * const socket,
                             QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    TextCommand::~TextCommand()
    {
    }

    bool TextCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->size() >= 1)
        {
            QObject * const obj = Utils::findObject(arguments->first());
            arguments->removeFirst();
            if (obj)
            {
                TextField * const field = qobject_cast<TextField*>(obj);
                if (field)
                {
                    const QString tmp = arguments->join(" ");
                    field->setText(tmp);
                    bb::cascades::Application::processEvents();
                    ret = true;
                }
                else
                {
                    TextArea * const area = qobject_cast<TextArea*>(obj);
                    if (area)
                    {
                        const QString tmp = arguments->join(" ");
                        area->setText(tmp);
                        bb::cascades::Application::processEvents();
                        ret = true;
                    }
                    else
                    {
                        this->client->write("ERROR: Object isn't a TextField or TextArea\r\n");
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
            this->client->write("ERROR: Not enough arguments, " \
                                "text <field> <str1> <str2> <strn>\r\n");
        }
        return ret;
    }

    void TextCommand::showHelp()
    {
        this->client->write("> text <object> <value>\r\n");
        this->client->write("Set the textual value of a TextField\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
