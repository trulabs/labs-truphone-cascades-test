/**
 * Copyright 2014 Truphone
 */
#include "SystemDialogCommand.h"

#include <QString>
#include <QList>
#include <QObject>

#include <bb/system/SystemDialog>
#include <bb/system/SystemPrompt>
#include <bb/system/SystemUiButton>
#include <bb/system/SystemUiResult>

#include "Connection.h"
#include "Utils.h"

using bb::system::SystemDialog;
using bb::system::SystemPrompt;
using bb::system::SystemUiButton;
using bb::system::SystemUiResult;
using bb::system::SystemUiReturnKeyAction;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString SystemDialogCommand::CMD_NAME("sysdialog");

    SystemDialogCommand::SystemDialogCommand(Connection * const socket,
                               QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    SystemDialogCommand::~SystemDialogCommand()
    {
    }

    bool SystemDialogCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->size() >= 1)
        {
            const QString dialogName = arguments->first();
            SystemDialog * const dialog =
                    qobject_cast<SystemDialog*>(Utils::findObject(dialogName));
            arguments->removeFirst();
            if (dialog)
            {
                if (arguments->isEmpty())
                {
                    ret = finishButton(dialog, SystemUiResult::None);
                }
                else
                {
                    const QString action = arguments->first();
                    arguments->removeFirst();
                    if (action == "confirm")
                    {
                        ret = finishButton(dialog, SystemUiResult::ConfirmButtonSelection);
                    }
                    else if (action == "cancel")
                    {
                        ret = finishButton(dialog, SystemUiResult::CancelButtonSelection);
                    }
                    else if (action == "custom")
                    {
                        ret = finishButton(dialog, SystemUiResult::CustomButtonSelection);
                    }
                    else if (action == "button")
                    {
                        ret = finishButton(dialog, SystemUiResult::ButtonSelection);
                    }
                    else
                    {
                        this->client->write(tr("ERROR: Need to specify cancel, " \
                                            "confirm or custom") + "\r\n");
                    }
                }
            }
            else
            {
                SystemPrompt * const prompt =
                        qobject_cast<SystemPrompt*>(Utils::findObject(dialogName));
                if (prompt)
                {
                    if (arguments->isEmpty())
                    {
                        ret = finishButton(
                                    prompt,
                                    SystemUiResult::None);
                    }
                    else
                    {
                        const QString action = arguments->first();
                        arguments->removeFirst();
                        if (action == "confirm")
                        {
                            ret = finishButton(
                                        prompt,
                                        SystemUiResult::ConfirmButtonSelection);
                        }
                        else if (action == "cancel")
                        {
                            ret = finishButton(
                                        prompt,
                                        SystemUiResult::CancelButtonSelection);
                        }
                        else if (action == "custom")
                        {
                            ret = finishButton(
                                        prompt,
                                        SystemUiResult::CustomButtonSelection);
                        }
                        else if (action == "button")
                        {
                            ret = finishButton(
                                        prompt,
                                        SystemUiResult::ButtonSelection);
                        }
                        else
                        {
                            this->client->write(tr("ERROR: Need to specify cancel, " \
                                                "confirm, custom or button") + "\r\n");
                        }
                    }
                }
                else
                {
                    this->client->write(tr("ERROR: Failed to find the System dialog") + "\r\n");
                }
            }
        }
        else
        {
            this->client->write(tr("ERROR: sysdialog <name> <action>") + "\r\n");
        }
        return ret;
    }

    bool SystemDialogCommand::finishButton(
            SystemDialog * const dialog,
            const SystemUiResult::Type result)
    {
        bool ret = false;
        if (dialog)
        {
            bb::cascades::Application::processEvents();
            ret = QMetaObject::invokeMethod(
                        dialog,
                        "finished",
                        Q_ARG(bb::system::SystemUiResult::Type,
                              result));
            ret |= QMetaObject::invokeMethod(
                        dialog,
                        "finished",
                        Q_ARG(int,
                              result));
            bb::cascades::Application::processEvents();
            dialog->cancel();
        }
        return ret;
    }

    bool SystemDialogCommand::finishButton(
            SystemPrompt * const prompt,
            const bb::system::SystemUiResult::Type result)
    {
        bool ret = false;
        if (prompt)
        {
            bb::cascades::Application::processEvents();
            ret = QMetaObject::invokeMethod(
                        prompt,
                        "finished",
                        Q_ARG(bb::system::SystemUiResult::Type,
                              result));
            ret |= QMetaObject::invokeMethod(
                        prompt,
                        "finished",
                        Q_ARG(int,
                              result));
            bb::cascades::Application::processEvents();
            prompt->cancel();
        }
        return ret;
    }

    void SystemDialogCommand::showHelp()
    {
        this->client->write(tr("> sysdialog <dialog> - Closed, no answer") + "\r\n");
        this->client->write(tr("> sysdialog <dialog> confirm - Confirm the dialog") + "\r\n");
        this->client->write(tr("> sysdialog <dialog> cancel - Cancel the dialog") + "\r\n");
        this->client->write(tr("> sysdialog <dialog> button - (Appended) Button to close dialog")
                            + "\r\n");
        this->client->write(tr("> sysdialog <dialog> custom - Close with custom button") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
