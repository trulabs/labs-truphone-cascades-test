/**
 * Copyright 2013 Truphone
 */
#include "include/ContactsCommand.h"

#include <QString>
#include <QStringList>
#include <bb/pim/contacts/ContactService>
#include <bb/pim/contacts/Contact>
#include <bb/pim/contacts/ContactBuilder>
#include <bb/pim/contacts/ContactAttributeBuilder>
#include <bb/pim/contacts/ContactSearchFilters>

#include "include/Connection.h"
#include "include/Buffer.h"
#include "include/Utils.h"

using bb::pim::contacts::ContactService;
using bb::pim::contacts::Contact;
using bb::pim::contacts::ContactId;
using bb::pim::contacts::ContactBuilder;
using bb::pim::contacts::ContactAttributeBuilder;
using bb::pim::contacts::AttributeKind;
using bb::pim::contacts::AttributeSubKind;
using bb::pim::contacts::ContactSearchFilters;
using bb::pim::contacts::ContactAttribute;

namespace truphone
{
namespace test
{
namespace cascades
{
    const QString ContactsCommand::CMD_NAME = "contacts";

    ContactsCommand::ContactsCommand(Connection * const connection, QObject * parent)
        : Command(parent),
          connection(connection),
          contactService(new ContactService(this)),
          listDelim(new Buffer(",")),
          settingDelim(new Buffer("="))
    {
    }

    ContactsCommand::~ContactsCommand()
    {
        delete listDelim;
        delete settingDelim;
    }

    bool ContactsCommand::findContact(const QString name, Contact * const contact)
    {
        bool ret = false;

        ContactSearchFilters options;
        options.setIncludeAttribute(AttributeKind::Name);
        options.setSearchValue(name);
        options.setLimit(1);
        const int contactCount = contactService->count(options);
        if (contactCount)
        {
            QList<Contact> contacts = contactService->searchContacts(options);
            *contact = contacts.first();
            ret = true;
        }

        return ret;
    }

    bool ContactsCommand::executeCommand(QStringList*const args)
    {
        bool ret = false;

        if (args->size() > 0)
        {
            const QString subCommand = args->first();
            if (not subCommand.isEmpty())
            {
                args->removeFirst();
                if (subCommand == "create")
                {
                    ret = createContact(connection, args);
                }
                else if (subCommand == "delete")
                {
                    ret = deleteContact(connection, args);
                }
                else if (subCommand == "modify")
                {
                    ret = modifyContact(connection, args);
                }
                else
                {
                    connection->write("ERROR: Unknown subcommand\n");
                }
            }
            else
            {
                connection->write("The subcommand seems to be empty\n");
            }
        }
        else
        {
            connection->write("You need to specify at least the subcommand\n");
        }

        return ret;
    }

    void ContactsCommand::showHelp(void)
    {
        this->connection->write("You can use this command to change contacts\n");
        this->connection->write("You can 'create' and 'delete' contacts\n");
        this->connection->write("You can also modify existing contacts\n");
        this->connection->write("The format is as such:\n");
        this->connection->write("contacts create forename=x, mobile=y, email=z, home=oops\n");
        this->connection->write("contact modify x, mobile=90210, email=me@email.com, home\n");
        this->connection->write("contacts delete x\n");
        this->connection->write("Empty modify variables unset the variable\n");
    }

    bool ContactsCommand::createContact(
            Connection * const connection,
            QStringList * const args)
    {
        bool ret = false;

        int count = args->size();
        if (count)
        {
            bool add = true;
            int attrs = 0;
            ContactBuilder contactBuilder;
            const QString line = args->join(" ");
            const Buffer lineBuffer(line.toUtf8().constData());
            QStringList settings = Utils::tokenise(listDelim, &lineBuffer, false);

            Q_FOREACH(const QString setting, settings)
            {
                const Buffer buffer(setting.toUtf8().constData());
                QStringList params = Utils::tokenise(settingDelim, &buffer);
                if (params.size() == 3)
                {
                    const QString varName = params.first();
                    params.removeFirst();
                    params.removeFirst();
                    const QString varVal = params.first();
                    params.removeFirst();

                    if (varName=="forename")
                    {
                        ContactAttributeBuilder nameAttributeBuilder;
                        nameAttributeBuilder.setKind(AttributeKind::Name)
                                            .setSubKind(AttributeSubKind::NameGiven)
                                            .setValue(varVal);
                        contactBuilder.addAttribute(nameAttributeBuilder);
                    }
                    else if (varName=="surname")
                    {
                        ContactAttributeBuilder nameAttributeBuilder;
                        nameAttributeBuilder.setKind(AttributeKind::Name)
                                            .setSubKind(AttributeSubKind::NameSurname)
                                            .setValue(varVal);
                        contactBuilder.addAttribute(nameAttributeBuilder);
                    }
                    else if (varName=="mobile")
                    {
                        ContactAttributeBuilder phoneAttributeBuilder;
                        phoneAttributeBuilder.setKind(AttributeKind::Phone)
                                              .setSubKind(AttributeSubKind::PhoneMobile)
                                              .setValue(varVal);
                        contactBuilder.addAttribute(phoneAttributeBuilder);
                    }
                    else if (varName=="work")
                    {
                        ContactAttributeBuilder phoneAttributeBuilder;
                        phoneAttributeBuilder.setKind(AttributeKind::Phone)
                                              .setSubKind(AttributeSubKind::Work)
                                              .setValue(varVal);
                        contactBuilder.addAttribute(phoneAttributeBuilder);
                    }
                    else if (varName=="home")
                    {
                        ContactAttributeBuilder phoneAttributeBuilder;
                        phoneAttributeBuilder.setKind(AttributeKind::Phone)
                                              .setSubKind(AttributeSubKind::Home)
                                              .setValue(varVal);
                        contactBuilder.addAttribute(phoneAttributeBuilder);
                    }
                    else if (varName=="other")
                    {
                        ContactAttributeBuilder phoneAttributeBuilder;
                        phoneAttributeBuilder.setKind(AttributeKind::Phone)
                                              .setSubKind(AttributeSubKind::Other)
                                              .setValue(varVal);
                        contactBuilder.addAttribute(phoneAttributeBuilder);
                    }
                    else if (varName=="email")
                    {
                        ContactAttributeBuilder emailAttributeBuilder;
                        emailAttributeBuilder.setKind(AttributeKind::Email)
                                              .setSubKind(AttributeSubKind::Personal)
                                              .setValue(varVal);
                        contactBuilder.addAttribute(emailAttributeBuilder);
                    }
                    else
                    {
                        add = false;
                        connection->write(QString("ERROR: " + varName + " isn't supported\n").
                                          toUtf8().constData());
                    }

                    attrs++;
                }
                else
                {
                    add = false;
                    connection->write(QString("ERROR: Parameter " + QString::number(attrs + 1) +
                                              " needs to be x=y\n").toUtf8().constData());
                    break;
                }
            }

            if (add)
            {
                if (attrs)
                {
                    const Contact createdContact = contactService->createContact(
                                contactBuilder, true);
                    const ContactId id = createdContact.id();
                    if (id)
                    {
                        ret = true;
                    }
                    else
                    {
                        connection->write("ERROR: Failed to add the contact to the address book\n");
                    }
                }
                else
                {
                    connection->write("ERROR: Didn't parse any parameters for the new contact\n");
                }
            }
        }
        else
        {
            connection->write("ERROR: No arguments specified\n");
        }

        return ret;
    }

    bool ContactsCommand::deleteContact(
            Connection * const connection,
            QStringList * const args)
    {
        bool ret = false;

        if (args->size() > 0)
        {
            const QString var = args->join(" ");
            Contact contact;
            const bool found = findContact(var, &contact);
            if (found)
            {
                contactService->deleteContact(contact.id());
                ret = true;
            }
            else
            {
                connection->write("ERROR: Couldn't find any contacts with that name");
            }
        }
        else
        {
            connection->write("ERROR: Only one parameter needed");
        }

        return ret;
    }

    bool ContactsCommand::modifyContact(
            Connection * const connection,
            QStringList * const args)
    {
        bool ret = false;

        const QString line = args->join(" ");
        const Buffer buffer(line.toUtf8().constData());
        QStringList params = Utils::tokenise(listDelim, &buffer, false);

        if (params.size() > 1)
        {
            const QString name = params.first();
            params.removeFirst();
            Contact contact;
            const bool found = findContact(name, &contact);
            if (found)
            {
                bool modify = true;
                contact = contactService->contactDetails(contact.id());
                ContactBuilder contactBuilder = contact.edit();
                Q_FOREACH(const QString setting, params)
                {
                    const Buffer buffer(setting.toUtf8().constData());
                    QStringList params = Utils::tokenise(settingDelim, &buffer);
                    if (params.size() == 1)
                    {
                        // remove the option
                        const QString varName = params.first().trimmed();
                        params.removeFirst();
                        if (varName=="forename")
                        {
                            modify = removeAttribute(&contact,
                                                     &contactBuilder,
                                                     AttributeKind::Name,
                                                     AttributeSubKind::NameGiven);
                        }
                        else if (varName=="surname")
                        {
                            modify = removeAttribute(&contact,
                                                     &contactBuilder,
                                                     AttributeKind::Name,
                                                     AttributeSubKind::NameSurname);
                        }
                        else if (varName=="mobile")
                        {
                            modify = removeAttribute(&contact,
                                                     &contactBuilder,
                                                     AttributeKind::Phone,
                                                     AttributeSubKind::PhoneMobile);
                        }
                        else if (varName=="work")
                        {
                            modify = removeAttribute(&contact,
                                                     &contactBuilder,
                                                     AttributeKind::Phone,
                                                     AttributeSubKind::Work);
                        }
                        else if (varName=="home")
                        {
                            modify = removeAttribute(&contact,
                                                     &contactBuilder,
                                                     AttributeKind::Phone,
                                                     AttributeSubKind::Home);
                        }
                        else if (varName=="other")
                        {
                            modify = removeAttribute(&contact,
                                                     &contactBuilder,
                                                     AttributeKind::Phone,
                                                     AttributeSubKind::Other);
                        }
                        else if (varName=="email")
                        {
                            modify = removeAttribute(&contact,
                                                     &contactBuilder,
                                                     AttributeKind::Email,
                                                     AttributeSubKind::Personal);
                        }
                        else
                        {
                            modify = false;
                            connection->write(QString("ERROR: " + varName + " isn't supported\n").
                                              toUtf8().constData());
                        }
                    }
                    else if (params.size() == 3)
                    {
                        // add/change the option
                        const QString varName = params.first().trimmed();
                        params.removeFirst();
                        params.removeFirst();
                        const QString varVal = params.first().trimmed();
                        params.removeFirst();

                        if (varName=="forename")
                        {
                            modify = removeAttribute(&contact,
                                                     &contactBuilder,
                                                     AttributeKind::Name,
                                                     AttributeSubKind::NameGiven);
                            ContactAttributeBuilder nameAttributeBuilder;
                            nameAttributeBuilder.setKind(AttributeKind::Name)
                                                .setSubKind(AttributeSubKind::NameGiven)
                                                .setValue(varVal);
                            contactBuilder = contactBuilder.addAttribute(nameAttributeBuilder);
                        }
                        else if (varName=="surname")
                        {
                            modify = removeAttribute(&contact,
                                                     &contactBuilder,
                                                     AttributeKind::Name,
                                                     AttributeSubKind::NameSurname);
                            ContactAttributeBuilder nameAttributeBuilder;
                            nameAttributeBuilder.setKind(AttributeKind::Name)
                                                .setSubKind(AttributeSubKind::NameSurname)
                                                .setValue(varVal);
                            contactBuilder = contactBuilder.addAttribute(nameAttributeBuilder);
                        }
                        else if (varName=="mobile")
                        {
                            ContactAttributeBuilder phoneAttributeBuilder;
                            phoneAttributeBuilder.setKind(AttributeKind::Phone)
                                                  .setSubKind(AttributeSubKind::PhoneMobile)
                                                  .setValue(varVal);
                            contactBuilder = contactBuilder.addAttribute(phoneAttributeBuilder);
                        }
                        else if (varName=="work")
                        {
                            ContactAttributeBuilder phoneAttributeBuilder;
                            phoneAttributeBuilder.setKind(AttributeKind::Phone)
                                                  .setSubKind(AttributeSubKind::Work)
                                                  .setValue(varVal);
                            contactBuilder = contactBuilder.addAttribute(phoneAttributeBuilder);
                        }
                        else if (varName=="home")
                        {
                            ContactAttributeBuilder phoneAttributeBuilder;
                            phoneAttributeBuilder.setKind(AttributeKind::Phone)
                                                  .setSubKind(AttributeSubKind::Home)
                                                  .setValue(varVal);
                            contactBuilder = contactBuilder.addAttribute(phoneAttributeBuilder);
                        }
                        else if (varName=="other")
                        {
                            ContactAttributeBuilder phoneAttributeBuilder;
                            phoneAttributeBuilder.setKind(AttributeKind::Phone)
                                                  .setSubKind(AttributeSubKind::Other)
                                                  .setValue(varVal);
                            contactBuilder = contactBuilder.addAttribute(phoneAttributeBuilder);
                        }
                        else if (varName=="email")
                        {
                            ContactAttributeBuilder emailAttributeBuilder;
                            emailAttributeBuilder.setKind(AttributeKind::Email)
                                                  .setSubKind(AttributeSubKind::Personal)
                                                  .setValue(varVal);
                            contactBuilder = contactBuilder.addAttribute(emailAttributeBuilder);
                        }
                        else
                        {
                            modify = false;
                            connection->write(QString("ERROR: " + varName + " isn't supported\n")
                                              .toUtf8().constData());
                        }
                    }
                    else
                    {
                        connection->write("ERROR: Unknown setting configuration\n");
                    }
                }
                if (modify)
                {
                    contact = contactService->updateContact(contactBuilder);
                    ret = true;
                }
            }
            else
            {
                connection->write("ERROR: Failed to find the contact\n");
            }
        }
        else
        {
            connection->write("ERROR: Need at least a user and a parameter to change\n");
        }

        return ret;
    }

    bool ContactsCommand::removeAttribute(
            const Contact* const contact,
            ContactBuilder* const builder,
            const AttributeKind::Type kind,
            const AttributeSubKind::Type subKind)
    {
        bool ret = false;

        const QList<ContactAttribute> attrs =  contact->attributes();
        Q_FOREACH(const ContactAttribute attr, attrs)
        {
            if ((attr.kind() == kind) && (attr.subKind() == subKind))
            {
                *builder = builder->deleteAttribute(attr);
                ret = true;
            }
        }

        return ret;
    }
}
}
}
