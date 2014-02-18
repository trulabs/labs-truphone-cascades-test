/**
 * Copyright 2013 Truphone
 */
#ifndef PAGECOMMAND_H_
#define PAGECOMMAND_H_

#include <QObject>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Page>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The PageCommand class is used to query the current page
     *
     * @since test-cascades 1.0.5
     */
    class PageCommand : public Command
    {
    Q_OBJECT
    public:
        /*!
         * \brief getCmd Return the name of this command
         *
         * \return Command name
         *
         * @since test-cascades 1.0.5
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
         * @since test-cascades 1.0.5
         */
        static Command* create(class Connection * const s,
                               QObject * parent = 0)
        {
            return new PageCommand(s, parent);
        }
        /*!
         * \brief PageCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        PageCommand(class Connection * const socket,
                    QObject* parent = 0);
        /*!
         * \brief ~PageCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        ~PageCommand();
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
         * \brief findCurrentPage Find the current page on top
         *
         * \param pane The root pane to search
         * \param callLevel The current recursive call level
         * \param maxCallLevel The maximum recursive call level
         * \return The top page or @c NULL if there isn't one
         *
         * @since test-cascades 1.0.0
         */
        bb::cascades::Page * findCurrentPage(
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


#endif  // PAGECOMMAND_H_
