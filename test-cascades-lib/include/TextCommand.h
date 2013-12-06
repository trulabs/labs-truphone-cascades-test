/**
 * Copyright 2013 Truphone
 */
#ifndef TEXTCOMMAND_H_
#define TEXTCOMMAND_H_

#include <QObject>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The TextCommand class is used to set the text value
     * on objects such as Text Fields
     *
     * @since test-cascades 1.0.0
     */
    class TextCommand : public Command
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
            return new TextCommand(s, parent);
        }
        /*!
         * \brief TextCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        TextCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~TextCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~TextCommand();
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
#endif  // TEXTCOMMAND_H_
