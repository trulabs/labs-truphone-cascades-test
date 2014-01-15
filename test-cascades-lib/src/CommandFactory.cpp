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
#include "ExitCommand.h"
#include "ListCommand.h"
#include "SegmentCommand.h"
#include "SystemDialogCommand.h"

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
using truphone::test::cascades::ExitCommand;
using truphone::test::cascades::ListCommand;
using truphone::test::cascades::SegmentCommand;
using truphone::test::cascades::SystemDialogCommand;

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The CommandFactoryEntry class is a wrapper around
     * command creation
     *
     * @since test-cascades 1.0.0
     */
    class CommandFactoryEntry
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
        explicit CommandFactoryEntry(createCommand * const cc)
            : commandCreate(cc)
        {
        }
        /*!
         * \brief ~CommandFactoryPrivate Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~CommandFactoryEntry()
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

    class CommandFactoryPrivate
    {
    public:
        static void insert(const QString& name,
                           CommandFactoryEntry * const entry);
        /*!
         * \brief initialise Installs all the commands into the factory.
         * This needs to be called before the factory is used
         *
         * @since test-cascades 1.0.0
         */
        static void initialise(void);
        /*!
         * \brief commandCache The cache of all the commands
         */
        static QHash<QString, CommandFactoryEntry*> commandCache;
    };

    QHash<QString, CommandFactoryEntry*>
        CommandFactoryPrivate::commandCache;

    CommandFactoryPrivate * CommandFactory::privateData;

    void CommandFactoryPrivate::insert(const QString& name,
                                       CommandFactoryEntry * const entry)
    {
        if (commandCache.contains(name))
        {
            if (commandCache[name])
            {
                delete commandCache[name];
            }
        }
        commandCache.insert(name, entry);
    }

    void CommandFactoryPrivate::initialise(void)
    {
        insert(ClickCommand::getCmd(),
               new CommandFactoryEntry(&ClickCommand::create));
        insert(LongClickCommand::getCmd(),
               new CommandFactoryEntry(&LongClickCommand::create));
        insert(TapCommand::getCmd(),
               new CommandFactoryEntry(&TapCommand::create));
        insert(TextCommand::getCmd(),
               new CommandFactoryEntry(&TextCommand::create));
        insert(TestCommand::getCmd(),
               new CommandFactoryEntry(&TestCommand::create));
        insert(ActionCommand::getCmd(),
               new CommandFactoryEntry(&ActionCommand::create));
        insert(SleepCommand::getCmd(),
               new CommandFactoryEntry(&SleepCommand::create));
        insert(TabCommand::getCmd(),
               new CommandFactoryEntry(&TabCommand::create));
        insert(KeyCommand::getCmd(),
               new CommandFactoryEntry(&KeyCommand::create));
        insert(TouchCommand::getCmd(),
               new CommandFactoryEntry(&TouchCommand::create));
        insert(RecordCommand::getCmd(),
               new CommandFactoryEntry(&RecordCommand::create));
        insert(RecordCommand::getStopCmd(),
               new CommandFactoryEntry(&RecordCommand::createStop));
        insert(HelpCommand::getCmd(),
               new CommandFactoryEntry(&HelpCommand::create));
        insert(QmlCommand::getCmd(),
               new CommandFactoryEntry(&QmlCommand::create));
        insert(ToastCommand::getCmd(),
               new CommandFactoryEntry(&ToastCommand::create));
        insert(PopCommand::getCmd(),
               new CommandFactoryEntry(&PopCommand::create));
        insert(ToggleCommand::getCmd(),
               new CommandFactoryEntry(&ToggleCommand::create));
        insert(DropDownCommand::getCmd(),
               new CommandFactoryEntry(&DropDownCommand::create));
        insert(SpyCommand::getCmd(),
               new CommandFactoryEntry(&SpyCommand::create));
        insert(ContactsCommand::getCmd(),
               new CommandFactoryEntry(&ContactsCommand::create));
        insert(PageCommand::getCmd(),
               new CommandFactoryEntry(&PageCommand::create));
        insert(ExitCommand::getCmd(),
               new CommandFactoryEntry(&ExitCommand::create));
        insert(ListCommand::getCmd(),
               new CommandFactoryEntry(&ListCommand::create));
        insert(SegmentCommand::getCmd(),
               new CommandFactoryEntry(&SegmentCommand::create));
        insert(SystemDialogCommand::getCmd(),
               new CommandFactoryEntry(&SystemDialogCommand::create));
    }

    Command * CommandFactory::getCommand(
            Connection * const client,
            const QString& command,
            QObject * parent)
    {
        Command * harnessCommand = NULL;
        if (not CommandFactory::privateData)
        {
            privateData = new CommandFactoryPrivate();
            privateData->initialise();
        }
        if (privateData->commandCache.contains(command))
        {
            harnessCommand = privateData->commandCache.value(
                        command)->create(client, parent);
        }
        return harnessCommand;
    }

    const QList<QString> CommandFactory::getAvailableCommands()
    {
        if (not CommandFactory::privateData)
        {
            privateData = new CommandFactoryPrivate();
            privateData->initialise();
        }
        return privateData->commandCache.keys();
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
