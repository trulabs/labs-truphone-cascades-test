/**
 * Copyright 2013 Truphone
 */
#include <QCoreApplication>
#include "include/cli.h"

int main(int argc, char *argv[])
{
    bool isRecord = false;

    QCoreApplication a(argc, argv);

    if (argc < 4)
    {
        qFatal("test-cascades-cli <host> <port> <test-file> --record");
        qFatal("----------------------------------------------------");
        qFatal("test-cascades-cli is the command line interface to the target");
        qFatal("You need to specify the host & port to connect to and a test file");
        qFatal("Optionally you can append '--record' in which case the script will");
        qFatal("be over-written with the events that occur & are transmitted from");
        qFatal("the application.");
        return -1;
    }

    if (argc >= 5)
    {
        isRecord = (strncmp("--record", argv[4], 8) == 0);
    }

    QFile testFile(argv[3], &a);
    QFile outputFile(QString(argv[3]) + ".xml", &a);
    QFile * pOutputFile = NULL;
    if (not isRecord)
    {
        if (not testFile.exists())
        {
            qFatal("Test file doesn't exist");
            return -2;
        }

        testFile.open(QIODevice::ReadOnly);
        if (not testFile.isOpen())
        {
            qFatal("Test file can't be opened");
            return -3;
        }
        if (not testFile.isReadable())
        {
            qFatal("Test file isn't readable");
            return -4;
        }

        pOutputFile = &outputFile;
        if (outputFile.exists())
        {
            outputFile.remove();
        }
        outputFile.open(QIODevice::WriteOnly bitor QIODevice::Text);
        if (not outputFile.isOpen())
        {
            qFatal("Failed to open the output file");
            return -5;
        }

        if (not outputFile.isWritable())
        {
            qFatal("Output file isn't writable");
            return -6;
        }
    }
    else
    {
        if (testFile.exists())
        {
            qWarning("Test file already exists and will be over-written");
        }

        testFile.open(QIODevice::WriteOnly bitor QIODevice::Text);
        if (not testFile.isOpen())
        {
            qFatal("Failed to open the script file for recording");
            return -7;
        }

        if (not testFile.isWritable())
        {
            qFatal("Recording script file isn't writable");
            return -6;
        }
    }

    new truphone::test::cascades::cli::HarnessCli(
            argv[1],
            QString(argv[2]).toUShort(),
            isRecord,
            &testFile,
            pOutputFile,
            &a);

    return a.exec();
}
