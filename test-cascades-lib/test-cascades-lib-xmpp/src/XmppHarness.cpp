/**
 * Copyright 2014 Truphone
 */
#include "XmppHarness.h"

#include "CommandFactory.h"
#include "XmppHelpCommand.h"
#include "XmppPresenceCommand.h"
#include "XmppMessageCommand.h"
#include "XmppDisconnectCommand.h"
#include "XmppChatStateCommand.h"
#include "XmppChatMarkerCommand.h"
#include "XmppReachabilityCommand.h"
#include "XmppCorrectCommand.h"
#include "XmppTestCommand.h"
#include "XmppPrintCommand.h"
#include "XmppCarbonsCommand.h"
#include "XmppDebugCommand.h"
#include "XmppConnectCommand.h"
#include "XmppResourceStore.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    XmppHarness::XmppHarness(QObject *parent) :
        QObject(parent)
    {
    }
    XmppHarness::~XmppHarness()
    {
    }
    bool XmppHarness::installHarness()
    {
        XMPPResourceStore::initialiseStore(this);
        CommandFactory::installCommand(
                    XMPPHelpCommand::getCmd(),
                    &XMPPHelpCommand::create);
        CommandFactory::installCommand(
                    XMPPConnectCommand::getCmd(),
                    &XMPPConnectCommand::create);
        CommandFactory::installCommand(
                    XMPPPresenceCommand::getCmd(),
                    &XMPPPresenceCommand::create);
        CommandFactory::installCommand(
                    XMPPMessageCommand::getCmd(),
                    &XMPPMessageCommand::create);
        CommandFactory::installCommand(
                    XMPPDisconnectCommand::getCmd(),
                    &XMPPDisconnectCommand::create);
        CommandFactory::installCommand(
                    XMPPChatStateCommand::getCmd(),
                    &XMPPChatStateCommand::create);
        CommandFactory::installCommand(
                    XMPPChatMarkerCommand::getCmd(),
                    &XMPPChatMarkerCommand::create);
        CommandFactory::installCommand(
                    XMPPReachabilityCommand::getCmd(),
                    &XMPPReachabilityCommand::create);
        CommandFactory::installCommand(
                    XMPPCorrectCommand::getCmd(),
                    &XMPPCorrectCommand::create);
        CommandFactory::installCommand(
                    XMPPTestCommand::getCmd(),
                    &XMPPTestCommand::create);
        CommandFactory::installCommand(
                    XMPPPrintCommand::getCmd(),
                    &XMPPPrintCommand::create);
        CommandFactory::installCommand(
                    XMPPDebugCommand::getCmd(),
                    &XMPPDebugCommand::create);
        CommandFactory::installCommand(
                    XMPPCarbonsCommand::getCmd(),
                    &XMPPCarbonsCommand::create);
        return true;
    }

    bool XmppHarness::loadLocale(const QLocale& locale, const QString& directory)
    {
        bool loaded = false;
        QString filename = QString("test-cascades-lib-xmpp_%1.qm").arg(locale.name());
        QTranslator translator;
        if (translator.load(filename, directory))
        {
            qApp->installTranslator(&translator);
            loaded = true;
        }
        return loaded;
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
