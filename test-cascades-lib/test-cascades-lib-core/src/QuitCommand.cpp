/**
 * Copyright 2014 Truphone
 */
#include "QuitCommand.h"

#include <QString>
#include <QList>
#include <QObject>
#include <bb/cascades/Application>

#include "Connection.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString QuitCommand::CMD_NAME = "quit";

    QuitCommand::QuitCommand(Connection * const socket,
                             QObject* parent)
        : Command(parent),
          client(socket)
    {
    }

    QuitCommand::~QuitCommand()
    {
    }

    bool QuitCommand::executeCommand(QStringList * const arguments)
    {
        Q_UNUSED(arguments);
        return true;
    }

    void QuitCommand::cleanUp(void)
    {
        bb::cascades::Application::instance()->exit();
    }

    void QuitCommand::showHelp()
    {
        this->client->write(tr("> quit") + "\r\n");
        this->client->write(tr("Terminate this application") + "\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
