/**
 * Copyright 2013 Truphone
 */
#ifndef CASCADESHARNESS_H_
#define CASCADESHARNESS_H_

#include <QObject>
#include <bb/cascades/Application>

#include "Buffer.h"
#include "CascadesTest.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * @brief Entry point for the Cascades Test Harness.
     *
     * This class is the entry point to the test harness that will
     * create a TCP server that will accept incoming commands from clients
     *
     * You can start the server as such
     * @code
     * CascadesHarness * harness = new CascadesHarness(app);
     * harness.startHarness(15000);
     * @endcode
     *
     * @since test-cascades 1.0.0
     */
    // cppcheck-suppress class_X_Y Ignored because this is the library export
    class TESTCASCADESLIBSHARED_EXPORT CascadesHarness : public QObject
    {
        Q_OBJECT
        public:
            /*!
             * \brief CascadesHarness
             *
             * The default constructor will create and initialise
             * a new test harness for you
             *
             * \param parent Optional: used as needed
             *
             * @since test-cascades 1.0.0
             */
            CascadesHarness(
                    QObject * parent = 0);
            /*!
             * \brief ~CascadesHarness
             *
             * The default destructor.
             *
             * @since test-cascades 1.0.0
             */
            ~CascadesHarness();
            /*!
             * \brief startHarness Start the TCP server and listen for
             * test client connections
             *
             * \param port The TCP port to listen for connections on
             * \return Returns @c true if the server was started
             *
             * @since test-cascades 1.0.0
             */
            bool startHarness(const quint16 port = (quint16)(15000u));
        protected:
        private:
            /*!
             * \brief serverSocket The TCP Server socket instance
             */
            class Server * const serverSocket;
            /*!
             * \brief delim A constant buffer containing all the delimiters we check
             */
            const Buffer delim;
        private slots:
            /*!
             * \brief processPacket Slot used to process a packet from a connection
             *
             * \param packet Buffer containing the packet received from the network
             *
             * @since test-cascades 1.0.0
             */
            void processPacket(class Connection * connection, const Buffer& packet);
            /*!
             * \brief handleNewConnection Handle a new client connection
             *
             * \param connection The new connection
             *
             * @since test-cascades 1.0.0
             */
            void handleNewConnection(class Connection * connection);
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // CASCADESHARNESS_H_
