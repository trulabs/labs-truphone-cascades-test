/**
 * Copyright 2013 Truphone
 */
#include "include/KeyCommand.h"

#include <bb/cascades/AbstractTextControl>
#include <bb/cascades/Application>
#include <bb/cascades/Control>

#include "include/Buffer.h"
#include "include/Utils.h"
#include "include/Connection.h"

using bb::cascades::AbstractTextControl;
using bb::cascades::Application;
using bb::cascades::Control;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString KeyCommand::CMD_NAME = "key";

    KeyCommand::KeyCommand(Connection * const socket,
                           QObject * parent)
        : Command(parent),
          client(socket)
    {
    }

    KeyCommand::~KeyCommand()
    {
    }

    bool KeyCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        int theKey = 0, isPressed = 0, isAlt = 0, isShift = 0, isCtrl = 0, processed;

        const Buffer args;
        while (arguments->size() > 1)
        {
            strncat(args.data(), arguments->first().toUtf8().constData(), args.length());
            arguments->removeFirst();
            strncat(args.data(), " ", args.length());
        }
        strncat(args.data(), arguments->first().toUtf8().constData(), args.length());
        arguments->removeFirst();

        const Buffer target;

        processed = sscanf(args.cdata(),  // NOLINT(runtime/printf)
                           "%3d %1d %1d %1d %1d %511s",
                           &theKey,
                           &isPressed,
                           &isAlt,
                           &isShift,
                           &isCtrl,
                           target.data());

        if (processed not_eq 6)  // 6 items are expected
        {
            this->client->write("ERROR: play key could not decode the numeric key value\r\n");
        }
        else
        {
            QObject * const targetObject = Utils::findObject(target);
            if (not targetObject)
            {
                this->client->write("ERROR: Couldn't find the target of the keypress\r\n");
            }
            else
            {
                Control * const ctrl = qobject_cast<Control*>(targetObject);
                if (ctrl)
                {
                    if (not ctrl->isFocused())
                    {
                        ctrl->requestFocus();
                    }
                    AbstractTextControl * const field =
                            qobject_cast<AbstractTextControl*>(ctrl);
                    if (field)
                    {
                        if (isPressed)
                        {
                            if (theKey >= ' ' and theKey <= 'z')
                            {
                                field->setProperty("text",
                                                   QVariant(field->property("text").toString()
                                                              + QChar(theKey)));
                            }
                            else
                            {
                                switch (theKey)
                                {
                                    case '\b':
                                        field->setText(field->text().left(
                                                           field->text().length()-1));
                                        break;
                                    /* might need some extra special cases here later */
                                    default:
                                        /* dropped */
                                        break;
                                }
                            }
                        }
                        ret = true;
                    }
                    else
                    {
                        // for propogation (parents might not understand it, thats fine)
                        ret = true;
                    }
                }
                else
                {
                    // for propogation (parents might not understand it, thats fine)
                    ret = true;
                }
            }
        }
        return ret;
    }

    void KeyCommand::showHelp()
    {
        this->client->write("> key <code> <pressed> <alt> <shift> <ctrl>\r\n");
        this->client->write("Simulate a key press\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
