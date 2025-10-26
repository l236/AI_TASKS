/*
 * contact_operations.c
 * Implementation of contact management operations
 */

#include "../include/contact_book.h"

void add_contact(ContactBook *book) {
    if (book->count >= MAX_CONTACTS) {
        printf("\nError: Maximum number of contacts reached (%d)\n", MAX_CONTACTS);
        return;
    }
    
    Contact new_contact;
    char input[MAX_NAME];
    
    printf("\n=== Add New Contact ===\n");
    
    /* Get name */
    printf("Enter name: ");
    fgets(input, MAX_NAME, stdin);
    trim_string(input);
    if (strlen(input) == 0) {
        printf("Error: Name cannot be empty.\n");
        return;
    }
    strncpy(new_contact.name, input, MAX_NAME - 1);
    new_contact.name[MAX_NAME - 1] = '\0';
    
    /* Get phone */
    printf("Enter phone number: ");
    fgets(input, MAX_PHONE, stdin);
    trim_string(input);
    strncpy(new_contact.phone, input, MAX_PHONE - 1);
    new_contact.phone[MAX_PHONE - 1] = '\0';
    
    /* Get email */
    printf("Enter email: ");
    fgets(input, MAX_EMAIL, stdin);
    trim_string(input);
    strncpy(new_contact.email, input, MAX_EMAIL - 1);
    new_contact.email[MAX_EMAIL - 1] = '\0';
    
    /* Assign ID and add to array */
    new_contact.id = book->next_id++;
    book->contacts[book->count++] = new_contact;
    
    printf("\n✓ Contact added successfully! (ID: %d)\n", new_contact.id);
}

void search_contacts(const ContactBook *book) {
    if (book->count == 0) {
        printf("\nNo contacts to search.\n");
        return;
    }
    
    char query[MAX_NAME];
    int found = 0;
    
    printf("\n=== Search Contacts ===\n");
    printf("Enter search term: ");
    fgets(query, MAX_NAME, stdin);
    trim_string(query);
    
    if (strlen(query) == 0) {
        printf("Error: Search term cannot be empty.\n");
        return;
    }
    
    char lower_query[MAX_NAME];
    strcpy(lower_query, query);
    to_lowercase(lower_query);
    
    printf("\nSearch results for '%s':\n", query);
    printf("---------------------------------------\n");
    
    for (int i = 0; i < book->count; i++) {
        char lower_name[MAX_NAME];
        strcpy(lower_name, book->contacts[i].name);
        to_lowercase(lower_name);
        
        if (strstr(lower_name, lower_query) != NULL ||
            strstr(book->contacts[i].phone, query) != NULL ||
            strstr(book->contacts[i].email, query) != NULL) {
            
            printf("[ID: %d]\n", book->contacts[i].id);
            printf("Name:  %s\n", book->contacts[i].name);
            printf("Phone: %s\n", book->contacts[i].phone);
            printf("Email: %s\n", book->contacts[i].email);
            printf("---------------------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No contacts found.\n");
    } else {
        printf("Found %d contact(s).\n", found);
    }
}

void list_all_contacts(const ContactBook *book) {
    if (book->count == 0) {
        printf("\nNo contacts in the book.\n");
        return;
    }
    
    printf("\n=== All Contacts ===\n");
    printf("Total: %d contact(s)\n\n", book->count);
    printf("---------------------------------------\n");
    
    for (int i = 0; i < book->count; i++) {
        printf("[ID: %d]\n", book->contacts[i].id);
        printf("Name:  %s\n", book->contacts[i].name);
        printf("Phone: %s\n", book->contacts[i].phone);
        printf("Email: %s\n", book->contacts[i].email);
        printf("---------------------------------------\n");
    }
}

void delete_contact(ContactBook *book) {
    if (book->count == 0) {
        printf("\nNo contacts to delete.\n");
        return;
    }
    
    list_all_contacts(book);
    int id = get_int_input("\nEnter contact ID to delete: ", 1, INT_MAX);
    
    int found = 0;
    for (int i = 0; i < book->count; i++) {
        if (book->contacts[i].id == id) {
            found = 1;
            printf("\nDeleting contact:\n");
            printf("Name:  %s\n", book->contacts[i].name);
            printf("Phone: %s\n", book->contacts[i].phone);
            printf("Email: %s\n", book->contacts[i].email);
            
            /* Shift array to remove contact */
            for (int j = i; j < book->count - 1; j++) {
                book->contacts[j] = book->contacts[j + 1];
            }
            book->count--;
            
            printf("\n✓ Contact deleted successfully.\n");
            break;
        }
    }
    
    if (!found) {
        printf("\nContact with ID %d not found.\n", id);
    }
}

void view_statistics(const ContactBook *book) {
    printf("\n=== Statistics ===\n");
    printf("Total contacts: %d\n", book->count);
    printf("Available slots: %d\n", MAX_CONTACTS - book->count);
    printf("Next contact ID: %d\n", book->next_id);
}

