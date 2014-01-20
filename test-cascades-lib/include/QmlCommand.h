/**
 * Copyright 2013 Truphone
 */
#ifndef QMLCOMMAND_H_
#define QMLCOMMAND_H_

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The QmlCommand class is used to execute qml code. It's quite
     * a simple command and is intended to be used to call parameter-less
     * functions. The main reason it's here is to call app.logout.
     *
     * @since test-cascades 1.0.0
     */
    class QmlCommand : public Command
    {
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
            return new QmlCommand(s, parent);
        }
        /*!
         * \brief QmlCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent Object
         *
         * @since test-cascades 1.0.0
         */
        explicit QmlCommand(class Connection * const socket,
                            QObject * parent = 0);
        /*!
         * \brief ~QmlCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        ~QmlCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const arguments);
        /*
         * See super
         */
        void showHelp(void);
    protected:
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

#endif  // QMLCOMMAND_H_
