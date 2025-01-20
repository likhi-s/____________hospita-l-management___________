#ifndef BILL_H
#define BILL_H

#define MAX_FIXED_PATIENT_NAME 50
#define MAX_FIXED_TREATMENT_NAME 100
#define MAX_FIXED_DOCTOR_NAME 15
#define MAX_FIXED_MEDICINE_NAME 50
#define MAX_FIXED_ROOM_TYPE 10
#define MAX_FIXED_BILL_DATE 15

typedef struct bill
{
    int billId;
    char patientName[MAX_FIXED_PATIENT_NAME];
    char treatmentName[MAX_FIXED_TREATMENT_NAME];
    float treatmentCost;
    char doctorName[MAX_FIXED_DOCTOR_NAME];
    float consultationFee;
    char medicineName[MAX_FIXED_MEDICINE_NAME];
    float medicineCost;
    float pharmacyFee;
    char roomType[MAX_FIXED_ROOM_TYPE];
    float roomFee;
    float totalBillAmount;
    char billDate[MAX_FIXED_BILL_DATE];
    char billStatus;
    struct bill *next;
}bill;


enum BillMenuOption
{
    GENERATE_BILL = 1,
    DISPLAY_BILL_DETAILS,
    DELETE_BILL_BY_ID,
    DISPLAY_DELETED_BILL_RECORDS,
    SEARCH_BILL_BY_PATIENT_ID,
    SORT_BILL_BY_BILL_ID,
    EXIT_BILL_MANAGEMENT
};



void loginAsBillManagementUser();
void generateBill();
void displayBillDetails();
void deleteBillById();
void displayDeletedBillRecords();
void searchBillByPatientName();
void sortByBillId();
void insertBillSortedById();


#endif // BILL_H
