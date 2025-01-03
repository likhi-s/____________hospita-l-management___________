#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bill.h"

#define USER_ID "123"
#define USER_PASSWORD "123"
#define FILE_NAME "bills.txt"

bill *billHead = NULL;
bill *billTemp;
bill *billNode;
FILE *fb;
void loadBillDataFromFile()
{
    if (fb == NULL)
    {
        printf("File is not open for reading.\n");
        return;
    }
    rewind(fb);

    char line[256];
    while (fgets(line, sizeof(line), fb))
    {
        billNode = (bill *)malloc(sizeof(bill));
        // Adjusted format specifiers to handle extra spaces in the input file
        if (sscanf(line, "%5d,%5d,%5d,%lf,%lf,%lf,%lf,%19s,%c",
                   &billNode->billId, &billNode->patientId, &billNode->treatmentId,
                   &billNode->consultationFee, &billNode->pharmacyFee, &billNode->roomFee,
                   &billNode->totalBillAmount, billNode->billDate, &billNode->billStatus) == 9)
        {
            billNode->next = NULL;
            insertBillSortedByBillId();
        }
        else
        {
            free(billNode);
        }
    }
    printf("Bill data loaded from file.\n");
}


void loginAsBillManagementUser()
{
    fb = fopen(FILE_NAME, "r+");
    if (fb == NULL)
    {
        fb = fopen(FILE_NAME, "w+");
        if (fb == NULL)
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
        loadBillDataFromFile();
        int option;
        while (true)
        {
            printf("\n--- Bill Management System ---\n");
            printf("1. Add bill\n2. Update bill details\n3. Display Bills\n4. Search Bill by Patient ID\n5. Calculate Bill\n6. Sort Bills by Bill ID\n7. Delete bill record by ID\n8. Exit from Bill Menu\n");
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
            case SORT_BILL_BY_BILL_ID:
                sortBillsByBillId();
                break;
            case DELETE_BILL_BY_ID:
                deleteBillById();
                break;
            case EXIT_BILL_MANAGEMENT:
                printf("Saved data and exiting from bill menu.\n");
                fclose(fb);
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
    fclose(fb);
}

void addBill()
{
    billNode = (bill *)malloc(sizeof(bill));
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

    billNode->totalBillAmount = billNode->consultationFee + billNode->pharmacyFee + billNode->roomFee;

    printf("Enter Bill Date (YYYY-MM-DD): ");
    scanf(" %[^\n]", billNode->billDate);

    billNode->billStatus = 'A';
    billNode->next = NULL;

    insertBillSortedByBillId();

    fseek(fb, 0, SEEK_END);
    fprintf(fb, "%5d,%5d,%5d,%10.2f,%10.2f,%10.2f,%10.2f,%19s,%c\n", billNode->billId, billNode->patientId, billNode->treatmentId, billNode->consultationFee, billNode->pharmacyFee, billNode->roomFee, billNode->totalBillAmount, billNode->billDate, billNode->billStatus);
    fflush(fb);
    printf("Bill added successfully and saved to file!\n");
}

void insertBillSortedByBillId()
{
    if (billHead == NULL || billHead->billId > billNode->billId)
    {
        billNode->next = billHead;
        billHead = billNode;
    }
    else
    {
        billTemp = billHead;
        while (billTemp->next != NULL && billTemp->next->billId < billNode->billId)
        {
            billTemp = billTemp->next;
        }
        billNode->next = billTemp->next;
        billTemp->next = billNode;
    }
}

void updateBillDetails()
{
    int id, choice;
    printf("Enter Bill ID to update: ");
    scanf("%d", &id);

    billTemp = billHead;
    while (billTemp != NULL)
    {
        if (billTemp->billId == id && billTemp->billStatus == 'A')
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
                scanf(" %[^\n]", billTemp->billDate);
                break;
            default:
                printf("Invalid choice.\n");
                return;
            }

            billTemp->totalBillAmount = billTemp->consultationFee + billTemp->pharmacyFee + billTemp->roomFee;
            printf("Bill details updated successfully in memory.\n");
            rewind(fb);
            char line[256];
            long position = 0;
            while (fgets(line, sizeof(line), fb))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);

                if (existingId == id)
                {
                    position = (ftell(fb)-1) -strlen(line);
                    fseek(fb, position, SEEK_SET);
                    fprintf(fb, "%5d,%5d,%5d,%10.2f,%10.2f,%10.2f,%10.2f,%19s,%c\n", billTemp->billId, billTemp->patientId, billTemp->treatmentId, billTemp->consultationFee, billTemp->pharmacyFee, billTemp->roomFee, billTemp->totalBillAmount, billTemp->billDate, billTemp->billStatus);
                    fflush(fb);
                    break;
                }
            }

            return;
        }
        billTemp = billTemp->next;
    }

    printf("Bill with ID %d not found.\n", id);
}

void deleteBillById()
{
    int id;
    printf("Enter Bill ID to delete: ");
    scanf("%d", &id);

    billTemp = billHead;
    while (billTemp != NULL)
    {
        if (billTemp->billId == id)
        {
            billTemp->billStatus = 'D';
            printf("Bill with ID %d marked as deleted.\n", id);

            rewind(fb);
            long position;
            char line[256];
            while (fgets(line, sizeof(line), fb))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);
                if (existingId == id)
                {
                    position = (ftell(fb) - 1) - strlen(line);
                    fseek(fb, position, SEEK_SET);
                    fprintf(fb, "%5d,%5d,%5d,%10.2f,%10.2f,%10.2f,%10.2f,%19s,%c\n", billTemp->billId, billTemp->patientId, billTemp->treatmentId, billTemp->consultationFee, billTemp->pharmacyFee, billTemp->roomFee, billTemp->totalBillAmount, billTemp->billDate, 'D');
                    fflush(fb);
                    break;
                }
            }
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
        printf("No bills available to display.\n");
        return;
    }

    printf("--- Bill Details ---\n");
    billTemp = billHead;
    while (billTemp != NULL)
    {
        if(billTemp->billStatus == 'A')
        {
            printf("Bill ID: %d\n", billTemp->billId);
            printf("Patient ID: %d\n", billTemp->patientId);
            printf("Treatment ID: %d\n", billTemp->treatmentId);
            printf("Consultation Fee: %.2f\n", billTemp->consultationFee);
            printf("Pharmacy Fee: %.2f\n", billTemp->pharmacyFee);
            printf("Room Fee: %.2f\n", billTemp->roomFee);
            printf("Total Bill Amount: %.2f\n", billTemp->totalBillAmount);
            printf("Bill Date: %s\n", billTemp->billDate);
            printf("Bill Status: %c\n", billTemp->billStatus);
            printf("----------------------\n");
        }

        billTemp = billTemp->next;
    }
}


void searchBillByPatientId()
{
    int id;
    printf("Enter Patient ID to search: ");
    scanf("%d", &id);
    int searchIdFound = 0;
    billTemp = billHead;
    while (billTemp != NULL)
    {
        if (billTemp->patientId == id && billTemp->billStatus == 'A')
        {
            printf("--- Bill Found ---\n");
            printf("Bill ID: %d\n", billTemp->billId);
            printf("Patient ID: %d\n", billTemp->patientId);
            printf("Treatment ID: %d\n", billTemp->treatmentId);
            printf("Consultation Fee: %.2f\n", billTemp->consultationFee);
            printf("Pharmacy Fee: %.2f\n", billTemp->pharmacyFee);
            printf("Room Fee: %.2f\n", billTemp->roomFee);
            printf("Total Bill: %.2f\n", billTemp->totalBillAmount);
            printf("Bill Date: %s\n", billTemp->billDate);
            printf("\n");
            searchIdFound = 1;
            break;
        }
        billTemp = billTemp->next;
    }
    if (!searchIdFound)
    {
        printf("Bill for Patient ID %d not found.\n", id);
    }
}

void calculateBill()
{
    int patientId, treatmentId;
    float consultationFee, pharmacyFee, roomFee, totalAmount;

    printf("Enter Patient ID: ");
    scanf("%d", &patientId);

    printf("Enter Treatment ID: ");
    scanf("%d", &treatmentId);

    printf("Enter Consultation Fee: ");
    scanf("%f", &consultationFee);

    printf("Enter Pharmacy Fee: ");
    scanf("%f", &pharmacyFee);

    printf("Enter Room Fee: ");
    scanf("%f", &roomFee);

    totalAmount = consultationFee + pharmacyFee + roomFee;
    printf("Total Bill Amount: %.2f\n", totalAmount);
}



void sortBillsByBillId()
{
    if (billHead == NULL)
    {
        printf("No bills found.\n");
        return;
    }

    bill *tempHead = NULL, *tempTail = NULL, *current = billHead;

    // Copy the linked list to avoid modifying the original list while sorting
    while (current != NULL)
    {
        bill *newNode = (bill *)malloc(sizeof(bill));
        if (!newNode)
        {
            printf("Memory allocation failed\n");
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

    // Function to split the list into two halves
    bill *splitList(bill *head)
    {
        bill *slow = head, *fast = head->next;
        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        bill *middle = slow->next;
        slow->next = NULL;
        return middle;
    }

    // Function to merge two sorted lists
    bill *mergeLists(bill *left, bill *right)
    {
        bill dummy;
        bill *tail = &dummy;
        dummy.next = NULL;

        while (left != NULL && right != NULL)
        {
            if (left->billId < right->billId)
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

    // Merge sort function
    bill *mergeSort(bill *head)
    {
        if (head == NULL || head->next == NULL)
        {
            return head;
        }

        bill *middle = splitList(head);
        bill *left = mergeSort(head);
        bill *right = mergeSort(middle);
        return mergeLists(left, right);
    }

    // Sort the bills by ID using merge sort
    bill *sortedList = mergeSort(tempHead);

    // Print the sorted list of bills
    printf("--- Bills Sorted by ID ---\n");
    bill *temp = sortedList;
    while (temp != NULL)
    {
        printf("Bill ID: %d\n", temp->billId);
        printf("Patient ID: %d\n", temp->patientId);
        printf("Treatment ID: %d\n", temp->treatmentId);
        printf("Consultation Fee: %.2f\n", temp->consultationFee);
        printf("Pharmacy Fee: %.2f\n", temp->pharmacyFee);
        printf("Room Fee: %.2f\n", temp->roomFee);
        printf("Total Bill Amount: %.2f\n", temp->totalBillAmount);
        printf("Bill Date: %s\n", temp->billDate);
        printf("Bill Status: %c\n", temp->billStatus);
        printf("\n");
        temp = temp->next;
    }

    // Free the sorted list after use
    while (sortedList != NULL)
    {
        bill *temp = sortedList;
        sortedList = sortedList->next;
        free(temp);
    }
}
