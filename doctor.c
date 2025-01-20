#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "doctor.h"
#define FILE_OPEN_ERROR 1
#define USER_ID "123"
#define USER_PASSWORD "123"
#define DOCTOR_FILE_NAME "doctors.txt"

doctor *doctorHead = NULL;
doctor *doctorTemp;
doctor *doctorNode;
FILE *fd;
int lastDoctorId=0;
void loadDoctorDataFromFile()
{
    fd = fopen(DOCTOR_FILE_NAME, "r+");
    if (fd == NULL)
    {
        fd = fopen(DOCTOR_FILE_NAME, "w+");
        if (fd == NULL)
        {
            printf("Unable to open or create file.\n");
            exit(FILE_OPEN_ERROR);
        }
    }

    if (fd == NULL)
    {
        printf("File is not open for reading.\n");
        return;
    }
    rewind(fd);
    int maxId =0;
    char line[256];
    while (fgets(line, sizeof(line), fd))
    {
        doctorNode = (doctor *)malloc(sizeof(doctor));
        if (sscanf(line, "%5d,%49[^,],%19[^,],%10f,%14[^,],%3d,%49[^\n],%c\n", &doctorNode->doctorId, doctorNode->doctorName, doctorNode->doctorSpecialization, &doctorNode->doctorConsultationFee, doctorNode->doctorContactNumber, &doctorNode->doctorExperience, doctorNode->doctorQualification, &doctorNode->doctorStatus) == 8)
        {
            if(doctorNode->doctorId > maxId)
            {
                maxId = doctorNode->doctorId;
            }
            doctorNode->next = NULL;
            insertDoctorSortedByName();
        }
        else
        {
            free(doctorNode);
        }
    }
    lastDoctorId = maxId;
    printf("Doctor data loaded from file.\n");
}


void loginAsDoctorManagementUser()
{

    char userId[15];
    char userPass[15];
    while(true)
    {
        printf("User ID:\n");
        char id[15];
        if(scanf("%s",&id) ==1 && strlen(id) <=15)
        {
            strcpy(userId,id);
            break;
        }
        else
        {
            printf("Invalid User Id,enter valid user id with less than 15 characters\n");
        }

    }
    while(true)
    {
        printf("User Password:\n");
        char password[15];
        if(scanf("%s",&password) ==1 && strlen(password) <=15)
        {
            strcpy(userPass,password);
            break;
        }
        else
        {
            printf("Invalid User password,enter valid user id with less than 15 characters\n");
        }

    }

    if (strcmp(userId, USER_ID) == 0 && strcmp(userPass, USER_PASSWORD) == 0)
    {
        loadDoctorDataFromFile();
        int option;
        while (true)
        {
            printf("\n--- Doctor Management System ---\n");
            printf("1. Add doctor\n");
            printf("2. Update doctor details\n");
            printf("3. Display Doctors\n");
            printf("4. Search Doctor by ID\n");
            printf("5. Search Doctor by Specialization\n");
            printf("6. Sort By ID\n");
            printf("7. Delete doctor record by ID\n");
            printf("8. Display Deleted Records\n");
            printf("9. Exit from Doctor Menu\n");
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
                printf("Exiting from doctor menu.\n");
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

    doctorNode->doctorId = ++lastDoctorId;
    printf("Generated doctor ID: %d\n",  doctorNode->doctorId);
    while(true)
    {
        char name[MAX_FIXED_DOCTOR_NAME];
        printf("Enter Doctor Name: ");
        if(scanf("%s",&name) ==1 && strlen(name)<MAX_FIXED_DOCTOR_NAME)
        {
            strcpy(doctorNode->doctorName , name);
            break;
        }
        else
        {
            printf("invalid doctor Name ,enter Name less than 50 characters\n");
        }

    }
    while(true)
    {
        char specialization [MAX_FIXED_SPECIALIZATION];
        printf("Enter Specialization: ");
        if(scanf("%s",&specialization) ==1 && strlen(specialization)<MAX_FIXED_SPECIALIZATION)
        {
            strcpy(doctorNode->doctorSpecialization,specialization);
            break;
        }
        else
        {
            printf("invalid Specialization, enter Specialization  less than 20 characters\n");

        }

    }

    while(true)
    {
        float fee;
        printf("Enter Consultation Fee: ");
        if(scanf("%f",&fee) ==1 && fee >0)
        {
            doctorNode->doctorConsultationFee = fee;
            break;
        }
        else
        {
            printf("invalid fee,enter fee greater than 0");
        }
    }

    while(true)
    {
        printf("Enter Contact Number: ");
        char contactNumber[MAX_FIXED_CONTACT_NUMBER];
        if (scanf("%s", contactNumber) == 1 && strlen(contactNumber) == 10)
        {
            strcpy(doctorNode->doctorContactNumber, contactNumber);
            break;

        }
        else
        {
            printf("Invalid contact number,Enter 10 digit number\n");
        }

    }
    while(true)
    {
        printf("Enter Experience (in years): ");
        int experience;
        if(scanf("%d", &experience)==1 && experience >= 0 && experience < 40)
        {
            doctorNode->doctorExperience = experience;
            break;
        }
        else
        {
            printf("Invalid Age ,Enter age between 0 and 100\n");
        }

    }

    while(true)
    {
        printf("Enter Qualification: ");
        char qualification[MAX_FIXED_QUALIFICATION];
        if(scanf("%s",qualification)==1 && strlen(qualification)<MAX_FIXED_QUALIFICATION)
        {
            strcpy(doctorNode->doctorQualification,qualification);
            break;
        }
        else
        {
            printf("Invalid Qualification , enter less than 50 characters\n");
        }

    }

    doctorNode->doctorStatus = 'A';
    doctorNode->next = NULL;

    insertDoctorSortedByName();
    // for(int i =0; i<=10000;i++)
    // {
    fseek(fd, 0, SEEK_END);
    fprintf(fd, "%5d,%-49s,%-19s,%10.2f,%-14s,%3d,%-49s,%c\n", doctorNode->doctorId, doctorNode->doctorName, doctorNode->doctorSpecialization, doctorNode->doctorConsultationFee, doctorNode->doctorContactNumber, doctorNode->doctorExperience, doctorNode->doctorQualification, doctorNode->doctorStatus);
    // }
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
        if (doctorTemp->doctorId == id && doctorTemp->doctorStatus == 'A' )
        {
            found =1;
            printf("Updating details for Dr. %s...\n", doctorTemp->doctorName);
            printf("1. Name\n");
            printf("2. Specialization\n");
            printf("3. Consultation Fee\n");
            printf("4. Contact Number\n");
            printf("5. Experience\n");
            printf("6. Qualification\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case UPDATE_DOCTOR_NAME:
                while(true)
                {
                    char name[MAX_FIXED_DOCTOR_NAME];
                    printf("New Name: ");
                    if(scanf("%s",&name) ==1 && strlen(name)<MAX_FIXED_DOCTOR_NAME)
                    {
                        strcpy(doctorTemp->doctorName , name);
                        break;
                    }
                    else
                    {
                        printf("invalid Name enter Name less than 50 characters\n");
                    }

                }
                break;
            case UPDATE_DOCTOR_SPECIALIZATION:
                while(true)
                {
                    char specialization [MAX_FIXED_SPECIALIZATION];
                    printf("New Specialization: ");
                    if(scanf("%s",&specialization) ==1 && strlen(specialization)<MAX_FIXED_SPECIALIZATION)
                    {
                        strcpy(doctorTemp->doctorSpecialization,specialization);
                        break;
                    }
                    else
                    {
                        printf("invalid Specialization, enter Specialization  less than 20 characters\n");

                    }

                }
                break;
            case UPDATE_DOCTOR_CONSULTATION_FEE:

                while(true)
                {
                    float fee;
                    printf("New Consultation Fee: ");
                    if(scanf("%f",&fee) ==1 && fee >0)
                    {
                        doctorTemp->doctorConsultationFee = fee;
                        break;
                    }
                    else
                    {
                        printf("invalid fee,enter fee greater than 0");
                    }
                }
                break;
            case UPDATE_DOCTOR_CONTACT_NUMBER:
                while(true)
                {
                    printf("Enter Contact Number: ");
                    char contactNumber[MAX_FIXED_CONTACT_NUMBER];
                    if (scanf("%s", contactNumber) == 1 && strlen(contactNumber) == 10)
                    {
                        strcpy(doctorTemp->doctorContactNumber, contactNumber);
                        break;

                    }
                    else
                    {
                        printf("Invalid contact number,Enter 10 digit number\n");
                    }

                }
                break;
            case UPDATE_DOCTOR_EXPERIENCE:
                while(true)
                {
                    printf("Enter Experience (in years): ");
                    int experience;
                    if(scanf("%d", &experience)==1 && experience >= 0 && experience < 40)
                    {
                        doctorTemp->doctorExperience = experience;
                        break;
                    }
                    else
                    {
                        printf("Invalid Age ,Enter age between 0 and 100\n");
                    }

                }

                break;
            case UPDATE_DOCTOR_QUALIFICATION:
                while(true)
                {
                    printf("New Qualification: ");
                    char qualification[MAX_FIXED_QUALIFICATION];
                    if(scanf("%s",qualification)==1 && strlen(qualification)<MAX_FIXED_QUALIFICATION)
                    {
                        strcpy(doctorTemp->doctorQualification,qualification);
                        break;
                    }
                    else
                    {
                        printf("Invalid Qualification , enter less than 50 characters\n");
                    }

                }

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
                        fseek(fd, position + sizeof(doctorTemp->doctorId) +1, SEEK_SET);
                        fprintf(fd, "%-49s", doctorTemp->doctorName);
                        break;
                    case UPDATE_DOCTOR_SPECIALIZATION:
                        fseek(fd, position + sizeof(doctorTemp->doctorId)+sizeof(doctorTemp->doctorName) +1, SEEK_SET);
                        fprintf(fd, "%-19s", doctorTemp->doctorSpecialization);
                        break;
                    case UPDATE_DOCTOR_CONSULTATION_FEE:
                        fseek(fd, position + sizeof(doctorTemp->doctorId)+sizeof(doctorTemp->doctorName) +sizeof(doctorTemp->doctorSpecialization)+1, SEEK_SET);
                        fprintf(fd, "%10.2f", doctorTemp->doctorConsultationFee);
                        break;
                    case UPDATE_DOCTOR_CONTACT_NUMBER:
                        fseek(fd, position + sizeof(doctorTemp->doctorId)+sizeof(doctorTemp->doctorName) +sizeof(doctorTemp->doctorSpecialization)+sizeof(doctorTemp->doctorConsultationFee)+8, SEEK_SET);
                        fprintf(fd, "%-14s", doctorTemp->doctorContactNumber);
                        break;
                    case UPDATE_DOCTOR_EXPERIENCE:
                        fseek(fd, position + sizeof(doctorTemp->doctorId)+sizeof(doctorTemp->doctorName) +sizeof(doctorTemp->doctorSpecialization)+sizeof(doctorTemp->doctorConsultationFee)+sizeof( doctorTemp->doctorContactNumber)+8 ,SEEK_SET);
                        fprintf(fd, "%3d", doctorTemp->doctorExperience);
                        break;
                    case UPDATE_DOCTOR_QUALIFICATION:
                        fseek(fd, position + sizeof(doctorTemp->doctorId)+sizeof(doctorTemp->doctorName) +sizeof(doctorTemp->doctorSpecialization)+sizeof(doctorTemp->doctorConsultationFee)+sizeof( doctorTemp->doctorContactNumber)+sizeof(doctorTemp->doctorExperience)+8 ,SEEK_SET);
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
        if (doctorTemp->doctorId == id &&  doctorTemp->doctorStatus == 'A')
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
                    fseek(fd, position + sizeof(doctorTemp->doctorId)+sizeof(doctorTemp->doctorName) +sizeof(doctorTemp->doctorSpecialization)+sizeof(doctorTemp->doctorConsultationFee)+sizeof( doctorTemp->doctorContactNumber)+sizeof(doctorTemp->doctorExperience)+sizeof(doctorTemp->doctorQualification)+8 ,SEEK_SET);
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

doctor* searchByDoctorId()
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
            return(doctorTemp);
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
            printf("--- Doctors with Specialization '%s' ---\n", specialization);

            printf("Doctor ID: %d\n", doctorTemp->doctorId);
            printf("Name: %s\n", doctorTemp->doctorName);
            printf("Specialization: %s\n", doctorTemp->doctorSpecialization);
            printf("Consultation Fee: %.2f\n", doctorTemp->doctorConsultationFee);
            printf("Contact Number: %s\n", doctorTemp->doctorContactNumber);
            printf("Experience: %d years\n", doctorTemp->doctorExperience);
            printf("Qualification: %s\n", doctorTemp->doctorQualification);
            printf("\n");
            searchSpecializationFound = 1;
        }

        doctorTemp = doctorTemp->next;
    }

    if (!searchSpecializationFound)
    {
        printf("\nNo active doctors found with specialization '%s'.\n", specialization);
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


