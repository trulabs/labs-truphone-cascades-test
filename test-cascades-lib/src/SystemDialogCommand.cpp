/**
 * Copyright 2014 Truphone
 */
#include "SystemDialogCommand.h"

#include <QString>
#include <QList>
#include <QObject>

#include <bb/system/SystemDialog>
#include <bb/system/SystemUiButton>
#include <bb/system/SystemUiResult>
#include <bb/system/SystemUiReturnKeyAction>
#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>

#include "Connection.h"

using bb::system::SystemDialog;
using bb::system::SystemUiButton;
using bb::system::SystemUiResult;
using bb::system::SystemUiReturnKeyAction;
using bb::cascades::AbstractPane;
using bb::cascades::Application;

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
            arguments->removeFirst();
            SystemDialog * dialog =
                    Application::instance()->scene()->findChild<SystemDialog*>(dialogName);
            if (not dialog)
            {
                dialog = Application::instance()->findChild<SystemDialog*>(dialogName);
            }
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
                        this->client->write("ERROR: Need to specify cancel, " \
                                            "confirm or custom\r\n");
                    }
                }
            }
            else
            {
                this->client->write("ERROR: Failed to find the System dialog\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: sysdialog <name> <action>\r\n");
        }
        return ret;
    }

    bool SystemDialogCommand::finishButton(SystemDialog * const dialog,
                                           const SystemUiResult::Type result)
    {
        bool ret = false;
        if (dialog)
        {
            Application::processEvents();
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
            QApplication::processEvents();
            dialog->cancel();
        }
        return ret;
    }

    void SystemDialogCommand::showHelp()
    {
        this->client->write("> sysdialog <dialog> - Closed, no answer\r\n");
        this->client->write("> sysdialog <dialog> confirm - Confirm the dialog\r\n");
        this->client->write("> sysdialog <dialog> cancel - Cancel the dialog\r\n");
        this->client->write("> sysdialog <dialog> button - (Appended) Button to close dialog\r\n");
        this->client->write("> sysdialog <dialog> custom - Close with custom button\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
