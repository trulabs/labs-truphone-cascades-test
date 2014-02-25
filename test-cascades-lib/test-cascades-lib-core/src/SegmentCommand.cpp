/**
 * Copyright 2014 Truphone
 */
#include "SegmentCommand.h"

#include <bb/cascades/SegmentedControl>
#include <bb/cascades/Option>
#include <QString>
#include <QList>
#include <QObject>

#include "Connection.h"
#include "Utils.h"

using bb::cascades::SegmentedControl;
using bb::cascades::Option;

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
            SegmentedControl * obj =
                    qobject_cast<SegmentedControl*>(Utils::findObject(arguments->first()));
            if (obj)
            {
                arguments->removeFirst();
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
                        this->client->write(tr("ERROR: Index isn't a valid number") + "\r\n");
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
                        this->client->write(tr("ERROR: Couldn't find the option") + "\r\n");
                    }
                }
                else
                {
                    this->client->write(tr("ERROR: Invalid selection method," \
                                           " use index or option")
                                        + "\r\n");
                }
            }
            else
            {
                this->client->write(tr("ERROR: Couldn't find the segment") + "\r\n");
            }
        }
        else
        {
            this->client->write(tr("ERROR: Need to specify a segment and an option") + "\r\n");
        }
        return ret;
    }

    void SegmentCommand::showHelp()
    {
        this->client->write(tr("> segment <segment> <index | option> <option text>") + "\r\n");
        this->client->write(tr("> i.e. segment mySegment index 1") + "\r\n");
        this->client->write(tr("> i.e. segment mySegment option option5") + "\r\n");
        this->client->write(tr("Select a segment option") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
