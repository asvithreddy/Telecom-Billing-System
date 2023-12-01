#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Customer {
    char name[50];
    char phno[15];
    float usage;
    float totalbill;
    struct Customer* next;
    struct Customer* prev;
};
struct rechargeplan {
    char name[50];
    float rate;
};

struct Customer* head = NULL; // Head of the doubly linked list
struct rechargeplan rechargeplans[4] = {
    {"Basic Plan", 0.1},
    {"Premium Plan", 0.15},
    {"Unlimited Plan", 0.2},
    {"Family Plan",0.5}
};
void saveRecordsToFile() {
    FILE* file = fopen("call_records.txt", "w");  // Use "a" for append mode
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    struct Customer* current = head;
    while (current != NULL) {
        fprintf(file, "%s %s %.2f %.2f\n", current->name, current->phno, current->usage, current->totalbill);
        current = current->next;
    }
    fclose(file);
}
void loadRecordsFromFile() {
    FILE* file = fopen("call_records.txt", "r");
    if (file == NULL) {
        printf("No previous records found.\n");
        return;
    }
    struct Customer temp;
    while (fscanf(file, "%s %s %f %f", temp.name, temp.phno, &temp.usage, &temp.totalbill) == 4) {
        struct Customer* newcustomer = (struct Customer*)malloc(sizeof(struct Customer));
        if (newcustomer == NULL) {
            printf("Memory allocation error.\n");
            exit(1);
        }

        memcpy(newcustomer, &temp, sizeof(struct Customer));

        newcustomer->next = head;
        newcustomer->prev = NULL;

        if (head != NULL) {
            head->prev = newcustomer;
        }
        head = newcustomer;
    }

    fclose(file);
}
// Function to add a new customer record
void addRecord() {
    char tempUsername[50];
    char tempphno[15];
    while (1) {
        printf("\nEnter username: ");
        scanf("%s", tempUsername);

        int valid = 1;

        // Validate that the username contains only alphabets
        for (int i = 0; tempUsername[i] != '\0'; i++) {
            if (!isalpha(tempUsername[i])) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            break;
        } else {
            printf("\nInvalid username! Please enter a username with alphabets only.\n");
        }
    }
    while (1) {
        printf("\nEnter 10-digit phone number: ");
        scanf("%s", tempphno);
        if (strlen(tempphno) == 10) {
            break;  // Exit the loop if the entered number is 10 digits
        } else {
            printf("\nInvalid phone number! Please enter a 10-digit number.\n");
        }
    }
    struct Customer* current = head;
    while (current != NULL) {
        if (strcmp(current->phno, tempphno) == 0) {
            printf("\nA record with this phone number already exists!\n");
            return;
        }
        current = current->next;
    }
    struct Customer* newcustomer = (struct Customer*)malloc(sizeof(struct Customer));
    if (newcustomer == NULL) {
        printf("\nCannot add a new user\n");
        exit(1);
    }

    strcpy(newcustomer->phno, tempphno);
    strcpy(newcustomer->name,tempUsername);

    printf("Enter usage (in minutes): ");
    scanf("%f", &newcustomer->usage);

    // Ask the user to choose a recharge plan
    printf("\nChoose a recharge plan:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, rechargeplans[i].name);
    }

    int rechargeplanchoice;
    printf("Enter your choice (1-4): ");
    scanf("%d", &rechargeplanchoice);

    if (rechargeplanchoice >= 1 && rechargeplanchoice <= 4) {
        // Update the total bill based on the chosen recharge plan
        newcustomer->totalbill = newcustomer->usage * rechargeplans[rechargeplanchoice - 1].rate;
        printf("Bill updated based on %s plan.\n", rechargeplans[rechargeplanchoice - 1].name);
    } else {
        printf("Invalid recharge plan choice. Defaulting to Basic Plan.\n");
        // Default to Basic Plan if an invalid choice is entered
        newcustomer->totalbill = newcustomer->usage * rechargeplans[0].rate;
    }

    newcustomer->next = head;
    newcustomer->prev = NULL;

    if (head != NULL) {
        head->prev = newcustomer;
    }

    head = newcustomer;

    printf("\nRecord added successfully!\n");

    saveRecordsToFile();
}
// Function to view the list of customer records
void viewRecords() {
    struct Customer* current = head;
    printf("\nName\tPhone Number\tUsage(min)\tTotal Bill\t\n");
    while (current != NULL) {
        printf("%s\t%s\t%.2f\t\t%.2f\t\n", current->name, current->phno, current->usage, current->totalbill);
        current = current->next;
    }
}
// Function to search for a customer record
struct Customer* searchRecord(char phno[]) {
    struct Customer* current = head;
    while (current != NULL) {
        if (strcmp(current->phno, phno) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to modify a customer record
void modifyRecord(char phno[]) {
    struct Customer* targetCustomer = searchRecord(phno);
    if (targetCustomer != NULL) {
        printf("\nEnter new usage (in minutes) for %s: ", targetCustomer->name);
        scanf("%f", &targetCustomer->usage);
        targetCustomer->totalbill = targetCustomer->usage * 0.1;
        printf("\nRecord modified successfully!\n");
    } else {
        printf("\nRecord not found!\n");
    }
    saveRecordsToFile();
}

// Function to delete a customer record
void deleteRecord(char phno[]) {
    struct Customer* targetCustomer = searchRecord(phno);
    if (targetCustomer != NULL) {
        if (targetCustomer->prev != NULL) {
            targetCustomer->prev->next = targetCustomer->next;
        } else {
            head = targetCustomer->next;
        }
        if (targetCustomer->next != NULL) {
            targetCustomer->next->prev = targetCustomer->prev;
        }

        free(targetCustomer);
        printf("\nRecord deleted successfully!\n");
    } else {
        printf("\nRecord not found!\n");
    }
    saveRecordsToFile();
}

void displayrechargeplans() {
    printf("\nAvailable Recharge Plans:\n");
    printf("Plan Name\tRate ($ per minute)\n");
    for (int i = 0; i < 4; i++) {
        printf("%s\t\t%.2f\n", rechargeplans[i].name, rechargeplans[i].rate);
    }
}
// Function to view bill payment for a customer
void billpayment(char phno[]) {
    struct Customer* targetCustomer = searchRecord(phno);
    if (targetCustomer != NULL) {
        printf("\nBill payment details for %s:\n", targetCustomer->name);
        printf("Name\tPhone Number\tUsage(min)\tTotal Bill($)\n");
        printf("%s\t%s\t%.2f\t\t%.2f\n", targetCustomer->name, targetCustomer->phno,
               targetCustomer->usage, targetCustomer->totalbill);

        //Ask the user to choose a recharge plan
        printf("\nChoose a recharge plan:\n");
        for (int i = 0; i < 4; i++) {
            printf("%d. %s\n", i + 1, rechargeplans[i].name);
        }

        int rechargeplanchoice;
        printf("Enter your choice (1-4): ");
        scanf("%d", &rechargeplanchoice);

        if (rechargeplanchoice >= 1 && rechargeplanchoice <= 4) {
            // Update the bill based on the chosen recharge plan
            targetCustomer->totalbill += targetCustomer->usage * rechargeplans[rechargeplanchoice - 1].rate;
            printf("Bill updated based on %s .\n", rechargeplans[rechargeplanchoice - 1].name);
        } else {
            printf("Invalid recharge plan choice.\n");
        }

        // Display the updated bill
        printf("\nUpdated Bill:\n");
        printf("Name\tPhone Number\tUsage(min)\tTotal Bill($)\n");
        printf("%s\t%s\t%.2f\t\t%.2f\n", targetCustomer->name, targetCustomer->phno,
               targetCustomer->usage, targetCustomer->totalbill);
    } else {
        printf("\nRecord not found!\n");
    }
}

// Function to search for call records based on the phone number
void searchCallRecords(char phno[]) {
    struct Customer* targetCustomer = searchRecord(phno);
    if (targetCustomer != NULL) {
        printf("\nCall records for %s:\n", targetCustomer->name);
        printf("Name\tPhone Number\tUsage(min)\tTotal Bill($)\n");
        printf("%s\t%s\t%.2f\t\t%.2f\n", targetCustomer->name, targetCustomer->phno,
               targetCustomer->usage, targetCustomer->totalbill);
    } else {
        printf("\nRecord not found!\n");
    }
}
// Function to free the memory allocated
void freeMemory() {
    struct Customer* current = head;
    while (current != NULL) {
        struct Customer* temp = current;
        current = current->next;
        free(temp);
    }
}
int main() {
    loadRecordsFromFile();
    int choice;
    char phno[15];
    while (1) {
        printf("\n1. Add New Record\n");
        printf("2. View List of Records\n");
        printf("3. Modify Record\n");
        printf("4. View Bill Payment\n");
        printf("5. Search Call Records\n");
        printf("6. Delete Record\n");
        printf("7. Recharge Plans\n");
        printf("8. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addRecord();
            break;
        case 2:
            viewRecords();
            break;
        case 3:
            printf("\nEnter phone number to modify record: ");
            scanf("%s", phno);
            modifyRecord(phno);
            break;
        case 4:
            printf("\nEnter phone number to view bill payment: ");
            scanf("%s", phno);
            billpayment(phno);
            break;
        case 5:
            printf("\nEnter phone number to search call records: ");
            scanf("%s", phno);
            searchCallRecords(phno);
            break;
        case 6:
            printf("\nEnter phone number to delete record: ");
            scanf("%s", phno);
            deleteRecord(phno);
            break;
        case 7:
            displayrechargeplans();
            break;
        case 8:
            // Free allocated memory before exiting
            freeMemory();
            printf("Exited");
            return 0;
        default:
            printf("\nInvalid choice! Please try again.\n");
        }
        saveRecordsToFile();
    }
}