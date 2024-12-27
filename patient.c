#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "patient.h"

#define USER_ID "patient_user"
#define USER_PASSWORD "patient@123"
#define FILE_NAME "patients.txt"

patient *patientHead = NULL;
patient *patientTemp;
patient *patientNode ;
FILE *fp;


void loadPatientDataFromFile()
{
    if (fp == NULL)
    {
        printf("File is not open for reading.\n");
        return;
    }
    rewind(fp);

    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        patientNode = (patient *)malloc(sizeof(patient));
        if (sscanf(line, "%d,%99[^,],%9[^,],%d,%99[^,],%19[^,],%19[^\n]",&patientNode->patientId, patientNode->patientName, patientNode->patientGender,&patientNode->patientAge, patientNode->patientAddress,patientNode->patientContactNumber, patientNode->patientEmergencyContactNumber) == 7)
        {
            patientNode->next = NULL;
            insertPatientSorted(patientNode);
        }
        else
        {
            free(patientNode);
        }
    }
    printf("Patient data loaded from file.\n");
}


void loginAsPatientManagementUser()
{
    fp = fopen(FILE_NAME, "r+");
    if (fp == NULL)
    {
        fp = fopen(FILE_NAME, "w+");
        if (fp == NULL)
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
        loadPatientDataFromFile();
        int option;
        while (true)
        {
            printf("\n--- Patient Management System ---\n");
            printf("1. Register patient\n2. Update patient details\n3. Display Patients by Name\n4. Search Patient by ID\n5. Search Patient by Name\n6. Sort By ID\n7. Exit from Patient Menu\n");
            printf("Enter your option: ");
            scanf("%d", &option);

            switch (option)
            {
            case REGISTER_PATIENT:
                registerPatient();
                break;
            case UPDATE_PATIENT_DETAILS:
                updatePatientDetails();
                break;
            case DISPLAY_AVAILABLE_PATIENTS_BY_NAME:
                displayPatientDetails();
                break;
            case SEARCH_PATIENT_BY_ID:
                searchByPatientId();
                break;
            case SEARCH_PATIENT_BY_NAME:
                searchByPatientName();
                break;
            case SORT_BY_PATIENT_ID:
                sortPatientsById();
                break;
            case EXIT_PATIENT_MANAGEMENT:
                printf("saved data and exiting from patient menu\n");
                fclose(fp);
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
    fclose(fp);
}

void registerPatient()
{
    patientNode = (patient *)malloc(sizeof(patient));
    if (!patientNode)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Patient ID: ");
    scanf("%d", &patientNode->patientId);

    patientTemp = patientHead;
    while (patientTemp != NULL)
    {
        if (patientTemp->patientId == patientNode->patientId)
        {
            printf("Patient with ID %d already exists.\n", patientNode->patientId);
            free(patientNode);
            return;
        }
        patientTemp = patientTemp->next;
    }

    printf("Enter Patient Name: ");
    scanf(" %[^\n]", patientNode->patientName);
    printf("Enter Gender: ");
    scanf("%s", patientNode->patientGender);
    printf("Enter Age: ");
    scanf("%d", &patientNode->patientAge);
    printf("Enter Address: ");
    scanf(" %[^\n]", patientNode->patientAddress);
    printf("Enter Contact Number: ");
    scanf("%s", patientNode->patientContactNumber);
    printf("Enter Emergency Contact Number: ");
    scanf("%s", patientNode->patientEmergencyContactNumber);

    patientNode->next = NULL;

    insertPatientSorted();

    fseek(fp, 0, SEEK_END);

    fprintf(fp, "%d,%s,%s,%d,%s,%s,%s\n", patientNode->patientId, patientNode->patientName, patientNode->patientGender, patientNode->patientAge, patientNode->patientAddress, patientNode->patientContactNumber, patientNode->patientEmergencyContactNumber);
    fflush(fp);
    printf("Patient registered successfully and saved to file!\n");

}

void insertPatientSorted()
{
    if (patientHead == NULL || strcasecmp(patientHead->patientName, patientNode->patientName) > 0)
    {
        patientNode->next = patientHead;
        patientHead = patientNode;
    }
    else
    {
        patientTemp = patientHead;
        while (patientTemp->next != NULL && strcasecmp(patientTemp->next->patientName, patientNode->patientName) < 0)
        {
            patientTemp = patientTemp->next;
        }
        patientNode->next = patientTemp->next;
        patientTemp->next = patientNode;
    }
}

void updatePatientDetails()
{
    int id, choice;
    printf("Enter Patient ID to update: ");
    scanf("%d", &id);

    patientTemp = patientHead;
    while (patientTemp != NULL)
    {
        if (patientTemp->patientId == id)
        {
            printf("Updating details for %s...\n", patientTemp->patientName);
            printf("1. Name\n2. Gender\n3. Age\n4. Address\n5. Contact Number\n6. Emergency Contact Number\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case UPDATE_PATIENT_NAME:
                printf("New Name: ");
                scanf(" %[^\n]", patientTemp->patientName);
                break;
            case UPDATE_PATIENT_GENDER:
                printf("New Gender: ");
                scanf("%s", patientTemp->patientGender);
                break;
            case UPDATE_PATIENT_AGE:
                printf("New Age: ");
                scanf("%d", &patientTemp->patientAge);
                break;
            case UPDATE_PATIENT_ADDRESS:
                printf("New Address: ");
                scanf(" %[^\n]", patientTemp->patientAddress);
                break;
            case UPDATE_PATIENT_CONTACT_NUMBER:
                printf("New Contact Number: ");
                scanf("%s", patientTemp->patientContactNumber);
                break;
            case UPDATE_PATIENT_EMERGENCY_CONTACT_NUMBER:
                printf("New Emergency Contact: ");
                scanf("%s", patientTemp->patientEmergencyContactNumber);
                break;
            default:
                printf("Invalid choice.\n");
                return;
            }
            printf("Patient details updated successfully in memory.\n");
            rewind(fp);
            long position;
            char line[256];
            while (fgets(line, sizeof(line), fp))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);

                if (existingId == id)
                {
                    position = ftell(fp) - strlen(line);
                    fseek(fp, position, SEEK_SET);
                    fprintf(fp, ",%s,%s,%d,%s,%s,%s\n", patientTemp->patientName, patientTemp->patientGender,patientTemp->patientAge, patientTemp->patientAddress,patientTemp->patientContactNumber, patientTemp->patientEmergencyContactNumber);
                    fflush(fp);
                    break;
                }
            }
            return;
        }
        patientTemp = patientTemp->next;
    }
    printf("Patient with ID %d not found.\n", id);
}

void displayPatientDetails()
{
    if (patientHead == NULL)
    {
        printf("No patients found.\n");
        return;
    }

    patientTemp = patientHead;
    while (patientTemp != NULL)
    {
        printf("Patient ID: %d\n", patientTemp->patientId);
        printf("Name: %s\n", patientTemp->patientName);
        printf("Gender: %s\n", patientTemp->patientGender);
        printf("Age: %d\n", patientTemp->patientAge);
        printf("Address: %s\n", patientTemp->patientAddress);
        printf("Contact Number: %s\n", patientTemp->patientContactNumber);
        printf("Emergency Contact Number: %s\n", patientTemp->patientEmergencyContactNumber);
        printf("\n");
        patientTemp = patientTemp->next;
    }
}

void searchByPatientId()
{
    int id;
    printf("Enter Patient ID to search: ");
    scanf("%d", &id);

    patientTemp = patientHead;
    while (patientTemp != NULL)
    {
        if (patientTemp->patientId == id)
        {
            printf("Patient ID: %d\n", patientTemp->patientId);
            printf("Name: %s\n", patientTemp->patientName);
            printf("Gender: %s\n", patientTemp->patientGender);
            printf("Age: %d\n", patientTemp->patientAge);
            printf("Address: %s\n", patientTemp->patientAddress);
            printf("Contact Number: %s\n", patientTemp->patientContactNumber);
            printf("Emergency Contact Number: %s\n", patientTemp->patientEmergencyContactNumber);
            printf("\n");
            return;
        }
        patientTemp = patientTemp->next;
    }
    printf("Patient with ID %d not found.\n", id);
}

void searchByPatientName()
{
    char name[50];
    printf("Enter Patient Name to search: ");
    scanf(" %[^\n]", name);

    patientTemp = patientHead;
    while (patientTemp != NULL)
    {
        if (strcasecmp(name, patientTemp->patientName) == 0)
        {
            printf("Patient ID: %d\n", patientTemp->patientId);
            printf("Name: %s\n", patientTemp->patientName);
            printf("Gender: %s\n", patientTemp->patientGender);
            printf("Age: %d\n", patientTemp->patientAge);
            printf("Address: %s\n", patientTemp->patientAddress);
            printf("Contact Number: %s\n", patientTemp->patientContactNumber);
            printf("Emergency Contact Number: %s\n", patientTemp->patientEmergencyContactNumber);
            printf("\n");
            return;
        }
        patientTemp = patientTemp->next;
    }
    printf("Patient with Name '%s' not found.\n", name);
}


void sortPatientsById()

{
    if (patientHead == NULL)
    {
        printf("No patients found.\n");
        return;
    }


    patient *tempHead = NULL, *tempTail = NULL, *current = patientHead;

    while (current != NULL)
    {

        patient *newNode = (patient *)malloc(sizeof(patient));
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

    patient *sortedList = NULL;

    patient *splitList(patient *head)
    {
        patient *slow = head, *fast = head->next;
        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        patient *middle = slow->next;
        slow->next = NULL;
        return middle;
    }

    patient *mergeLists(patient *left, patient *right)
    {
        patient dummy;
        patient *tail = &dummy;
        dummy.next = NULL;

        while (left != NULL && right != NULL)
        {
            if (left->patientId < right->patientId)
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

    patient *mergeSort(patient *head)
    {
        if (head == NULL || head->next == NULL)
        {
            return head;
        }

        patient *middle = splitList(head);
        patient *left = mergeSort(head);
        patient *right = mergeSort(middle);
        return mergeLists(left, right);
    }

    sortedList = mergeSort(tempHead);


    printf("--- Patients Sorted by ID ---\n");
    patient *temp = sortedList;
    while (temp != NULL)
    {
        printf("Patient ID: %d\n", temp->patientId);
        printf("Name: %s\n", temp->patientName);
        printf("Gender: %s\n", temp->patientGender);
        printf("Age: %d\n", temp->patientAge);
        printf("Address: %s\n", temp->patientAddress);
        printf("Contact Number: %s\n", temp->patientContactNumber);
        printf("Emergency Contact Number: %s\n", temp->patientEmergencyContactNumber);
        printf("\n");
        temp = temp->next;
    }

    while (sortedList != NULL)
    {
        patient *temp = sortedList;
        sortedList = sortedList->next;
        free(temp);
    }
}
