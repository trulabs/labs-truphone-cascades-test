/**
 * Copyright 2013 Truphone
 */
#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <QObject>
#include <QTcpSocket>
#include <bb/cascades/Application>

#include "include/Buffer.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The Connection class is a wrapper around the clients connection
     * to the server and listens for requests from the client, looks up the
     * command and executes it.
     *
     * @since test-cascades 1.0.0
     */
    class Connection : public QObject
    {
        Q_OBJECT
        public:
            /*!
             * \brief Connection Create a new connection
             *
             * \param clientSocket The TCP socket associated with the client
             * \param parent The parent object
             *
             * @since test-cascades 1.0.0
             */
            Connection(
                    QTcpSocket * const clientSocket,
                    QObject* parent = 0);
            /*!
             * \brief ~Connection Destroy the connection
             *
             * @since test-cascades 1.0.0
             */
            ~Connection();
            /*!
             * \brief close Closes the connection
             *
             * @since test-cascades 1.0.0
             */
            void close()
            {
                if (this->socket)
                {
                    this->socket->close();
                }
            }

            inline qint64 write(const char * const data)
            {
                return this->socket->write(data);
            }
            bool flush(void)
            {
                return this->socket->flush();
            }

        signals:
            void packetReceived(Connection* connection, const Buffer& packet);
        protected:
        private:
            /*!
             * \brief socket Client socket
             */
            QTcpSocket * const socket;
        Q_SIGNALS:
            /*!
             * \brief disconnected Signal emitted when the client disconnects
             *
             * \param connection The connection that has terminated
             *
             * @since test-cascades 1.0.0
             */
            void disconnected(Connection * const connection);
        private slots:
            /*!
             * \brief connectionDied Slot called from the socket when it is terminated
             *
             * @since test-cascades 1.0.0
             */
            void connectionDied(void);
            /*!
             * \brief processPacket Slot called from the socket when new data arrives
             *
             * @since test-cascades 1.0.0
             */
            void processPacket(void);
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone


#endif  // CONNECTION_H_
