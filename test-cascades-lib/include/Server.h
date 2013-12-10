/**
 * Copyright 2013 Truphone
 */
#ifndef SERVER_H_
#define SERVER_H_

#include <QObject>
#include <QTcpServer>

#include "Buffer.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * @brief TCP Server
     *
     * @since test-cascades 1.0.0
     */
    class Server : public QObject
    {
        Q_OBJECT
        public:
            /*!
             * \brief Server
             *
             * The default constructor will create and initialise
             * a new Server
             *
             * \param parent Optional: used as needed
             *
             * @since test-cascades 1.0.0
             */
            Server(QObject * parent = 0);
            /*!
             * \brief ~Server
             *
             * The default destructor.
             *
             * @since test-cascades 1.0.0
             */
            ~Server();
            /*!
             * \brief startServer Start the TCP server and listen for
             * test client connections
             *
             * \param port The TCP port to listen for connections on
             * \return Returns @c true if the server was started
             *
             * @since test-cascades 1.0.0
             */
            bool startServer(const quint16 port = (quint16)(15000u));
            /*!
             * \brief close Close down the server
             *
             * @since test-cascades 1.0.0
             */
            void close();
        protected:
        private:
            /*!
             * \brief serverSocket The TCP Server socket instance
             */
            QTcpServer * const serverSocket;
        signals:
            /*!
             * \brief newConnection Signal emitted when a new client connects to the server
             *
             * \param connection The new connection
             *
             * @since test-cascades 1.0.1
             */
            void newConnection(class Connection * connection);
        private slots:
            /*!
             * \brief acceptConnection Slot used to accept a new connection
             *
             * @since test-cascades 1.0.0
             */
            void acceptConnection(void);
            /*!
             * \brief connectionDied Slot used when a connection has been terminated
             *
             * \param connection The connection that's died
             *
             * @since test-cascades 1.0.0
             */
            static void connectionDied(const class Connection * const connection);
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // SERVER_H_
