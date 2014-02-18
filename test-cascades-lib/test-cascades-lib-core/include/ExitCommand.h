/**
 * Copyright 2013 Truphone
 */
#ifndef EXITCOMMAND_H_
#define EXITCOMMAND_H_

#include <QObject>
#include <QSemaphore>
#include <QTimer>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The ExitCommand class is used to terminate the current
     * conncetion to the server.
     *
     * @since test-cascades 1.0.5
     */
    class ExitCommand : public Command
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
            return new ExitCommand(s, parent);
        }
        /*!
         * \brief ExitCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.5
         */
        ExitCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~ExitCommand Destructor
         *
         * @since test-cascades 1.0.5
         */
        ~ExitCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const rguments);
        /*
         * See super
         */
        void cleanUp(void);
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
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // EXITCOMMAND_H_
