/**
 * Copyright 2013 Truphone
 */
#include "Server.h"
#include "Connection.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    Server::Server(
            QObject* parent) :
        QObject(parent),
        serverSocket(new QTcpServer(this))
    {
        if (this->serverSocket)
        {
            connect(this->serverSocket,
                    SIGNAL(newConnection()),
                    SLOT(acceptConnection()));
        }
    }

    Server::~Server()
    {
        if (this->serverSocket)
        {
            try
            {
                this->serverSocket->close();
            }
            catch (...)  // NOLINT(whitespace/parens)
            {
                /* we don't care just catch it */
            }
            delete this->serverSocket;
        }
    }

    void Server::close()
    {
        this->serverSocket->close();
    }

    bool Server::startServer(const quint16 port)
    {
        return this->serverSocket->listen(QHostAddress::Any, port);
    }

    void Server::acceptConnection(void)
    {
        QTcpSocket * const socket = this->serverSocket->nextPendingConnection();
        if (socket)
        {
            Connection * const client = new Connection(socket, this);
            if (client)
            {
                connect(client,
                        SIGNAL(disconnected(Connection*const)),
                        SLOT(connectionDied(Connection*const)));
                socket->write("OK\r\n");
                emit newConnection(client);
            }
            else
            {
                socket->write("ERROR: Failed to create a Harness for the new TCP Client\r\n");
                client->close();  // will disconnect and call connectionDied
            }
        }
        else
        {
            qErrnoWarning(-1, "ERROR: Failed to create a new client connection\r\n");
        }
    }

    void Server::connectionDied(const Connection * const connection)
    {
        if (connection)
        {
            const_cast<Connection*>(connection)->deleteLater();
        }
    }
}
}
}
