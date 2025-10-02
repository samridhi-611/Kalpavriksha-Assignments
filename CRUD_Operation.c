#include <stdio.h>
#include <stdlib.h>

// structure for user records
typedef struct {
    int id;
    char name[50];
    int age;
} User;

#define FILENAME "users.txt"

// Function declarations 
void check();
void createUser();  
void readUsers();    
void updateUser();   
void deleteUser();  

int main() {
    int choice;
    check();  // Ensure the file exists

    while (1) {
        printf("\n--- User Record System ---\n");
        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createUser(); break;
            case 2: readUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: 
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice, try again.\n");
        }
    }
    return 0;
}

// Ensure the file exists
void check() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Error creating file.\n");
        exit(1);
    }
    fclose(file);
}

// Create: Add a new user
void createUser() {
    FILE *file = fopen(FILENAME, "r");
    User u;
    int newId, exists = 0;

    printf("Enter User ID: ");
    scanf("%d", &newId);

    // Check if ID already exists
    while (fscanf(file, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        if (u.id == newId) {
            exists = 1;
            break;
        }
    }
    fclose(file);

    if (exists) {
        printf("Error: A user with ID %d already exists.\n", newId);
        return;
    }

    // Add new user
    file = fopen(FILENAME, "a");
    u.id = newId;
    printf("Enter Name: ");
    scanf("%s", u.name);
    printf("Enter Age: ");
    scanf("%d", &u.age);

    fprintf(file, "%d %s %d\n", u.id, u.name, u.age);
    fclose(file);

    printf("User added successfully.\n");
}


// Read: Display all users
void readUsers() {
    FILE *file = fopen(FILENAME, "r");
    User u;
    int found = 0;

    printf("\n--- List of Users ---\n");
    while (fscanf(file, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        printf("ID: %d, Name: %s, Age: %d\n", u.id, u.name, u.age);
        found = 1;
    }
    fclose(file);

    if (!found) {
        printf("No users found.\n");
    }
}

// Update: Modify user by ID
void updateUser() {
    FILE *file = fopen(FILENAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    User u;
    int userId, found = 0;

    printf("Enter ID of user to update: ");
    scanf("%d", &userId);

    while (fscanf(file, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        if (u.id == userId) {
            found = 1;
            printf("Enter new Name: ");
            scanf("%s", u.name);
            printf("Enter new Age: ");
            scanf("%d", &u.age);
        }
        fprintf(tempFile, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(file);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found) {
        printf("User updated successfully.\n");
    } else {
        printf("User not found.\n");
    }
}

// Delete: Remove a user by ID
void deleteUser() {
    FILE *file = fopen(FILENAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    User u;
    int userId, found = 0;

    printf("Enter ID of user to delete: ");
    scanf("%d", &userId);

    while (fscanf(file, "%d %s %d", &u.id, u.name, &u.age) == 3) {
        if (u.id == userId) {
            found = 1;
            continue; // skip this record
        }
        fprintf(tempFile, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(file);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found) {
        printf("User deleted successfully.\n");
    } else {
        printf("User not found.\n");
    }
}
