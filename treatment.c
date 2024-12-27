#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "treatment.h"

#define USER_ID "treatment_user"
#define USER_PASSWORD "treatment@123"
#define FILE_NAME "treatments.txt"

treatment *treatmentHead = NULL;
treatment *treatmentTemp;
treatment *treatmentNode;
FILE *ft;

void loadTreatmentDataFromFile()
{
    if (ft == NULL)
    {
        printf("File is not open for reading.\n");
        return;
    }
    rewind(ft);

    char line[256];
    while (fgets(line, sizeof(line), ft))
    {
        treatmentNode = (treatment *)malloc(sizeof(treatment));
        if (sscanf(line, "%d,%99[^,],%d,%d\n", &treatmentNode->treatmentId, treatmentNode->treatmentName, &treatmentNode->treatmentCost, &treatmentNode->treatmentDuration) == 4)
        {
            treatmentNode->next = NULL;
            insertTreatmentSorted(treatmentNode);
        }
        else
        {
            free(treatmentNode);
        }
    }
    printf("Treatment data loaded from file.\n");
}

void loginAsTreatmentAndPriceManagementUser()
{
    ft = fopen(FILE_NAME, "r+");
    if (ft == NULL)
    {
        ft = fopen(FILE_NAME, "w+");
        if (ft == NULL)
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
        loadTreatmentDataFromFile();
        int option;
        while (true)
        {
            printf("\n--- Treatment Management System ---\n");
            printf("1. Add New Treatment\n2. Update Treatment Details\n3. Display All Treatments\n4. Search Treatment by ID\n5. Search Treatment by Name\n6. Sort By Treatment ID\n7. Exit Treatment Menu\n");
            printf("Enter your option: ");
            scanf("%d", &option);

            switch (option)
            {
            case ADD_TREATMENT:
                addTreatment();
                break;
            case UPDATE_TREATMENT_DETAILS:
                updateTreatmentDetails();
                break;
            case DISPLAY_ALL_TREATMENTS:
                displayTreatment();
                break;
            case SEARCH_TREATMENT_BY_ID:
                searchByTreatmentId();
                break;
            case SEARCH_TREATMENT_BY_NAME:
                searchByTreatmentName();
                break;
            case SORT_TREATMENT_BY_ID:
                sortTreatmentById();
                break;
            case EXIT_TREATMENT_MANAGEMENT:
                printf("Saved data and exiting from treatment menu.\n");
                fclose(ft);
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
    fclose(ft);
}

void addTreatment()
{
    treatmentNode = (treatment *)malloc(sizeof(treatment));
    if (!treatmentNode)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Treatment ID: ");
    scanf("%d", &treatmentNode->treatmentId);

    treatmentTemp = treatmentHead;
    while (treatmentTemp != NULL)
    {
        if (treatmentTemp->treatmentId == treatmentNode->treatmentId)
        {
            printf("Treatment with ID %d already exists.\n", treatmentNode->treatmentId);
            free(treatmentNode);
            return;
        }
        treatmentTemp = treatmentTemp->next;
    }

    printf("Enter Treatment Name: ");
    scanf(" %[^\n]", treatmentNode->treatmentName);
    printf("Enter Treatment Cost: ");
    scanf("%d", &treatmentNode->treatmentCost);
    printf("Enter Treatment Duration (in days): ");
    scanf("%d", &treatmentNode->treatmentDuration);

    treatmentNode->next = NULL;

    insertTreatmentSorted();

    fseek(ft, 0, SEEK_END);
    fprintf(ft, "%d,%s,%d,%d\n", treatmentNode->treatmentId, treatmentNode->treatmentName, treatmentNode->treatmentCost, treatmentNode->treatmentDuration);
    fflush(ft);
    printf("Treatment added successfully and saved to file!\n");
}

void insertTreatmentSorted()
{
    if (treatmentHead == NULL || strcasecmp(treatmentHead->treatmentName, treatmentNode->treatmentName)>0)
    {
        treatmentNode->next = treatmentHead;
        treatmentHead = treatmentNode;
    }
    else
    {
        treatmentTemp = treatmentHead;
        while (treatmentTemp->next != NULL && strcasecmp(treatmentTemp->next->treatmentName, treatmentNode->treatmentName) <0)
        {
            treatmentTemp = treatmentTemp->next;
        }
        treatmentNode->next = treatmentTemp->next;
        treatmentTemp->next = treatmentNode;
    }
}

void updateTreatmentDetails()
{
    int id, choice;
    printf("Enter Treatment ID to update: ");
    scanf("%d", &id);

    treatmentTemp = treatmentHead;
    while (treatmentTemp != NULL)
    {
        if (treatmentTemp->treatmentId == id)
        {
            printf("Updating details for Treatment ID %d...\n", treatmentTemp->treatmentId);
            printf("1. Treatment Name\n2. Treatment Cost\n3. Treatment Duration\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case UPDATE_TREATMENT_NAME:
                printf("New Treatment Name: ");
                scanf(" %[^\n]", treatmentTemp->treatmentName);
                break;
            case UPDATE_TREATMENT_COST:
                printf("New Treatment Cost: ");
                scanf("%d", &treatmentTemp->treatmentCost);
                break;
            case UPDATE_TREATMENT_DURATION:
                printf("New Treatment Duration: ");
                scanf("%d", &treatmentTemp->treatmentDuration);
                break;
            default:
                printf("Invalid choice.\n");
                return;
            }

            printf("Treatment details updated successfully in memory.\n");

            rewind(ft);
            long position;
            char line[256];
            while (fgets(line, sizeof(line), ft))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);

                if (existingId == id)
                {
                    position = ftell(ft) - strlen(line);
                    fseek(ft, position, SEEK_SET);
                    fprintf(ft, ",%s,%d,%d\n", treatmentTemp->treatmentName, treatmentTemp->treatmentCost, treatmentTemp->treatmentDuration);
                    fflush(ft);
                    break;
                }
            }
            return;
        }
        treatmentTemp = treatmentTemp->next;
    }

    printf("Treatment with ID %d not found.\n", id);
}

void displayTreatment()
{
    if (treatmentHead == NULL)
    {
        printf("No treatments found.\n");
        return;
    }

    treatmentTemp = treatmentHead;
    while (treatmentTemp != NULL)
    {
        printf("Treatment ID: %d\n", treatmentTemp->treatmentId);
        printf("Name: %s\n", treatmentTemp->treatmentName);
        printf("Cost: %d\n", treatmentTemp->treatmentCost);
        printf("Duration: %d days\n", treatmentTemp->treatmentDuration);
        printf("\n");
        treatmentTemp = treatmentTemp->next;
    }
}

void searchByTreatmentId()
{
    int id;
    printf("Enter Treatment ID to search: ");
    scanf("%d", &id);

    treatmentTemp = treatmentHead;
    while (treatmentTemp != NULL)
    {
        if (treatmentTemp->treatmentId == id)
        {
            printf("--- Treatment Found ---\n");
            printf("Name: %s\n", treatmentTemp->treatmentName);
            printf("Cost: %d\n", treatmentTemp->treatmentCost);
            printf("Duration: %d days\n", treatmentTemp->treatmentDuration);
            printf("\n");
            return;
        }
        treatmentTemp = treatmentTemp->next;
    }
    printf("Treatment with ID %d not found.\n", id);
}

void searchByTreatmentName()
{
    char name[100];
    printf("Enter Treatment Name to search: ");
    scanf(" %[^\n]", name);

    treatmentTemp = treatmentHead;
    while (treatmentTemp != NULL)
    {
        if (strcasecmp(name, treatmentTemp->treatmentName) == 0)
        {
            printf("--- Treatment Found ---\n");
            printf("Treatment ID: %d\n", treatmentTemp->treatmentId);
            printf("Cost: %d\n", treatmentTemp->treatmentCost);
            printf("Duration: %d days\n", treatmentTemp->treatmentDuration);
            printf("\n");
            return;
        }
        treatmentTemp = treatmentTemp->next;
    }
    printf("Treatment with Name '%s' not found.\n", name);
}

void sortTreatmentById()
{
    if (treatmentHead == NULL)
    {
        printf("No treatments found.\n");
        return;
    }

    treatment *tempHead = NULL, *tempTail = NULL, *current = treatmentHead;

    while (current != NULL)
    {
        treatment *newNode = (treatment *)malloc(sizeof(treatment));
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

    treatment *sortedList = NULL;


    treatment *splitList(treatment *head)
    {
        treatment *slow = head, *fast = head->next;
        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        treatment *middle = slow->next;
        slow->next = NULL;
        return middle;
    }


    treatment *mergeLists(treatment *left, treatment *right)
    {
        treatment dummy;
        treatment *tail = &dummy;
        dummy.next = NULL;

        while (left != NULL && right != NULL)
        {
            if (left->treatmentId < right->treatmentId)
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


    treatment *mergeSort(treatment *head)
    {
        if (head == NULL || head->next == NULL)
        {
            return head;
        }

        treatment *middle = splitList(head);
        treatment *left = mergeSort(head);
        treatment *right = mergeSort(middle);
        return mergeLists(left, right);
    }

    sortedList = mergeSort(tempHead);


    printf("--- Treatments Sorted by ID ---\n");
    treatment *temp = sortedList;
    while (temp != NULL)
    {
        printf("Treatment ID: %d\n", temp->treatmentId);
        printf("Treatment Name: %s\n", temp->treatmentName);
        printf("Cost: %d\n", temp->treatmentCost);
        printf("Duration: %d days\n", temp->treatmentDuration);
        printf("\n");
        temp = temp->next;
    }


    while (sortedList != NULL)
    {
        treatment *temp = sortedList;
        sortedList = sortedList->next;
        free(temp);
    }
}
