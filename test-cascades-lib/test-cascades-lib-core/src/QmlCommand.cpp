/**
 * Copyright 2013 Truphone
 */
#include "include/QmlCommand.h"

#include <QStringList>
#include <string.h>
#include <bb/cascades/Application>

#include "Utils.h"
#include "Connection.h"

using bb::cascades::Application;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString QmlCommand::CMD_NAME = "qml";

    QmlCommand::QmlCommand(Connection * const socket,
                           QObject * parent)
        : Command(parent),
          client(socket)
    {
    }

    QmlCommand::~QmlCommand()
    {
    }

    bool QmlCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;

        // check args
        if (arguments->size() not_eq 1)
        {
            this->client->write(tr("ERROR: qml <var>.<func> (i.e. app.func") + "\r\n");
        }
        else
        {
            const QString delim(".");
            const QString qmlCommand = arguments->first();
            QStringList tokens = Utils::tokenise(delim, qmlCommand);
            if (tokens.size() not_eq 3)
            {
                this->client->write(tr("ERROR: qml <var>.<func> (i.e. app.func)") + "\r\n");
            }
            else
            {
                // see if the object exists and execute the function
                QObject * const obj = Utils::findObject(tokens.first());
                tokens.removeFirst();
                tokens.removeFirst();  // remove the period
                if (obj)
                {
                    const QString methodName(tokens.first() + "()");
                    tokens.removeFirst();
                    const QMetaObject * const objectMeta = obj->metaObject();

                    const int methodIndex = objectMeta->indexOfMethod(methodName.toUtf8());
                    if (methodIndex not_eq -1)
                    {
                        const QMetaMethod methodMeta = objectMeta->method(methodIndex);
                        const QMetaMethod::MethodType type = methodMeta.methodType();
                        if ((type == QMetaMethod::Method or
                                type == QMetaMethod::Slot)
                                and methodMeta.parameterNames().isEmpty())
                        {
                            // not translated; protocol
                            this->client->write(QString("OK") + "\r\n");
                            Application::processEvents();
                            // we know the method exists so return that it's completed early (above)
                            methodMeta.invoke(obj);
                            // possible we may never get here if the signal above
                            // quits or logs us out so we need to send the reply
                            // immediately
                            // ret = true; <- don't call for reason above
                        }
                        else
                        {
                            this->client->write(tr("ERROR: Method must be a method " \
                                "(not constructor/slot/signal) and have no parameters") + "\r\n");
                        }
                    }
                    else
                    {
                        this->client->write(tr("ERROR: The method doesn't exist for that object")
                                            + "\r\n");
                    }
                }
                else
                {
                    this->client->write(tr("ERROR: Unable to find the object") + "\r\n");
                }
            }
        }

        return ret;
    }

    void QmlCommand::showHelp()
    {
        this->client->write(tr("> qml <var>.<func>") + "\r\n");
        this->client->write(tr("Execute a zero-parameter QML function using " \
                            "variables setup in the") + "\r\n"
                            + tr("context to access C++ classes") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
