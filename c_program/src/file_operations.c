/*
 * file_operations.c
 * File I/O operations for contact persistence
 */

#include "../include/contact_book.h"

void load_contacts(ContactBook *book) {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        /* File doesn't exist yet, start fresh */
        return;
    }
    
    book->count = 0;
    book->next_id = 1;
    
    while (book->count < MAX_CONTACTS && fscanf(fp, "%d\n", &book->contacts[book->count].id) == 1) {
        fgets(book->contacts[book->count].name, MAX_NAME, fp);
        trim_string(book->contacts[book->count].name);
        
        fgets(book->contacts[book->count].phone, MAX_PHONE, fp);
        trim_string(book->contacts[book->count].phone);
        
        fgets(book->contacts[book->count].email, MAX_EMAIL, fp);
        trim_string(book->contacts[book->count].email);
        
        if (book->contacts[book->count].id >= book->next_id) {
            book->next_id = book->contacts[book->count].id + 1;
        }
        
        book->count++;
    }
    
    fclose(fp);
}

void save_contacts(const ContactBook *book) {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("Error: Could not save contacts to file.\n");
        return;
    }
    
    for (int i = 0; i < book->count; i++) {
        fprintf(fp, "%d\n", book->contacts[i].id);
        fprintf(fp, "%s\n", book->contacts[i].name);
        fprintf(fp, "%s\n", book->contacts[i].phone);
        fprintf(fp, "%s\n", book->contacts[i].email);
    }
    
    fclose(fp);
}

