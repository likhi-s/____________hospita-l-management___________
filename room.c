#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "room.h"

#define USER_ID "room_user"
#define USER_PASSWORD "room@123"
#define FILE_NAME "rooms.txt"

room *roomHead = NULL;
room *roomTemp;
room *roomNode;
FILE *fr;

void loadRoomDataFromFile()
{
    if (fr == NULL)
    {
        printf("File is not open for reading.\n");
        return;
    }
    rewind(fr);

    char line[256];
    while (fgets(line, sizeof(line), fr))
    {
        roomNode = (room *)malloc(sizeof(room));
        if (sscanf(line, "%d,%9[^,],%d,%d,%9[^,],%f\n", &roomNode->roomId, roomNode->roomType, &roomNode->bedCount, &roomNode->availableBeds, roomNode->bedStatus, &roomNode->roomFee) == 6)
        {
            roomNode->next = NULL;
            insertRoomSorted(roomNode);
        }
        else
        {
            free(roomNode);
        }
    }
    printf("Room data loaded from file.\n");
}

void loginAsRoomManagementUser()
{
    fr = fopen(FILE_NAME, "r+");
    if (fr == NULL)
    {
        fr = fopen(FILE_NAME, "w+");
        if (fr == NULL)
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
        loadRoomDataFromFile();
        int option;
        while (true)
        {
            printf("\n--- Room Management System ---\n");
            printf("1. Add New Room\n2. Update Room Details\n3. Display Rooms\n4. Search Room by ID\n5. Search Room by Type\n6. Check Room Availability\n7. Sort By Room ID\n8. Exit Room Menu\n");
            printf("Enter your option: ");
            scanf("%d", &option);

            switch (option)
            {
            case ADD_ROOM:
                addRoom();
                break;
            case UPDATE_ROOM_DETAILS:
                updateRoomDetails();
                break;
            case DISPLAY_AVAILABLE_ROOMS:
                displayRoomDetails();
                break;
            case SEARCH_ROOM_BY_ID:
                searchByRoomId();
                break;
            case SEARCH_ROOM_BY_TYPE:
                searchByRoomType();
                break;
            case CHECK_ROOM_AVAILABILITY:
                checkAvailability();
                break;
            case SORT_BY_ROOM_ID:
                sortByRoomId();
                break;
            case EXIT_ROOM_MANAGEMENT:
                printf("Saved data and exiting from room menu.\n");
                fclose(fr);
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
    fclose(fr);
}

void addRoom()
{
    roomNode = (room *)malloc(sizeof(room));
    if (!roomNode)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter Room ID: ");
    scanf("%d", &roomNode->roomId);

    roomTemp = roomHead;
    while (roomTemp != NULL)
    {
        if (roomTemp->roomId == roomNode->roomId)
        {
            printf("Room with ID %d already exists.\n", roomNode->roomId);
            free(roomNode);
            return;
        }
        roomTemp = roomTemp->next;
    }

    printf("Enter Room Type (icu/general/private): ");
    scanf(" %[^\n]", roomNode->roomType);
    printf("Enter Bed Count: ");
    scanf("%d", &roomNode->bedCount);
    printf("Enter Available Beds: ");
    scanf("%d", &roomNode->availableBeds);
    printf("Enter Bed Status (occupied/vacant): ");
    scanf(" %[^\n]", roomNode->bedStatus);
    printf("Enter Room Fee: ");
    scanf("%f", &roomNode->roomFee);

    roomNode->next = NULL;

    insertRoomSorted(roomNode);

    fseek(fr, 0, SEEK_END);
    fprintf(fr, "%d,%s,%d,%d,%s,%.2f\n", roomNode->roomId, roomNode->roomType, roomNode->bedCount, roomNode->availableBeds, roomNode->bedStatus, roomNode->roomFee);
    fflush(fr);
    printf("Room added successfully and saved to file!\n");
}

void insertRoomSorted(room *roomNode)
{
    if (roomHead == NULL || roomHead->roomType > roomNode->roomType)
    {
        roomNode->next = roomHead;
        roomHead = roomNode;
    }
    else
    {
        roomTemp = roomHead;
        while (roomTemp->next != NULL && roomTemp->next->roomType < roomNode->roomType)
        {
            roomTemp = roomTemp->next;
        }
        roomNode->next = roomTemp->next;
        roomTemp->next = roomNode;
    }
}

void updateRoomDetails()
{
    int id, choice;
    printf("Enter Room ID to update: ");
    scanf("%d", &id);

    roomTemp = roomHead;
    while (roomTemp != NULL)
    {
        if (roomTemp->roomId == id)
        {
            printf("Updating details for Room ID %d...\n", roomTemp->roomId);
            printf("1. Room Type\n2. Bed Count\n3. Available Beds\n4. Bed Status\n5. Room Fee\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case UPDATE_ROOM_TYPE:
                printf("New Room Type: ");
                scanf(" %[^\n]", roomTemp->roomType);
                break;
            case UPDATE_BED_COUNT:
                printf("New Bed Count: ");
                scanf("%d", &roomTemp->bedCount);
                break;
            case UPDATE_AVAILABLE_BEDS:
                printf("New Available Beds: ");
                scanf("%d", &roomTemp->availableBeds);
                break;
            case UPDATE_BED_STATUS:
                printf("New Bed Status: ");
                scanf(" %[^\n]", roomTemp->bedStatus);
                break;
            case UPDATE_ROOM_FEE:
                printf("New Room Fee: ");
                scanf("%f", &roomTemp->roomFee);
                break;
            default:
                printf("Invalid choice.\n");
                return;
            }

            printf("Room details updated successfully in memory.\n");

            rewind(fr);
            long position;
            char line[256];
            while (fgets(line, sizeof(line), fr))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);

                if (existingId == id)
                {
                    position = ftell(fr) - strlen(line);
                    fseek(fr, position, SEEK_SET);
                    fprintf(fr, ",%s,%d,%d,%s,%.2f\n", roomTemp->roomType, roomTemp->bedCount, roomTemp->availableBeds, roomTemp->bedStatus, roomTemp->roomFee);
                    fflush(fr);
                    break;
                }
            }
            return;
        }
        roomTemp = roomTemp->next;
    }

    printf("Room with ID %d not found.\n", id);
}


void displayRoomDetails()
{
    if (roomHead == NULL)
    {
        printf("No rooms found.\n");
        return;
    }

    roomTemp = roomHead;
    while (roomTemp != NULL)
    {
        printf("Room ID: %d\n", roomTemp->roomId);
        printf("Type: %s\n", roomTemp->roomType);
        printf("Bed Count: %d\n", roomTemp->bedCount);
        printf("Available Beds: %d\n", roomTemp->availableBeds);
        printf("Bed Status: %s\n", roomTemp->bedStatus);
        printf("Room Fee: %.2f\n", roomTemp->roomFee);
        printf("\n");
        roomTemp = roomTemp->next;
    }
}

void searchByRoomId()
{
    int id;
    printf("Enter Room ID to search: ");
    scanf("%d", &id);

    roomTemp = roomHead;
    while (roomTemp != NULL)
    {
        if (roomTemp->roomId == id)
        {
            printf("--- Room Found ---\n");
            printf("Room Type: %s\n", roomTemp->roomType);
            printf("Bed Count: %d\n", roomTemp->bedCount);
            printf("Available Beds: %d\n", roomTemp->availableBeds);
            printf("Bed Status: %s\n", roomTemp->bedStatus);
            printf("Room Fee: %.2f\n", roomTemp->roomFee);
            printf("\n");
            return;
        }
        roomTemp = roomTemp->next;
    }
    printf("Room with ID %d not found.\n", id);
}

void searchByRoomType()
{
    char type[10];
    printf("Enter Room Type (icu/general/private) to search: ");
    scanf(" %[^\n]", type);

    roomTemp = roomHead;
    while (roomTemp != NULL)
    {
        if (strcasecmp(type, roomTemp->roomType) == 0)
        {
            printf("--- Room Found ---\n");
            printf("Room ID: %d\n", roomTemp->roomId);
            printf("Bed Count: %d\n", roomTemp->bedCount);
            printf("Available Beds: %d\n", roomTemp->availableBeds);
            printf("Bed Status: %s\n", roomTemp->bedStatus);
            printf("Room Fee: %.2f\n", roomTemp->roomFee);
            printf("\n");
            return;
        }
        roomTemp = roomTemp->next;
    }
    printf("Room with Type '%s' not found.\n", type);
}

void checkAvailability()
{
    int id;
    printf("Enter Room ID to check availability: ");
    scanf("%d", &id);

    roomTemp = roomHead;
    while (roomTemp != NULL)
    {
        if (roomTemp->roomId == id)
        {
            printf("Room ID %d has %d available beds.\n", roomTemp->roomId, roomTemp->availableBeds);
            return;
        }
        roomTemp = roomTemp->next;
    }
    printf("Room with ID %d not found.\n", id);
}
void sortByRoomId()
{
    if (roomHead == NULL)
    {
        printf("No rooms found.\n");
        return;
    }

    room *tempHead = NULL, *tempTail = NULL, *current = roomHead;


    while (current != NULL)
    {
        room *newNode = (room *)malloc(sizeof(room));
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

    room *sortedList = NULL;


    room *splitList(room *head)
    {
        room *slow = head, *fast = head->next;
        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        room *middle = slow->next;
        slow->next = NULL;
        return middle;
    }


    room *mergeLists(room *left, room *right)
    {
        room dummy;
        room *tail = &dummy;
        dummy.next = NULL;

        while (left != NULL && right != NULL)
        {
            if (left->roomId < right->roomId)
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


    room *mergeSort(room *head)
    {
        if (head == NULL || head->next == NULL)
        {
            return head;
        }

        room *middle = splitList(head);
        room *left = mergeSort(head);
        room *right = mergeSort(middle);
        return mergeLists(left, right);
    }

    sortedList = mergeSort(tempHead);


    printf("--- Rooms Sorted by ID ---\n");
    room *temp = sortedList;
    while (temp != NULL)
    {
        printf("Room ID: %d\n",temp->roomId);
        printf("Room Type: %s\n", temp->roomType);
        printf("Total Beds: %d\n", temp->bedCount);
        printf("Available Beds: %d\n", temp->availableBeds);
        printf("Bed Status: %s\n", temp->bedStatus);
        printf("Room Fee: %f\n", temp->roomFee);
        printf("\n");
        temp = temp->next;
    }


    while (sortedList != NULL)
    {
        room *temp = sortedList;
        sortedList = sortedList->next;
        free(temp);
    }
}
