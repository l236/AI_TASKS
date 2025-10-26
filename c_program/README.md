# Contact Book - A C Programming Example

A command-line contact management system written in C that demonstrates:
- Data structures (structs)
- File I/O operations
- String manipulation
- Memory management
- User input validation
- Modular program design

## Features

- ✨ **Add Contacts**: Store name, phone, and email
- 🔍 **Search Contacts**: Find by name, phone, or email
- 📋 **List All Contacts**: View all saved contacts
- 🗑️ **Delete Contacts**: Remove contacts by ID
- 📊 **Statistics**: View total contact count
- 💾 **Persistent Storage**: Contacts saved to file automatically

## Compilation

### Using Make (recommended)
```bash
make
```

### Manual compilation
```bash
gcc -Wall -Wextra -std=c99 -O2 -o contact_book contact_book.c
```

## Running the Program

```bash
make run
```

or

```bash
./contact_book
```

## Usage

1. **Add Contact** - Select option 1 and enter contact details
2. **Search** - Select option 2 and enter search term
3. **List All** - Select option 3 to view all contacts
4. **Delete** - Select option 4 and enter contact ID
5. **Statistics** - Select option 5 to view contact count
6. **Exit** - Select option 0 to save and exit

## Data Storage

Contacts are automatically saved to `contacts.txt` in the program directory. The file is created automatically on first run.

## Project Structure

```
c_program/
├── include/
│   └── contact_book.h      # Header file with definitions
├── src/
│   ├── main.c              # Main program and UI loop
│   ├── contact_operations.c  # Add, search, list, delete
│   ├── file_operations.c   # Load/save from file
│   └── utils.c             # Utility functions
├── Makefile                # Build configuration
├── README.md               # This file
└── contact_book            # Compiled executable
```

### Modular Architecture

The project follows best practices with:
- **Separation of concerns**: Each module handles a specific responsibility
- **Header files**: `.h` files define interfaces between modules
- **Compilation units**: Each `.c` file compiles to an object file (`.o`)
- **Linking**: Object files are linked together to create the final executable

This makes the code:
- More maintainable
- Easier to test
- Follows C best practices
- Similar to professional projects

## Code Features Demonstrated

### Data Structure
```c
typedef struct {
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
    int id;
} Contact;
```

### Key Functions
- `add_contact()` - Adds new contacts with input validation
- `search_contacts()` - Case-insensitive search functionality
- `delete_contact()` - Removes contacts and shifts array
- `save_contacts()` / `load_contacts()` - File persistence
- `trim_string()` - Removes whitespace
- `to_lowercase()` - Case-insensitive search support

## Clean Build

Remove compiled files:
```bash
make clean
```

## Learning Points

This program teaches:
1. **Struct Usage**: Organizing related data
2. **File Operations**: Reading/writing data for persistence
3. **String Handling**: Safe string manipulation in C
4. **Input Validation**: Ensuring safe user input
5. **Array Management**: Adding and removing elements
6. **Modular Design**: Separating concerns into functions

## Platform Support

- ✅ macOS
- ✅ Linux
- ✅ Windows (with MinGW or WSL)

---

*Great for learning C programming fundamentals!*

