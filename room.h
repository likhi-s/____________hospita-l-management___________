
#ifndef ROOM_H
#define ROOM_H

typedef struct roomManagement
{
    int roomId;
    char roomType[10]; //icu/general/private
    int bedCount;
    int availableBeds;
    char bedStatus[10];// occupied/vacant
    float roomFee;
    char roomStatus;
    struct roomManagement *next;
}room;


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
void searchByRoomId();
void searchByRoomType();
void checkAvailability();
void sortByRoomId();
void deleteRoomById();

#endif // ROOM_H
