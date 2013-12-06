/**
 * Copyright 2013 Truphone
 */
#ifndef CLI_H_
#define CLI_H_

#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>
#include <QSemaphore>
#include "Buffer.h"

namespace truphone
{
namespace test
{
namespace cascades
{
namespace cli
{
    /*!
     * \brief The HarnessCli class is responsible for
     * processing script files and recording the results to
     * XML files. It can also handle the recording of events
     * from the target.
     *
     * @since test-cascades 1.0.0
     */
    class HarnessCli : public QObject
    {
        Q_OBJECT
        public:
            /*!
             * \brief HarnessCli Create a new CLI
             *
             * \param host The address of the target
             * \param port The port number to connect to on @c host
             * \param isRecord @c true if we intend to record rather than replay
             * \param inFile The input file to send to @c host
             * \param outFile The output file that we record the XML results in
             * \param parent The parent object
             *
             * @since test-cascades 1.0.0
             */
            HarnessCli(QString host,
                       quint16 port,
                       bool isRecord,
                       QFile * const inFile,
                       QFile * const outFile = NULL,
                       QObject * parent = 0);
            /*!
             * \brief ~HarnessCli Destructor
             *
             * @since test-cascades 1.0.0
             */
            virtual ~HarnessCli();
        protected:
        private:
            /*!
             * \brief STATE_NAMES The string names of all the states
             */
            static const char * STATE_NAMES[];
            /*!
             * \brief EVENT_NAMES The string names of all the events
             */
            static const char * EVENT_NAMES[];

            /*!
             * The states the CLI can be in
             */
            typedef enum state
            {
                /*!
                 * Waiting for the initial message
                 * from the server
                 */
                WAITING_FOR_SERVER,
                /*!
                 * Waiting for a replay to a playback command
                 */
                WAITING_FOR_REPLY,
                /*!
                 * Waiting for a message to say recording started
                 */
                WAITING_FOR_RECORDING_START,
                /*!
                 * Waiting for an incoming recording command
                 */
                WAITING_FOR_RECORDED_COMMAND,
                /*!
                 * The disconnected state
                 */
                DISCONNECTED
            } state_t;

            /*!
             * state machine events
             */
            typedef enum event
            {
                /*!
                 * The initial hello from the server was received
                 */
                RECEIVED_INITIAL_MESSAGE,
                /*!
                 * Transmitted a command to the server
                 */
                RECEIVED_COMMAND_REPLY,
                /*!
                 * Recorded a command
                 */
                RECEIVED_RECORD_COMMAND,
                /*!
                 * Run out of commands to playback
                 */
                NO_MORE_COMMANDS_TO_PLAY,
                /*!
                 * Disconnected
                 */
                DISCONNECT
            } event_t;

            /*!
             * The state machine
             */
            typedef struct stateMachine
            {
            public:
                stateMachine(const state_t initial)
                    : initialState(initial),
                      currentState(initialState)
                {
#if defined (QT_DEBUG)
                    qDebug() << "## Initialising state machine to "
                             << STATE_NAMES[currentState];
#endif  // QT_DEBUG
                }
                inline state_t state()
                {
                    return this->currentState;
                }
                inline void setState(const state_t state)
                {
#if defined (QT_DEBUG)
                    qDebug() << "## State changing from "
                             << STATE_NAMES[currentState]
                             << " to "
                             << STATE_NAMES[state];
#endif  // QT_DEBUG
                    this->currentState = state;
                }

            private:
                const state_t initialState;
                state_t currentState;
            } stateMachine_t;

            /*!
             * \brief stateMachine The state machine for the cli
             */
            stateMachine_t stateMachine;

            /*!
             * \brief recordingMode @c True if this client is a recording client
             */
            const bool recordingMode;
            /*!
             * \brief stream The TCP socket connection to the target
             */
            QTcpSocket * const stream;
            /*!
             * \brief input_file Input data
             */
            QFile * const inputFile;
            /*!
             * \brief output_file Output data
             */
            QFile * const outputFile;

            /*!
             * \brief stripNl Strip the new lines from a buffer
             *
             * \param buffer The buffer to strip
             * \param max_len The maximum length of the buffer
             *
             * @since test-cascades 1.0.0
             */
            static void stripNl(char * const buffer, const size_t max_len);
            /*!
             * \brief startRecording Send the recording command to the server
             *
             * @since test-cascades 1.0.0
             */
            virtual void startRecording();
            /*!
             * \brief waitForCommandToRecord Wait for a response
             *
             * @since test-cascades 1.0.0
             */
            virtual void waitForCommandToRecord();
            /*!
             * \brief transmitNextCommand Reads the next command from the disk
             * and transmits it
             *
             * @since test-cascades 1.0.0
             */
            virtual void transmitNextCommand();
            /*!
             * \brief unexpectedTransition Record an unexpected state machine transition
             *
             * \param event The event that was unexpected
             *
             * @since test-cascades 1.0.0
             */
            virtual void unexpectedTransition(const event_t event);
            /*!
             * \brief shutdown Shutdown everything
             *
             * @since test-cascades 1.0.0
             */
            virtual void shutdown();
            /*!
             * \brief postEventToStateMachine Post an event to the
             * classes state machine
             *
             * \param event The event to post
             *
             * @since test-cascades 1.0.0
             */
            virtual void postEventToStateMachine(const event_t event);
        private slots:
            /*!
             * \brief disconnected Slot for disconnection
             *
             * @since test-cascades 1.0.0
             */
            virtual void disconnected(void);
            /*!
             * \brief connectedOk Slot for connection to the target
             *
             * @since test-cascades 1.0.0
             */
            virtual void connectedOk(void);
            /*!
             * \brief dataReady Slot for data being received from the target
             *
             * @since test-cascades 1.0.0
             */
            virtual void dataReady(void);
    };
}  // namespace cli
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // CLI_H_
