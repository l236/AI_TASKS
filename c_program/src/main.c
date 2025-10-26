/*
 * main.c
 * Main program entry point and UI loop
 */

#include "../include/contact_book.h"

void print_menu(void) {
    printf("\n================================\n");
    printf("1. Add Contact\n");
    printf("2. Search Contacts\n");
    printf("3. List All Contacts\n");
    printf("4. Delete Contact\n");
    printf("5. View Statistics\n");
    printf("0. Exit\n");
    printf("================================\n");
}

void init_contact_book(ContactBook *book) {
    book->count = 0;
    book->next_id = 1;
}

void cleanup_contact_book(ContactBook *book) {
    /* Currently no dynamic memory to free */
    (void)book; /* Suppress unused parameter warning */
}

int main(void) {
    int choice;
    ContactBook book;
    
    printf("=== Contact Book Management System ===\n\n");
    
    /* Initialize contact book */
    init_contact_book(&book);
    
    /* Load existing contacts from file */
    load_contacts(&book);
    
    while (1) {
        print_menu();
        choice = get_int_input("Enter your choice: ", 0, 5);
        
        switch (choice) {
            case 1:
                add_contact(&book);
                save_contacts(&book);
                break;
            case 2:
                search_contacts(&book);
                break;
            case 3:
                list_all_contacts(&book);
                break;
            case 4:
                delete_contact(&book);
                save_contacts(&book);
                break;
            case 5:
                view_statistics(&book);
                break;
            case 0:
                printf("\nSaving contacts...\n");
                save_contacts(&book);
                printf("Thank you for using Contact Book. Goodbye!\n");
                cleanup_contact_book(&book);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        
        wait_for_enter();
    }
    
    return 0;
}
