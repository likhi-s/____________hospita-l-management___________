#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "doctor.h"

#define USER_ID "123"
#define USER_PASSWORD "123"
#define FILE_NAME "doctors.txt"

doctor *doctorHead = NULL;
doctor *doctorTemp;
doctor *doctorNode;
FILE *fd;

void loadDoctorDataFromFile()
{
    if (fd == NULL)
    {
        printf("File is not open for reading.\n");
        return;
    }
    rewind(fd);

    char line[256];
    while (fgets(line, sizeof(line), fd))
    {
        doctorNode = (doctor *)malloc(sizeof(doctor));
        if (sscanf(line, "%5d,%49[^,],%19[^,],%10f,%14[^,],%3d,%49[^\n],%c\n", &doctorNode->doctorId, doctorNode->doctorName, doctorNode->doctorSpecialization, &doctorNode->doctorConsultationFee, doctorNode->doctorContactNumber, &doctorNode->doctorExperience, doctorNode->doctorQualification, &doctorNode->doctorStatus) == 8)
        {
            doctorNode->next = NULL;
            insertDoctorSortedByName();
        }
        else
        {
            free(doctorNode);
        }
    }
    printf("Doctor data loaded from file.\n");
}


void loginAsDoctorManagementUser()
{
    fd = fopen(FILE_NAME, "r+");
    if (fd == NULL)
    {
        fd = fopen(FILE_NAME, "w+");
        if (fd == NULL)
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
        loadDoctorDataFromFile();
        int option;
        while (true)
        {
            printf("\n--- Doctor Management System ---\n");
            printf("1. Add doctor\n2. Update doctor details\n3. Display Doctors\n4. Search Doctor by ID\n5. Search Doctor by Specialization\n6. Sort By ID\n7. delete doctor record by ID\n8. Display Deleted Records\n9. Exit from Doctor Menu\n");
            printf("Enter your option: ");
            scanf("%d", &option);

            switch (option)
            {
            case ADD_DOCTOR:
                addDoctor();
                break;
            case UPDATE_DOCTOR_DETAILS:
                updateDoctorDetails();
                break;
            case DISPLAY_AVAILABLE_DOCTORS:
                displayDoctorDetails();
                break;
            case SEARCH_DOCTOR_BY_ID:
                searchByDoctorId();
                break;
            case SEARCH_DOCTOR_BY_SPECIALIZATION:
                searchByDoctorSpecialization();
                break;
            case SORT_DOCTORS_BY_ID:
                sortDoctorsById();
                break;
            case DELETE_DOCTOR_BY_ID:
                deleteDoctorById();
                break;
            case DISPLAY_DELETED_DOCTORS_RECORD:
                displayDeletedDoctorDetails();
            case EXIT_DOCTOR_MANAGEMENT:
                printf("Saved data and exiting from doctor menu.\n");
                fclose(fd);
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
    fclose(fd);
}

void addDoctor()
{
    doctorNode = (doctor *)malloc(sizeof(doctor));
    if (!doctorNode)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Doctor ID: ");
    scanf("%d", &doctorNode->doctorId);

    doctorTemp = doctorHead;
    while (doctorTemp != NULL)
    {
        if (doctorTemp->doctorId == doctorNode->doctorId)
        {
            printf("Doctor with ID %d already exists.\n", doctorNode->doctorId);
            free(doctorNode);
            return;
        }
        doctorTemp = doctorTemp->next;
    }

    printf("Enter Doctor Name: ");
    scanf(" %[^\n]", doctorNode->doctorName);
    printf("Enter Specialization: ");
    scanf(" %[^\n]", doctorNode->doctorSpecialization);
    printf("Enter Consultation Fee: ");
    scanf("%f", &doctorNode->doctorConsultationFee);
    printf("Enter Contact Number: ");
    scanf("%s", doctorNode->doctorContactNumber);
    printf("Enter Experience (in years): ");
    scanf("%d", &doctorNode->doctorExperience);
    printf("Enter Qualification: ");
    scanf(" %[^\n]", doctorNode->doctorQualification);

    doctorNode->doctorStatus = 'A';
    doctorNode->next = NULL;

    insertDoctorSortedByName();

    fseek(fd, 0, SEEK_END);
    fprintf(fd, "%5d,%-49s,%-19s,%10f,%-14s,%3d,%-49s,%c\n", doctorNode->doctorId, doctorNode->doctorName, doctorNode->doctorSpecialization, doctorNode->doctorConsultationFee, doctorNode->doctorContactNumber, doctorNode->doctorExperience, doctorNode->doctorQualification, doctorNode->doctorStatus);
    fflush(fd);
    printf("Doctor added successfully and saved to file!\n");
}

void insertDoctorSortedByName()
{
    if (doctorHead == NULL || strcmp(doctorHead->doctorName, doctorNode->doctorName) > 0)
    {
        doctorNode->next = doctorHead;
        doctorHead = doctorNode;
    }
    else
    {
        doctorTemp = doctorHead;
        while (doctorTemp->next != NULL && strcmp(doctorTemp->next->doctorName, doctorNode->doctorName) < 0)
        {
            doctorTemp = doctorTemp->next;
        }
        doctorNode->next = doctorTemp->next;
        doctorTemp->next = doctorNode;
    }
}


void updateDoctorDetails()
{
    int id, choice;
    int found =0;
    printf("Enter Doctor ID to update: ");
    scanf("%d", &id);

    doctorTemp = doctorHead;
    while (doctorTemp != NULL)
    {
        if (doctorTemp->doctorId == id && doctorTemp->doctorStatus)
        {
            found =1;
            printf("Updating details for Dr. %s...\n", doctorTemp->doctorName);
            printf("1. Name\n2. Specialization\n3. Consultation Fee\n4. Contact Number\n5. Experience\n6. Qualification\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case UPDATE_DOCTOR_NAME:
                printf("New Name: ");
                scanf(" %[^\n]", doctorTemp->doctorName);
                break;
            case UPDATE_DOCTOR_SPECIALIZATION:
                printf("New Specialization: ");
                scanf(" %[^\n]", doctorTemp->doctorSpecialization);
                break;
            case UPDATE_DOCTOR_CONSULTATION_FEE:
                printf("New Consultation Fee: ");
                scanf("%f", &doctorTemp->doctorConsultationFee);
                break;
            case UPDATE_DOCTOR_CONTACT_NUMBER:
                printf("New Contact Number: ");
                scanf("%s", doctorTemp->doctorContactNumber);
                break;
            case UPDATE_DOCTOR_EXPERIENCE:
                printf("New Experience (in years): ");
                scanf("%d", &doctorTemp->doctorExperience);
                break;
            case UPDATE_DOCTOR_QUALIFICATION:
                printf("New Qualification: ");
                scanf(" %[^\n]", doctorTemp->doctorQualification);
                break;
            default:
                printf("Invalid choice.\n");
                return;
            }

            printf("Doctor details updated successfully in memory.\n");

            rewind(fd);
            char line[256];
            long position;

            while (fgets(line, sizeof(line), fd))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);

                if (existingId == id)
                {
                    found = 1;

                    position = ftell(fd) - strlen(line);

                    fseek(fd, position, SEEK_SET);
                    switch (choice)
                    {
                    case UPDATE_DOCTOR_NAME:
                        fseek(fd, position + 5, SEEK_SET);
                        fprintf(fd, "%-49s", doctorTemp->doctorName);
                        break;
                    case UPDATE_DOCTOR_SPECIALIZATION:
                        fseek(fd, position + 55, SEEK_SET);
                        fprintf(fd, "%-19s", doctorTemp->doctorSpecialization);
                        break;
                    case UPDATE_DOCTOR_CONSULTATION_FEE:
                        fseek(fd, position + 75, SEEK_SET);
                        fprintf(fd, "%10.2f", doctorTemp->doctorConsultationFee);
                        break;
                    case UPDATE_DOCTOR_CONTACT_NUMBER:
                        fseek(fd, position + 86, SEEK_SET);
                        fprintf(fd, "%-14s", doctorTemp->doctorContactNumber);
                        break;
                    case UPDATE_DOCTOR_EXPERIENCE:
                        fseek(fd, position + 101, SEEK_SET);
                        fprintf(fd, "%3d", doctorTemp->doctorExperience);
                        break;
                    case UPDATE_DOCTOR_QUALIFICATION:
                        fseek(fd, position + 105, SEEK_SET);
                        fprintf(fd, "%-49s", doctorTemp->doctorQualification);
                        break;
                    }
                    printf("doctor details updated successfully in file\n");
                    fflush(fd);
                    break;
                }
            }

            if (!found)
            {
                printf("Doctor with ID %d not found.\n", id);
            }

            return;
        }
        doctorTemp = doctorTemp->next;
    }
    if (!found)
    {
        printf("Doctor with ID %d not found.\n", id);
    }
}

void deleteDoctorById()
{
    int id;
    int found =0;
    printf("Enter Doctor ID to delete: ");
    scanf("%d", &id);

    doctorTemp = doctorHead;
    while (doctorTemp != NULL)
    {
        if (doctorTemp->doctorId == id)
        {
            found =1;
            doctorTemp->doctorStatus = 'D';

            rewind(fd);
            long position;
            char line[256];
            while (fgets(line, sizeof(line), fd))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);
                if (existingId == id)
                {
                    position = ftell(fd) - strlen(line);
                    fseek(fd, position + 155, SEEK_SET);
                    fprintf(fd, "%c\n", 'D');
                    printf("Doctor with ID %d marked as deleted.\n", id);

                    fflush(fd);

                    break;
                }
            }
            return;
        }
        doctorTemp = doctorTemp->next;
    }
    if (!found)
    {
        printf("Doctor with ID %d not found.\n", id);
    }
}

void searchByDoctorId()
{
    int id;
    printf("Enter Doctor ID to search: ");
    scanf("%d", &id);
    int searchIdFound = 0;
    doctorTemp = doctorHead;
    while (doctorTemp != NULL)
    {
        if (doctorTemp->doctorId == id && doctorTemp->doctorStatus == 'A')
        {
            printf("--- Doctor Found ---\n");
            printf("Name: %s\n", doctorTemp->doctorName);
            printf("Specialization: %s\n", doctorTemp->doctorSpecialization);
            printf("Consultation Fee: %.2f\n", doctorTemp->doctorConsultationFee);
            printf("Contact Number: %s\n", doctorTemp->doctorContactNumber);
            printf("Experience: %d years\n", doctorTemp->doctorExperience);
            printf("Qualification: %s\n", doctorTemp->doctorQualification);
            printf("\n");
            searchIdFound = 1;
            break;
        }
        doctorTemp = doctorTemp->next;
    }
    if (!searchIdFound)
    {
        printf("Doctor with ID %d not found.\n", id);
    }
}
void searchByDoctorSpecialization()
{
    char specialization[50];
    printf("Enter Doctor Specialization to search: ");
    scanf(" %[^\n]", specialization);

    doctorTemp = doctorHead;
    int searchSpecializationFound = 0;

    while (doctorTemp != NULL)
    {
        if (strcasecmp(specialization, doctorTemp->doctorSpecialization) == 0 && doctorTemp->doctorStatus == 'A')
        {
            printf("--- Doctor Found ---\n");
            printf("Doctor ID: %d\n", doctorTemp->doctorId);
            printf("Name: %s\n", doctorTemp->doctorName);
            printf("Specialization: %s\n", doctorTemp->doctorSpecialization);
            printf("Consultation Fee: %.2f\n", doctorTemp->doctorConsultationFee);
            printf("Contact Number: %s\n", doctorTemp->doctorContactNumber);
            printf("Experience: %d years\n", doctorTemp->doctorExperience);
            printf("Qualification: %s\n", doctorTemp->doctorQualification);
            printf("\n");
            searchSpecializationFound = 1;
            break;
        }
        doctorTemp = doctorTemp->next;
    }

    if (!searchSpecializationFound)
    {
        printf("Doctor with Specialization '%s' not found.\n", specialization);
    }
}

void displayDoctorDetails()
{
    int found =0;
    if (doctorHead == NULL)
    {
        printf("No doctors found.\n");
        return;
    }

    doctorTemp = doctorHead;
    while (doctorTemp != NULL)
    {
        if(doctorTemp->doctorStatus == 'A')
        {
            found =1;
            printf("Doctor ID: %d\n", doctorTemp->doctorId);
            printf("Name: %s\n", doctorTemp->doctorName);
            printf("Specialization: %s\n", doctorTemp->doctorSpecialization);
            printf("Consultation Fee: %.2f\n", doctorTemp->doctorConsultationFee);
            printf("Contact Number: %s\n", doctorTemp->doctorContactNumber);
            printf("Experience: %d years\n", doctorTemp->doctorExperience);
            printf("Qualification: %s\n", doctorTemp->doctorQualification);
            printf("\n");
        }

        doctorTemp = doctorTemp->next;
    }
    if (!found)
    {
        printf("No doctors found.\n");
    }
}

void sortDoctorsById()
{
    if (doctorHead == NULL)
    {
        printf("No doctors found.\n");
        return;
    }
    doctor *tempHead = NULL, *tempTail = NULL, *current= doctorHead;
    while(current != NULL)
    {
        doctor *newNode = (doctor *) malloc(sizeof(doctor));
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

    doctor *sortedList = NULL;

    doctor *splitList(doctor *head)
    {
        doctor *slow = head, *fast = head->next;
        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        doctor *middle = slow->next;
        slow->next = NULL;
        return middle;
    }

    doctor *mergeLists(doctor *left, doctor *right)
    {
        doctor dummy;
        doctor *tail = &dummy;
        dummy.next = NULL;

        while (left != NULL && right != NULL)
        {
            if (left->doctorId < right->doctorId)
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


    doctor *mergeSort(doctor *head)
    {
        if (head == NULL || head->next == NULL)
        {
            return head;
        }

        doctor *middle = splitList(head);
        doctor *left = mergeSort(head);
        doctor *right = mergeSort(middle);
        return mergeLists(left, right);
    }


    sortedList = mergeSort(tempHead);


    printf("--- Doctors Sorted by ID ---\n");
    doctor *doctorTemp = sortedList;
    while (doctorTemp != NULL)
    {
        printf("Doctor ID: %d\n", doctorTemp->doctorId);
        printf("Name: %s\n", doctorTemp->doctorName);
        printf("Specialization: %s\n", doctorTemp->doctorSpecialization);
        printf("Consultation Fee: %.2f\n", doctorTemp->doctorConsultationFee);
        printf("Contact Number: %s\n", doctorTemp->doctorContactNumber);
        printf("Experience: %d years\n", doctorTemp->doctorExperience);
        printf("Qualification: %s\n", doctorTemp->doctorQualification);
        printf("\n");
        doctorTemp = doctorTemp->next;
    }
    while(sortedList != NULL)
    {
        doctor *doctorTemp = sortedList;
        sortedList = sortedList->next;
        free(doctorTemp);
    }
}
void displayDeletedDoctorDetails()
{
    int found =0;
    if (doctorHead == NULL)
    {
        printf("No doctors found.\n");
        return;
    }

    doctorTemp = doctorHead;
    while (doctorTemp != NULL)
    {
        if (doctorTemp->doctorStatus == 'D')
        {
            found =1;
            printf("Doctor ID: %d\n", doctorTemp->doctorId);
            printf("Name: %s\n", doctorTemp->doctorName);
            printf("Specialization: %s\n", doctorTemp->doctorSpecialization);
            printf("Consultation Fee: %.2f\n", doctorTemp->doctorConsultationFee);
            printf("Contact Number: %s\n", doctorTemp->doctorContactNumber);
            printf("Experience: %d years\n", doctorTemp->doctorExperience);
            printf("Qualification: %s\n", doctorTemp->doctorQualification);
            printf("Status: Deleted\n");
            printf("\n");
        }
        doctorTemp = doctorTemp->next;
    }
    if(!found)
    {
        printf("No deleted doctors found.\n");

    }

}
