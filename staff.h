#ifndef STAFF_H
#define STAFF_H
typedef struct staff
{
    int staffId;
    char staffName[100];
    char staffRole[50];      //eg:nurse,technician,receptionist
    char staffShift[10];    //day or night
    float staffSalary;
    char staffContactNumber[15];
    struct staff *next;
}staff;



enum StaffManagementMenu
{
    ADD_STAFF = 1,
    UPDATE_STAFF_DETAILS,
    DISPLAY_AVAILABLE_STAFF,
    SEARCH_STAFF_BY_ID,
    SEARCH_STAFF_BY_ROLE,
    SORT_BY_STAFF_ID,
    EXIT_STAFF_MANAGEMENT
} ;

enum StaffUpdateDetail
{
    UPDATE_STAFF_NAME=1,
    UPDATE_STAFF_ROLE,
    UPDATE_STAFF_SHIFT,
    UPDATE_STAFF_SALARY,
    UPDATE_STAFF_CONTACT_NUMBER,
};


void loginAsStaffManagementUser();
void addStaff();
void updateStaffDetails();
void displayStaffDetails();
void searchByStaffId();
void searchByStaffRole();
void sortByStaffId();


#endif // STAFF_H
