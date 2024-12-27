#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pharmacy.h"

#define USER_ID "pharmacy_user"
#define USER_PASSWORD "pharmacy@123"
#define FILE_NAME "medicines.txt"

pharmacy *pharmacyHead = NULL;
pharmacy *pharmacyTemp;
pharmacy *pharmacyNode;
FILE *fm;

void loadMedicineDataFromFile()
{
    if (fm == NULL)
    {
        printf("File is not open for reading.\n");
        return;
    }
    rewind(fm);

    char line[256];
    while (fgets(line, sizeof(line), fm))
    {
        pharmacyNode = (pharmacy *)malloc(sizeof(pharmacy));
        if (sscanf(line, "%d,%99[^,],%f,%d,%19[^,],%99[^\n]", &pharmacyNode->medicineId, pharmacyNode->medicineName, &pharmacyNode->medicineCost, &pharmacyNode->medicineStockQuantity, pharmacyNode->medicineType, pharmacyNode->medicineDosage) == 6)
        {
            pharmacyNode->next = NULL;
            insertMedicineSorted(pharmacyNode);
        }
        else
        {
            free(pharmacyNode);
        }
    }
    printf("Medicine data loaded from file.\n");
}

void loginAsPharmacyManagementUser()
{
    fm = fopen(FILE_NAME, "r+");
    if (fm == NULL)
    {
        fm = fopen(FILE_NAME, "w+");
        if (fm == NULL)
        {
            printf("Unable to open or create file.\n");
            exit(1);
        }
    }

    char userId[15];
    char userPass[15];
    printf("User ID:\n");
    scanf(" %[^\n]", userId);
    printf("User Password:\n");
    scanf(" %[^\n]", userPass);

    if (strcmp(userId, USER_ID) == 0 && strcmp(userPass, USER_PASSWORD) == 0)
    {
        loadMedicineDataFromFile();
        int option;
        while (true)
        {
            printf("\n--- Pharmacy Management System ---\n");
            printf("1. Add New Medicine\n2. Update Medicine Details\n3. Display Medicines\n4. Search Medicine by ID\n5. Search Medicine by Name\n6. Check Medicine Stock\n7. Sort By Medicine ID\n8. Exit Pharmacy Menu\n");
            printf("Enter your option: ");
            scanf("%d", &option);

            switch (option)
            {
            case ADD_NEW_MEDICINE:
                addMedicine();
                break;
            case UPDATE_MEDICINE_DETAILS:
                updateMedicineDetails();
                break;
            case DISPLAY_AVAILABLE_MEDICINES:
                displayMedicineDetails();
                break;
            case SEARCH_MEDICINE_BY_ID:
                searchByMedicineId();
                break;
            case SEARCH_MEDICINE_BY_NAME:
                searchByMedicineName();
                break;
            case CHECK_STOCK:
                checkMedicineStock();
                break;
            case SORT_BY_MEDICINE_ID:
                sortByMedicineId();
                break;
            case EXIT_PHARMACY_MANAGEMENT:
                printf("Saved data and exiting from pharmacy menu.\n");
                fclose(fm);
                return;
            default:
                printf("Invalid choice, try again.\n");
                break;
            }
        }
    }
    else
    {
        printf("Invalid User ID or Password!\n");
    }
    fclose(fm);
}

void addMedicine()
{
    pharmacyNode = (pharmacy *)malloc(sizeof(pharmacy));
    if (!pharmacyNode)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Medicine ID: ");
    scanf("%d", &pharmacyNode->medicineId);

    pharmacyTemp = pharmacyHead;
    while (pharmacyTemp != NULL)
    {
        if (pharmacyTemp->medicineId == pharmacyNode->medicineId)
        {
            printf("Medicine with ID %d already exists.\n", pharmacyNode->medicineId);
            free(pharmacyNode);
            return;
        }
        pharmacyTemp = pharmacyTemp->next;
    }

    printf("Enter Medicine Name: ");
    scanf(" %[^\n]", pharmacyNode->medicineName);
    printf("Enter Medicine Cost: ");
    scanf("%f", &pharmacyNode->medicineCost);
    printf("Enter Medicine Stock Quantity: ");
    scanf("%d", &pharmacyNode->medicineStockQuantity);
    printf("Enter Medicine Type (tablet/injection): ");
    scanf(" %[^\n]", pharmacyNode->medicineType);
    printf("Enter Medicine Dosage: ");
    scanf(" %[^\n]", pharmacyNode->medicineDosage);

    pharmacyNode->next = NULL;

    insertMedicineSorted();

    fseek(fm, 0, SEEK_END);
    fprintf(fm, "%d,%s,%.2f,%d,%s,%s\n", pharmacyNode->medicineId, pharmacyNode->medicineName, pharmacyNode->medicineCost, pharmacyNode->medicineStockQuantity, pharmacyNode->medicineType, pharmacyNode->medicineDosage);
    fflush(fm);
    printf("Medicine added successfully and saved to file!\n");
}

void insertMedicineSorted()
{
    if (pharmacyHead == NULL || strcasecmp(pharmacyHead->medicineName, pharmacyNode->medicineName) > 0) {
        pharmacyNode->next = pharmacyHead;
        pharmacyHead = pharmacyNode;
    } else {
        pharmacyTemp = pharmacyHead;
        while (pharmacyTemp->next != NULL && strcasecmp(pharmacyTemp->next->medicineName, pharmacyNode->medicineName) < 0) {
            pharmacyTemp = pharmacyTemp->next;
        }
        pharmacyNode->next = pharmacyTemp->next;
        pharmacyTemp->next = pharmacyNode;
    }
}


void updateMedicineDetails()
{
    int id, choice;
    printf("Enter Medicine ID to update: ");
    scanf("%d", &id);

    pharmacyTemp = pharmacyHead;
    while (pharmacyTemp != NULL)
    {
        if (pharmacyTemp->medicineId == id)
        {
            printf("Updating details for %s...\n", pharmacyTemp->medicineName);
            printf("1. Medicine Name\n2. Medicine Cost\n3. Medicine Stock Quantity\n4. Medicine Type\n5. Medicine Dosage\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case UPDATE_MEDICINE_NAME:
                printf("New Medicine Name: ");
                scanf(" %[^\n]", pharmacyTemp->medicineName);
                break;
            case UPDATE_MEDICINE_COST:
                printf("New Medicine Cost: ");
                scanf("%f", &pharmacyTemp->medicineCost);
                break;
            case UPDATE_MEDICINE_STOCK:
                printf("New Medicine Stock Quantity: ");
                scanf("%d", &pharmacyTemp->medicineStockQuantity);
                break;
            case UPDATE_MEDICINE_TYPE:
                printf("New Medicine Type (tablet/injection): ");
                scanf(" %[^\n]", pharmacyTemp->medicineType);
                break;
            case UPDATE_MEDICINE_DOSAGE:
                printf("New Medicine Dosage: ");
                scanf(" %[^\n]", pharmacyTemp->medicineDosage);
                break;
            default:
                printf("Invalid choice.\n");
                return;
            }
            printf("Medicine details updated successfully in memory.\n");

            rewind(fm);
            long position;
            char line[256];
            while (fgets(line, sizeof(line), fm))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);

                if (existingId == id)
                {
                    position = ftell(fm) - strlen(line);
                    fseek(fm, position, SEEK_SET);
                    fprintf(fm, ",%s,%.2f,%d,%s,%s\n", pharmacyTemp->medicineName, pharmacyTemp->medicineCost, pharmacyTemp->medicineStockQuantity, pharmacyTemp->medicineType, pharmacyTemp->medicineDosage);
                    fflush(fm);
                    break;
                }
            }
            return;
        }
        pharmacyTemp = pharmacyTemp->next;
    }
    printf("Medicine with ID %d not found.\n", id);
}

void displayMedicineDetails()
{
    if (pharmacyHead == NULL)
    {
        printf("No medicines found.\n");
        return;
    }

    pharmacyTemp = pharmacyHead;
    while (pharmacyTemp != NULL)
    {
        printf("Medicine ID: %d\n", pharmacyTemp->medicineId);
        printf("Name: %s\n", pharmacyTemp->medicineName);
        printf("Cost: %.2f\n", pharmacyTemp->medicineCost);
        printf("Stock Quantity: %d\n", pharmacyTemp->medicineStockQuantity);
        printf("Type: %s\n", pharmacyTemp->medicineType);
        printf("Dosage: %s\n", pharmacyTemp->medicineDosage);
        printf("\n");
        pharmacyTemp = pharmacyTemp->next;
    }
}

void searchByMedicineId()
{
    int id;
    printf("Enter Medicine ID to search: ");
    scanf("%d", &id);

    pharmacyTemp = pharmacyHead;
    while (pharmacyTemp != NULL)
    {
        if (pharmacyTemp->medicineId == id)
        {
            printf("--- Medicine Found ---\n");
            printf("Name: %s\n", pharmacyTemp->medicineName);
            printf("Cost: %.2f\n", pharmacyTemp->medicineCost);
            printf("Stock Quantity: %d\n", pharmacyTemp->medicineStockQuantity);
            printf("Type: %s\n", pharmacyTemp->medicineType);
            printf("Dosage: %s\n", pharmacyTemp->medicineDosage);
            printf("\n");
            return;
        }
        pharmacyTemp = pharmacyTemp->next;
    }
    printf("Medicine with ID %d not found.\n", id);
}

void searchByMedicineName()
{
    char name[100];
    printf("Enter Medicine Name to search: ");
    scanf(" %[^\n]", name);

    pharmacyTemp = pharmacyHead;
    while (pharmacyTemp != NULL)
    {
        if (strcasecmp(name, pharmacyTemp->medicineName) == 0)
        {
            printf("--- Medicine Found ---\n");
            printf("ID: %d\n", pharmacyTemp->medicineId);
            printf("Cost: %.2f\n", pharmacyTemp->medicineCost);
            printf("Stock Quantity: %d\n", pharmacyTemp->medicineStockQuantity);
            printf("Type: %s\n", pharmacyTemp->medicineType);
            printf("Dosage: %s\n", pharmacyTemp->medicineDosage);
            printf("\n");
            return;
        }
        pharmacyTemp = pharmacyTemp->next;
    }
    printf("Medicine with Name '%s' not found.\n", name);
}

void checkMedicineStock()
{
    int id;
    printf("Enter Medicine ID to check stock: ");
    scanf("%d", &id);

    pharmacyTemp = pharmacyHead;
    while (pharmacyTemp != NULL)
    {
        if (pharmacyTemp->medicineId == id)
        {
            printf("Medicine %s has %d units in stock.\n", pharmacyTemp->medicineName, pharmacyTemp->medicineStockQuantity);
            return;
        }
        pharmacyTemp = pharmacyTemp->next;
    }
    printf("Medicine with ID %d not found.\n", id);
}

void sortByMedicineId()
{
    if (pharmacyHead == NULL)
    {
        printf("No medicines found.\n");
        return;
    }
    pharmacy *tempHead = NULL, *tempTail = NULL, *current= pharmacyHead;
    while(current != NULL)
    {
        pharmacy *newNode = (pharmacy *) malloc(sizeof(pharmacy));
        if(!newNode)
        {
            printf("Memory Alloctaion Failed\n");
            return;
        }
        *newNode = *current;
        newNode->next = NULL;
        if (tempHead == NULL)
        {
            tempHead = newNode;
            tempTail = newNode;
        }
        else
        {
            tempTail->next = newNode;
            tempTail = newNode;
        }

        current = current->next;
    }

    pharmacy *sortedList = NULL;

    pharmacy *splitList(pharmacy *head)
    {
        pharmacy *slow = head, *fast = head->next;
        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        pharmacy *middle = slow->next;
        slow->next = NULL;
        return middle;
    }

    pharmacy *mergeLists(pharmacy *left, pharmacy *right)
    {
        pharmacy dummy;
        pharmacy *tail = &dummy;
        dummy.next = NULL;

        while (left != NULL && right != NULL)
        {
            if (left->medicineId < right->medicineId)
            {
                tail->next = left;
                left = left->next;
            }
            else
            {
                tail->next = right;
                right = right->next;
            }
            tail = tail->next;
        }
        tail->next = (left != NULL) ? left : right;
        return dummy.next;
    }


    pharmacy *mergeSort(pharmacy *head)
    {
        if (head == NULL || head->next == NULL)
        {
            return head;
        }

        pharmacy *middle = splitList(head);
        pharmacy *left = mergeSort(head);
        pharmacy *right = mergeSort(middle);
        return mergeLists(left, right);
    }


    sortedList = mergeSort(tempHead);
    printf("--- medicines Sorted by ID ---\n");
    pharmacy *pharmacyTemp = sortedList;
    while (pharmacyTemp != NULL)
    {
        printf("Medicine ID: %d\n", pharmacyTemp->medicineId);
        printf("Name: %s\n", pharmacyTemp->medicineName);
        printf("Cost: %f\n", pharmacyTemp->medicineCost);
        printf("Stock Quantity: %d\n", pharmacyTemp->medicineStockQuantity);
        printf("Type: %s\n", pharmacyTemp->medicineType);
        printf("Dosage: %s\n", pharmacyTemp->medicineDosage);
        printf("\n");
        pharmacyTemp = pharmacyTemp->next;
    }
}
