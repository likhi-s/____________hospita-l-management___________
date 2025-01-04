#ifndef PHARMACY_H
#define PHARMACY_H
typedef struct pharmacy
{
    int medicineId;
    char medicineName[50];
    float medicineCost;
    int medicineStockQuantity;
    char medicineType[20];// tablet/injection
    char medicineDosage[20];
    char medicineStatus;
    struct pharmacy *next;
}pharmacy;


enum PharmacyMenuOption
{
    ADD_NEW_MEDICINE = 1,
    UPDATE_MEDICINE_DETAILS,
    DISPLAY_AVAILABLE_MEDICINES,
    SEARCH_MEDICINE_BY_ID,
    SEARCH_MEDICINE_BY_NAME,
    CHECK_STOCK,
    SORT_BY_MEDICINE_ID,
    DELETE_BY_MEDICINE_ID,
    DISPLAY_DELETED_MEDICINE_RECORDS,
    EXIT_PHARMACY_MANAGEMENT
};
enum MedicineDetailOption
{

    UPDATE_MEDICINE_NAME=1,
    UPDATE_MEDICINE_COST,
    UPDATE_MEDICINE_STOCK,
    UPDATE_MEDICINE_TYPE,
    UPDATE_MEDICINE_DOSAGE
};
void  loginAsPharmacyManagementUser();
void addMedicine();
void updateMedicineDetails();
void displayMedicineDetails();
void searchByMedicineId();
void searchByMedicineName();
void checkMedicineStock();
void sortByMedicineId();
void deleteMedicineById();

#endif // PHARMACY_H
