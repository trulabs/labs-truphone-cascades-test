/**
 * Copyright 2014 Truphone
 */
#ifndef XMPPHARNESS_H_
#define XMPPHARNESS_H_

#include <QObject>
#include <bb/cascades/Application>

#include "CascadesTest.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * @brief Entry point for the XMPP Test Harness.
     *
     * @since test-cascades 1.1.0
     */
    // cppcheck-suppress class_X_Y Ignored because this is the library export
    class TESTCASCADESLIBSHARED_EXPORT XmppHarness : public QObject
    {
        Q_OBJECT
        public:
            /*!
             * \brief XmppHarness
             *
             * The default constructor will create and initialise
             * a new test harness for you
             *
             * \param parent Optional: used as needed
             *
             * @since test-cascades 1.1.0
             */
            XmppHarness(
                    QObject * parent = 0);
            /*!
             * \brief ~XmppHarness
             *
             * The default destructor.
             *
             * @since test-cascades 1.1.0
             */
            ~XmppHarness();
            /*!
             * \brief installHarness Install the additional XMPP commands
             *
             * \return Returns @c true if the harness is installed
             *
             * @since test-cascades 1.1.0
             */
            bool installHarness();
            /*!
             * \brief loadLocale Load a locale
             *
             * \param locale The locale you want the library to load
             * \param directory The app directory with translations
             *
             * \return @c True if the locale was loaded
             *
             * @since test-cascades 1.1.0
             */
            bool loadLocale(const QLocale& locale, const QString& directory);
        protected:
        private:
        private slots:
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // XMPPHARNESS_H_
