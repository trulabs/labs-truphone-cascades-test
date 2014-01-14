/**
 * Copyright 2013 Truphone
 */
#ifndef UTILS_H_
#define UTILS_H_

#include <QObject>
#include <QList>

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The Utils class contains a number of utility functions we use
     *
     * @since test-cascades 1.0.0
     */
    class Utils
    {
    public:
        /*!
         * \brief tokenise Take a string in a buffer and tokenise it
         * using @c delim as the delimiters
         *
         * \param delim A string containing all the characters that act
         * as delimiters
         * \param buffer The string that we want to tokenise
         * \param includeDelim Include the delimiter as a token
         * \return A list of all the String tokens including the delimiters as
         * their own tokens
         *
         * @since test-cascades 1.0.0
         */
        static QStringList tokenise(const QString& delim,
                                    const QString& buffer,
                                    const bool includeDelim = true);

        // from Hooq

        /*!
         * \brief objectPath Get a string representing the path to an Object
         *
         * \param obj The object to generate the path for
         * \return A string representing the path to that object
         *
         * @since test-cascades 1.0.0
         */
        static const QString objectPath(const QObject * const obj);

        /*!
         * \brief findObject Given a string path to an object, return
         * a pointer to that object
         *
         * \param path The path to the object
         * \return A pointer to an @c QObject or @c NULL if it couldn't
         * be found
         *
         * @since test-cascades 1.0.0
         */
        static QObject* findObject(const QString& path);
    protected:
        /*!
         * \brief isDelim Works out if a char is a delimiter
         *
         * \param delim The buffer of all the delimiters
         * \param c The char we're checking
         * \return @c true if @c c is in @c delim
         *
         * @since test-cascades 1.0.0
         */
        static bool isDelim(const QString& delim,
                            const char c);
        // from Hooq

        /*!
         * \brief objectName Generate a string for an object name
         *
         * \param obj The object
         * \return The string for its name
         *
         * @since test-cascades 1.0.0
         */
        static const QString objectName(const QObject * const obj);

        /*!
         * \brief rawObjectName Return the raw name for an object
         *
         * \param obj The object to generate the name for
         * \return A string representing the raw name of @c obj
         *
         * @since test-cascades 1.0.0
         */
        static const QString rawObjectName(const QObject * const obj);
    private:
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // UTILS_H_
