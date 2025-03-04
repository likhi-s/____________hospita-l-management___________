
#ifndef ROOM_H
#define ROOM_H

#define MAX_FIXED_ROOM_TYPE 10
#define MAX_FIXED_BED_STATUS 10

typedef struct roomManagement
{
    int roomId;
    char roomType[MAX_FIXED_ROOM_TYPE];
    int bedCount;
    int availableBeds;
    char bedStatus[MAX_FIXED_BED_STATUS];
    float roomFee;
    char roomStatus;
    struct roomManagement *next;
} room;



enum RoomManagementOptions
{
    ADD_ROOM = 1,
    UPDATE_ROOM_DETAILS,
    DISPLAY_AVAILABLE_ROOMS,
    SEARCH_ROOM_BY_ID,
    SEARCH_ROOM_BY_TYPE,
    CHECK_ROOM_AVAILABILITY,
    SORT_BY_ROOM_ID,
    DELETE_ROOM_BY_ID,
    DISPLAY_DELETED_ROOM_RECORDS,
    EXIT_ROOM_MANAGEMENT
};

enum RoomUpdateChoices
{
    UPDATE_ROOM_TYPE = 1,
    UPDATE_BED_COUNT,
    UPDATE_AVAILABLE_BEDS,
    UPDATE_BED_STATUS,
    UPDATE_ROOM_FEE
};


void loginAsRoomManagementUser();
void addRoom();
void updateRoomDetails();
void displayRoomDetails();
room* searchByRoomId();
void searchByRoomType();
void checkAvailability();
void sortByRoomId();
void deleteRoomById();
void generateRoomData();

#endif // ROOM_H
