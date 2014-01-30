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

    qint64 Connection::write(const char * const data)
    {
        const QByteArray qData(data);
        this->socket->flush();
        const qint64 written = this->socket->write(qData);
        if (written == qData.length())
        {
            this->socket->flush();
        }
        else
        {
            qWarning("Connection transmitted {%d} of {%d}, data {%s}",
                     (int)(written), qData.length(), data);
        }
        return written;
    }

    bool Connection::flush(void)
    {
        return this->socket->flush();
    }

    void Connection::connectionDied(void)
    {
        emit this->disconnected(this);
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
