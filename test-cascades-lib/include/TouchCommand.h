/**
 * Copyright 2013 Truphone
 */
#ifndef TOUCHCOMMAND_H_
#define TOUCHCOMMAND_H_

#include <bb/cascades/Application>

#include "include/Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The TouchCommand class simulates a screen touch
     *
     * @since test-cascades 1.0.0
     */
    class TouchCommand : public Command
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
            return new TouchCommand(s, parent);
        }
        /*!
         * \brief TouchCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        explicit TouchCommand(class Connection * const socket,
                                  QObject * parent = 0);
        /*!
         * \brief ~TouchCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~TouchCommand();
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
         * \brief longPressHandlers A collection of handlers to monitor long
         * presses
         */
        static QHash<QObject*, class TouchCommandLPHandler*> longPressHandlers;
        /*!
         * \brief client The TCP socket associated with the client
         */
        class Connection * const client;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // TOUCHCOMMAND_H_
