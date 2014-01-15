/**
 * Copyright 2014 Truphone
 */
#include "SegmentCommand.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/SegmentedControl>
#include <bb/cascades/Option>
#include <QString>
#include <QList>
#include <QObject>

#include "Connection.h"

using bb::cascades::SegmentedControl;
using bb::cascades::Option;
using bb::cascades::Application;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString SegmentCommand::CMD_NAME = "segment";

    SegmentCommand::SegmentCommand(Connection * const socket,
                               QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    SegmentCommand::~SegmentCommand()
    {
    }

    bool SegmentCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;
        if (arguments->size() == 3)
        {
            SegmentedControl * const obj =
                    Application::instance()->scene()->findChild<SegmentedControl*>
                        (arguments->first());
            arguments->removeFirst();
            if (obj)
            {
                const QString mode = arguments->first();
                arguments->removeFirst();
                if (mode == "index")
                {
                    const int optionCount = obj->count();
                    bool ok = false;
                    const int index = arguments->first().toUInt(&ok);
                    if (ok && index < optionCount)
                    {
                        obj->setSelectedIndex(index);
                        ret = true;
                    }
                    else
                    {
                        this->client->write("ERROR: Index isn't a valid number\r\n");
                    }
                }
                else if (mode == "option")
                {
                    const int optionCount = obj->count();
                    for (int i = 0 ; (i < optionCount) and (not ret) ; i++)
                    {
                        const Option * const option = obj->at(i);
                        if (option)
                        {
                            // basic - the text is set
                            if (option->text() == arguments->first())
                            {
                                obj->setSelectedIndex(i);
                                ret = true;
                            }
                        }
                    }
                    if (not ret)
                    {
                        this->client->write("ERROR: Couldn't find the option\r\n");
                    }
                }
                else
                {
                    this->client->write("ERROR: Invalid selection method, use index or option\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Couldn't find the segment\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: Need to specify a segment and an option\r\n");
        }
        return ret;
    }

    void SegmentCommand::showHelp()
    {
        this->client->write("> segment <segment> <index | option> <option text>\r\n");
        this->client->write("> i.e. segment mySegment index 1\r\n");
        this->client->write("> i.e. segment mySegment option option5\r\n");
        this->client->write("Select a segment option\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
