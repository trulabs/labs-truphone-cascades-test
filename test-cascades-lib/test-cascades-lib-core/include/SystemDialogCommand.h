/**
 * Copyright 2014 Truphone
 */
#ifndef SYSTEMDIALOGCOMMAND_H_
#define SYSTEMDIALOGCOMMAND_H_

#include <QObject>
#include <bb/system/SystemUiResult>

#include "Command.h"

namespace bb
{
    namespace system
    {
        class SystemDialog;
        class SystemPrompt;
    }  // namespace system
}  // namespace bb

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The SystemDialogCommand class is used to control
     * system dialogs
     *
     * @since test-cascades 1.0.10
     */
    class SystemDialogCommand : public Command
    {
    Q_OBJECT
    public:
        /*!
         * \brief getCmd Return the name of this command
         *
         * \return Command name
         *
         * @since test-cascades 1.0.10
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
         * @since test-cascades 1.0.10
         */
        static Command* create(class Connection * const s,
                               QObject * parent = 0)
        {
            return new SystemDialogCommand(s, parent);
        }
        /*!
         * \brief SystemDialogCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.10
         */
        SystemDialogCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~SystemDialogCommand Destructor
         *
         * @since test-cascades 1.0.10
         */
        ~SystemDialogCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const rguments);
        /*
         * See super
         */
        void showHelp(void);
    protected slots:
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
         * \brief finishButton Complete the dialog
         *
         * \param dialog The dialog to complete
         * \param result The result you want to return
         *
         * \return True if the finish completes Ok
         *
         * @since test-cascades 1.0.10
         */
        bool finishButton(bb::system::SystemDialog * const dialog,
                          const bb::system::SystemUiResult::Type result);
        /*!
         * \brief finishButton Complete the prompt
         *
         * \param prompt The prompt to complete
         * \param result The result you want to return
         *
         * \return True if the finish completes Ok
         *
         * @since test-cascades 1.1.0
         */
        bool finishButton(bb::system::SystemPrompt * const prompt,
                          const bb::system::SystemUiResult::Type result);
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // SYSTEMDIALOGCOMMAND_H
