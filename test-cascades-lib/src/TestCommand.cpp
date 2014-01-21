/**
 * Copyright 2013 Truphone
 */
#include "TestCommand.h"

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
          client(socket)
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

            const QObject * const obj = Utils::findObject(element);
            if (obj)
            {
                bb::cascades::Application::processEvents();
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
                        QString data("ERROR: The value is {");
                        data += actual;
                        data += "} which is not expected {";
                        if (expected)
                        {
                            data += expected;
                        }
                        else
                        {
                            data += "<null>";
                        }
                        data += "}\r\n";
                        this->client->write(data.trimmed().toUtf8());
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
