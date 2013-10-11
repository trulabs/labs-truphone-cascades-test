/**
 * Copyright 2013 Truphone
 */
#ifndef RECORDCOMMANDKEYHANDLER_H_
#define RECORDCOMMANDKEYHANDLER_H_

#include <QObject>
#include <bb/cascades/KeyEvent>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Control>
#include <bb/cascades/UIObject>

#include "include/RecordCommand.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The RecordCommandKeyHandler class is used to listen for key
     * events and notify the recorder
     *
     * @since test-cascades 1.0.0
     */
    class RecordCommandKeyHandler : public RecordCommand::RecordCommandHandler
    {
        Q_OBJECT
        public:
            /*!
             * \brief RecordCommandKeyHandler Create a new key handler
             *
             * \param recorder The recorder to notify when a key is pressed
             * \param receiver The object that received the key presses
             * \param parent The parent object
             *
             * @since test-cascades 1.0.0
             */
            RecordCommandKeyHandler(RecordCommand * const recorder,
                                    bb::cascades::UIObject * const receiver,
                                    QObject * parent = 0):
                RecordCommandHandler(parent),
                recorder(recorder)
            {
                this->receiver.theOwner = unknown;
                this->receiver.owners.receiver = receiver;

                this->keyHandler = new bb::cascades::KeyListener(receiver);
                connect(this->keyHandler,
                        SIGNAL(keyEvent(bb::cascades::KeyEvent*)),
                        SLOT(keyed(bb::cascades::KeyEvent*)));

                bb::cascades::AbstractPane * const pane =
                        qobject_cast<bb::cascades::AbstractPane*>(receiver);
                if (pane)
                {
                    this->receiver.theOwner = abstractPane;
                    pane->addKeyListener(keyHandler);
                }
                else
                {
                    bb::cascades::Control * const cntrl =
                            qobject_cast<bb::cascades::Control*>(receiver);
                    if (cntrl)
                    {
                        this->receiver.theOwner = control;
                        cntrl->addKeyListener(keyHandler);
                    }
                }
            }
            /*!
             * \brief ~RecordCommandKeyHandler Destructor
             *
             * @since test-cascades 1.0.0
             */
            virtual ~RecordCommandKeyHandler()
            {
                // the ownership of the keyhandler is with the
                // receiver and remains with the owner and it's
                // responsibility to delete it
            }
        protected:
        private:
            /*!
             * \brief recorder The recorder to notify
             */
            RecordCommand * const recorder;
            /*!
             * \brief keyHandler The key handler we create to bind to the object
             */
            bb::cascades::KeyListener * keyHandler;
            /*!
             * \brief The owner_t struct Enumeration of all the receiver types of key events
             */
            typedef enum owner
            {
                unknown,
                abstractPane,
                control
            } owner_t;
            /*!
             * \brief The receiver_t struct Structure for holding the receiver of the event
             */
            struct receiver_t
            {
                /*!
                 * \brief theOwner The owner type (pane or control)
                 */
                owner_t theOwner;
                /*!
                 * \brief The owners_t union used to store the @c QObject as different things
                 */
                union owners_t
                {
                    /*!
                     * \brief receiver Generic pointer to the owner
                     */
                    QObject * receiver;
                    /*!
                     * \brief receiverPane The owner as a pane
                     */
                    bb::cascades::AbstractPane * receiverPane;
                    /*!
                     * \brief receiverControl The owner as a control
                     */
                    bb::cascades::Control * receiverControl;
                } owners;
            } receiver;
        private slots:
            /*!
             * \brief keyed Slot used to listen to key events on an object
             *
             * \param event The event that has fired
             *
             * @since test-cascades 1.0.0
             */
            virtual void keyed(bb::cascades::KeyEvent *event)
            {
                this->recorder->keyed(this->receiver.owners.receiver, event);
            }
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // RECORDCOMMANDKEYHANDLER_H_
