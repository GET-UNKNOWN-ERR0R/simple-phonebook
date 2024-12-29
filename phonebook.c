#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 15
#define FILE_NAME "phonebook.dat"

typedef struct {
    char name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
} Contact;

// Function prototypes
void addContact();
void viewContacts();
void updateContact();
void deleteContact();

int main() {
    int choice;

    do {
        printf("\n--- Phonebook ---\n");
        printf("1. Add Contact\n");
        printf("2. View Contacts\n");
        printf("3. Update Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addContact(); break;
            case 2: viewContacts(); break;
            case 3: updateContact(); break;
            case 4: deleteContact(); break;
            case 5: printf("Exiting the program.\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

void addContact() {
    FILE *file = fopen(FILE_NAME, "ab");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    Contact contact;
    printf("Enter name: ");
    getchar(); 
    fgets(contact.name, MAX_NAME_LEN, stdin);
    contact.name[strcspn(contact.name, "\n")] = '\0'; 

    printf("Enter phone number: ");
    fgets(contact.phone, MAX_PHONE_LEN, stdin);
    contact.phone[strcspn(contact.phone, "\n")] = '\0'; 

    fwrite(&contact, sizeof(Contact), 1, file);
    fclose(file);
    printf("Contact added successfully!\n");
}

void viewContacts() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    Contact contact;
    printf("\n--- Contacts ---\n");
    while (fread(&contact, sizeof(Contact), 1, file)) {
        printf("Name: %s, Phone: %s\n", contact.name, contact.phone);
    }

    fclose(file);
}

void updateContact() {
    FILE *file = fopen(FILE_NAME, "rb+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char name[MAX_NAME_LEN];
    printf("Enter the name of the contact to update: ");
    getchar(); 
    fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0'; 

    Contact contact;
    int found = 0;
    long position;

    while (fread(&contact, sizeof(Contact), 1, file)) {
        if (strcmp(contact.name, name) == 0) {
            found = 1;
            position = ftell(file) - sizeof(Contact);
            break;
        }
    }

    if (found) {
        fseek(file, position, SEEK_SET);

        printf("Enter new phone number: ");
        fgets(contact.phone, MAX_PHONE_LEN, stdin);
        contact.phone[strcspn(contact.phone, "\n")] = '\0'; 

        fwrite(&contact, sizeof(Contact), 1, file);
        printf("Contact updated successfully!\n");
    } else {
        printf("Contact not found.\n");
    }

    fclose(file);
}

void deleteContact() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    FILE *tempFile = fopen("temp.dat", "wb");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        fclose(file);
        return;
    }

    char name[MAX_NAME_LEN];
    printf("Enter the name of the contact to delete: ");
    getchar();
    fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0'; 

    Contact contact;
    int found = 0;

    while (fread(&contact, sizeof(Contact), 1, file)) {
        if (strcmp(contact.name, name) != 0) {
            fwrite(&contact, sizeof(Contact), 1, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found) {
        printf("Contact deleted successfully!\n");
    } else {
        printf("Contact not found.\n");
    }
}
