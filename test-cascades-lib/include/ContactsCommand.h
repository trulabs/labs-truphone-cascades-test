/**
 * Copyright 2013 Truphone
 */
#ifndef CONTACTSCOMMAND_H_
#define CONTACTSCOMMAND_H_

#include <bb/pim/contacts/ContactAttribute.hpp>

#include "include/Command.h"
#include "include/Buffer.h"

namespace bb
{
    namespace pim
    {
        namespace contacts
        {
            class ContactService;
            class Contact;
            class ContactBuilder;
        }  // namespace contacts
    }  // namespace pim
}  // namespace bb

namespace truphone
{
namespace test

{
namespace cascades
{
    /*!
     * \brief The ContactsCommand class is used to change the native
     * address book.
     *
     * @since test-cascades 1.0.1
     */
    class ContactsCommand : public Command
    {
    public:
        /*!
         * \brief getCmd Return the name of this command
         *
         * \return Command name
         *
         * @since test-cascades 1.0.1
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
         * @since test-cascades 1.0.1
         */
        static Command* create(class Connection * const s,
                               QObject * parent = 0)
        {
            return new ContactsCommand(s, parent);
        }
        /*!
         * \brief ContactsCommand Constructor
         *
         * \param connection The socket associated with the client
         * \param parent The parent object
         *
         * @since test-cascades 1.0.1
         */
        ContactsCommand(class Connection * const connection, QObject * parent = NULL);

        /*!
         * \brief ~ContactsCommand Destructor
         *
         * @since test-cascades 1.0.1
         */
        ~ContactsCommand();
        /*
         * See super
         */
        bool executeCommand(QStringList* const args);
        /*
         * See super
         */
        void showHelp(void);
    protected:
        /*!
         * \brief findContact For a given name, find and return a native contact
         *
         * \param name The name of the contact
         * \param contact The contact, if found
         * \return True if the contact was found and contact is valid
         *
         * @since test-cascades 1.0.1
         */
        bool findContact(const QString name, class bb::pim::contacts::Contact * const contact);
        /*!
         * \brief createContact Create a new native contact
         *
         * \param connection The connection to report errors on
         * \param args The arguments of the contact to create
         * \return True if the contact was created ok
         *
         * @since test-cascades 1.0.1
         */
        bool createContact(class Connection * const connection, QStringList * const args);
        /*!
         * \brief deleteContact Delete a native contact
         *
         * \param connection The connection to report errors on
         * \param args The arguments of the contact to delete
         * \return True if the contact was found and removed
         *
         * @since test-cascades 1.0.1
         */
        bool deleteContact(class Connection * const connection, QStringList * const args);
        /*!
         * \brief modifyContact Change the attributes of an existing contact
         *
         * \param connection The connection to report errors on
         * \param args The arguments to change
         * \return True if the contact was modified successfully
         *
         * @since test-cascades 1.0.1
         */
        bool modifyContact(class Connection * const connection, QStringList * const args);
        /*!
         * \brief removeAttribute Completely remove an attribute from a contact
         *
         * \param contact The contact (full details)
         * \param builder The current builder being used to edit the contact
         * \param kind The 'kind' of attribute to remove
         * \param subKind The 'subKind' of attribute to remove
         * \return True if the attribute was removed
         *
         * @since test-cascades 1.0.1
         */
        bool removeAttribute(
                const bb::pim::contacts::Contact* const contact,
                bb::pim::contacts::ContactBuilder* const builder,
                const bb::pim::contacts::AttributeKind::Type kind,
                const bb::pim::contacts::AttributeSubKind::Type subKind);
    private:
        /*!
         * \brief CMD_NAME The name of this command
         */
        static const QString CMD_NAME;
        /*!
         * \brief connection The connection/socket to report issues on
         */
        class Connection * const connection;
        /*!
         * \brief contactService The native contact service to make modifications with
         */
        bb::pim::contacts::ContactService * const contactService;
        /*!
         * \brief listDelim The list delimiter for settings (,)
         */
        const Buffer * listDelim;
        /*!
         * \brief settingDelim The delimiter for settings (=)
         */
        const Buffer * settingDelim;
    };
}  // namespace cascades
}  // namespace test
}  // namespace truphone

#endif  // CONTACTSCOMMAND_H_
