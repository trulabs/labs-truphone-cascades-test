/**
 * Copyright 2013 Truphone
 */
#ifndef HELPCOMMAND_H_
#define HELPCOMMAND_H_

#include "include/Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The HelpCommand class is used to print out the list of available
     * commands and additional help if a the command(s) are specified.
     *
     * @since test-cascades 1.0.0
     */
    class HelpCommand : public Command
    {
    public:
        /*!
         * \brief getCmd Return the name of this command
         *
         * \return Command name
         *
         * @since test-cascades 1.0.0
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
            return new HelpCommand(s, parent);
        }
        /*!
         * \brief HelpCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent Object
         *
         * @since test-cascades 1.0.0
         */
        explicit HelpCommand(class Connection * const socket,
                             QObject * parent = 0);
        /*!
         * \brief ~HelpCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~HelpCommand();
        /*
         * See super
         */
        virtual bool executeCommand(QStringList * const arguments);
        /*
         * See super
         */
        virtual void showHelp(void);
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

#endif  // HELPCOMMAND_H_
