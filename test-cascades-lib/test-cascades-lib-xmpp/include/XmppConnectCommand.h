/**
 * Copyright 2014 Truphone
 */
#ifndef XMPPCONNECTCOMMAND_H_
#define XMPPCONNECTCOMMAND_H_

#include <QObject>
#include <QXmppClient.h>
#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The XMPPConnectCommand lets you connect to an XMPP server.
     *
     * @since test-cascades 1.1.0
     */
    class XMPPConnectCommand : public Command
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
            return new XMPPConnectCommand(s, parent);
        }
        /*!
         * \brief XMPPConnectCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.1.0
         */
        XMPPConnectCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~XMPPConnectCommand Destructor
         *
         * @since test-cascades 1.1.0
         */
        ~XMPPConnectCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const arguments);
        /*
         * See super
         */
        void cleanUp(void)
        {
            /* do nothing */
        }
        /*
         * See super
         */
        void showHelp(void);
    protected slots:
        void connected();
        void disconnected();
        void error(QXmppClient::Error);
    private:
        /*!
         * \brief CMD_NAME The name of this command
         */
        static const QString CMD_NAME;
        /*!
         * \brief client The TCP socket associated with the client
         */
        class Connection * const client;

        class QXmppClient * const xmppClient;

        QString resourceName;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // XMPPCONNECTCOMMAND_H_
