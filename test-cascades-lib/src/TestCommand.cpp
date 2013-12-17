/**
 * Copyright 2013 Truphone
 */
#include "TestCommand.h"

#include <bb/cascades/Application>
#include <bb/cascades/UIObject>
#include <bb/cascades/AbstractActionItem>
#include <bb/cascades/TouchEvent>

#include "Utils.h"
#include "Connection.h"

using truphone::test::cascades::Utils;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString TestCommand::CMD_NAME = "test";

    TestCommand::TestCommand(Connection * const socket,
                             QObject * parent)
        : Command(parent),
          client(socket),
          scenePane(bb::cascades::Application::instance()->scene())
    {
    }

    TestCommand::~TestCommand()
    {
    }

    bool TestCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->size() >= 2)
        {
            const QString element = arguments->at(0);
            const QString property = arguments->at(1);
            arguments->removeFirst();
            arguments->removeFirst();
            const QString * expected = NULL;
            const QString expectedValue = arguments->join(" ");
            if (arguments->size())
            {
                expected = reinterpret_cast<const QString*>(&expectedValue);
            }

            // try and find the element by object name, otherwise
            // use the path/index to try and find it
            const QObject * tmp = this->scenePane->findChild<QObject*>(element);
            if (not tmp)
            {
                tmp = Utils::findObject(element);
            }
            const QObject * const obj = tmp;
            if (obj)
            {
                const QVariant var = obj->property(property.toUtf8().constData());
                if (var.isNull())
                {
                    // if no expected value is specified we're expected it to be null
                    if (not expected)
                    {
                        ret = true;
                    }
                    else
                    {
                        this->client->write("ERROR: Element property is null\r\n");
                    }
                }
                else if (not var.isValid())
                {
                    this->client->write("ERROR: Element property isn't valid\r\n");
                }
                else
                {
                    const QString actual = var.toString();
                    if (actual.compare(expected) == 0)
                    {
                        ret = true;
                    }
                    else
                    {
                        this->client->write("ERROR: The value is {");
                        this->client->write(actual.toUtf8().constData());
                        this->client->write("} which is not expected {");
                        if (expected)
                        {
                            this->client->write(expected->toUtf8().constData());
                        }
                        else
                        {
                            this->client->write("<null>");
                        }
                        this->client->write("}\r\n");
                    }
                }
            }
            else
            {
                this->client->write("ERROR: The element doesn't exist\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: test <element> <property> <expected value>\r\n");
        }
        return ret;
    }

    void TestCommand::showHelp()
    {
        this->client->write("> test <object> <property> <value>\r\n");
        this->client->write("Test if a QT property matches an expected value. "\
                            "This can be used to check\r\n");
        this->client->write("that a field on the UI is setup correctly or if a " \
                            "dialog has been displayed or\r\n");
        this->client->write("if an image or button is enabled or disabled\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
