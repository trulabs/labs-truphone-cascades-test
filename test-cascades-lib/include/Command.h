/**
 * Copyright 2013 Truphone
 */
#ifndef COMMAND_H_
#define COMMAND_H_

#include <QObject>
#include <QString>
#include <QList>

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The Command class is an interface for all commands
     *
     * @since test-cascades 1.0.0
     */
    class Command : public QObject
    {
    public:
        /*!
         * \brief Command Constructor
         *
         * @since test-cascades 1.0.0
         */
        explicit Command(QObject * parent = 0)
            : QObject(parent) {}
        /*!
         * \brief ~Command Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~Command() {}
        /*!
         * \brief executeCommand Execute the Command
         *
         * \param arguments A list of arguments for the command
         * \return Return @c true if the command works and @c false
         * if the command fails. If @c false then the command will
         * write the reason to the socket. If @c true then the command
         * won't write anything.
         *
         * @since test-cascades 1.0.0
         */
        virtual bool executeCommand(QStringList * const arguments) = 0;
        /*!
         * \brief showHelp Show help for the command
         *
         * @since test-cascades 1.0.0
         */
        virtual void showHelp(void) = 0;
        /*!
         * \brief cleanUp Clean up this object
         *
         * @since test-cascades 1.0.0
         */
        virtual void cleanUp(void)
        {
            this->deleteLater();
        }
    protected:
    private:
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone


#endif  // COMMAND_H_
