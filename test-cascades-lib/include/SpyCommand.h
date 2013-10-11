/**
 * Copyright 2013 Truphone
 */
#ifndef SPYCOMMAND_H_
#define SPYCOMMAND_H_

#include <QObject>
#include <QSemaphore>
#include <QTimer>

#include "include/Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    class SpyCommandPrivate;
    /*!
     * \brief The SleepCommand class is used to delay execution until
     * the period has expired
     *
     * @since test-cascades 1.0.0
     */
    class SpyCommand : public Command
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
            return new SpyCommand(s, parent);
        }
        /*!
         * \brief SpyCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        SpyCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~SpyCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~SpyCommand();
        /*
         * See super
         */
        virtual bool executeCommand(QStringList * const rguments);
        /*
         * See super
         */
        virtual void cleanUp(void)
        {
            /* do nothing */
        }
        /*
         * See super
         */
        virtual void showHelp(void);
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
         * \brief spyPrivate Private data
         */
        SpyCommandPrivate * const spyPrivate;
        /*!
         * \brief spyPrivateSingleton Private data
         */
        static SpyCommandPrivate * spyPrivateSingleton;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // SPYCOMMAND_H_
