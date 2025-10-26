/*
 * utils.c
 * Utility functions
 */

#include "../include/contact_book.h"

void trim_string(char *str) {
    int start = 0, end = strlen(str) - 1;
    
    /* Remove leading whitespace */
    while (isspace((unsigned char)str[start])) {
        start++;
    }
    
    /* Remove trailing whitespace */
    while (end >= start && isspace((unsigned char)str[end])) {
        end--;
    }
    
    /* Shift string to beginning */
    int i;
    for (i = 0; i <= end - start; i++) {
        str[i] = str[start + i];
    }
    str[i] = '\0';
}

void to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int get_int_input(const char *prompt, int min, int max) {
    int value;
    char input[20];
    
    while (1) {
        printf("%s", prompt);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }
        
        char *endptr;
        value = (int)strtol(input, &endptr, 10);
        
        if (*endptr != '\n' && *endptr != '\0' && *endptr != ' ') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value < min || value > max) {
            printf("Please enter a number between %d and %d.\n", min, max);
            continue;
        }
        
        return value;
    }
}

void wait_for_enter(void) {
    printf("\nPress Enter to continue...");
    getchar();
}

