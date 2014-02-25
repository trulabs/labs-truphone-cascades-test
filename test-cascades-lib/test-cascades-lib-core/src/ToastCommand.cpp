/**
 * Copyright 2013 Truphone
 */
#include "ToastCommand.h"

#include <bb/cascades/Application>
#include <QList>
#include <QString>
#include <QObject>

#include "Connection.h"

using bb::cascades::Application;
using bb::system::SystemToast;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString ToastCommand::CMD_NAME = "toast";

    ToastCommand::ToastCommand(Connection * const socket,
                               QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    ToastCommand::~ToastCommand()
    {
    }

    bool ToastCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->size() >= 1)
        {
            const SystemToast * toast = findVisibleToast(Application::instance());
            const QString first = arguments->first();
            arguments->removeFirst();
            if (first == "true" or first == "false")
            {
                const bool expectedVisible = first == "true";
                const bool actualVisible = toast not_eq NULL;
                if (expectedVisible == actualVisible)
                {
                    ret = true;
                }
                else
                {
                    this->client->write(tr("ERROR: The Toast has an unexpected visiblity value")
                                        + "\r\n");
                }
            }
            else
            {
                if (toast)
                {
                    QString toastText = first;
                    toastText += ' ';
                    toastText += arguments->join(" ");
                    // TODO(struscott): Find the toast and check the body text - might be possible
                    // if we don't know which one is visible
                    if (toast->body() == toastText)
                    {
                        ret = true;
                    }
                    else
                    {
                        QString data(tr("ERROR: Toast text is "));
                        data += toast->body();
                        data += tr(" but we expected ");
                        data += toastText;
                        data += "\r\n";
                        this->client->write(data);
                    }
                }
                else
                {
                    this->client->write(tr("ERROR: No Toast is visible") + "\r\n");
                }
            }
        }
        else
        {
            this->client->write(tr("ERROR: Not enough arguments, " \
                                "toast <true|false/toast text>") + "\r\n");
        }
        return ret;
    }

    const SystemToast * ToastCommand::findVisibleToast(
            QObject * const object,
            const size_t searchLevel,
            const size_t maxSearchLevel)
    {
        const SystemToast * returnObject = NULL;

        if (searchLevel < maxSearchLevel)
        {
            const SystemToast * const toast = qobject_cast<SystemToast*>(object);
            if (toast)
            {
                const bool propVisible = toast->property("visible").toBool();
                if (propVisible)
                {
                    returnObject = toast;
                }
            }

            if (not returnObject)
            {
                Q_FOREACH(QObject * const child, object->children())
                {
                    returnObject = findVisibleToast(child, searchLevel + 1, maxSearchLevel);
                    if (returnObject)
                    {
                        break;
                    }
                }
            }
        }

        return returnObject;
    }

    void ToastCommand::showHelp()
    {
        this->client->write(tr("> toast <true|false/toast text>") + "\r\n");
        this->client->write(tr("toast true|false will check if a toast is visible otherswise")
                            + "\r\n");
        this->client->write(tr("it will assert that the parameters match the current toast.")
                            + "\r\n");
        this->client->write(tr("if no toast is visible then the test will fail") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
