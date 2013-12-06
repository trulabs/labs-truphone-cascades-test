/**
 * Copyright 2013 Truphone
 */
#ifndef SLEEPCOMMAND_H_
#define SLEEPCOMMAND_H_

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
     * \brief The SleepCommand class is used to delay execution until
     * the period has expired
     *
     * @since test-cascades 1.0.0
     */
    class SleepCommand : public Command
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
            return new SleepCommand(s, parent);
        }
        /*!
         * \brief SleepCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        SleepCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~SleepCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        ~SleepCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const rguments);
        /*
         * See super
         */
        void cleanUp(void)
        {
            /* do nothing */
        }
        /*
         * See super
         */
        void showHelp(void);
    protected slots:
        /*!
         * \brief timedOut Slot for when the timer expires
         *
         * @since test-cascades 1.0.0
         */
        void timedOut();
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
         * \brief timer The timer used to do the async timeout
         */
        QTimer timer;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // SLEEPCOMMAND_H_
