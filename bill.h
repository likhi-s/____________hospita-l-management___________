#ifndef BILL_H
#define BILL_H

typedef struct bill
{
    int billId;
    char patientName[50];
    char treatmentName[100];
    float treatmentCost;
    char doctorName[15];
    float consultationFee;
    char medicineName[50];
    float medicineCost;
    float pharmacyFee;
    char roomType[10];
    float roomFee;
    float totalBillAmount;
    char billDate[15];
    char billStatus;
    struct bill *next;
} bill;


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
