/**
 * Copyright 2013 Truphone
 */
#ifndef COMMANDFACTORY_H_
#define COMMANDFACTORY_H_

#include <QString>
#include <QTcpSocket>
#include <QHash>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * Internal class
     */
    class CommandFactoryPrivate;
    /*!
     * \brief The CommandFactory class generates commands for a given name.
     *
     * @since test-cascades 1.0.0
     */
    class CommandFactory
    {
    public:
        /*!
         * \brief getCommand Create a command object with the name @c command.
         *
         * \param client The TCP socket to associate with the client
         * \param command The command name we want to create
         * \param parent The parent object
         * \return A Command object or @c NULL if that command doesn't exist
         *
         * @since test-cascades 1.0.0
         */
        static Command* getCommand(
                class Connection * const client,
                const QString& command,
                QObject * parent = 0);
        /*!
         * \brief getAvailableCommands Get a list of commands available to the server
         *
         * \return A list with all the known command names
         *
         * @since test-cascades 1.0.0
         */
        static const QList<QString> getAvailableCommands();
    protected:
    private:
        static CommandFactoryPrivate * privateData;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // COMMANDFACTORY_H_
