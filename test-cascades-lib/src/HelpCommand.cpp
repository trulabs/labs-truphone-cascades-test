/**
 * Copyright 2013 Truphone
 */
#include "include/HelpCommand.h"

#include <QString>
#include <QList>

#include "include/CommandFactory.h"
#include "include/Connection.h"

using truphone::test::cascades::CommandFactory;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString HelpCommand::CMD_NAME = "help";

    HelpCommand::HelpCommand(Connection * const socket,
                             QObject * parent)
        : Command(parent),
          client(socket)
    {
    }

    HelpCommand::~HelpCommand()
    {
    }

    bool HelpCommand::executeCommand(QStringList * const arguments)
    {
        if (arguments->isEmpty())
        {
            this->client->write("type help <cmd1> <cmd2> <cmdn> for more help\r\n"\
                                "\r\n> COMMANDS:\r\n");
            const QList<QString> commands = CommandFactory::getAvailableCommands();
            Q_FOREACH(QString commandName, commands)
            {
                this->client->write(commandName.toUtf8().constData());
                this->client->write("\r\n");
            }
            this->client->write("\r\n");
        }
        else
        {
            Q_FOREACH(QString commandName, *arguments)
            {
                Command * const cmd =
                        CommandFactory::getCommand(this->client,
                                                   commandName,
                                                   this);
                if (cmd)
                {
                    cmd->showHelp();
                    cmd->cleanUp();
                }
            }
        }
        return true;
    }

    void HelpCommand::showHelp()
    {
        this->client->write("> help - <command>\r\n");
        this->client->write("Print out help for the commands\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
