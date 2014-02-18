/**
 * Copyright 2014 Truphone
 */
#include "XmppHarness.h"

#include "CommandFactory.h"
#include "XmppHelpCommand.h"

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
        CommandFactory::installCommand(XMPPHelpCommand::getCmd(),
                                       &XMPPHelpCommand::create);
        return true;
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
