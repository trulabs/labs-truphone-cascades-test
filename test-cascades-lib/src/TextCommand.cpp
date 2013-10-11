/**
 * Copyright 2013 Truphone
 */
#include "include/TextCommand.h"

#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/TextField>
#include <QList>
#include <QString>
#include <QObject>

#include "include/Connection.h"

using bb::cascades::Application;
using bb::cascades::TextField;

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
        if (arguments->size() >= 2)
        {
            QObject * const obj =
                    Application::instance()->scene()->findChild<QObject*>(arguments->first());
            if (obj)
            {
                TextField * const field = qobject_cast<TextField*>(obj);
                if (field)
                {
                    arguments->removeFirst();
                    const QString tmp = arguments->join(" ");
                    field->setText(tmp);
                    ret = true;
                }
                else
                {
                    this->client->write("ERROR: Object isn't a TextField\r\n");
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
