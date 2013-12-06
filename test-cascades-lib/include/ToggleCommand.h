/**
 * Copyright 2013 Truphone
 */
#ifndef TOGGLECOMMAND_H_
#define TOGGLECOMMAND_H_

#include <QObject>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The ToggleCommand class is used to change the toggle state
     * of the toggle button
     *
     * @since test-cascades 1.0.0
     */
    class ToggleCommand : public Command
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
            return new ToggleCommand(s, parent);
        }
        /*!
         * \brief ToggleCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        ToggleCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~ToggleCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~ToggleCommand();
        /*
         * See super
         */
        virtual bool executeCommand(QStringList * const arguments);
        /*
         * See super
         */
        virtual void showHelp(void);
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

#endif  // TOGGLECOMMAND_H_
