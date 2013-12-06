/**
 * Copyright 2013 Truphone
 */
#ifndef ACTIONCOMMAND_H_
#define ACTIONCOMMAND_H_

#include <bb/cascades/AbstractActionItem>
#include <bb/cascades/Page>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The ActionCommand class
     *
     * @since test-cascades 1.0.0
     */
    class ActionCommand : public Command
    {
    public:
        /*!
         * \brief getCmd Return the name of this command
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
            return new ActionCommand(s, parent);
        }
        /*!
         * \brief ActionCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent Object
         *
         * @since test-cascades 1.0.0
         */
        explicit ActionCommand(class Connection * const socket,
                               QObject * parent = 0);
        /*!
         * \brief ~ActionCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        ~ActionCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const arguments);
        /*
         * See super
         */
        void showHelp(void);
    protected:
        /*!
         * \brief executeAction Execute an abstract action
         *
         * \param action A pointer to the action to execute
         * \return @see executeCommand
         *
         * @since test-cascades 1.0.0
         */
        static bool executeAction(const bb::cascades::AbstractActionItem * const action);
        /*!
         * \brief findAction Find an action for a given name
         *
         * \param obj Object to search. The object will be checked and so will its children
         * \param name The name of the action to find
         * \param call_depth The current level of call depth
         * \param max_call_depth The maximum level of call depth
         * \return Return an @c AbstractActionItem or @c NULL if
         * the action can't be found or the recursive call depth
         * exceeds the maximum call depth
         *
         * @since test-cascades 1.0.0
         */
        static bb::cascades::AbstractActionItem * findAction(
                const QObject * const obj,
                const QString& name,
                const size_t call_depth = 0,
                // we don't want search depth > 10 objects
                const size_t max_call_depth = 10);
        /*!
         * \brief findCurrentPage Search (from @c pane) to find the current Page being
         * displayed
         *
         * \param pane The starting point of our search, typically the @c scene()
         * \param callLevel The current level of call depth
         * \param maxCallLevel The maximum level of call depth
         * \return Return a @c Page if we find the page or @c NULL if the
         * current page can't be found or if @see callLevel exceeded @see maxCallLevel
         *
         * @since test-cascades 1.0.0
         */
        static bb::cascades::Page * findCurrentPage(
                const bb::cascades::AbstractPane * const pane,
                const size_t callLevel = 0,
                const size_t maxCallLevel = 100);
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

#endif  // ACTIONCOMMAND_H_
