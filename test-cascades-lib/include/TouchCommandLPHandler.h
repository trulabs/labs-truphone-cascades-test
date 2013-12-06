/**
 * Copyright 2013 Truphone
 */
#ifndef TOUCHCOMMANDLPHANDLER_H_
#define TOUCHCOMMANDLPHANDLER_H_

#include <QObject>
#include <QTimer>
#include <bb/cascades/VisualNode>

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The TouchCommandLPHandler class is used to handle long presses
     *
     * @since test-cascades 1.0.0
     */
    class TouchCommandLPHandler : public QObject
    {
    Q_OBJECT
    public:
        /*!
         * \brief TouchCommandLPHandler Create a new long press handler
         *
         * \param node The visual node we're listening to
         * \param parent The parent object
         *
         * @since test-cascades 1.0.0
         */
        explicit TouchCommandLPHandler(bb::cascades::VisualNode * node,
                                       QObject * parent = 0)
            : QObject(parent),
              visualNode(node),
              expired(false)
        {
            this->timer.setSingleShot(true);
            this->timer.setInterval(500);
            const bool connected = connect(&this->timer,
                    SIGNAL(timeout()),
                    SLOT(onTimeout()));
            if (connected)
            {
                this->timer.start();
            }
        }
        /*!
         * \brief ~TouchCommandLPHandler Destructor
         *
         * @since test-cascades 1.0.0
         */
        ~TouchCommandLPHandler()
        {
            this->cancel();
        }
        /*!
         * \brief cancel Cancel the timer on the long press listener
         *
         * @since test-cascades 1.0.0
         */
        void cancel()
        {
            timer.stop();
        }
        /*!
         * \brief hasExpired Returns whether the timer has expired or not
         *
         * \return @c true if the timer has expired (i.e. it's a long press)
         *
         * @since test-cascades 1.0.0
         */
        bool hasExpired() const
        {
            return this->expired;
        }
    protected slots:
        /*!
         * \brief onTimeout Slot for when the timer expires
         *
         * @since test-cascades 1.0.0
         */
        void onTimeout()
        {
            this->expired = true;
            bool fired = QMetaObject::invokeMethod(
                    this->visualNode,
                    "longClicked");
            if (not fired)
            {
                fired = QMetaObject::invokeMethod(
                        this->visualNode->parent(),
                        "longClicked");
            }
            if (not fired)
            {
                fired = QMetaObject::invokeMethod(
                        this->visualNode,
                        "longPressed");
            }
            if (not fired)
            {
                QMetaObject::invokeMethod(
                            this->visualNode->parent(),
                            "longPressed");
            }
        }
    private:
        /*!
         * \brief timer The long press timer
         */
        QTimer timer;
        /*!
         * \brief visualNode The visual node to listen to
         */
        bb::cascades::VisualNode * const visualNode;
        /*!
         * \brief expired State for the expiry of the timer
         */
        bool expired;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone
#endif  // TOUCHCOMMANDLPHANDLER_H_
