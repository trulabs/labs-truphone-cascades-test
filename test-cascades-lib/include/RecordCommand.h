/**
 * Copyright 2013 Truphone
 */
#ifndef RECORDCOMMAND_H_
#define RECORDCOMMAND_H_

#include <QObject>
#include <QHash>
#include <QList>
#include <bb/cascades/Application>
#include <bb/cascades/KeyListener>
#include <bb/cascades/Page>
#include <bb/cascades/TouchEvent>
#include <bb/cascades/AbstractToggleButton>
#include <bb/cascades/DropDown>
#include <bb/cascades/Option>
#include <bb/cascades/Tab>
#include <bb/system/SystemToast>
#include <bb/system/SystemUiResult>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    class RecordCommandKeyHandler;
    class RecordCommandTouchHandler;
    class RecordCommandActionHandler;
    class RecordCommandToggleHandler;
    class RecordCommandDropDownHandler;
    /*!
     * \brief The RecordCommand class is used to monitor execution of events
     * on the phone and transmit them back to the client so they can be replayed
     * later on.
     *
     * @since test-cascades 1.0.0
     */
    class RecordCommand : public Command
    {
    Q_OBJECT
    public:
        /*!
         * \brief The RecordCommandHandler class is the base class
         * for all record command handlers/listeners
         *
         * @since test-cascades 1.0.0
         */
        class RecordCommandHandler : public QObject
        {
            protected:
            /*!
             * \brief RecordCommandHandler
             *
             * \param parent
             *
             * @since test-cascades 1.0.0
             */
            explicit RecordCommandHandler(QObject * parent = 0 )
                : QObject(parent) {}
            /*!
             * \brief ~RecordCommandHandler Destructor
             *
             *@since test-cascades 1.0.0
             */
            ~RecordCommandHandler() {}
        };

        /*!
         * \brief getCmd Return the name of this command
         *
         * \return Command name
         *
         * @since test-cascades 1.0.0
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
         * @since test-cascades 1.0.0
         */
        static Command* create(class Connection * const s,
                               QObject * parent = 0)
        {
            instance = new RecordCommand(s, parent);
            return instance;
        }
        /*!
         * \brief getStopCmd Return the name of the stop command
         *
         * \return Command name
         *
         * @since test-cascades 1.0.0
         */
        static QString getStopCmd()
        {
            return "stop";
        }

        /*!
         * \brief The StopRecordingCommand class is used to stop recording
         *
         * @since test-cascades 1.0.0
         */
        class StopRecordingCommand : public Command
        {
        public:
            /*!
             * \brief StopRecordingCommand Constructor
             *
             * \param parent The parent object
             *
             * @since test-cascades 1.0.0
             */
            StopRecordingCommand(class Connection * const s,
                                 QObject * parent = 0);
            /*!
             * \brief ~StopRecordingCommand Destructor
             *
             * @since test-cascades 1.0.0
             */
            ~StopRecordingCommand();
            bool executeCommand(QStringList * const arguments);
            void showHelp(void);
        protected:
        private:
            /*!
             * \brief client The TCP socket associated with the client
             */
            class Connection * const client;
        };

        /*!
         * \brief createStop Create a command to stop the recording
         *
         * \param s The TCP socket associated with the client
         * \param parent The parent object
         *
         * \return Returns a new instance of the Command
         *
         * @since test-cascades 1.0.0
         */
        static Command* createStop(class Connection * const s,
                                   QObject * parent = 0)
        {
            if (instance)
            {
                delete instance;
            }
            return new StopRecordingCommand(s, parent);
        }

        /*!
         * \brief RecordCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        RecordCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~RecordCommand Destructor
         *
         * @since test-cascades 1.0.0
         */
        ~RecordCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const arguments);
        /*!
         * \brief eventFilter Used to work out when new objects are added
         * or removed from the scene. We use this to install our listeners.
         *
         * \param receiver The object that receives the event
         * \param event The event, includes the target for the event
         * \return @c true if we filter it out, @c if we leave it to happen.
         * Because we're transparent and just using it for hooking into things
         * we always return @c false
         *
         * @since test-cascades 1.0.0
         */
        bool eventFilter(QObject * const receiver, QEvent * const event);
        /*!
         * \brief touched Call-back called from a listener. Used to write out
         * the event details back to the client
         *
         * \param receiver The receiver of the event
         * \param event The event that occured
         *
         * @since test-cascades 1.0.0
         */
        void touched(const QObject * const receiver,
                     const bb::cascades::TouchEvent * const event);
        /*!
         * \brief keyed Call-back called from a listener. Used to write out
         * the event details back to the client
         *
         * \param receiver The receiver of the event
         * \param event The event that occured
         *
         * @since test-cascades 1.0.0
         */
        void keyed(const QObject * const receiver,
                   const bb::cascades::KeyEvent * const event);
        /*!
         * \brief toggled Call-back called from a listener. Used to write out
         * the event details back to the client
         *
         * \param button The ToggleButton that changed
         * \param newState The new state of the button
         *
         * @since test-cascades 1.0.0
         */
         void toggled(const bb::cascades::AbstractToggleButton * const button,
                      const bool newState);
        /*!
         * \brief dropDownChanged Call-back called from a listener. Used to write out
         * the event details back to the client
         *
         * \param dropDown The drop down list that changed
         * \param option The new option that was selected
         *
         * @since test-cascades 1.0.0
         */
         void dropDownChanged(
                const bb::cascades::DropDown * const dropDown,
                const bb::cascades::Option* const option);
        /*!
         * \brief actionExecuted Call-back called from a listener. Used to write out
         * the event details back to the client
         *
         * \param action The action that was performed
         *
         * @since test-cascades 1.0.0
         */
        void actionExecuted(bb::cascades::AbstractActionItem * action);
        /*!
         * \brief cleanUp Special case - never clean up the record command.
         *
         * @since test-cascades 1.0.0
         */
        void cleanUp(void)
        {
            /* do nothing */
        }
        /*
         * See super
         */
        void showHelp(void);
    private slots:
        /*!
         * \brief tabChanged Slot for when tabs change
         *
         * \param tab The new tab
         *
         * @since test-cascades 1.0.0
         */
        void tabChanged(bb::cascades::Tab* tab);
        /*!
         * \brief onPopFinished Slot for when a Page is popped from a NavigationPane
         *
         * \param page The page that was popped from the pane
         *
         * @since test-cascades 1.0.0
         */
        void onPopFinished(bb::cascades::Page* page);
        /*!
         * \brief toastStarted Slot for when a new toast is opened
         *
         * \param text The text for the toast
         *
         * @since test-cascades 1.0.0
         */
        void toastStarted(const QString& text);
        /*!
         * \brief toastEnded Slot for when a toast is removed/finished
         *
         * \param result The result of the toast
         *
         * @since test-cascades 1.0.0
         */
        void toastEnded(bb::system::SystemUiResult::Type result);
    private:
        /*!
         * \brief CMD_NAME The name of this command
         */
        static const QString CMD_NAME;
        /*!
         * \brief client The TCP socket associated with the client
         */
        class Connection * const client;
        /*!
         * \brief lastEventTime The time at which the last event occured. Used
         * to record how long the user takes between commands and these are written
         * out as sleep commands.
         */
        QTime lastEventTime;
        /*!
         * \brief lastReceiver The last object to receive an event
         */
        const QObject * lastReceiver;
        /*!
         * \brief lastTarget The last object to be be the target of the event
         */
        QObject * lastTarget;
        /*!
         * \brief lastTouchType The last kind of touch event
         */
        bb::cascades::TouchType::Type lastTouchType;
        /*!
         * \brief ignoreEvents Should we ignore events? Used to stop
         * recursion when objects are added
         */
        bool ignoreEvents;
        /*!
         * \brief ctrlAndShiftPressed Used with CTRL+SHIFT
         * to not record events whilst we're clicking on things which instead
         * writes out test commands.
         */
        bool ctrlAndShiftPressed;
        /*!
         * \brief keyListeners Hash of all the objects that have a key listener
         */
        QHash<QObject*const, RecordCommandKeyHandler*> keyListeners;
        /*!
         * \brief touchListeners Hash of all the objects that have a touch listener
         */
        QHash<QObject*const, RecordCommandTouchHandler*> touchListeners;
        /*!
         * \brief actionListeners Hash of all the objects that have an action listener
         */
        QHash<QObject*const, RecordCommandActionHandler*> actionListeners;
        /*!
         * \brief toggleListeners Hash of all the objects that have a toggle listener
         */
        QHash<QObject*const, RecordCommandToggleHandler*> toggleListeners;
        /*!
         * \brief dropDownListeners Hash of all the objects that have a drop down listener
         */
        QHash<QObject*const, RecordCommandDropDownHandler*> dropDownListeners;
        /*!
         * \brief connectedPanes A hash of all the panes that we have a connection to
         */
        QHash<const bb::cascades::AbstractPane*const, QBool> connectedPanes;
        /*!
         * \brief connectedToasts A hash of all the System Toasts we have a connection to
         */
        QHash<const bb::system::SystemToast*const, QBool> connectedToasts;
        /*!
         * \brief instance The current instance of the recorder
         */
        static RecordCommand * instance;

        /*!
         * \brief addListenersToUiObjects When objects are added (or at startup) we need to
         * add listeners to objects.
         *
         * \param obj The object we want to add listeners to
         * \param childrenToo If @c true, listeners will be added all the children of @c obj
         * \param callLevel The current call level
         * \param maxCallLevel The maximum call level
         *
         * @since test-cascades 1.0.0
         */
        void addListenersToUiObjects(QObject * const obj,
                                     const bool childrenToo,
                                     const size_t callLevel = 0,
                                     const size_t maxCallLevel = 50);

        /*!
         * \brief updateSleepValue Update the time being used since the last
         * event and if needed write a sleep command out to the client
         *
         * \return The time, in milliseconds, since the last event occured
         *
         * @since test-cascades 1.0.0
         */
        int updateSleepValue();

        /*!
         * \brief testObjectProperties Called when we want to dump test
         * commands for all the properties of the current object
         *
         * @since test-cascades 1.0.0
         */
        void testObjectProperties(const QObject * const);
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // RECORDCOMMAND_H_
