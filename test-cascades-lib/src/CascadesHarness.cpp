/**
 * Copyright 2013 Truphone
 */
#include <bb/cascades/Application>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/Tab>
#include <bb/cascades/Page>
#include <bb/cascades/ActionItem>
#include <QTcpSocket>

#include "CascadesHarness.h"
#include "Connection.h"
#include "CommandFactory.h"
#include "Utils.h"
#include "Server.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    CascadesHarness::CascadesHarness(
            QObject* parent) :
        QObject(parent),
        serverSocket(new Server(this)),
        delim(", ")
    {
        if (this->serverSocket)
        {
            connect(this->serverSocket,
                    SIGNAL(newConnection(Connection*)),
                    SLOT(handleNewConnection(Connection*)));
        }
    }

    CascadesHarness::~CascadesHarness()
    {
        if (this->serverSocket)
        {
            try
            {
                this->serverSocket->close();
            }
            catch (...)  // NOLINT(whitespace/parens)
            {
                qWarning("Caught an unexpected exception on socket close");
            }
            delete this->serverSocket;
        }
    }

    // cppcheck-suppress unusedFunction This is the entry point for clients
    bool CascadesHarness::startHarness(const quint16 port)
    {
        return this->serverSocket->startServer(port);
    }

    void CascadesHarness::handleNewConnection(Connection * connection)
    {
        connect(connection,
                SIGNAL(packetReceived(Connection*, const QString&)),
                SLOT(processPacket(Connection*, const QString&)));
    }

    void CascadesHarness::processPacket(Connection * connection, const QString& packet)
    {
        QStringList tokens = Utils::tokenise(this->delim, packet.trimmed());
        qDebug() << "test-cascades-lib: " << packet.trimmed();
        if (not tokens.empty())
        {
            const QString command = tokens.first();
            if (not command.startsWith("#", Qt::CaseInsensitive))
            {
                tokens.removeFirst();
                Command * const cmd = CommandFactory::getCommand(
                            connection,
                            command,
                            this);
                if (cmd)
                {
                    const bool cmdOk = cmd->executeCommand(&tokens);
                    if (cmdOk)
                    {
                        connection->write("OK\r\n");
                    }
                    // may not actually clean/delete anything right
                    // now if the command is async
                    cmd->cleanUp();
                }
                else
                {
                    connection->write("ERROR: I don't understand that command\r\n");
                }
            }
        }
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
