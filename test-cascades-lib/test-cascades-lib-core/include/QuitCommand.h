/**
 * Copyright 2014 Truphone
 */
#ifndef QUITCOMMAND_H_
#define QUITCOMMAND_H_

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
     * \brief The QuitCommand class is used to terminate the current
     * application.
     *
     * @since test-cascades 1.0.19
     */
    class QuitCommand : public Command
    {
    Q_OBJECT
    public:
        /*!
         * \brief getCmd Return the name of this command
         *
         * \return Command name
         *
         * @since test-cascades 1.0.19
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
         * @since test-cascades 1.0.19
         */
        static Command* create(class Connection * const s,
                               QObject * parent = 0)
        {
            return new QuitCommand(s, parent);
        }
        /*!
         * \brief QuitCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.19
         */
        QuitCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~QuitCommand Destructor
         *
         * @since test-cascades 1.0.19
         */
        ~QuitCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const arguments);
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

#endif  // QUITCOMMAND_H_
