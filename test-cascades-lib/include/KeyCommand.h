/**
 * Copyright 2013 Truphone
 */
#ifndef KEYCOMMAND_H_
#define KEYCOMMAND_H_

#include <bb/cascades/Application>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The KeyCommand class simulates a key press
     *
     * @since test-cascades 1.0.0
     */
    class KeyCommand : public Command
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
            return new KeyCommand(s, parent);
        }
        /*!
         * \brief KeyCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        explicit KeyCommand(class Connection * const socket,
                                  QObject * parent = 0);
        /*!
         * \brief ~KeyCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        ~KeyCommand();
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

#endif  // KEYCOMMAND_H_
