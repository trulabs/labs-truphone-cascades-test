/**
 * Copyright 2013 Truphone
 */
#ifndef LISTCOMMAND_H_
#define LISTCOMMAND_H_

#include <bb/cascades/ListView>
#include <QVariantList>

#include "Command.h"

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The List class is used to check and manipulate lists
     *
     * @since test-cascades 1.0.5
     */
    class ListCommand : public Command
    {
    Q_OBJECT
    public:
        /*!
         * \brief getCmd Return the name of this command
         *
         * \return Command name
         *
         * @since test-cascades 1.0.5
         */
        static QString getCmd()
        {
            return CMD_NAME;
        }
        /*!
         * \brief create Create a new instance of this Command
         *
         * \param s The TCP socket associated with the client
         * \param parent The parent object
         * \return Returns a new instance of the Command
         *
         * @since test-cascades 1.0.5
         */
        static Command* create(class Connection * const s,
                               QObject * parent = 0)
        {
            return new ListCommand(s, parent);
        }
        /*!
         * \brief ListCommand Constructor
         *
         * \param socket The TCP socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.5
         */
        ListCommand(class Connection * const socket,
                      QObject* parent = 0);
        /*!
         * \brief ~ListCommand Destructor
         *
         * @since test-cascades 1.0.5
         */
        ~ListCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList * const rguments);
        /*
         * See super
         */
        void cleanUp(void)
        {
            /* do nothing */
        }
        /*
         * See super
         */
        void showHelp(void);
    protected slots:
    private:
        /*!
         * \brief findElementByIndex Find an index path based on numeric index
         *
         * \param index The index (i.e. 0~1~2)
         * \param elementIndexPath The result index path
         *
         * \return @c true if the find was successful
         *
         * @since test-cascades 1.0.5
         */
        bool findElementByIndex(
                const QString& index,
                QVariantList& elementIndexPath) const;

        /*!
         * \brief findElementByName Find an index path based on a name
         *
         * \param list The list to work on
         * \param index The named index (i.e. "S~Name=Sam^" or "S~Sam")
         * \param elementIndexPath The result index path
         *
         * \return @c true if the find was successful
         *
         * @since test-cascades 1.0.5
         */
        bool findElementByName(
                bb::cascades::ListView * const list,
                const QString& index,
                QVariantList& elementIndexPath) const;

        /*!
         * \brief checkElement Check if an element matches the expected value
         *
         * \param element The element to check
         * \param check The check value we wish to use or @c NULL if it should be empty
         *
         * \return @c true if it's correct
         *
         * @since test-cascades 1.0.5
         */
        bool checkElement(
                const QVariant element,
                const QString& check = 0) const;

        /*!
         * \brief normalisePath Removed @c namedPathEnd from the end of paths
         *
         * \param value The value that's changed
         * \param endOfPath The end of path
         *
         * @since test-cascades 1.0.5
         */
        static void normalisePath(
                QString * const value,
                const QString& endOfPath);

        /*!
         * \brief extractNamedPath Iterate over arguments building up
         * a list of separated elements until the @c endOfPath is seen
         *
         * \param arguments The arguments to parse
         * \param endOfPath The end of path variable to look for
         *
         * \return A string for the full path
         *
         * @since test-cascades 1.0.5
         */
        static QString extractNamedPath(
                QStringList * const arguments,
                const QString& endOfPath);

        /*!
         * \brief selectUnselectPath (Un)Select a path
         *
         * \param arguments The arguments for the selection
         * \param listView The list view to work on
         * \param select @c true to select, @c false to unselect
         *
         * \return @c true if the selection works
         *
         * @since test-cascades 1.0.5
         */
        bool selectUnselectPath(
                QStringList * const arguments,
                bb::cascades::ListView * const listView,
                const bool select);

        /*!
         * \brief tapPath Execute (onTriggered) for an item
         * as if it was being clicked on.
         *
         * \param arguments The arguments for the tap
         * \param listView The list view to work on
         *
         * \return
         *
         * @since test-cascades 1.0.7
         */
        bool tapPath(
                QStringList * const arguments,
                bb::cascades::ListView * const listView);

        /*!
         * \brief tapPath Execute (selectionChanged) for an item
         * as if it was being clicked on and held (long-press).
         *
         * \param arguments The arguments for the tap
         * \param listView The list view to work on
         *
         * \return
         *
         * @since test-cascades 1.0.10
         */
        bool holdPath(
                QStringList * const arguments,
                bb::cascades::ListView * const listView,
                const bool select = true);

        /*!
         * \brief scrollToPath Scrolls to a known path
         *
         * \param arguments The arguments used to scroll to
         * \param listView The list view to work on
         *
         * \return @c true if the scoll works
         *
         * @since test-cascades 1.0.5
         */
        bool scrollToPath(
                QStringList * const arguments,
                bb::cascades::ListView * const listView);

        /*!
         * \brief showKeysOnPath Show the keys for a paths QVaraintMap
         *
         * \param arguments The arguments to use
         * \param listView The list view to work on
         *
         * \return @c true if the lookup works and element is a map
         *
         * @since test-cascades 1.0.5
         */
        bool showKeysOnPath(
                QStringList * const arguments,
                bb::cascades::ListView * const listView);

        /*!
         * \brief convertPathToIndex Convert a path to an index
         *
         * \param arguments The arguments (path) to process
         * \param listView The ListView to use to navigate
         * \param index The resulting index
         *
         * \return @c True if it works
         *
         * @since test-cascades 1.0.18
         */
        bool convertPathToIndex(
                QStringList * const arguments,
                bb::cascades::ListView * const listView,
                QVariantList &index);

        /*!
         * \brief CMD_NAME The name of this command
         */
        static const QString CMD_NAME;
        /*!
         * \brief client The TCP socket associated with the client
         */
        class Connection * const client;
        /*!
         * @brief namedPathEnd Path end marker
         */
        const QString namedPathEnd;
        /*!
         * \brief namedPathSep Path/Index separator
         */
        const QString namedPathSep;
        /*!
         * \brief assignSep Assignment separator
         */
        const QString assignSep;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // LISTCOMMAND_H_
