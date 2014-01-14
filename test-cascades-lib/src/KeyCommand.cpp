/**
 * Copyright 2013 Truphone
 */
#include "include/KeyCommand.h"

#include <bb/cascades/AbstractTextControl>
#include <bb/cascades/Application>
#include <bb/cascades/Control>

#include "Utils.h"
#include "Connection.h"

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
        QString target;

        for (int param = 0 ; param < 6 ; param++)
        {
            bool ok = false;
            switch (param)
            {
            case 0:
                theKey = QString(arguments->first()).toInt(&ok);
                break;
            case 1:
                isPressed = QString(arguments->first()).toInt(&ok);
                break;
            case 2:
                isAlt = QString(arguments->first()).toInt(&ok);
                break;
            case 3:
                isShift = QString(arguments->first()).toInt(&ok);
                break;
            case 4:
                isCtrl = QString(arguments->first()).toInt(&ok);
                break;
            case 5:
                target = arguments->first();
                ok = not target.isNull() and not target.isEmpty();
                break;
            default:
                break;
            }
            if (ok)
            {
                processed++;
            }
            arguments->removeFirst();
        }

        Q_UNUSED(isAlt);
        Q_UNUSED(isShift);
        Q_UNUSED(isCtrl);

        if (processed not_eq 6)  // 6 items are expected
        {
            this->client->write("ERROR: wrong number of arguments to key command\r\n");
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
        this->client->write("> key <code> <pressed> <alt> <shift> <ctrl> <target>\r\n");
        this->client->write("Simulate a key press\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
