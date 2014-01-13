/**
 * Copyright 2013 Truphone
 */
#ifndef CLI_H_
#define CLI_H_

#include <QFile>

namespace truphone
{
namespace test
{
namespace cascades
{
namespace cli
{
    /*!
     * Internal data
     */
    class HarnessCliPrviate;
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
             * \brief privateData Private data for the CLI class.
             */
            HarnessCliPrviate * const pData;
    private slots:
    private slots:
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
}  // namespace cli
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // CLI_H_
