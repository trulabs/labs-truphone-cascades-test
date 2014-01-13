/**
 * Copyright 2013 Truphone
 */
#include "include/cli.h"
#include <QCoreApplication>
#if defined(QT_DEBUG)
#include <QDebug>
#endif
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QSemaphore>
#include <QStack>
#include "Buffer.h"
#include <QTcpSocket>

namespace truphone
{
namespace test
{
namespace cascades
{
namespace cli
{
    class HarnessCliPrviate : public QObject
    {
    public:
        HarnessCliPrviate(bool isRecord,
                          QFile * const inFile,
                          QFile * const outFile,
                          QObject * const parent);

        /*!
         * \brief STATE_NAMES The string names of all the states
         */
        static const char * STATE_NAMES[];
        /*!
         * \brief EVENT_NAMES The string names of all the events
         */
        static const char * EVENT_NAMES[];
        /*!
         * \brief SETTING_RETRY The name of the retry settings value
         */
        static const QString SETTING_RETRY;
        /*!
         * \brief SETTING_RETRY_DEFAULT Default setting for retries
         */
        static const QVariant SETTING_RETRY_DEFAULT;
        /*!
         * \brief SETTING_RETRY_INTERVAL Name of the retry internval setting
         */
        static const QString SETTING_RETRY_INTERVAL;
        /*!
         * \brief SETTING_RETRY_INTERVAL_DEFAULT Default value for interval
         */
        static const QVariant SETTING_RETRY_INTERVAL_DEFAULT;
        /*!
         * \brief SETTING_RETRY_MAX_TIME Name of the max retry time
         */
        static const QString SETTING_RETRY_MAX_INTERVALS;
        /*!
         * \brief SETTING_RETRY_MAX_TIME_DEFAULT Default max retry time
         */
        static const QVariant SETTING_RETRY_MAX_INTERVALS_DEFAULT;

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
            DISCONNECT,
            /*!
             * There was an error in the script
             */
            ERROR
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
        QFile * const rootFile;
        /*!
         * \brief output_file Output data
         */
        QFile * const outputFile;

        /*!
         * \brief currentFile The current file we're reading from
         */
        QFile * currentFile;

        /*!
         * \brief inputFiles Stack of input files
         */
        QStack<QFile*> * const inputFiles;

        /*!
         * \brief settings Settings for configuring the CLI.
         */
        QMap<QString, QVariant> * const settings;
        /*!
         * \brief lastCommandWritten A copy of the last command written out
         */
        Buffer lastCommandWritten;
        /*!
         * \brief retryTimer Timer for retries
         */
        QTimer * const retryTimer;
        /*!
         * \brief maxRetries The maximum number of retries to do
         */
        uint maxRetries;
        /*!
         * \brief retryCount The current number of retries
         */
        uint retryCount;

        /*!
         * \brief readNextLine Read the next line from the file.
         * If there's no file, try the file stack
         *
         * \param line A pointer to the next line
         *
         * \return The number of bytes read
         *
         * @since test-cascades 1.0.7
         */
        qint64 readNextLine(const Buffer * const line);

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
        void startRecording();
        /*!
         * \brief waitForCommandToRecord Wait for a response
         *
         * @since test-cascades 1.0.0
         */
        void waitForCommandToRecord();
        /*!
         * \brief transmitNextCommand Reads the next command from the disk
         * and transmits it
         *
         * @since test-cascades 1.0.0
         */
        void transmitNextCommand();
        /*!
         * \brief unexpectedTransition Record an unexpected state machine transition
         *
         * \param event The event that was unexpected
         *
         * @since test-cascades 1.0.0
         */
        void unexpectedTransition(const event_t event);
        /*!
         * \brief shutdown Shutdown everything
         *
         * @since test-cascades 1.0.0
         */
        void shutdown(const int exitCode = 0);
        /*!
         * \brief postEventToStateMachine Post an event to the
         * classes state machine
         *
         * \param event The event to post
         *
         * @since test-cascades 1.0.0
         */
        void postEventToStateMachine(const event_t event);
        /*!
         * \brief processSetting Process a setting string from the script
         *
         * \param setting The setting string to process.
         *
         * @since test-cascades 1.0.9
         */
        void processSetting(const char * const setting);
        /*!
         * \brief getSetting Get a setting
         *
         * \param key The setting to lookup
         * \param defaultValue The value to use if the setting value isn't known
         *
         * \return The value for the specified setting or the default value
         *
         * @since test-cascades 1.0.9
         */
        QVariant getSetting(const QString& key, const QVariant defaultValue);
        /*!
         * \brief disconnected Slot for disconnection
         *
         * @since test-cascades 1.0.0
         */
        void disconnected(void);
        /*!
         * \brief dataReady Slot for data being received from the target
         *
         * @since test-cascades 1.0.0
         */
        void dataReady(void);
        /*!
         * \brief retryTimeoutExpired Slot for the retry timeout expiring.
         * Means that we've not got a good response and need to try again.
         *
         * @since test-cascades 1.0.9
         */
        void retryTimeoutExpired(void);
    };

    const QString HarnessCliPrviate::SETTING_RETRY("retry");
    const QVariant HarnessCliPrviate::SETTING_RETRY_DEFAULT(0);
    const QString HarnessCliPrviate::SETTING_RETRY_INTERVAL("retry-interval");
    const QVariant HarnessCliPrviate::SETTING_RETRY_INTERVAL_DEFAULT(1000);
    const QString HarnessCliPrviate::SETTING_RETRY_MAX_INTERVALS("retry-max-intervals");
    const QVariant HarnessCliPrviate::SETTING_RETRY_MAX_INTERVALS_DEFAULT(30);

    const char * HarnessCliPrviate::STATE_NAMES[] =
    {
        "Waiting for Server",
        "Waiting for Reply",
        "Waiting for Recording to start",
        "Waiting for a Recorded Command",
        "Disconnected"
    };

    const char * HarnessCliPrviate::EVENT_NAMES[] =
    {
        "Received Initial Message",
        "Received Command Reply",
        "Received Record Command",
        "No more commands to play",
        "Disconnected",
        "Error"
    };

    HarnessCliPrviate::HarnessCliPrviate(
            bool isRecord,
            QFile * const inFile,
            QFile * const outFile,
            QObject * const parent)
        : QObject(parent),
          stateMachine(WAITING_FOR_SERVER),
          recordingMode(isRecord),
          stream(new QTcpSocket(this)),
          rootFile(inFile),
          outputFile(outFile),
          currentFile(rootFile),
          inputFiles(new QStack<QFile*>()),
          settings(new QMap<QString, QVariant>()),
          retryTimer(new QTimer(this))
    {
        inputFiles->push_back(rootFile);
    }

    HarnessCli::HarnessCli(QString host,
                           quint16 port,
                           bool isRecord,
                           QFile * const inFile,
                           QFile * const outFile,
                           QObject * parent)
        : QObject(parent),
          pData(new HarnessCliPrviate(isRecord, inFile, outFile, this))
    {
        connect(pData->retryTimer, SIGNAL(timeout()), SLOT(retryTimeoutExpired()));
        if (this->pData->stream)
        {
            bool ok;

            ok = connect(this->pData->stream,
                         SIGNAL(disconnected()),
                         this,
                         SLOT(disconnected()));
            if (ok)
            {
                const bool ok = connect(
                            this->pData->stream,
                            SIGNAL(readyRead()),
                            this,
                            SLOT(dataReady()));
                if (ok)
                {
                    this->pData->stream->connectToHost(host, port);

                    if (not this->pData->recordingMode)
                    {
                        this->pData->outputFile->write(
                                    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
                        this->pData->outputFile->write("<results>\r\n");
                    }
                }
            }
        }
    }

    HarnessCli::~HarnessCli()
    {
        if (this->pData->stream)
        {
            if (this->pData->stream->isOpen())
            {
                this->pData->stream->close();
            }
        }
        if (this->pData->inputFiles)
        {
            delete this->pData->inputFiles;
        }
        if (this->pData->settings)
        {
            delete this->pData->settings;
        }
    }

    qint64 HarnessCliPrviate::readNextLine(const Buffer * const line)
    {
        qint64 bytesRead = this->currentFile->readLine(
                    line->data(),
                    line->length());
        if (bytesRead <= 0)
        {
#if defined(QT_DEBUG)
        qDebug() << "readNextLine checking for another file on stack";
#endif  // QT_DEBUG
            if (not this->inputFiles->isEmpty())
            {
                this->inputFiles->pop();
                if (not this->inputFiles->isEmpty())
                {
                    currentFile = this->inputFiles->front();
                    bytesRead = this->currentFile->readLine(line->data(),
                                                            line->length());
#if defined(QT_DEBUG)
                    qDebug() << "readNextLine read" << bytesRead <<
                            "from" << this->currentFile->fileName();
#endif  // QT_DEBUG
                }
            }
        }

#if defined(QT_DEBUG)
        qDebug() << "readNextLine read" << bytesRead << "bytes";
#endif  // QT_DEBUG
        return bytesRead;
    }

    void HarnessCliPrviate::postEventToStateMachine(const event_t event)
    {
#if defined(QT_DEBUG)
        qDebug() << "------------------------------------";
        qDebug() << "## Current State: " \
                 << STATE_NAMES[this->stateMachine.state()] \
                 << " event: " \
                 << EVENT_NAMES[event];
#endif  // QT_DEBUG

        // process any transitions
        switch (this->stateMachine.state())
        {
        case WAITING_FOR_SERVER:
            switch (event)
            {
            case RECEIVED_INITIAL_MESSAGE:
                if (this->recordingMode)
                {
                    this->startRecording();
                }
                else
                {
                    this->transmitNextCommand();
                }
                break;
            case ERROR:
                this->shutdown(1);
                break;
            case DISCONNECT:
                this->shutdown();
                break;
            default:
                this->unexpectedTransition(event);
                break;
            }
            break;

        case WAITING_FOR_REPLY:
            switch (event)
            {
            case RECEIVED_COMMAND_REPLY:
                this->transmitNextCommand();
                break;
            case NO_MORE_COMMANDS_TO_PLAY:
                this->shutdown();
                break;
            case ERROR:
                this->shutdown(1);
                break;
            case DISCONNECT:
                this->outputFile->write("\t\t<fail terminated=\"true\"/>\r\n");
                this->shutdown();
                break;
            default:
                this->unexpectedTransition(event);
                break;
            }
            break;

        case WAITING_FOR_RECORDING_START:
            switch (event)
            {
            case RECEIVED_COMMAND_REPLY:
                this->waitForCommandToRecord();
                break;
            case DISCONNECT:
                this->shutdown();
                break;
            default:
                this->unexpectedTransition(event);
                break;
            }
            break;

        case WAITING_FOR_RECORDED_COMMAND:
            switch (event)
            {
            case RECEIVED_RECORD_COMMAND:
                this->waitForCommandToRecord();
                break;
            case DISCONNECT:
                this->shutdown();
                break;
            default:
                this->unexpectedTransition(event);
                break;
            }
            break;
        case DISCONNECTED:
            /* ignore all events */
            break;
        }
#if defined(QT_DEBUG)
        qDebug() << "## New State    : " \
                 << STATE_NAMES[this->stateMachine.state()];
#endif  // QT_DEBUG
    }

    void HarnessCliPrviate::unexpectedTransition(const event_t event)
    {
        qDebug() << "Unexpected state transition, State: "  \
                 << STATE_NAMES[this->stateMachine.state()] \
                 << ", event " \
                 << EVENT_NAMES[event];
    }

    void HarnessCliPrviate::shutdown(const int exitCode)
    {
        this->stateMachine.setState(DISCONNECTED);
        if (this->rootFile)
        {
            if (not this->recordingMode)
            {
                const qint64 bytesAvailable = this->rootFile->bytesAvailable();
                if (bytesAvailable > 0)
                {
                    this->outputFile->write("\t<command>\r\n");
                    this->outputFile->write("\t\t<request terminated=\"true\"/>\r\n");
                    this->outputFile->write("\t\t<fail terminated=\"true\" bytesLeftInFile=\"");
                    this->outputFile->write(QString::number(bytesAvailable).toUtf8().constData());
                    this->outputFile->write("\"/>\r\n");
                    this->outputFile->write("\t</command>\r\n");
                }
                this->outputFile->write("</results>\r\n");
            }
        }
        if (this->stream)
        {
            if (this->stream->isOpen())
            {
                this->stream->close();
            }
        }
        QCoreApplication::exit(exitCode);
    }

    void HarnessCliPrviate::startRecording()
    {
        this->stateMachine.setState(WAITING_FOR_RECORDING_START);
        this->stream->write("record\r\n");
    }

    void HarnessCliPrviate::waitForCommandToRecord()
    {
        this->stateMachine.setState(WAITING_FOR_RECORDED_COMMAND);
    }

    void HarnessCliPrviate::transmitNextCommand()
    {
        const Buffer outputBuffer;
        this->stateMachine.setState(WAITING_FOR_REPLY);

        qint64 bytesRead;
        if (this->retryTimer->isActive())
        {
            this->stream->write(this->lastCommandWritten.cdata(),
                                this->lastCommandWritten.strlen());
            this->outputFile->write("\t<retry count=\"");
            this->outputFile->write(QString::number(this->retryCount).toUtf8().constData());
            this->outputFile->write("\" command=\"");
            stripNl(this->lastCommandWritten.data(), this->lastCommandWritten.length());
            qDebug() << "RT" << this->lastCommandWritten.cdata();
            this->outputFile->write(this->lastCommandWritten.cdata());
            this->outputFile->write("\"/>\r\n");
        }
        else
        {
            bool waitingForReply = false;
            while ((bytesRead=this->readNextLine(&outputBuffer)) > 0)
            {
                const char * const raw = outputBuffer.cdata();
                if (raw[0] == '#')
                {
                    qDebug() << "CC" << QString(raw).trimmed();
                }
                else if (strncmp(raw, "call ", 5) == 0)
                {
                    QString filename(raw + 5);
                    filename = filename.trimmed();
                    QFile * const newFile = new QFile(filename, this);
                    if (not newFile->open(QIODevice::ReadOnly))
                    {
                        const QFileInfo info(*this->rootFile);
                        newFile->setFileName(
                                    info.path() +
                                    QDir::separator() +
                                    newFile->fileName());
                        if (not newFile->open(QIODevice::ReadOnly))
                        {
                            this->postEventToStateMachine(NO_MORE_COMMANDS_TO_PLAY);
                        }
                    }
                    this->inputFiles->push_back(newFile);
                    this->currentFile = newFile;
                }
                else if (strncmp(raw, "cli-setting ", 12) == 0)
                {
                    processSetting(raw);
                }
                else if (strcmp(raw, "\r\n") == 0 or strcmp(raw, "\n") == 0)
                {
                    qDebug() << "";
                }
                else
                {
                    lastCommandWritten.clean();
                    strncpy(lastCommandWritten.data(),
                            outputBuffer.cdata(),
                            bytesRead);
                    this->stream->write(outputBuffer.cdata(), bytesRead);
                    this->outputFile->write("\t<command request sent=\"");
                    stripNl(outputBuffer.data(), outputBuffer.length());
                    qDebug() << "<<" << outputBuffer.cdata();
                    this->outputFile->write(outputBuffer.cdata());
                    this->outputFile->write("\"/>\r\n");
                    waitingForReply = true;
                    break;
                }
            }
            if (not waitingForReply)
            {
                this->postEventToStateMachine(NO_MORE_COMMANDS_TO_PLAY);
            }
        }
    }

    void HarnessCli::disconnected(void)
    {
        this->pData->disconnected();
    }

    void HarnessCliPrviate::disconnected(void)
    {
        this->postEventToStateMachine(HarnessCliPrviate::DISCONNECT);
    }

    void HarnessCliPrviate::stripNl(char * const buffer, const size_t max_len)
    {
        const size_t slen = strnlen(buffer, max_len);
        for (size_t i = 0 ; i < slen ; i++)
        {
            if (buffer[i] == '\r' or buffer[i] == '\n')
            {
                buffer[i] = '\0';
            }
        }
    }

    void HarnessCliPrviate::processSetting(const char * const setting)
    {
        const QStringList tokens(QString(setting).trimmed().split(" "));
        if (tokens.size() == 3
                and tokens.at(0) == "cli-setting")
        {
#if defined(QT_DEBUG)
            qDebug() << "# CLI Setting set: " << tokens.at(1) << "=" << tokens.at(2);
#endif  // QT_DEBUG
            this->settings->insert(tokens.at(1), tokens.at(2));
        }
        else if (tokens.size() == 2 and tokens.at(0) == "cli-setting")
        {
#if defined(QT_DEBUG)
            qDebug() << "# CLI Setting delete: " << tokens.at(1) << "=" << tokens.at(2);
#endif  // QT_DEBUG
            this->settings->remove(tokens.at(1));
        }
    }

    QVariant HarnessCliPrviate::getSetting(const QString& key,
                                    const QVariant defaultValue)
    {
        return this->settings->value(key, defaultValue);
    }

    void HarnessCli::retryTimeoutExpired(void)
    {
        this->pData->retryTimeoutExpired();
    }

    void HarnessCliPrviate::retryTimeoutExpired(void)
    {
        this->transmitNextCommand();
    }

    void HarnessCli::dataReady(void)
    {
        this->pData->dataReady();
    }

    void HarnessCliPrviate::dataReady(void)
    {
        if (this->stream)
        {
            while (this->stream->bytesAvailable())
            {
                const Buffer inputBuffer;
                this->stream->readLine(inputBuffer.data(),
                                       inputBuffer.length());
                // don't strip the new lines from a recording buffer
                // as the buffer may be too big for the tcp frames
                // and get truncated into multiple packets and we'll
                // get truncated lines in the script file
                if (not this->recordingMode)
                {
                    stripNl(inputBuffer.data(), inputBuffer.length());
                }
                qDebug() << ">>" <<  inputBuffer.cdata();
                switch (this->stateMachine.state())
                {
                case WAITING_FOR_SERVER:
                    if (not this->recordingMode)
                    {
                        this->outputFile->write("\t<welcome message=\"");
                        this->outputFile->write(inputBuffer.cdata());
                        this->outputFile->write("\"/>\r\n");
                    }
                    this->postEventToStateMachine(RECEIVED_INITIAL_MESSAGE);
                    break;

                case WAITING_FOR_RECORDING_START:
                    this->postEventToStateMachine(RECEIVED_COMMAND_REPLY);
                    break;

                case WAITING_FOR_REPLY:
                {
                    const bool ok = strncmp(inputBuffer.cdata(), "OK", 2) == 0;
                    bool confirmedFailed = true;
                    if (ok)
                    {
                        // thats fine
                        this->outputFile->write("\t<pass recv=\"");
                        this->outputFile->write(inputBuffer.cdata());
                        this->outputFile->write("\"/>\r\n");
                    }
                    else
                    {
                        if (this->getSetting(SETTING_RETRY,
                                             SETTING_RETRY_DEFAULT).toInt())
                        {
#if defined(QT_DEBUG)
                            qDebug() << "Command failed, retries enabled...";
#endif  // QT_DEBUG
                            if (retryTimer->isActive())
                            {
#if defined(QT_DEBUG)
                                qDebug() << "Timer already active";
#endif  // QT_DEBUG
                                if (this->retryCount == this->maxRetries)
                                {
#if defined(QT_DEBUG)
                                    qDebug() << "Maximum number of retries";
#endif  // QT_DEBUG
                                    this->retryTimer->stop();
                                }
                                else
                                {
                                    this->retryCount++;
#if defined(QT_DEBUG)
                                    qDebug() << "Retry count increased to" << \
                                                this->retryCount << "/" << this->maxRetries;
#endif  // QT_DEBUG
                                    confirmedFailed = false;
                                }
                            }
                            else
                            {
                                this->retryTimer->setInterval(
                                            this->getSetting(
                                                SETTING_RETRY_INTERVAL,
                                                SETTING_RETRY_INTERVAL_DEFAULT).toInt());
#if defined(QT_DEBUG)
                                qDebug() << "First time, starting up retry timer with interval" \
                                         << this->retryTimer->interval();
#endif  // QT_DEBUG
                                this->maxRetries = this->getSetting(
                                            SETTING_RETRY_MAX_INTERVALS,
                                            SETTING_RETRY_MAX_INTERVALS_DEFAULT).toInt();
                                this->retryCount = 0;
                                this->retryTimer->setSingleShot(false);
                                this->retryTimer->start();
                                confirmedFailed = false;
                            }
                        }
                        if (confirmedFailed)
                        {
                            this->outputFile->write("\t<fail recv=\"");
                            this->outputFile->write(inputBuffer.cdata());
                            this->outputFile->write("\"/>\r\n");
                        }
                    }

                    if (ok or confirmedFailed)
                    {
                        this->retryTimer->stop();
                        if (ok)
                        {
                            this->postEventToStateMachine(RECEIVED_COMMAND_REPLY);
                        }
                        else
                        {
                            this->postEventToStateMachine(ERROR);
                        }
                    }
                    break;
                }
                case WAITING_FOR_RECORDED_COMMAND:
                    this->currentFile->write(inputBuffer.cdata());
                    this->currentFile->flush();

                    this->postEventToStateMachine(RECEIVED_RECORD_COMMAND);
                    break;
                default:
                    break;
                }
            }
        }
    }
}  // namespace cli
}  // namespace cascades
}  // namespace test
}  // namespace truphone
