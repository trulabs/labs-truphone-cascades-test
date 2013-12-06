/**
 * Copyright 2013 Truphone
 */
#include "include/CommandFactory.h"

#include <bb/cascades/Application>
#include <QObject>

#include "ActionCommand.h"
#include "ClickCommand.h"
#include "TapCommand.h"
#include "RecordCommand.h"
#include "TextCommand.h"
#include "SleepCommand.h"
#include "TabCommand.h"
#include "TestCommand.h"
#include "LongClickCommand.h"
#include "HelpCommand.h"
#include "QmlCommand.h"
#include "ToastCommand.h"
#include "PopCommand.h"
#include "ToggleCommand.h"
#include "DropDownCommand.h"
#include "KeyCommand.h"
#include "TouchCommand.h"
#include "SpyCommand.h"
#include "ContactsCommand.h"
#include "PageCommand.h"

using truphone::test::cascades::Command;
using truphone::test::cascades::ClickCommand;
using truphone::test::cascades::LongClickCommand;
using truphone::test::cascades::TapCommand;
using truphone::test::cascades::TextCommand;
using truphone::test::cascades::TestCommand;
using truphone::test::cascades::ActionCommand;
using truphone::test::cascades::SleepCommand;
using truphone::test::cascades::TabCommand;
using truphone::test::cascades::KeyCommand;
using truphone::test::cascades::TouchCommand;
using truphone::test::cascades::RecordCommand;
using truphone::test::cascades::HelpCommand;
using truphone::test::cascades::QmlCommand;
using truphone::test::cascades::ToastCommand;
using truphone::test::cascades::PopCommand;
using truphone::test::cascades::ToggleCommand;
using truphone::test::cascades::DropDownCommand;
using truphone::test::cascades::SpyCommand;
using truphone::test::cascades::ContactsCommand;
using truphone::test::cascades::PageCommand;

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The CommandFactoryPrivate class is a wrapper around
     * command creation
     *
     * @since test-cascades 1.0.0
     */
    class CommandFactoryPrivate
    {
    public:
        /*!
         * \brief createCommand Type declaration for creating a command
         *
         * \param parent The parent object
         * \return A pointer to the new command or @c NULL if it failed
         *
         * @since test-cascades 1.0.0
         */
        typedef Command * createCommand(Connection * const,
                                        QObject * parent = 0);

        /*!
         * \brief CommandFactoryPrivate Construct a new wrapper
         *
         * \param cc A pointer to the function that will create the command
         *
         * @since test-cascades 1.0.0
         */
        explicit CommandFactoryPrivate(createCommand * const cc)
            : commandCreate(cc)
        {
        }
        /*!
         * \brief ~CommandFactoryPrivate Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~CommandFactoryPrivate()
        {
        }
        /*!
         * \brief create Short-cut way of calling the create routine
         *
         * \param s The TCP socket associated with the client connection
         * \param parent The parent object
         *
         * \return A pointer to the new command or @c NULL if it failed
         *
         * @since test-cascades 1.0.0
         */
        inline Command * create(Connection * const s,
                                QObject * parent = 0)
        {
            return this->commandCreate(s, parent);
        }
    protected:
    private:
        /*!
         * \brief commandCreate A pointer to the create routine
         */
        createCommand * const commandCreate;
    };

    QHash<QString, CommandFactoryPrivate*>
        CommandFactory::commandCache;

    #define INSERT(cmd, data) \
        if (CommandFactory::commandCache.contains(cmd)) \
        { \
            if (CommandFactory::commandCache[cmd]) \
            { \
                delete CommandFactory::commandCache[cmd]; \
            } \
        } \
        CommandFactory::commandCache.insert(cmd, data); \

    void CommandFactory::initialise(void)
    {
        INSERT(ClickCommand::getCmd(), new CommandFactoryPrivate(&ClickCommand::create));
        INSERT(LongClickCommand::getCmd(), new CommandFactoryPrivate(&LongClickCommand::create));
        INSERT(TapCommand::getCmd(), new CommandFactoryPrivate(&TapCommand::create));
        INSERT(TextCommand::getCmd(), new CommandFactoryPrivate(&TextCommand::create));
        INSERT(TestCommand::getCmd(), new CommandFactoryPrivate(&TestCommand::create));
        INSERT(ActionCommand::getCmd(), new CommandFactoryPrivate(&ActionCommand::create));
        INSERT(SleepCommand::getCmd(), new CommandFactoryPrivate(&SleepCommand::create));
        INSERT(TabCommand::getCmd(), new CommandFactoryPrivate(&TabCommand::create));
        INSERT(KeyCommand::getCmd(), new CommandFactoryPrivate(&KeyCommand::create));
        INSERT(TouchCommand::getCmd(), new CommandFactoryPrivate(&TouchCommand::create));
        INSERT(RecordCommand::getCmd(), new CommandFactoryPrivate(&RecordCommand::create));
        INSERT(RecordCommand::getStopCmd(), new CommandFactoryPrivate(&RecordCommand::createStop));
        INSERT(HelpCommand::getCmd(), new CommandFactoryPrivate(&HelpCommand::create));
        INSERT(QmlCommand::getCmd(), new CommandFactoryPrivate(&QmlCommand::create));
        INSERT(ToastCommand::getCmd(), new CommandFactoryPrivate(&ToastCommand::create));
        INSERT(PopCommand::getCmd(), new CommandFactoryPrivate(&PopCommand::create));
        INSERT(ToggleCommand::getCmd(), new CommandFactoryPrivate(&ToggleCommand::create));
        INSERT(DropDownCommand::getCmd(), new CommandFactoryPrivate(&DropDownCommand::create));
        INSERT(SpyCommand::getCmd(), new CommandFactoryPrivate(&SpyCommand::create));
        INSERT(ContactsCommand::getCmd(), new CommandFactoryPrivate(&ContactsCommand::create));
        INSERT(PageCommand::getCmd(), new CommandFactoryPrivate(&PageCommand::create));
    }

    Command * CommandFactory::getCommand(
            Connection * const client,
            const QString& command,
            QObject * parent)
    {
        Command * harnessCommand = NULL;
        if (CommandFactory::commandCache.contains(command))
        {
            harnessCommand = CommandFactory::commandCache.value(command)->create(client, parent);
        }
        return harnessCommand;
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
