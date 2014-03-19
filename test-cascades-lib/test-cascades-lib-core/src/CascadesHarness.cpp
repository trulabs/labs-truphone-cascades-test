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
        telnetSocket(new Server(this)),
        delim(", ")
    {
        if (this->serverSocket)
        {
            connect(this->serverSocket,
                    SIGNAL(newConnection(Connection*)),
                    SLOT(handleNewConnection(Connection*)));
        }
        if (this->telnetSocket)
        {
            connect(this->telnetSocket,
                    SIGNAL(newConnection(Connection*)),
                    SLOT(handleNewTelnetConnection(Connection*)));
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
        }
        if (this->telnetSocket)
        {
            try
            {
                this->telnetSocket->close();
            }
            catch (...)  // NOLINT(whitespace/parens)
            {
                qWarning("Caught an unexpected exception on tsocket close");
            }
        }
    }

    // cppcheck-suppress unusedFunction This is the entry point for clients
    bool CascadesHarness::startHarness(const quint16 port,
                                       const quint16 telnetPort)
    {
        bool serverOk = this->serverSocket->startServer(port);
        if (serverOk and telnetPort not_eq 0)
        {
            serverOk = this->telnetSocket->startServer(telnetPort);
        }
        return serverOk;
    }

    bool CascadesHarness::loadLocale(const QLocale& locale, const QString& directory)
    {
        bool loaded = false;
        QString filename = QString("test-cascades-lib-core_%1.qm").arg(locale.name());
        QTranslator translator;
        if (translator.load(filename, directory))
        {
            qApp->installTranslator(&translator);
            loaded = true;
        }
        return loaded;
    }

    void CascadesHarness::handleNewConnection(Connection * connection)
    {
        connect(connection,
                SIGNAL(packetReceived(Connection*, const QString&)),
                SLOT(processPacket(Connection*, const QString&)));
    }

    void CascadesHarness::handleNewTelnetConnection(Connection * connection)
    {
        connect(connection,
                SIGNAL(packetReceived(Connection*, const QString&)),
                SLOT(processTelnetPacket(Connection*, const QString&)));
    }

    void CascadesHarness::processTelnetPacket(Connection * connection, const QString& packet)
    {
        Q_FOREACH(QChar c, packet)
        {
            const int asciiCode = (int)(c.toAscii());
            if (13 == asciiCode)
            {
                if (not telnetBuffer.isEmpty())
                {
                    processPacket(connection, telnetBuffer);
                    telnetBuffer.clear();
                }
            }
            if (10 == asciiCode) { /* ignore */ }
            else if (8 == asciiCode)
            {
                if (not telnetBuffer.isEmpty())
                {
                    telnetBuffer.remove(telnetBuffer.length() - 1, 1);
                }
            }
            else
            {
                telnetBuffer += c;
            }
        }
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
                        // not translated; protocol
                        connection->write(QString("OK") + "\r\n");
                    }
                    // may not actually clean/delete anything right
                    // now if the command is async
                    cmd->cleanUp();
                }
                else
                {
                    connection->write(tr("ERROR: I don't understand that command") + "\r\n");
                }
            }
        }
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
