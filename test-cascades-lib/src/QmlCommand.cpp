/**
 * Copyright 2013 Truphone
 */
#include "include/QmlCommand.h"

#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/QmlDocument>
#include <QStringList>
#include <string.h>

#include "Utils.h"
#include "Connection.h"

using bb::cascades::Application;
using bb::cascades::QmlDocument;
using truphone::test::cascades::Buffer;

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

    static QObject * scan(QObject * const obj,
                          const QString varName,
                          const int level = 0,
                          const int maxLevel = 20)
    {
        // too deep
        if (level > maxLevel)
        {
            return NULL;
        }

        // check this object
        QmlDocument * const doc = qobject_cast<QmlDocument*>(obj);
        if (doc)
        {
            QVariant var = doc->documentContext()->contextProperty(varName);
            if (not var.isNull() and var.isValid())
            {
                QObject * const varPropertyObj = var.value<QObject*>();
                if (varPropertyObj)
                {
                    return varPropertyObj;
                }
            }
        }

        // no luck, check all the children
        Q_FOREACH(QObject * const child, obj->children())
        {
            QObject * const o = scan(child, varName, level+1, maxLevel);
            if (o)
            {
                return o;
            }
        }
        return NULL;
    }

    bool QmlCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;

        // check args
        if (arguments->size() not_eq 1)
        {
            this->client->write("ERROR: qml <var>.<func> (i.e. app.func)\r\n");
        }
        else
        {
            const Buffer delim, qmlCommand;
            strncpy(delim.data(), ".", delim.length());
            strncpy(qmlCommand.data(),
                    arguments->first().toUtf8().constData(),
                    qmlCommand.length());

            QStringList tokens = Utils::tokenise(&delim,
                                                 &qmlCommand);
            if (tokens.size() not_eq 3)
            {
                this->client->write("ERROR: qml <var>.<func> (i.e. app.func)\r\n");
            }
            else
            {
                // see if the object exists and execute the function
                QObject * const obj = scan(Application::instance(), tokens.first(), 0);
                tokens.removeFirst();
                tokens.removeFirst();  // remove the period
                if (obj)
                {
                    const char * const methodName = tokens.first().toUtf8().constData();
                    tokens.removeFirst();
                    const QMetaObject * const objectMeta = obj->metaObject();

                    const Buffer tmp;
                    snprintf(tmp.data(), tmp.length(), "%s()", methodName);

                    const int methodIndex = objectMeta->indexOfMethod(tmp.cdata());
                    if (methodIndex not_eq -1)
                    {
                        const QMetaMethod methodMeta = objectMeta->method(methodIndex);
                        const QMetaMethod::MethodType type = methodMeta.methodType();
                        if ((type == QMetaMethod::Method or
                                type == QMetaMethod::Slot)
                                and methodMeta.parameterNames().isEmpty())
                        {
                            this->client->write("OK\r\n");
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
                            this->client->write("ERROR: Method must be a method " \
                                "(not constructor/slot/signal) and have no parameters\r\n");
                        }
                    }
                    else
                    {
                        this->client->write("ERROR: The method doesn't exist for that object\r\n");
                    }
                }
                else
                {
                    this->client->write("ERROR: Unable to find the object\r\n");
                }
            }
        }

        return ret;
    }

    void QmlCommand::showHelp()
    {
        this->client->write("> qml <var>.<func>\r\n");
        this->client->write("Execute a zero-parameter QML function using " \
                            "variables setup in the\r\n" \
                            "context to access C++ classes\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
