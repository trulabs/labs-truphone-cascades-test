/**
 * Copyright 2013 Truphone
 */
#include "include/SleepCommand.h"

#include <QString>
#include <QList>
#include <QObject>

#include "include/Connection.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString SleepCommand::CMD_NAME = "sleep";

    SleepCommand::SleepCommand(Connection * const socket,
                               QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    SleepCommand::~SleepCommand()
    {
    }

    bool SleepCommand::executeCommand(QStringList * const arguments)
    {
        if (arguments->size() == 1)
        {
            const int periodInMs = arguments->first().toInt();
            this->timer.setInterval(periodInMs);
            this->timer.setSingleShot(true);
            this->timer.start();
            connect(&this->timer,
                    SIGNAL(timeout()),
                    SLOT(timedOut()));

            // IMPORTANT: Return false and don't send any messages.
            // This will silently fail and when the timer expires
            // it will send the passed OK back to the client
            //
            // The client should be enforcing flow control and not
            // execute anything until the OK message goes back.
            //
            // It also means that this thread of execution (event)
            // can continue to process events without being held
            // by the sleep
        }
        else
        {
            this->client->write("ERROR: Not enough arguments, sleep <timeInMs>\r\n");
        }
        return false;
    }

    void SleepCommand::timedOut()
    {
        this->client->write("OK\r\n");
        this->deleteLater();
    }

    void SleepCommand::showHelp()
    {
        this->client->write("> sleep <time in ms>\r\n");
        this->client->write("Waits for a period of time - once the time has elapsed " \
                            "'OK' will be transmitted\r\n");
        this->client->write("back to the client\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
