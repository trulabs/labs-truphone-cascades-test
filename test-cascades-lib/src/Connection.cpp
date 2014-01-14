/**
 * Copyright 2013 Truphone
 */
#include "Connection.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    Connection::Connection(
            QTcpSocket * const clientSocket,
            QObject* parent) :
        QObject(parent),
        socket(clientSocket)
    {
        connect(this->socket,
                SIGNAL(readyRead()),
                SLOT(processPacket()));

        connect(socket,
                SIGNAL(disconnected()),
                SLOT(connectionDied()));
    }

    Connection::~Connection()
    {
    }

    void Connection::processPacket(void)
    {
        while (this->socket->bytesAvailable())
        {
            // read in a command from the client

            const QString newPacket(this->socket->readLine(1024));

            if (not newPacket.isNull() and not newPacket.isEmpty())
            {
                // string out all the tokens of the command
                // and if there's something to process strip
                // off the first word (the command) and execute it
                emit this->packetReceived(this, newPacket);
            }
        }
    }

    void Connection::connectionDied(void)
    {
        emit this->disconnected(this);
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
