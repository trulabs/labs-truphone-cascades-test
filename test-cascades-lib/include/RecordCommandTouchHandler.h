/**
 * Copyright 2013 Truphone
 */
#ifndef RECORDCOMMANDTOUCHHANDLER_H_
#define RECORDCOMMANDTOUCHHANDLER_H_

#include <QObject>
#include <bb/cascades/TouchEvent>

#include "RecordCommand.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The RecordCommandTouchHandler class is used to listen to touch events
     *
     * @since test-cascades 1.0.0
     */
    class RecordCommandTouchHandler : public RecordCommand::RecordCommandHandler
    {
        Q_OBJECT
        public:
            /*!
             * \brief RecordCommandTouchHandler Create a new Touch Handler
             *
             * \param recorder The recorder to notify when events occur
             * \param receiver The object that received the event
             * \param parent The parent Object
             *
             * @since test-cascades 1.0.0
             */
            RecordCommandTouchHandler(RecordCommand * const recorder,
                                      QObject * const receiver,
                                      QObject * parent = 0):
                RecordCommandHandler(parent),
                recorder(recorder),
                receiver(receiver)
            {
                connect(this->receiver,
                        SIGNAL(touch(bb::cascades::TouchEvent*)),
                        SLOT(touched(bb::cascades::TouchEvent*)));
            }
            /*!
             * \brief ~RecordCommandTouchHandler Destructor
             *
             * @since test-cascades 1.0.0
             */
            virtual ~RecordCommandTouchHandler()
            {
            }
        protected:
        private:
            /*!
             * \brief recorder The recorder to notify
             */
            RecordCommand * const recorder;
            /*!
             * \brief receiver The receiver of the events
             */
            QObject * const receiver;
        private slots:
            /*!
             * \brief touched Slot used to listen to touch events on objects
             *
             * \param event The event that fired
             *
             * @since test-cascades 1.0.0
             */
            virtual void touched(bb::cascades::TouchEvent *event)
            {
                this->recorder->touched(this->receiver, event);
            }
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // RECORDCOMMANDTOUCHHANDLER_H_
