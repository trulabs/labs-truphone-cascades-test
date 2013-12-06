/**
 * Copyright 2013 Truphone
 */
#ifndef CLICKCOMMAND_H_
#define CLICKCOMMAND_H_

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
     * \brief The ClickCommand class is used to simulate onClick(ed) events
     *
     * @since test-cascades 1.0.0
     */
    class ClickCommand : public Command
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
            return new ClickCommand(s, parent);
        }
        /*!
         * \brief ClickCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        explicit ClickCommand(class Connection * const socket,
                              QObject * parent = 0);
        /*!
         * \brief ~ClickCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        ~ClickCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const arguments);
        /*
         * See super
         */
        void showHelp(void);
    protected:
    private:
        /*!
         * \brief CMD_NAME The name of this command
         */
        static const QString CMD_NAME;
        /*!
         * \brief scenePane The scene that we're working with
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


#endif  // CLICKCOMMAND_H_
