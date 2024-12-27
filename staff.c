#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "staff.h"

#define USER_ID "staff_user"
#define USER_PASSWORD "staff@123"
#define FILE_NAME "staff.txt"

staff *staffHead = NULL;
staff *staffTemp;
staff *staffNode;
FILE *fs;

void loadStaffDataFromFile()
{
    if (fs == NULL)
    {
        printf("File is not open for reading.\n");
        return;
    }
    rewind(fs);

    char line[256];
    while (fgets(line, sizeof(line), fs))
    {
        staffNode = (staff *)malloc(sizeof(staff));
        if (sscanf(line, "%d,%99[^,],%49[^,],%9[^,],%f,%14[^\n]", &staffNode->staffId, staffNode->staffName, staffNode->staffRole, staffNode->staffShift, &staffNode->staffSalary, staffNode->staffContactNumber) == 6)
        {
            staffNode->next = NULL;
            insertStaffSorted(staffNode);
        }
        else
        {
            free(staffNode);
        }
    }
    printf("Staff data loaded from file.\n");
}

void loginAsStaffManagementUser()
{
    fs = fopen(FILE_NAME, "r+");
    if (fs == NULL)
    {
        fs = fopen(FILE_NAME, "w+");
        if (fs == NULL)
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
        loadStaffDataFromFile();
        int option;
        while (true)
        {
            printf("\n--- Staff Management System ---\n");
            printf("1. Add staff\n2. Update staff details\n3. Display Staff\n4. Search Staff by ID\n5. Search Staff by Role\n6. Sort By ID\n7. Exit from Staff Menu\n");
            printf("Enter your option: ");
            scanf("%d", &option);

            switch (option)
            {
            case ADD_STAFF:
                addStaff();
                break;
            case UPDATE_STAFF_DETAILS:
                updateStaffDetails();
                break;
            case DISPLAY_AVAILABLE_STAFF:
                displayStaffDetails();
                break;
            case SEARCH_STAFF_BY_ID:
                searchByStaffId();
                break;
            case SEARCH_STAFF_BY_ROLE:
                searchByStaffRole();
                break;
            case SORT_BY_STAFF_ID:
                sortByStaffId();
                break;
            case EXIT_STAFF_MANAGEMENT:
                printf("Saved data and exiting from staff menu.\n");
                fclose(fs);
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
    fclose(fs);
}

void addStaff()
{
    staffNode = (staff *)malloc(sizeof(staff));
    if (!staffNode)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Staff ID: ");
    scanf("%d", &staffNode->staffId);

    staffTemp = staffHead;
    while (staffTemp != NULL)
    {
        if (staffTemp->staffId == staffNode->staffId)
        {
            printf("Staff with ID %d already exists.\n", staffNode->staffId);
            free(staffNode);
            return;
        }
        staffTemp = staffTemp->next;
    }

    printf("Enter Staff Name: ");
    scanf(" %[^\n]", staffNode->staffName);
    printf("Enter Staff Role (e.g., Nurse, Technician, Receptionist): ");
    scanf(" %[^\n]", staffNode->staffRole);
    printf("Enter Staff Shift (Day/Night): ");
    scanf("%s", staffNode->staffShift);
    printf("Enter Staff Salary: ");
    scanf("%f", &staffNode->staffSalary);
    printf("Enter Staff Contact Number: ");
    scanf("%s", staffNode->staffContactNumber);


    staffNode->next = NULL;

    insertStaffSorted(staffNode);

    fseek(fs, 0, SEEK_END);
    fprintf(fs, "%d,%s,%s,%s,%.2f,%s\n", staffNode->staffId, staffNode->staffName, staffNode->staffRole, staffNode->staffShift, staffNode->staffSalary, staffNode->staffContactNumber);
    fflush(fs);
    printf("Staff added successfully and saved to file!\n");
}

void insertStaffSorted()
{
    if (staffHead == NULL || strcmp(staffHead->staffName, staffNode->staffName) > 0)
    {
        staffNode->next = staffHead;
        staffHead = staffNode;
    }
    else
    {
        staffTemp = staffHead;
        while (staffTemp->next != NULL && strcmp(staffTemp->next->staffName, staffNode->staffName) < 0)
        {
            staffTemp = staffTemp->next;
        }
        staffNode->next = staffTemp->next;
        staffTemp->next = staffNode;
    }
}
void updateStaffDetails()
{
    int id, choice;
    printf("Enter Staff ID to update: ");
    scanf("%d", &id);

    staffTemp = staffHead;
    while (staffTemp != NULL)
    {
        if (staffTemp->staffId == id)
        {
            printf("Updating details for %s...\n", staffTemp->staffName);
            printf("1. Name\n2. Role\n3. Shift\n4. Salary\n5. Contact Number\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case UPDATE_STAFF_NAME:
                printf("New Name: ");
                scanf(" %[^\n]", staffTemp->staffName);
                break;
            case UPDATE_STAFF_ROLE:
                printf("New Role: ");
                scanf(" %[^\n]", staffTemp->staffRole);
                break;
            case UPDATE_STAFF_SHIFT:
                printf("New Shift (day/night): ");
                scanf(" %[^\n]", staffTemp->staffShift);
                break;
            case UPDATE_STAFF_SALARY:
                printf("New Salary: ");
                scanf("%f", &staffTemp->staffSalary);
                break;
            case UPDATE_STAFF_CONTACT_NUMBER:
                printf("New Contact Number: ");
                scanf("%s", staffTemp->staffContactNumber);
                break;
            default:
                printf("Invalid choice.\n");
                return;
            }

            printf("Staff details updated successfully in memory.\n");
            fseek(fs, 0, SEEK_SET);

            long position = 0;
            char line[256];

            while (fgets(line, sizeof(line), fs))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);

                if (existingId == id)
                {
                    position = ftell(fs) - strlen(line);
                    fseek(fs, position, SEEK_SET);

                    fprintf(fs, ",%s,%s,%s,%.2f,%s\n", staffTemp->staffName,
                            staffTemp->staffRole, staffTemp->staffShift, staffTemp->staffSalary, staffTemp->staffContactNumber);

                    fflush(fs);
                    printf("Staff details updated successfully in file.\n");
                    return;
                }
            }

            return;
        }
        staffTemp = staffTemp->next;
    }

    printf("Staff with ID %d not found.\n", id);
}

void searchByStaffId()
{
    int id;
    printf("Enter Staff ID to search: ");
    scanf("%d", &id);
    int searchIdFound = 0;
    staffTemp = staffHead;
    while (staffTemp != NULL)
    {
        if (staffTemp->staffId == id)
        {
            printf("--- Staff Found ---\n");
            printf("Name: %s\n", staffTemp->staffName);
            printf("Role: %s\n", staffTemp->staffRole);
            printf("Shift: %s\n", staffTemp->staffShift);
            printf("Salary: %.2f\n", staffTemp->staffSalary);
            printf("Contact Number: %s\n", staffTemp->staffContactNumber);
            printf("\n");
            searchIdFound = 1;
            break;
        }
        staffTemp = staffTemp->next;
    }
    if (!searchIdFound)
    {
        printf("Staff with ID %d not found.\n", id);
    }
}

void searchByStaffRole()
{
    char role[50];
    printf("Enter Staff Role to search: ");
    scanf(" %[^]", role);

    staffTemp = staffHead;
    int searchRoleFound = 0;

    while (staffTemp != NULL)
    {
        if (strcasecmp(role, staffTemp->staffRole) == 0)
        {
            printf("--- Staff Found ---\n");
            printf("Staff ID: %d\n", staffTemp->staffId);
            printf("Name: %s\n", staffTemp->staffName);
            printf("Role: %s\n", staffTemp->staffRole);
            printf("Shift: %s\n", staffTemp->staffShift);
            printf("Salary: %.2f\n", staffTemp->staffSalary);
            printf("Contact Number: %s\n", staffTemp->staffContactNumber);
            printf("\n");
            searchRoleFound = 1;
            break;
        }
        staffTemp = staffTemp->next;
    }

    if (!searchRoleFound)
    {
        printf("Staff with Role '%s' not found.\n", role);
    }
}

void displayStaffDetails()
{
    if (staffHead == NULL)
    {
        printf("No staff members found.\n");
        return;
    }

    staffTemp = staffHead;
    while (staffTemp != NULL)
    {
        printf("Staff ID: %d\n", staffTemp->staffId);
        printf("Name: %s\n", staffTemp->staffName);
        printf("Role: %s\n", staffTemp->staffRole);
        printf("Shift: %s\n", staffTemp->staffShift);
        printf("Salary: %.2f\n", staffTemp->staffSalary);
        printf("Contact Number: %s\n", staffTemp->staffContactNumber);
        printf("\n");
        staffTemp = staffTemp->next;
    }
}

void sortByStaffId()
{
    if (staffHead == NULL)
    {
        printf("No staff found.\n");
        return;
    }
    staff *tempHead = NULL, *tempTail = NULL, *current= staffHead;
    while(current != NULL)
    {
        staff *newNode = (staff *) malloc(sizeof(staff));
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

    staff *sortedList = NULL;

    staff *splitList(staff *head)
    {
        staff *slow = head, *fast = head->next;
        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        staff *middle = slow->next;
        slow->next = NULL;
        return middle;
    }

    staff *mergeLists(staff *left, staff *right)
    {
        staff dummy;
        staff *tail = &dummy;
        dummy.next = NULL;

        while (left != NULL && right != NULL)
        {
            if (left->staffId < right->staffId)
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


    staff *mergeSort(staff *head)
    {
        if (head == NULL || head->next == NULL)
        {
            return head;
        }

        staff *middle = splitList(head);
        staff *left = mergeSort(head);
        staff *right = mergeSort(middle);
        return mergeLists(left, right);
    }


    sortedList = mergeSort(tempHead);


    printf("--- Staff Sorted by ID ---\n");
    staff *staffTemp = sortedList;
    while (staffTemp != NULL)
    {
        printf("Staff ID: %d\n", staffTemp->staffId);
        printf("Name: %s\n", staffTemp->staffName);
        printf("Role: %s\n", staffTemp->staffRole);
        printf("Shift: %s\n", staffTemp->staffShift);
        printf("Salary: %f\n", staffTemp->staffSalary);
        printf("Contact Number: %s\n", staffTemp->staffContactNumber);
        printf("\n");
        staffTemp = staffTemp->next;
    }
    while(sortedList != NULL)
    {
        staff *staffTemp = sortedList;
        sortedList = sortedList->next;
        free(staffTemp);
    }

}
