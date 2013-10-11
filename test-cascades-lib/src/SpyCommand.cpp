/**
 * Copyright 2013 Truphone
 */
#include "include/SpyCommand.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <QString>
#include <QList>
#include <QObject>
#include <QSignalSpy>

#include "include/Utils.h"
#include "include/Connection.h"

using truphone::test::cascades::Utils;
using bb::cascades::AbstractPane;
using bb::cascades::Application;

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The Spy class is a wrapper for the Signal Spy
     *
     * @since test-cascades 1.0.0
     */
    class Spy : public QObject
    {
    public:
        /*!
         * \brief Spy Create a new spy
         *
         * \param name The name of the spy
         * \param object The object name to spy on
         * \param signal The signal on @c object to spy on
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        Spy(const QString &name,
            const QString &object,
            const QString &signal,
            QObject * parent = 0)
            : QObject(parent),
              mySpyName(name),
              myObject(object),
              mySignal(signal),
              spy(NULL),
              spyObject(NULL)
        {
        }

        /*!
         * \brief ~Spy Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~Spy()
        {
            this->kill();
        }

        /*!
         * \brief connect Create and connect the SignalSpy to the object
         *
         * \return True if the connection worked
         *
         * @since test-cascades 1.0.0
         */
        bool connect()
        {
            bool ret = true;
            if (this->spy)
            {
                delete this->spy;
            }
            if (this->myObject.isNull() or this->myObject.isEmpty())
            {
                ret = false;
            }
            if (ret)
            {
                this->spyObject = Application::instance()->findChild<QObject*>(this->myObject);
            }
            if (not this->spyObject)
            {
                this->spyObject =
                        Application::instance()->scene()->findChild<QObject*>(this->myObject);
            }
            if (not this->spyObject)
            {
                this->spyObject = Utils::findObject(this->myObject);
            }
            if (not this->spyObject)
            {
                ret = false;
            }
            if (this->mySignal.isNull() or this->mySignal.isEmpty())
            {
                ret = false;
            }
            if (ret)
            {
                const char * const signalName = this->mySignal.toUtf8().constData();
                this->spy = new QSignalSpy(this->spyObject, signalName);
                ret = this->spy->isValid();
            }
            return ret;
        }

        /*!
         * \brief count The number of signals that were fired once spying started
         *
         * \param valid Pointer to a boolean valid that states if the return count
         * can be trusted. If the spy is broken or not connected then @c valid
         * will be set to false.
         *
         * \return The number of times the assigned signal has fired
         *
         * @since test-cascades 1.0.0
         */
        int count(bool * valid = 0) const
        {
            int c = 0;

            if (this->spy and this->spy->isValid())
            {
                c = this->spy->count();
                if (valid)
                {
                    *valid = true;
                }
            }

            return c;
        }

        /*!
         * \brief kill Kills the spy and removes it from the object
         *
         * @since test-cascades 1.0.0
         */
        void kill()
        {
            if (this->spy)
            {
                delete this->spy;
            }
            this->spy = NULL;
        }

    protected:
    private:
        /*!
         * \brief mySpyName The name of the spy
         */
        const QString mySpyName;
        /*!
         * \brief myObject The object to spy on
         */
        const QString myObject;
        /*!
         * \brief mySignal The signal to spy on
         */
        const QString mySignal;
        /*!
         * \brief spy Our spy
         */
        QSignalSpy * spy;
        /*!
         * \brief spyObject The object to spy on
         */
        QObject * spyObject;
    };

    /*!
     * \brief The SpyCommandPrivate class is used to maintain all the spies
     *
     * @since test-cascades 1.0.0
     */
    class SpyCommandPrivate : public QObject
    {
    public:
        /*!
         * \brief SpyCommandPrivate Constructor
         *
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        explicit SpyCommandPrivate(QObject * parent = 0)
            : QObject(parent)
        {
        }
        /*!
         * \brief ~SpyCommandPrivate Destructor.
         *
         * @since test-cascades 1.0.0
         */
        virtual ~SpyCommandPrivate()
        {
            Q_FOREACH(Spy * spy, this->spies)
            {
                if (spy)
                {
                    spy->kill();
                    delete spy;
                }
            }
            this->spies.clear();
        }
        /*!
         * \brief addSpy Add a new spy
         *
         * \param name The name of the spy
         * \param object The object to spy on
         * \param signal The signal to spy on
         * \return @c true if the signal is created and connected ok
         *
         * @since test-cascades 1.0.0
         */
        bool addSpy(const QString &name,
                    const QString &object,
                    const QString &signal)
        {
            bool ret = false;
            Spy * const spy = new Spy(name, object, signal, this);
            if (spy)
            {
                ret = spy->connect();
                if (ret)
                {
                    this->spies.insert(name, spy);
                }
                else
                {
                    delete spy;
                }
            }

            return ret;
        }
        /*!
         * \brief getSpy Return a spy
         *
         * \param name The name of the spy to get
         * \return The spy instance
         *
         * @since test-cascades 1.0.0
         */
        Spy * getSpy(const QString &name)
        {
            return this->spies[name];
        }
        /*!
         * \brief removeSpy Remove, disconnet and delete as spy
         *
         * \param name The name of the spy
         * \return @c true if the spy was found and deleted
         *
         * @since test-cascades 1.0.0
         */
        bool removeSpy(const QString &name)
        {
            bool found = false;
            Spy * const spy = this->spies[name];
            if (spy)
            {
                spy->kill();
                delete spy;
                this->spies.remove(name);
                found = true;
            }
            return found;
        }

    protected:
    private:
        /*!
         * \brief spies All the spies.
         */
        QHash<QString, Spy*> spies;
    };

    const QString SpyCommand::CMD_NAME = "spy";
    SpyCommandPrivate * SpyCommand::spyPrivateSingleton;

    SpyCommand::SpyCommand(Connection * const socket,
                           QObject* parent)
        : Command(parent),
          client(socket),
          spyPrivate(((spyPrivateSingleton == NULL)
                      ? spyPrivateSingleton = new SpyCommandPrivate() : spyPrivateSingleton))
    {
    }

    SpyCommand::~SpyCommand()
    {
    }

    bool SpyCommand::executeCommand(QStringList * const arguments)
    {
        bool ret = false;

        if (arguments->size() >= 2)
        {
            const QString command = arguments->first();
            arguments->removeFirst();
            if (command == "create")
            {
                if (arguments->size() == 3)
                {
                    const QString name = arguments->first();
                    arguments->removeFirst();;
                    const QString object = arguments->first();
                    arguments->removeFirst();
                    const QString signal = arguments->first();
                    arguments->removeFirst();
                    ret = this->spyPrivate->addSpy(name,
                                                   object,
                                                   "2" + signal);
                    if (not ret)
                    {
                        this->client->write
                                ("ERROR: Couldn't connect to the signal on that object\r\n");
                    }
                }
                else
                {
                    this->client->write("ERROR: The create command needs a name," \
                                        "object and signal\r\n");
                }
            }
            else if (command == "count")
            {
                if (arguments->size() >=2 )
                {
                    const QString name = arguments->first();
                    arguments->removeFirst();;
                    bool countOk = false;
                    const int expectedCount = arguments->first().toInt(&countOk);
                    if (not countOk)
                    {
                        this->client->write("ERROR: The expected count number isn't a number\r\n");
                    }
                    else
                    {
                        const Spy * const spy = this->spyPrivate->getSpy(name);
                        if (not spy)
                        {
                            this->client->write("ERROR: Couldn't find a spy with that name\r\n");
                        }
                        else
                        {
                            bool valid = false;
                            const int count = spy->count(&valid);
                            if (not valid)
                            {
                                this->client->write("ERROR: Couldn't get the signal " \
                                                    "count for the spy\r\n");
                            }
                            else
                            {
                                if (count == expectedCount)
                                {
                                    ret = true;
                                }
                                else
                                {
                                    this->client->write("ERROR: The spy's signal " \
                                        "count doesn't match the expected count\r\n");
                                }
                            }
                        }
                    }
                }
                else
                {
                    this->client->write("ERROR: Count needs at least two parameters\r\n");
                }
            }
            else if (command == "kill")
            {
                const QString name = arguments->first();
                if (this->spyPrivate->removeSpy(name))
                {
                    ret = true;
                }
                else
                {
                    this->client->write("ERROR: Can't delete an unknown spy\r\n");
                }
            }
            else
            {
                this->client->write("ERROR: Unknown spy command\r\n");
            }
        }
        else
        {
            this->client->write("ERROR: Spy commands need at least two parameters\r\n");
        }

        return ret;
    }

    void SpyCommand::showHelp()
    {
        this->client->write("> spy create <spyName> <object> <signal> - create a new Spy\r\n");
        this->client->write("> spy count <spyName> <count> - check that the signal count for " \
                            "a spy is <count>\r\n");
        this->client->write("> spy kill <spyName> - remove a spy\r\n");
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
