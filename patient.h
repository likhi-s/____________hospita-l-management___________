#ifndef PATIENT_H
#define PATIENT_H


#define MAX_FIXED_PATIENT_NAME 50
#define MAX_FIXED_GENDER 10
#define MAX_FIXED_ADDRESS 50
#define MAX_FIXED_CONTACT_NUMBER 20
#define MAX_FIXED_EMERGENCY_CONTACT 20

typedef struct patient
{
    int patientId;
    char patientName[MAX_FIXED_PATIENT_NAME];
    char patientGender[MAX_FIXED_GENDER];
    int patientAge;
    char patientAddress[MAX_FIXED_ADDRESS];
    char patientContactNumber[MAX_FIXED_CONTACT_NUMBER];
    char patientEmergencyContactNumber[MAX_FIXED_EMERGENCY_CONTACT];
    char patientStatus;
    struct patient* next;
} patient;



enum PatientManagementMenu
{
    REGISTER_PATIENT = 1,
    UPDATE_PATIENT_DETAILS,
    DISPLAY_AVAILABLE_PATIENTS_BY_NAME,
    SEARCH_PATIENT_BY_ID,
    SEARCH_PATIENT_BY_NAME,
    SORT_BY_PATIENT_ID,
    DELETE_PATIENT_ID,
    DISPLAY_DELETED_PATIENTS_RECORDS,
    EXIT_PATIENT_MANAGEMENT
} ;
enum PatientUpdateDetail
{

    UPDATE_PATIENT_NAME=1,
    UPDATE_PATIENT_GENDER,
    UPDATE_PATIENT_AGE,
    UPDATE_PATIENT_ADDRESS,
    UPDATE_PATIENT_CONTACT_NUMBER,
    UPDATE_PATIENT_EMERGENCY_CONTACT_NUMBER
};


void loadPatientDataFromFile();
void loginAsPatientManagementUser();
void registerPatient();//insert by name while registration only
void displayPatientDetails();
void updatePatientDetails();
patient* searchByPatientId();
void searchByPatientName();
void sortPatientsById();
void deletePatient();
void displayDeletedRecords();
void insertPatientSorted();


#endif // PATIENT_H
