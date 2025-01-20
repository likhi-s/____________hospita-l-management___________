#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "patient.h"

#define FILE_OPEN_ERROR 1
#define USER_ID "123"
#define USER_PASSWORD "123"
#define FILE_NAME "patients.txt"

patient *patientHead = NULL;
patient *patientTemp;
patient *patientNode;
FILE *fp = NULL;
int lastPatientId =0;

void loadPatientDataFromFile()
{
    fp = fopen(FILE_NAME, "r+");
    if (fp == NULL)
    {
        fp = fopen(FILE_NAME, "w+");
        if (fp == NULL)
        {
            printf("Unable to open or create file.\n");
            exit(FILE_OPEN_ERROR);
        }
    }
    if (fp == NULL)
    {
        printf("File is not open for reading.\n");
        return;
    }
    rewind(fp);
    int maxId =0;
    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        patientNode = (patient *)malloc(sizeof(patient));
        if (sscanf(line, "%5d,%49[^,],%9[^,],%3d,%49[^,],%19[^,],%19[^,],%c\n", &patientNode->patientId, patientNode->patientName, patientNode->patientGender, &patientNode->patientAge, patientNode->patientAddress, patientNode->patientContactNumber, patientNode->patientEmergencyContactNumber, &patientNode->patientStatus) == 8)
        {
            if(patientNode->patientId > maxId)
            {
                maxId = patientNode->patientId;
            }

            patientNode->next = NULL;
            insertPatientSorted();

        }
        else
        {
            free(patientNode);
        }
    }
    lastPatientId = maxId;
    printf("Patient data loaded from file.\n");
}

void loginAsPatientManagementUser()
{


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
            printf("1. Register patient\n2. Update patient details\n3. Display Patients by Name\n4. Search Patient by ID\n5. Search Patient by Name\n6. Sort By ID\n7. Delete Patient\n8. Display deleted records\n9. Exit from Patient Menu\n");
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
            case DELETE_PATIENT_ID:
                deletePatient();
                break;
            case DISPLAY_DELETED_PATIENTS_RECORDS:
                displayDeletedRecords();
                break;
            case 9:
                printf("Exiting from patient menu\n");
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

    patientNode->patientId = ++lastPatientId;
    printf("Generated Patient Id: %d\n",patientNode->patientId);
    printf("Enter Patient Name: ");
    scanf(" %[^\n]", patientNode->patientName);
    printf("Enter Gender: ");
    scanf("%s", patientNode->patientGender);
    while(true)
    {
        printf("Enter Age: ");
        int age;
        if(scanf("%d", &age)==1 && age > 0 && age <100)
        {
            patientNode->patientAge = age;
            break;
        }
        else
        {
            printf("Invalid Age ,Enter age between 0 and 100\n");
        }

    }

    printf("Enter Address: ");
    scanf(" %[^\n]", patientNode->patientAddress);
    while(true)
    {
        printf("Enter Contact Number: ");
        char contactNumber[15];
        if (scanf("%s", contactNumber) == 1 && strlen(contactNumber) == 10)
        {
            strcpy(patientNode->patientContactNumber, contactNumber);
            break;

        }
        else
        {
            printf("Invalid contact number,Enter 10 digit number\n");
        }

    }

    while(true)
    {
        printf("Enter Emergency Contact Number: ");
        char contactNumber[15];
        if (scanf("%s", contactNumber) == 1 && strlen(contactNumber) == 10)
        {
            strcpy(patientNode->patientEmergencyContactNumber, contactNumber);
            break;

        }
        else
        {
            printf("Invalid emergency contact number,Enter 10 digit number\n");
        }
    }

    patientNode->patientStatus = 'A';
    patientNode->next = NULL;

    insertPatientSorted();
   // for(int i =0; i<=10000;i++)
    //{
        fseek(fp, 0, SEEK_END);
        fprintf(fp, "%5d,%-49s,%-9s,%3d,%-49s,%-19s,%-19s,%c\n", patientNode->patientId, patientNode->patientName, patientNode->patientGender, patientNode->patientAge, patientNode->patientAddress, patientNode->patientContactNumber, patientNode->patientEmergencyContactNumber, patientNode->patientStatus);
   // }
    printf("Patient registered successfully and saved to file!\n");
    fflush(fp);
}

void deletePatient()
{
    int id;
    int found =0;
    printf("Enter Patient ID to delete: ");
    scanf("%d", &id);

    patientTemp = patientHead;
    while (patientTemp != NULL)
    {
        if (patientTemp->patientId == id && patientTemp->patientStatus == 'A')
        {
            found =1;
            patientTemp->patientStatus = 'D';

            rewind(fp);
            long position;
            char line[256];
            while (fgets(line, sizeof(line), fp))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);
                if (existingId == id)
                {
                    position = ftell(fp)- strlen(line);
                    fseek(fp,position +sizeof(patientTemp->patientId)+sizeof(patientTemp->patientName)+sizeof(patientTemp->patientGender)+sizeof(patientTemp->patientAge)+sizeof(patientTemp->patientAddress)+sizeof(patientNode->patientContactNumber)+sizeof(patientNode->patientEmergencyContactNumber)+1, SEEK_SET);
                    fprintf(fp, "%c", 'D');
                    printf("Patient with ID %d marked as deleted.\n", id);
                    fflush(fp);
                    break;
                }
            }
        }
        patientTemp = patientTemp->next;
    }
    if(!found)
    {
        printf("Patient with ID %d not found.\n", id);

    }
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
    int found = 0;
    printf("Enter Patient ID to update: ");
    scanf("%d", &id);

    patientTemp = patientHead;
    while (patientTemp != NULL)
    {
        if (patientTemp->patientId == id && patientTemp->patientStatus == 'A')
        {
            found =1;
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
                while(true)
                {
                    printf("New Age: ");
                    int age;
                    if(scanf("%d", &age)==1 && age > 0 && age <100)
                    {
                        patientTemp->patientAge = age;
                        break;
                    }
                    else
                    {
                        printf("Invalid Age ,Enter age between 0 and 100\n");
                    }
                }

                break;
            case UPDATE_PATIENT_ADDRESS:
                printf("New Address: ");
                scanf(" %[^\n]", patientTemp->patientAddress);
                break;
            case UPDATE_PATIENT_CONTACT_NUMBER:
                while(true)
                {
                    printf("Enter New Contact Number: ");
                    char contactNumber[15];
                    if (scanf("%s", contactNumber) == 1 && strlen(contactNumber) == 10)
                    {
                        strcpy(patientTemp->patientContactNumber, contactNumber);
                        break;

                    }
                    else
                    {
                        printf("Invalid contact number,Enter 10 digit number\n");
                    }

                }

                break;
            case UPDATE_PATIENT_EMERGENCY_CONTACT_NUMBER:
                while(true)
                {
                    printf("Enter New Emergency Contact Number: ");
                    char contactNumber[15];
                    if (scanf("%s", contactNumber) == 1 && strlen(contactNumber) == 10)
                    {
                        strcpy(patientTemp->patientEmergencyContactNumber, contactNumber);
                        break;

                    }
                    else
                    {
                        printf("Invalid emergency contact number,Enter 10 digit number\n");
                    }
                }
                break;
            default:
                printf("Invalid choice.\n");
                return;
            }

            printf("Patient details updated successfully in memory.\n");


            rewind(fp);
            char line[256];
            long position;

            while (fgets(line, sizeof(line), fp))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);

                if (existingId == id)
                {
                    found = 1;
                    position = ftell(fp) - strlen(line);

                    fseek(fp, position, SEEK_SET);//position =1

                    switch (choice)
                    {
                    case UPDATE_PATIENT_NAME:
                        fseek(fp,position+sizeof(patientTemp->patientId)+1, SEEK_SET);
                        fprintf(fp, "%-49s", patientTemp->patientName);
                        break;
                    case UPDATE_PATIENT_GENDER:
                        fseek(fp, position+sizeof(patientTemp->patientId)+sizeof(patientTemp->patientName)+1, SEEK_SET);
                        fprintf(fp, "%-9s",patientTemp->patientGender);
                        break;
                    case UPDATE_PATIENT_AGE:                     
                        fseek(fp, position + sizeof(patientTemp->patientId)+sizeof(patientTemp->patientName)+sizeof(patientTemp->patientGender)+1, SEEK_SET);
                        fprintf(fp, "%3d", patientTemp->patientAge);
                        break;
                    case UPDATE_PATIENT_ADDRESS:
                        fseek(fp, position + sizeof(patientTemp->patientId)+sizeof(patientTemp->patientName)+sizeof(patientTemp->patientGender)+sizeof(patientTemp->patientAge)+1, SEEK_SET);
                        fprintf(fp, "%-49s", patientTemp->patientAddress);
                        break;
                    case UPDATE_PATIENT_CONTACT_NUMBER:
                        fseek(fp, position + sizeof(patientTemp->patientId)+sizeof(patientTemp->patientName)+sizeof(patientTemp->patientGender)+sizeof(patientTemp->patientAge)+sizeof(patientTemp->patientAddress)+1, SEEK_SET);
                        fprintf(fp, "%-19s", patientTemp->patientContactNumber);
                        break;
                    case UPDATE_PATIENT_EMERGENCY_CONTACT_NUMBER:
                        fseek(fp, position +sizeof(patientTemp->patientId)+sizeof(patientTemp->patientName)+sizeof(patientTemp->patientGender)+sizeof(patientTemp->patientAge)+sizeof(patientTemp->patientAddress)+sizeof(patientNode->patientContactNumber)+1, SEEK_SET);
                        fprintf(fp, "%-19s", patientTemp->patientEmergencyContactNumber);
                        break;
                    }

                    printf("patient details updated sucessfully in file\n");
                    fflush(fp);
                    break;
                }
            }

            if (!found)
            {
                printf("Patient with ID %d not found in file.\n", id);
            }
            return;

        }
        patientTemp = patientTemp->next;
    }
    if(!found)
    {
        printf("Patient with ID %d not found \n", id);

    }

}



void displayPatientDetails()
{
    int found=0;
    if (patientHead == NULL)
    {
        printf("No patients found.\n");
    }
    else
    {
        patientTemp = patientHead;
        while (patientTemp != NULL)
        {
            if (patientTemp->patientStatus == 'A')
            {
                found =1;
                printf("Patient ID: %d\n", patientTemp->patientId);
                printf("Name: %s\n", patientTemp->patientName);
                printf("Gender: %s\n", patientTemp->patientGender);
                printf("Age: %d\n", patientTemp->patientAge);
                printf("Address: %s\n", patientTemp->patientAddress);
                printf("Contact Number: %s\n", patientTemp->patientContactNumber);
                printf("Emergency Contact Number: %s\n", patientTemp->patientEmergencyContactNumber);
                printf("\n");
            }
            patientTemp = patientTemp->next;
        }
    }


    if(!found)
    {
        printf("no patients found\n");
    }
}

patient* searchByPatientId()
{
    int found =0;
    int id;
    printf("Enter Patient ID to search: ");
    scanf("%d", &id);

    patientTemp = patientHead;
    while (patientTemp != NULL)
    {
        if (patientTemp->patientId == id && patientTemp->patientStatus == 'A')
        {
            found =1;
            printf("Patient ID: %d\n", patientTemp->patientId);
            printf("Name: %s\n", patientTemp->patientName);
            printf("Gender: %s\n", patientTemp->patientGender);
            printf("Age: %d\n", patientTemp->patientAge);
            printf("Address: %s\n", patientTemp->patientAddress);
            printf("Contact Number: %s\n", patientTemp->patientContactNumber);
            printf("Emergency Contact Number: %s\n", patientTemp->patientEmergencyContactNumber);
            printf("\n");
            return patientTemp;
        }
        patientTemp = patientTemp->next;
    }
    if(!found)
    {
        printf("Patient with ID %d not found.\n", id);

    }
}

void searchByPatientName()
{
    int found =0;
    char name[50];
    printf("Enter Patient Name to search: ");
    scanf(" %[^\n]", name);

    patientTemp = patientHead;
    while (patientTemp != NULL)
    {
        if (strcasecmp(name, patientTemp->patientName) == 0 && patientTemp->patientStatus == 'A')
        {

            printf("Patient ID: %d\n", patientTemp->patientId);
            printf("Name: %s\n", patientTemp->patientName);
            printf("Gender: %s\n", patientTemp->patientGender);
            printf("Age: %d\n", patientTemp->patientAge);
            printf("Address: %s\n", patientTemp->patientAddress);
            printf("Contact Number: %s\n", patientTemp->patientContactNumber);
            printf("Emergency Contact Number: %s\n", patientTemp->patientEmergencyContactNumber);
            printf("\n");
            found =1;
        }
        patientTemp = patientTemp->next;
    }
    if(!found)
    {
        printf("Patient with Name '%s' not found.\n", name);

    }
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

        *newNode = *current; //copying data
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
void displayDeletedRecords()
{
    int found =0;
    if (patientHead == NULL)
    {
        printf("No patients found.\n");
    }
    else
    {
        patientTemp = patientHead;

        printf("--- Deleted Patient Records ---\n");

        while (patientTemp != NULL)
        {
            if (patientTemp->patientStatus == 'D')
            {
                found =1;
                printf("Patient ID: %d\n", patientTemp->patientId);
                printf("Name: %s\n", patientTemp->patientName);
                printf("Gender: %s\n", patientTemp->patientGender);
                printf("Age: %d\n", patientTemp->patientAge);
                printf("Address: %s\n", patientTemp->patientAddress);
                printf("Contact Number: %s\n", patientTemp->patientContactNumber);
                printf("Emergency Contact Number: %s\n", patientTemp->patientEmergencyContactNumber);
                printf("\n");
            }
            patientTemp = patientTemp->next;
        }
    }

    if(!found)
    {
        printf("No Patients found.\n");

    }
}

