/**
 * Copyright 2013 Truphone
 */
#ifndef RECORDCOMMANDDROPDOWNHANDLER_H_
#define RECORDCOMMANDDROPDOWNHANDLER_H_

#include <QObject>
#include <bb/cascades/DropDown>
#include <bb/cascades/Option>

#include "include/RecordCommand.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The RecordCommandDropDownHandler class is used to listen
     * for changes to drop down lists
     *
     * @since test-cascades 1.0.0
     */
    class RecordCommandDropDownHandler : public RecordCommand::RecordCommandHandler
    {
        Q_OBJECT
        public:
            /*!
             * \brief RecordCommandDropDownHandler Create a new handler
             *
             * \param recorder The recorder to notify when the drop down changes
             * \param dropDown The drop down to listen to
             * \param parent The parent object
             *
             * @since test-cascades 1.0.0
             */
            RecordCommandDropDownHandler(RecordCommand * const recorder,
                                         bb::cascades::DropDown * const dropDown,
                                         QObject * parent = 0):
                RecordCommandHandler(parent),
                recorder(recorder),
                dropDown(dropDown)
            {
                connect(this->dropDown,
                        SIGNAL(selectedOptionChanged
                               (bb::cascades::Option*)),  // NOLINT(readability/function)
                        SLOT(dropDownChange
                             (bb::cascades::Option*)));  // NOLINT(readability/function)
            }
            /*!
             * \brief ~RecordCommandDropDownHandler Destructor
             *
             * @since test-cascades 1.0.0
             */
            virtual ~RecordCommandDropDownHandler()
            {
            }
        protected:
        private:
            /*!
             * \brief recorder The recorder to notify
             */
            RecordCommand * const recorder;
            /*!
             * \brief dropDown The drop down list we're listening to
             */
            bb::cascades::DropDown * const dropDown;
        private slots:
            /*!
             * \brief dropDownChange Slot for when the drop down changes
             *
             * \param option The new option that's selected
             *
             * @since test-cascades 1.0.0
             */
            virtual void dropDownChange(bb::cascades::Option * option)
            {
                this->recorder->dropDownChanged(this->dropDown, option);
            }
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // RECORDCOMMANDDROPDOWNHANDLER_H_
