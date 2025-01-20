#ifndef STAFF_H
#define STAFF_H
#define MAX_FIXED_STAFF_NAME 50
#define MAX_FIXED_STAFF_ROLE 20
#define MAX_FIXED_STAFF_SHIFT 10
#define MAX_FIXED_STAFF_CONTACT_NUMBER 15

typedef struct staff
{
    int staffId;
    char staffName[MAX_FIXED_STAFF_NAME];
    char staffRole[MAX_FIXED_STAFF_ROLE];
    char staffShift[MAX_FIXED_STAFF_SHIFT];
    float staffSalary;
    char staffContactNumber[MAX_FIXED_STAFF_CONTACT_NUMBER];
    char staffStatus;
    struct staff *next;
} staff;


enum StaffManagementMenu
{
    ADD_STAFF = 1,
    UPDATE_STAFF_DETAILS,
    DISPLAY_AVAILABLE_STAFF,
    SEARCH_STAFF_BY_ID,
    SEARCH_STAFF_BY_ROLE,
    SORT_BY_STAFF_ID,
    DELETE_STAFF_BY_ID,
    DISPLAY_DELETED_STAFF_RECORDS,
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
void deleteStaffById();

#endif // STAFF_H
