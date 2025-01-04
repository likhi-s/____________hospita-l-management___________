#ifndef BILL_H
#define BILL_H

typedef struct bill
{
    int billId;
    int patientId;
    int treatmentId;
    float consultationFee;
    float pharmacyFee;
    float roomFee;
    float totalBillAmount;
    char billDate[20];
    char billStatus;
    struct bill *next;
} bill;


enum BillMenuOption
{
    ADD_BILL = 1,
    UPDATE_BILL_DETAILS,
    DISPLAY_BILL_DETAILS,
    SEARCH_BILL_BY_PATIENT_ID,
    CALCULATE_BILL,
    SORT_BILL_BY_BILL_ID,
    DELETE_BILL_BY_ID,
    DISPLAY_DELETED_BILL_RECORDS,
    EXIT_BILL_MANAGEMENT
};
enum BillUpdateChoice
{

    UPDATE_BILL_WITH_PATIENT_ID=1,
    UPDATE_BILL_WITH_TREATMENT_ID,
    UPDATE_BILL_WITH_CONSULTATION_FEE,
    UPDATE_BILL_WITH_PHARMACY_FEE,
    UPDATE_BILL_WITH_ROOM_FEE,
    UPDATE_BILL_DATE
};


void loginAsBillManagementUser();
void addBill();
void updateBillDetails();
void displayBillDetails();
void searchBillByPatientId();
void calculateBill();
void deleteBillById();
void sortByBillId();

#endif // BILL_H
