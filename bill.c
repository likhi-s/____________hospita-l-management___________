#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bill.h"

#define BILL_USER_ID "bill_user"
#define BILL_USER_PASSWORD "bill@123"
#define BILL_FILE_NAME "bills.txt"

Bill *billHead = NULL;
Bill *billTemp;
Bill *billNode;
FILE *billFp;

void loadBillDataFromFile()
{
    if (billFp == NULL)
    {
        printf("File is not open for reading.\n");
        return;
    }
    rewind(billFp);

    char line[256];
    while (fgets(line, sizeof(line), billFp))
    {
        billNode = (Bill *)malloc(sizeof(Bill));
        if (sscanf(line, "%d,%d,%d,%f,%f,%f,%f,%19[^\n]",
                   &billNode->billId, &billNode->patientId, &billNode->treatmentId,
                   &billNode->consultationFee, &billNode->pharmacyFee, &billNode->roomFee,
                   &billNode->totalBillAmount, billNode->billDate) == 8)
        {
            billNode->next = billHead;
            billHead = billNode;
        }
        else
        {
            free(billNode);
        }
    }
    printf("Bill data loaded from file.\n");
}

void verifyBillManagementUser()
{
    billFp = fopen(BILL_FILE_NAME, "r+");
    if (billFp == NULL)
    {
        billFp = fopen(BILL_FILE_NAME, "w+");
        if (billFp == NULL)
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

    if (strcmp(userId, BILL_USER_ID) == 0 && strcmp(userPass, BILL_USER_PASSWORD) == 0)
    {
        loadBillDataFromFile();
        int option;
        while (true)
        {
            printf("\n--- Bill Management System ---\n");
            printf("1. Add Bill\n2. Update Bill Details\n3. Display Bill Details\n4. Search Bill by Patient ID\n5. Calculate Bill\n6. Exit Bill Management\n");
            printf("Enter your option: ");
            scanf("%d", &option);

            switch (option)
            {
            case ADD_BILL:
                addBill();
                break;
            case UPDATE_BILL_DETAILS:
                updateBillDetails();
                break;
            case DISPLAY_BILL_DETAILS:
                displayBillDetails();
                break;
            case SEARCH_BILL_BY_PATIENT_ID:
                searchBillByPatientId();
                break;
            case CALCULATE_BILL:
                calculateBill();
                break;
            case EXIT_BILL_MANAGEMENT:
                printf("Saving data and exiting Bill Management System.\n");
                fclose(billFp);
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
    fclose(billFp);
}

void addBill()
{
    billNode = (Bill *)malloc(sizeof(Bill));
    if (!billNode)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Bill ID: ");
    scanf("%d", &billNode->billId);

    billTemp = billHead;
    while (billTemp != NULL)
    {
        if (billTemp->billId == billNode->billId)
        {
            printf("Bill with ID %d already exists.\n", billNode->billId);
            free(billNode);
            return;
        }
        billTemp = billTemp->next;
    }

    printf("Enter Patient ID: ");
    scanf("%d", &billNode->patientId);
    printf("Enter Treatment ID: ");
    scanf("%d", &billNode->treatmentId);
    printf("Enter Consultation Fee: ");
    scanf("%f", &billNode->consultationFee);
    printf("Enter Pharmacy Fee: ");
    scanf("%f", &billNode->pharmacyFee);
    printf("Enter Room Fee: ");
    scanf("%f", &billNode->roomFee);
    printf("Enter Bill Date (YYYY-MM-DD): ");
    scanf("%s", billNode->billDate);

    billNode->totalBillAmount = billNode->consultationFee + billNode->pharmacyFee + billNode->roomFee;
    billNode->next = billHead;
    billHead = billNode;

    fseek(billFp, 0, SEEK_END);
    fprintf(billFp, "%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%s\n",
            billNode->billId, billNode->patientId, billNode->treatmentId,
            billNode->consultationFee, billNode->pharmacyFee, billNode->roomFee,
            billNode->totalBillAmount, billNode->billDate);
    fflush(billFp);
    printf("Bill added successfully and saved to file!\n");
}

void updateBillDetails()
{
    int id, choice;
    printf("Enter Bill ID to update: ");
    scanf("%d", &id);

    billTemp = billHead;
    while (billTemp != NULL)
    {
        if (billTemp->billId == id)
        {
            printf("Updating details for Bill ID %d...\n", billTemp->billId);
            printf("1. Patient ID\n2. Treatment ID\n3. Consultation Fee\n4. Pharmacy Fee\n5. Room Fee\n6. Bill Date\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case UPDATE_BILL_WITH_PATIENT_ID:
                printf("New Patient ID: ");
                scanf("%d", &billTemp->patientId);
                break;
            case UPDATE_BILL_WITH_TREATMENT_ID:
                printf("New Treatment ID: ");
                scanf("%d", &billTemp->treatmentId);
                break;
            case UPDATE_BILL_WITH_CONSULTATION_FEE:
                printf("New Consultation Fee: ");
                scanf("%f", &billTemp->consultationFee);
                break;
            case UPDATE_BILL_WITH_PHARMACY_FEE:
                printf("New Pharmacy Fee: ");
                scanf("%f", &billTemp->pharmacyFee);
                break;
            case UPDATE_BILL_WITH_ROOM_FEE:
                printf("New Room Fee: ");
                scanf("%f", &billTemp->roomFee);
                break;
            case UPDATE_BILL_DATE:
                printf("New Bill Date (YYYY-MM-DD): ");
                scanf("%s", billTemp->billDate);
                break;
            default:
                printf("Invalid choice.\n");
                return;
            }

            billTemp->totalBillAmount = billTemp->consultationFee + billTemp->pharmacyFee + billTemp->roomFee;
            printf("Bill details updated successfully in memory.\n");

            rewind(billFp);
            FILE *tempFile = fopen("temp_bills.txt", "w");
            Bill *current = billHead;
            while (current != NULL)
            {
                fprintf(tempFile, "%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%s\n",
                        current->billId, current->patientId, current->treatmentId,
                        current->consultationFee, current->pharmacyFee, current->roomFee,
                        current->totalBillAmount, current->billDate);
                current = current->next;
            }
            fclose(tempFile);
            fclose(billFp);
            remove(BILL_FILE_NAME);
            rename("temp_bills.txt", BILL_FILE_NAME);
            billFp = fopen(BILL_FILE_NAME, "r+");
            return;
        }
        billTemp = billTemp->next;
    }
    printf("Bill with ID %d not found.\n", id);
}

void displayBillDetails()
{
    if (billHead == NULL)
    {
        printf("No bills found.\n");
        return;
    }

    Bill *current = billHead;
    while (current != NULL)
    {
        printf("Bill ID: %d\n", current->billId);
        printf("Patient ID: %d\n", current->patientId);
        printf("Treatment ID: %d\n", current->treatmentId);
        printf("Consultation Fee: %.2f\n", current->consultationFee);
        printf("Pharmacy Fee: %.2f\n", current->pharmacyFee);
        printf("Room Fee: %.2f\n", current->roomFee);
        printf("Total Bill Amount: %.2f\n", current->totalBillAmount);
        printf("Bill Date: %s\n", current->billDate);
        printf("\n");
        current = current->next;
    }
}

void searchBillByPatientId()
{
    int patientId;
    printf("Enter Patient ID to search: ");
    scanf("%d", &patientId);

    Bill *current = billHead;
    while (current != NULL)
    {
        if (current->patientId == patientId)
        {
            printf("Bill ID: %d\n", current->billId);
            printf("Patient ID: %d\n", current->patientId);
            printf("Treatment ID: %d\n", current->treatmentId);
            printf("Consultation Fee: %.2f\n", current->consultationFee);
            printf("Pharmacy Fee: %.2f\n", current->pharmacyFee);
            printf("Room Fee: %.2f\n", current->roomFee);
            printf("Total Bill Amount: %.2f\n", current->totalBillAmount);
            printf("Bill Date: %s\n", current->billDate);
            printf("\n");
            return;
        }
        current = current->next;
    }
    printf("No bills found for Patient ID %d.\n", patientId);
}

void calculateBill()
{
    int billId;
    printf("Enter Bill ID to calculate total amount: ");
    scanf("%d", &billId);

    Bill *current = billHead;
    while (current != NULL)
    {
        if (current->billId == billId)
        {
            current->totalBillAmount = current->consultationFee + current->pharmacyFee + current->roomFee;
            printf("Total Bill Amount for Bill ID %d: %.2f\n", current->billId, current->totalBillAmount);
            return;
        }
        current = current->next;
    }
    printf("Bill with ID %d not found.\n", billId);
}
