/**
 * Copyright 2014 Truphone
 */
#ifndef XMPPDEBUGCOMMAND_H_
#define XMPPDEBUGCOMMAND_H_

#include <QObject>
#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The XMPPDebugCommand lets you enable/disable debugging.
     * If it's enabled, all XMPP messages are printed to the network
     * stream.
     *
     * @since test-cascades 1.1.0
     */
    class XMPPDebugCommand : public Command
    {
    Q_OBJECT
    public:
        /*!
         * \brief getCmd Return the name of this command
         *
         * \return Command name
         *
         * @since test-cascades 1.1.0
         */
        static QString getCmd()
        {
            return CMD_NAME;
        }
        /*!
         * \brief create Create a new instance of this Command
         *
         * \param s The TCP socket associated with the client
         * \param parent The parent object
         * \return Returns a new instance of the Command
         *
         * @since test-cascades 1.1.0
         */
        static Command* create(class Connection * const s,
                               QObject * parent = 0)
        {
            return new XMPPDebugCommand(s, parent);
        }
        /*!
         * \brief XMPPDebugCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.1.0
         */
        XMPPDebugCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~XMPPDebugCommand Destructor
         *
         * @since test-cascades 1.1.0
         */
        ~XMPPDebugCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const arguments);
        /*
         * See super
         */
        void showHelp(void);
        /*!
         * \brief isDebugEnabled Returns the state of debugging
         *
         * \return @c True if debugging is enabled
         *
         * @since test-cascades 1.1.0
         */
        static bool isDebugEnabled();
        /*!
         * \brief debugClients Get a list of clients with debugging enabled
         *
         * \return A list of debug connections
         *
         * @since test-cascades 1.1.0
         */
        static QList<class Connection*> debugClients();
    private:
        /*!
         * \brief CMD_NAME The name of this command
         */
        static const QString CMD_NAME;
        /*!
         * \brief client The TCP socket associated with the client
         */
        class Connection * const client;
        /*!
         * \brief debuggingEnabled Enable or disable debugging
         */
        static bool debuggingEnabled;
        static QList<class Connection*> debugClientList;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // XMPPDEBUGCOMMAND_H_
