/**
 * Copyright 2013 Truphone
 */
#ifndef POPCOMMAND_H_
#define POPCOMMAND_H_

#include <QObject>
#include <bb/cascades/AbstractPane>

#include "include/Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The PopCommand class is used to pop classes from a
     * navigation pane
     *
     * @since test-cascades 1.0.0
     */
    class PopCommand : public Command
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
            return new PopCommand(s, parent);
        }
        /*!
         * \brief ActionCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        PopCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~ActionCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~PopCommand();
        /*
         * See super
         */
        virtual bool executeCommand(QStringList * const arguments);
        /*
         * See super
         */
        virtual void showHelp(void);
    protected:
        /*!
         * \brief popFromAbstractPane Given a current pane, find a navigation pane
         * that's active/being used/on-top and pop the a page off the stack
         *
         * \param pane The root pane to search
         * \param callLevel The current recursive call level
         * \param maxCallLevel The maximum recursive call level
         * \return @c true if a Navigation Pane is found and there's more than
         * one page and if so that on the stack and that a single page is popped
         *
         * @since test-cascades 1.0.0
         */
        virtual bool popFromAbstractPane(
                bb::cascades::AbstractPane * const pane,
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


#endif  // POPCOMMAND_H_
