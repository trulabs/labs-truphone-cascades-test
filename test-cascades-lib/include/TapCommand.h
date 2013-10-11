/**
 * Copyright 2013 Truphone
 */
#ifndef TAPCOMMAND_H_
#define TAPCOMMAND_H_

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
     * \brief The TapCommand class is used to simulate taps
     * on the screen
     *
     * @since test-cascades 1.0.0
     */
    class TapCommand : public Command
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
            return new TapCommand(s, parent);
        }
        /*!
         * \brief TapCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        explicit TapCommand(class Connection * socket,
                            QObject * parent = 0);
        /*!
         * \brief ~TapCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~TapCommand();
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
         * \brief scenePane The scene we use to search for the receiver
         */
        bb::cascades::AbstractPane * const scenePane;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // TAPCOMMAND_H_
