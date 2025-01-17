#ifndef PATIENT_H
#define PATIENT_H


typedef struct patient
{
    int patientId;
    char patientName[50];
    char patientGender[10];
    int patientAge;
    char patientAddress[50];
    char patientContactNumber[20];
    char patientEmergencyContactNumber[20];
    char patientStatus;  // 'A' for Active, 'D' for Deleted
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
void generatePatientData();


#endif // PATIENT_H
