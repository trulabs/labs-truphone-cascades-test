/**
 * Copyright 2013 Truphone
 */
#ifndef LONGCLICKCOMMAND_H_
#define LONGCLICKCOMMAND_H_

#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The LongClickCommand class simulates a long press on a control
     *
     * @since test-cascades 1.0.0
     */
    class LongClickCommand : public Command
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
            return new LongClickCommand(s, parent);
        }
        /*!
         * \brief LongClickCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        explicit LongClickCommand(class Connection * const socket,
                                  QObject * parent = 0);
        /*!
         * \brief ~LongClickCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        ~LongClickCommand();
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
         * \brief scenePane The root scene that we're trying to access
         */
        bb::cascades::AbstractPane * const scenePane;
        /*!
         * \brief client The TCP socket associated with the client
         */
        class Connection * const client;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // LONGCLICKCOMMAND_H_
