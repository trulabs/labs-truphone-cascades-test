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
        return true;
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
