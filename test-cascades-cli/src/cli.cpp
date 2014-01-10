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

namespace truphone
{
namespace test
{
namespace cascades
{
namespace cli
{

const char * HarnessCli::STATE_NAMES[] =
{
    "Waiting for Server",
    "Waiting for Reply",
    "Waiting for Recording to start",
    "Waiting for a Recorded Command",
    "Disconnected"
};

const char * HarnessCli::EVENT_NAMES[] =
{
    "Received Initial Message",
    "Received Command Reply",
    "Received Record Command",
    "No more commands to play",
    "Disconnected",
    "Error"
};

    HarnessCli::HarnessCli(QString host,
                           quint16 port,
                           bool isRecord,
                           QFile * const inFile,
                           QFile * const outFile,
                           QObject * parent)
        : QObject(parent),
          stateMachine(WAITING_FOR_SERVER),
          recordingMode(isRecord),
          stream(new QTcpSocket(this)),
          rootFile(inFile),
          outputFile(outFile),
          currentFile(rootFile),
          inputFiles(new QStack<QFile*>())
    {
        inputFiles->push_back(rootFile);
        if (this->stream)
        {
            bool ok;

            ok = connect(this->stream,
                         SIGNAL(connected()),
                         SLOT(connectedOk()));
            if (ok)
            {
                ok = connect(this->stream,
                             SIGNAL(disconnected()),
                             SLOT(disconnected()));
                if (ok)
                {
                    this->stream->connectToHost(host, port);

                    if (not this->recordingMode)
                    {
                        this->outputFile->write(
                                    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
                        this->outputFile->write("<results>\r\n");
                    }
                }
            }
        }
    }

    HarnessCli::~HarnessCli()
    {
        if (this->stream)
        {
            if (this->stream->isOpen())
            {
                this->stream->close();
            }
        }
        if (this->inputFiles)
        {
            delete this->inputFiles;
        }
    }

    void HarnessCli::connectedOk(void)
    {
        if (this->stream)
        {
            const bool ok = connect(this->stream,
                         SIGNAL(readyRead()),
                         SLOT(dataReady()));
            if (not ok)
            {
                this->stream->close();
            }
        }
    }

    qint64 HarnessCli::readNextLine(const Buffer * const line)
    {
        qint64 bytesRead = this->currentFile->readLine(line->data(),
                                                     line->length());
        if (bytesRead <= 0)
        {
#if defined(QT_DEBUG)
        qDebug() << "readNextLine checking for another file on stack";
#endif
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
                }
#endif
            }
        }

#if defined(QT_DEBUG)
        qDebug() << "readNextLine read" << bytesRead << "bytes";
#endif
        return bytesRead;
    }

    void HarnessCli::postEventToStateMachine(const event_t event)
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

    void HarnessCli::unexpectedTransition(const event_t event)
    {
        qDebug() << "Unexpected state transition, State: "  \
                 << STATE_NAMES[this->stateMachine.state()] \
                 << ", event " \
                 << EVENT_NAMES[event];
    }

    void HarnessCli::shutdown(const int exitCode)
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

    void HarnessCli::startRecording()
    {
        this->stateMachine.setState(WAITING_FOR_RECORDING_START);
        this->stream->write("record\r\n");
    }

    void HarnessCli::waitForCommandToRecord()
    {
        this->stateMachine.setState(WAITING_FOR_RECORDED_COMMAND);
    }

    void HarnessCli::transmitNextCommand()
    {
        const Buffer outputBuffer;
        this->stateMachine.setState(WAITING_FOR_REPLY);

        qint64 bytesRead = this->readNextLine(&outputBuffer);

        if (bytesRead <= 0)
        {
            this->postEventToStateMachine(NO_MORE_COMMANDS_TO_PLAY);
        }

        while (bytesRead > 0)
        {
            if (bytesRead > 0)
            {
                const char * const raw = outputBuffer.cdata();
                if (raw[0] == '#')
                {
                    qDebug() << "CC" << QString(raw).trimmed();
                    bytesRead = readNextLine(&outputBuffer);
                    if (bytesRead <= 0)
                    {
                        this->postEventToStateMachine(NO_MORE_COMMANDS_TO_PLAY);
                    }
                }
                else if (strncmp(raw, "exec ", 5) == 0)
                {

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
                    bytesRead = readNextLine(&outputBuffer);
                    if (bytesRead <= 0)
                    {
                        this->postEventToStateMachine(NO_MORE_COMMANDS_TO_PLAY);
                    }
                }
                else if (strcmp(raw, "\r\n") == 0 or strcmp(raw, "\n") == 0)
                {
                    qDebug() << "";
                    bytesRead = readNextLine(&outputBuffer);
                    if (bytesRead <= 0)
                    {
                        this->postEventToStateMachine(NO_MORE_COMMANDS_TO_PLAY);
                    }
                }
                else
                {
#if defined(QT_DEBUG)
                    qDebug() << "transmitNextCommand writing" << bytesRead << "bytes";
#endif
                    this->stream->write(outputBuffer.cdata(), bytesRead);
                    this->outputFile->write("\t<command>\r\n");
                    this->outputFile->write("\t\t<request sent=\"");
                    stripNl(outputBuffer.data(), outputBuffer.length());
                    qDebug() << "<<" << outputBuffer.cdata();
                    this->outputFile->write(outputBuffer.cdata());
                    this->outputFile->write("\"/>\r\n");
                    break;
                }
            }
            else
            {
                this->postEventToStateMachine(NO_MORE_COMMANDS_TO_PLAY);
            }
        }
    }

    void HarnessCli::disconnected(void)
    {
        this->postEventToStateMachine(DISCONNECT);
    }

    void HarnessCli::stripNl(char * const buffer, const size_t max_len)
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

    void HarnessCli::dataReady(void)
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
                    if (ok)
                    {
                        // thats fine
                        this->outputFile->write("\t\t<pass recv=\"");
                        this->outputFile->write(inputBuffer.cdata());
                        this->outputFile->write("\"/>\r\n");
                    }
                    else
                    {
                        this->outputFile->write("\t\t<fail recv=\"");
                        this->outputFile->write(inputBuffer.cdata());
                        this->outputFile->write("\"/>\r\n");
                    }
                    this->outputFile->write("\t</command>\r\n");

                    if (ok)
                    {
                        this->postEventToStateMachine(RECEIVED_COMMAND_REPLY);
                    }
                    else
                    {
                        this->postEventToStateMachine(ERROR);
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
