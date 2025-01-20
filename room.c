#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "room.h"

#define ROOM_USER_ID "123"
#define ROOM_USER_PASSWORD "123"
#define ROOM_FILE_NAME "rooms.txt"
#define FILE_OPEN_ERROR 1
#define STATUS_1 "Occupied"
#define STATUS_2 "Vacant"
#define TYPE_1 "icu"
#define TYPE_2 "Private"
#define TYPE_3 "General"

room *roomHead = NULL;
room *roomTemp;
room *roomNode;
FILE *rm;
int lastRoomId=0;

void loadRoomDataFromFile()
{
    rm = fopen(ROOM_FILE_NAME, "r+");
    if (rm == NULL)
    {
        rm = fopen(ROOM_FILE_NAME, "w+");
        if (rm == NULL)
        {
            printf("Unable to open or create file.\n");
            exit(FILE_OPEN_ERROR);
        }
    }
    if (rm == NULL)
    {
        printf("File is not open for reading.\n");
        return;
    }
    rewind(rm);

    char line[256];
    while (fgets(line, sizeof(line), rm))
    {
        roomNode = (room *)malloc(sizeof(room));
        if (sscanf(line, "%5d,%9[^,],%10d,%10d,%9[^,],%10f,%c\n", &roomNode->roomId, roomNode->roomType, &roomNode->bedCount, &roomNode->availableBeds, roomNode->bedStatus, &roomNode->roomFee, &roomNode->roomStatus) == 7)
        {
            roomNode->next = NULL;
            insertRoomSorted();
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
            printf("Invalid User Id,enter valid user id with less than 15 characters\n ");
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
            printf("Invalid User password,enter valid user password with less than 15 characters\n ");
        }

    }

    if (strcmp(userId, ROOM_USER_ID) == 0 && strcmp(userPass, ROOM_USER_PASSWORD) == 0)
    {
        loadRoomDataFromFile();
        int option;
        while (true)
        {
            printf("\n--- Room Management System ---\n");
            printf("1. Add Room\n2. Update Room Details\n3. Display Available Rooms\n4. Search Room by ID\n5. Search Room by Type\n6. Check Room Availability\n7. Sort By Room ID\n8. Delete Room\n9. Display Deleted Room Records\n10. Exit from Room Menu\n");
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
            case DELETE_ROOM_BY_ID:
                deleteRoomById();
                break;
            case DISPLAY_DELETED_ROOM_RECORDS:
                displayDeletedRooms();
                break;
            case EXIT_ROOM_MANAGEMENT:
                printf(" Exiting from room menu\n");
                fclose(rm);
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
    fclose(rm);
}

void addRoom()
{
    roomNode = (room *)malloc(sizeof(room));
    if (!roomNode)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    roomNode->roomId =++lastRoomId;
    printf("Generated Room ID: %d\n", roomNode->roomId);
    while(true)
    {
        char type[MAX_FIXED_ROOM_TYPE];
        printf("Enter Room Type (icu/general/private): ");
        if(scanf("%s",type)==1 && strcasecmp(type,TYPE_1)==0 ||strcasecmp(type,TYPE_2)==0 || strcasecmp(type,TYPE_3)==0 )
        {
            strcpy(roomNode->roomType ,type);
            break;
        }
        else
        {
            printf("Invalid RoomType,Enter Room Type icu/general/private\n ");
        }

    }
    while(true)
    {
        printf("Enter Bed Count: ");
        int count =0;
        if(scanf("%d", &count)==1 && count >=0 )
        {
            roomNode->bedCount = count;
            break;
        }
        else
        {
            printf("Invalid count ,enter positive number\n");
        }

    }

    while(true)
    {
        printf("Enter Available Beds: ");
        int beds =0;
        if(scanf("%d", &beds) ==1 && beds >=0)
        {
            roomNode->availableBeds = beds;
            break;
        }
        else
        {
            printf("Invalid count ,enter positive number\n");

        }

    }
    while(true)
    {
        printf("Enter Bed Status (occupied/vacant): ");
        char status[MAX_FIXED_BED_STATUS];
        if(scanf("%s",&status)==1 && strcasecmp(status,STATUS_1)==0 || strcasecmp(status,STATUS_2)==0)
        {
            strcpy(roomNode->bedStatus, status);
            break;
        }
        else
        {
            printf("Invalid Status,Enter Occupied / Vacant\n");
        }

    }
    while(true)
    {
        float fee;
        printf("Enter Room Fee: ");
        if(scanf("%f",&fee) ==1 && fee >0)
        {
            roomNode->roomFee = fee;
            break;
        }
        else
        {
            printf("invalid fee,enter fee greater than 0");
        }
    }

    roomNode->roomStatus = 'A';
    roomNode->next = NULL;

    insertRoomSorted();
    // for(int i =0; i<10000; i++)
    // {
        fseek(rm, 0, SEEK_END);
        fprintf(rm, "%5d,%-9s,%10d,%10d,%-9s,%10.2f,%c\n", roomNode->roomId, roomNode->roomType, roomNode->bedCount, roomNode->availableBeds, roomNode->bedStatus, roomNode->roomFee, roomNode->roomStatus);

    // }
    fflush(rm);
    printf("Room added successfully and saved to file!\n");
}

void deleteRoomById()
{
    int id;
    printf("Enter Room ID to delete: ");
    scanf("%d", &id);

    roomTemp = roomHead;
    while (roomTemp != NULL)
    {
        if (roomTemp->roomId == id && roomTemp->roomStatus == 'A')
        {
            roomTemp->roomStatus = 'D';

            rewind(rm);
            long position;
            char line[256];
            while (fgets(line, sizeof(line), rm))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);
                if (existingId == id)
                {
                    position = ftell(rm) - strlen(line);
                    fseek(rm, position + sizeof(roomTemp->roomId)+sizeof(roomTemp->roomType)+sizeof(roomTemp->bedCount)+sizeof(roomTemp->availableBeds)+sizeof(roomTemp->bedStatus)+sizeof(roomTemp->roomFee)+22, SEEK_SET);
                    fprintf(rm, "%c", 'D');
                    printf("Room with ID %d marked as deleted.\n", id);

                    fflush(rm);
                    break;
                }
            }
            return;
        }
        roomTemp = roomTemp->next;
    }
    printf("Room with ID %d not found.\n", id);
}

void insertRoomSorted()
{
    if (roomHead == NULL || roomHead->roomId > roomNode->roomId)
    {
        roomNode->next = roomHead;
        roomHead = roomNode;
    }
    else
    {
        roomTemp = roomHead;
        while (roomTemp->next != NULL && roomTemp->next->roomId < roomNode->roomId)
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
        if (roomTemp->roomId == id && roomTemp->roomStatus == 'A')
        {
            printf("Updating details for Room ID %d...\n", roomTemp->roomId);
            printf("1. Room Type\n2. Bed Count\n3. Available Beds\n4. Bed Status\n5. Room Fee\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case UPDATE_ROOM_TYPE:
                while(true)
                {
                    char type[MAX_FIXED_ROOM_TYPE];
                    printf("New Room Type (icu/general/private): ");
                    if(scanf("%s",type)==1 && strcasecmp(type,TYPE_1)==0 ||strcasecmp(type,TYPE_2)==0 || strcasecmp(type,TYPE_3)==0 )
                    {
                        strcpy(roomTemp->roomType ,type);
                        break;
                    }
                    else
                    {
                        printf("Invalid RoomType,Enter Room Type icu/general/private\n ");
                    }

                }

                break;
            case UPDATE_BED_COUNT:
                while(true)
                {
                    printf("Enter Bed Count: ");
                    int count =0;
                    if(scanf("%d", &count)==1 && count >=0 )
                    {
                        roomTemp->bedCount = count;
                        break;
                    }
                    else
                    {
                        printf("Invalid count ,enter positive number\n");
                    }

                }
                break;
            case UPDATE_AVAILABLE_BEDS:
                while(true)
                {
                    printf("Enter Available Beds: ");
                    int beds =0;
                    if(scanf("%d", &beds) ==1 && beds >=0)
                    {
                        roomTemp->availableBeds = beds;
                        break;
                    }
                    else
                    {
                        printf("Invalid count ,enter positive number\n");

                    }

                }
                break;
            case UPDATE_BED_STATUS:
                while(true)
                {
                    printf("Enter Bed Status (occupied/vacant): ");
                    char status[MAX_FIXED_BED_STATUS];
                    if(scanf("%s",&status)==1 && strcasecmp(status,STATUS_1)==0 || strcasecmp(status,STATUS_2)==0)
                    {
                        strcpy(roomTemp->bedStatus, status);
                        break;
                    }
                    else
                    {
                        printf("Invalid Status,Enter Occupied / Vacant\n");
                    }

                }
                break;
            case UPDATE_ROOM_FEE:
                while(true)
                {
                    float fee;
                    printf("New Room Fee: ");
                    if(scanf("%f",&fee) ==1 && fee >0)
                    {
                        roomTemp->roomFee = fee;
                        break;
                    }
                    else
                    {
                        printf("invalid fee,enter fee greater than 0");
                    }
                }
                break;
            default:
                printf("Invalid choice.\n");
                return;
            }

            printf("Room details updated successfully.\n");

            rm = fopen("rooms.txt", "r+");
            if (rm == NULL)
            {
                printf("Unable to open file.\n");
                return;
            }
            rewind(rm);
            char line[256];
            long position;
            int found = 0;

            while (fgets(line, sizeof(line), rm))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);

                if (existingId == id)
                {
                    found = 1;

                    position = ftell(rm) - strlen(line);

                    fseek(rm, position, SEEK_SET);
                    switch (choice)
                    {
                    case UPDATE_ROOM_TYPE:
                        fseek(rm, position + sizeof(roomTemp->roomId)+1, SEEK_SET);
                        fprintf(rm, "%-9s", roomTemp->roomType);
                        break;
                    case UPDATE_BED_COUNT:
                        fseek(rm, position + sizeof(roomTemp->roomId)+sizeof(roomTemp->roomType)+1, SEEK_SET);
                        fprintf(rm, "%10d", roomTemp->bedCount);
                        break;
                    case UPDATE_AVAILABLE_BEDS:
                        fseek(rm, position + sizeof(roomTemp->roomId)+sizeof(roomTemp->roomType)+sizeof(roomTemp->bedCount)+8, SEEK_SET);
                        fprintf(rm, "%10d", roomTemp->availableBeds);
                        break;
                    case UPDATE_BED_STATUS:
                        fseek(rm, position + sizeof(roomTemp->roomId)+sizeof(roomTemp->roomType)+sizeof(roomTemp->bedCount)+sizeof(roomTemp->availableBeds)+15, SEEK_SET);
                        fprintf(rm, "%-9s", roomTemp->bedStatus);
                        break;
                    case UPDATE_ROOM_FEE:
                        fseek(rm, position + sizeof(roomTemp->roomId)+sizeof(roomTemp->roomType)+sizeof(roomTemp->bedCount)+sizeof(roomTemp->availableBeds)+sizeof(roomTemp->bedStatus)+15, SEEK_SET);
                        fprintf(rm, "%10.2f", roomTemp->roomFee);
                        break;
                    }
                    fflush(rm);
                    break;
                }
            }

            if (!found)
            {
                printf("Room with ID %d not found.\n", id);
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
        if (roomTemp->roomStatus == 'A')
        {
            printf("Room ID: %d\n", roomTemp->roomId);
            printf("Room Type: %s\n", roomTemp->roomType);
            printf("Bed Count: %d\n", roomTemp->bedCount);
            printf("Available Beds: %d\n", roomTemp->availableBeds);
            printf("Bed Status: %s\n", roomTemp->bedStatus);
            printf("Room Fee: %.2f\n", roomTemp->roomFee);
            printf("\n");
        }
        roomTemp = roomTemp->next;
    }
}

room* searchByRoomId()
{
    int id;
    printf("Enter Room ID to search: ");
    scanf("%d", &id);

    roomTemp = roomHead;
    while (roomTemp != NULL)
    {
        if (roomTemp->roomId == id && roomTemp->roomStatus == 'A')
        {
            printf("Room ID: %d\n", roomTemp->roomId);
            printf("Room Type: %s\n", roomTemp->roomType);
            printf("Bed Count: %d\n", roomTemp->bedCount);
            printf("Available Beds: %d\n", roomTemp->availableBeds);
            printf("Bed Status: %s\n", roomTemp->bedStatus);
            printf("Room Fee: %.2f\n", roomTemp->roomFee);
            printf("\n");
            return(roomTemp);
        }
        roomTemp = roomTemp->next;
    }
    printf("Room with ID %d not found.\n", id);
}

void searchByRoomType()
{
    char type[10];
    printf("Enter Room Type to search: ");
    scanf("%s", type);

    roomTemp = roomHead;
    while (roomTemp != NULL)
    {
        if (strcmp(roomTemp->roomType, type) == 0 && roomTemp->roomStatus == 'A')
        {
            printf("Room ID: %d\n", roomTemp->roomId);
            printf("Room Type: %s\n", roomTemp->roomType);
            printf("Bed Count: %d\n", roomTemp->bedCount);
            printf("Available Beds: %d\n", roomTemp->availableBeds);
            printf("Bed Status: %s\n", roomTemp->bedStatus);
            printf("Room Fee: %.2f\n", roomTemp->roomFee);
            printf("\n");
        }
        roomTemp = roomTemp->next;
    }
}


void checkAvailability()
{
    int id;
    printf("Enter Room ID to check availability: ");
    scanf("%d", &id);

    roomTemp = roomHead;
    while (roomTemp != NULL)
    {
        if (roomTemp->roomId == id && roomTemp->roomStatus == 'A')
        {
            if (roomTemp->availableBeds > 0)
            {
                printf("Room ID %d is available with %d beds.\n", id, roomTemp->availableBeds);
            }
            else
            {
                printf("Room ID %d is fully occupied.\n", id);
            }
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
void displayDeletedRooms()
{
    if (roomHead == NULL)
    {
        printf("No rooms to display.\n");
        return;
    }

    roomTemp = roomHead;
    printf("--- Deleted Room Records ---\n");
    while (roomTemp != NULL)
    {
        if (roomTemp->roomStatus == 'D')
        {
            printf("Room ID: %d\n", roomTemp->roomId);
            printf("Type: %s\n", roomTemp->roomType);
            printf("Bed Count: %d\n", roomTemp->bedCount);
            printf("Available Beds: %d\n", roomTemp->availableBeds);
            printf("Bed Status: %s\n", roomTemp->bedStatus);
            printf("Room Fee: %.2f\n", roomTemp->roomFee);
            printf("\n");
        }
        roomTemp = roomTemp->next;
    }

}


