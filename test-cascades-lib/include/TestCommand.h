/**
 * Copyright 2013 Truphone
 */
#ifndef TESTCOMMAND_H_
#define TESTCOMMAND_H_

#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>

#include "include/Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The TestCommand class is used to test Qt properties
     *
     * @since test-cascades 1.0.0
     */
    class TestCommand : public Command
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
            return new TestCommand(s, parent);
        }
        /*!
         * \brief TestCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        explicit TestCommand(class Connection * const socket,
                             QObject * parent = 0);
        /*!
         * \brief ~TestCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~TestCommand();
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
        /*!
         * \brief scenePane The root scene we used to search for objects
         */
        bb::cascades::AbstractPane * const scenePane;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // TESTCOMMAND_H_
