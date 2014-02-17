/**
 * Copyright 2014 Truphone
 */
#include "XmppHarness.h"

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
        return true;
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
