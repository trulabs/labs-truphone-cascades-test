/**
 * Copyright 2014 Truphone
 */
#ifndef XMPPCARBONSCOMMAND_H_
#define XMPPCARBONSCOMMAND_H_

#include <QObject>
#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The XMPPCarbonsCommand lets you enable or disable carbons.
     *
     * @since test-cascades 1.1.1
     */
    class XMPPCarbonsCommand : public Command
    {
    Q_OBJECT
    public:
        /*!
         * \brief getCmd Return the name of this command
         *
         * \return Command name
         *
         * @since test-cascades 1.1.1
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
         * @since test-cascades 1.1.1
         */
        static Command* create(class Connection * const s,
                               QObject * parent = 0)
        {
            return new XMPPCarbonsCommand(s, parent);
        }
        /*!
         * \brief XMPPCarbonsCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.1.1
         */
        XMPPCarbonsCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~XMPPPresenceCommand Destructor
         *
         * @since test-cascades 1.1.1
         */
        ~XMPPCarbonsCommand();
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

#endif  // XMPPCARBONSCOMMAND_H_
