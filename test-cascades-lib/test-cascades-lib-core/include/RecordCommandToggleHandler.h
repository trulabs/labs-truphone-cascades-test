/**
 * Copyright 2013 Truphone
 */
#ifndef RECORDCOMMANDTOGGLEHANDLER_H_
#define RECORDCOMMANDTOGGLEHANDLER_H_

#include <QObject>
#include <bb/cascades/AbstractToggleButton>

#include "RecordCommand.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The RecordCommandToggleHandler class is used to listen to ToggleButton
     * events and notify the recorder
     *
     * @since test-cascades 1.0.0
     */
    class RecordCommandToggleHandler : public RecordCommand::RecordCommandHandler
    {
        Q_OBJECT
        public:
            /*!
             * \brief RecordCommandToggleHandler Create a new Toggle Handler
             *
             * \param recorder The recorder to notify
             * \param button The button to listen to
             * \param parent The parent object
             *
             * @since test-cascades 1.0.0
             */
            RecordCommandToggleHandler(RecordCommand * const recorder,
                                       bb::cascades::AbstractToggleButton * const button,
                                       QObject * parent = 0):
                RecordCommandHandler(parent),
                recorder(recorder),
                button(button)
            {
                connect(this->button,
                        SIGNAL(checkedChanged(bool)),  // NOLINT(readability/function)
                        SLOT(toggled(bool)));  // NOLINT(readability/function)
            }
            /*!
             * \brief ~RecordCommandToggleHandler Destructor
             *
             * @since test-cascades 1.0.0
             */
            ~RecordCommandToggleHandler()
            {
            }
        protected:
        private:
            /*!
             * \brief recorder The recorder to notify
             */
            RecordCommand * const recorder;
            /*!
             * \brief button The button we're listening to
             */
            bb::cascades::AbstractToggleButton * const button;
        private slots:
            /*!
             * \brief toggled Slot for when the button state is toggled
             *
             * \param newState The new state of the toggle
             *
             * @since test-cascades 1.0.0
             */
            void toggled(const bool newState)
            {
                this->recorder->toggled(this->button, newState);
            }
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // RECORDCOMMANDTOGGLEHANDLER_H_
