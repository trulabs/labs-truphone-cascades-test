/**
 * Copyright 2013 Truphone
 */
#include "Connection.h"
#include "Buffer.h"

using truphone::test::cascades::Buffer;

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
        qint64 bytesRead = 0;
        const Buffer buffer;

        while (this->socket->bytesAvailable())
        {
            // read in a command from the client

            bytesRead = this->socket->readLine(buffer.data(), buffer.length());

            if (bytesRead)
            {
                // string out all the tokens of the command
                // and if there's something to process strip
                // off the first word (the command) and execute it
                emit this->packetReceived(this, buffer);
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
