/**
 * Copyright 2014 Truphone
 */
#ifndef XMPPDISCONNECTCOMMAND_H_
#define XMPPDISCONNECTCOMMAND_H_

#include <QObject>
#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The XMPPDisconnectCommand lets you disconnect a resource.
     *
     * @since test-cascades 1.1.0
     */
    class XMPPDisconnectCommand : public Command
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
            return new XMPPDisconnectCommand(s, parent);
        }
        /*!
         * \brief XMPPDisconnectCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.1.0
         */
        XMPPDisconnectCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~XMPPDisconnectCommand Destructor
         *
         * @since test-cascades 1.1.0
         */
        ~XMPPDisconnectCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const arguments);
        /*
         * See super
         */
        void showHelp(void);
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

#endif  // XMPPDISCONNECTCOMMAND_H_
