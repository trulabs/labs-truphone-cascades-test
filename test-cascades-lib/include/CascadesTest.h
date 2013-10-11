/**
 * Copyright 2013 Truphone
 */
#ifndef CASCADESTEST_H_
#define CASCADESTEST_H_

#include <QtCore/qglobal.h>

#if defined(TESTCASCADESLIB_LIBRARY)
#  define TESTCASCADESLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TESTCASCADESLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif  // CASCADESTEST_H_
