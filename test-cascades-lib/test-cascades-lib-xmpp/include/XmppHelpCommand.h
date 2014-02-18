/**
 * Copyright 2014 Truphone
 */
#ifndef XMPPHELPCOMMAND_H_
#define XMPPHELPCOMMAND_H_

#include <QObject>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The XMPPHelpCommand shows help about the XMPP addin.
     *
     * @since test-cascades 1.1.0
     */
    class XMPPHelpCommand : public Command
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
         * @since test-cascades 1.0.0
         */
        static Command* create(class Connection * const s,
                               QObject * parent = 0)
        {
            return new XMPPHelpCommand(s, parent);
        }
        /*!
         * \brief XMPPHelpCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        XMPPHelpCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~XMPPHelpCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        ~XMPPHelpCommand();
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

#endif  // XMPPHELPCOMMAND_H_
