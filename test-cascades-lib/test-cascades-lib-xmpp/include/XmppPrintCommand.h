/**
 * Copyright 2014 Truphone
 */
#ifndef XMPPPRINTCOMMAND_H_
#define XMPPPRINTCOMMAND_H_

#include <QObject>
#include "Command.h"
#include <QXmppMessage.h>

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The XMPPPrintCommand lets you print the last message send or received.
     *
     * @since test-cascades 1.1.0
     */
    class XMPPPrintCommand : public Command
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
            return new XMPPPrintCommand(s, parent);
        }
        /*!
         * \brief XMPPPrintCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.1.0
         */
        XMPPPrintCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~XMPPPrintCommand Destructor
         *
         * @since test-cascades 1.1.0
         */
        ~XMPPPrintCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const arguments);
        /*
         * See super
         */
        void showHelp(void);
        /*!
         * \brief printMessage Print an XMPP XML message out to the stream.
         *
         * \param tx @c True if transmit, @c False if receive
         * \param message The message to debug
         *
         * @since test-cascades 1.1.0
         */
        static void printMessage(
                const bool tx,
                const QXmppStanza& message);
    private:
        /*!
         * \brief CMD_NAME The name of this command
         */
        static const QString CMD_NAME;
        /*!
         * \brief client The TCP socket associated with the client
         */
        class Connection * const client;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // XMPPPRINTCOMMAND_H_
