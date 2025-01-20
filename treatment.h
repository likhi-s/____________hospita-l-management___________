#ifndef TREATMENT_H
#define TREATMENT_H
#define MAX_FIXED_TREATMENT_NAME 100

typedef struct treatment
{
    int treatmentId;
    char treatmentName[MAX_FIXED_TREATMENT_NAME];
    int treatmentCost;
    int treatmentDuration;
    char treatmentStatus;
    struct treatment *next;
} treatment;


enum TreatmentManagementOptions
{
    ADD_TREATMENT = 1,
    UPDATE_TREATMENT_DETAILS,
    DISPLAY_ALL_TREATMENTS,
    SEARCH_TREATMENT_BY_ID,
    SEARCH_TREATMENT_BY_NAME,
    SORT_TREATMENT_BY_ID,
    DELETE_TREATMENT_BY_ID,
    DISPLAY_DELETED_TREATMENT_RECORDS,
    EXIT_TREATMENT_MANAGEMENT
};

enum TreatmentUpdateOptions
{
    UPDATE_TREATMENT_NAME=1,
    UPDATE_TREATMENT_COST,
    UPDATE_TREATMENT_DURATION
};



void loginAsTreatmentAndPriceManagementUser();
void loadTreatmentDataFromFile();
void addTreatment();
void updateTreatmentDetails();
void displayTreatment();
treatment* searchByTreatmentId();
void searchByTreatmentName();
void sortTreatmentById();
void deleteTreatmentById();



#endif // TREATMENT_H
