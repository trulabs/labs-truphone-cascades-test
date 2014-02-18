/**
 * Copyright 2014 Truphone
 */
#ifndef SEGMENTCOMMAND_H_
#define SEGMENTCOMMAND_H_

#include <QObject>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The SegmentCommand class is used to control a
     * SegmentedControl.
     *
     * @since test-cascades 1.0.7
     */
    class SegmentCommand : public Command
    {
    Q_OBJECT
    public:
        /*!
         * \brief getCmd Return the name of this command
         *
         * \return Command name
         *
         * @since test-cascades 1.0.7
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
         * @since test-cascades 1.0.7
         */
        static Command* create(class Connection * const s,
                               QObject * parent = 0)
        {
            return new SegmentCommand(s, parent);
        }
        /*!
         * \brief SegmentCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.7
         */
        SegmentCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~SegmentCommand Destructor
         *
         * @since test-cascades 1.0.7
         */
        ~SegmentCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const rguments);
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

#endif  // SEGMENTCOMMAND_H_
