/**
 * Copyright 2013 Truphone
 */
#ifndef TOASTCOMMAND_H_
#define TOASTCOMMAND_H_

#include <QObject>
#include <bb/system/SystemToast>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The ToastCommand class is used to check that
     * toasts are visible and that their text is correct
     *
     * @since test-cascades 1.0.0
     */
    class ToastCommand : public Command
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
            return new ToastCommand(s, parent);
        }
        /*!
         * \brief ActionCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        ToastCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~ActionCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~ToastCommand();
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
         * \brief findVisibleToast From top object, scan and find a toast that's visible
         *
         * \param object The starting point of the search
         * \param searchLevel The current recursive search level
         * \param maxSearchLevel The maximum recursive search level
         * \return A @c SystemToast or @c NULL if one wasn't found or
         * none are visible
         *
         * @since test-cascades 1.0.0
         */
        static const bb::system::SystemToast * findVisibleToast(
                QObject * const object,
                const size_t searchLevel = 0,
                const size_t maxSearchLevel = 100);
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

#endif  // TOASTCOMMAND_H_
