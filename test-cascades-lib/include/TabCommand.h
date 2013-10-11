/**
 * Copyright 2013 Truphone
 */
#ifndef TABCOMMAND_H_
#define TABCOMMAND_H_

#include <QObject>
#include <bb/cascades/Tab>
#include <bb/cascades/TabbedPane>

#include "include/Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The TabCommand class is used to change the active tab
     * on a tabbed pane
     *
     * @since test-cascades 1.0.0
     */
    class TabCommand : public Command
    {
    Q_OBJECT
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
            return new TabCommand(s, parent);
        }
        /*!
         * \brief TabCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        TabCommand(class Connection * const socket,
                   QObject* parent = 0);
        /*!
         * \brief ~TabCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~TabCommand();
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
         * \brief selectTab Select a tab on a tabbed pane
         *
         * \param pane The pane to use
         * \param tab The pane to select on @c pane
         *
         * @since test-cascades 1.0.0
         */
        virtual void selectTab(bb::cascades::TabbedPane * const pane,
                               bb::cascades::Tab * const tab);
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // TABCOMMAND_H_
