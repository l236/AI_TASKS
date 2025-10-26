/*
 * contact_book.h - Header file for Contact Book application
 * Defines data structures and function prototypes
 */

#ifndef CONTACT_BOOK_H
#define CONTACT_BOOK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

/* Constants */
#define MAX_NAME 100
#define MAX_PHONE 20
#define MAX_EMAIL 100
#define MAX_CONTACTS 1000
#define FILENAME "contacts.txt"

/* Data Structures */
typedef struct {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
    int id;
} Contact;

typedef struct {
    Contact contacts[MAX_CONTACTS];
    int count;
    int next_id;
} ContactBook;

/* Function Prototypes */

/* Menu and UI */
void print_menu(void);
void wait_for_enter(void);
int get_int_input(const char *prompt, int min, int max);

/* Contact Operations */
void add_contact(ContactBook *book);
void search_contacts(const ContactBook *book);
void list_all_contacts(const ContactBook *book);
void delete_contact(ContactBook *book);
void view_statistics(const ContactBook *book);

/* File Operations */
void load_contacts(ContactBook *book);
void save_contacts(const ContactBook *book);

/* Utility Functions */
void trim_string(char *str);
void to_lowercase(char *str);

/* Initialize/Cleanup */
void init_contact_book(ContactBook *book);
void cleanup_contact_book(ContactBook *book);

#endif /* CONTACT_BOOK_H */

