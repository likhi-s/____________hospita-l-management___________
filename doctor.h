#ifndef DOCTOR_H
#define DOCTOR_H

#define MAX_FIXED_DOCTOR_NAME 50
#define MAX_FIXED_SPECIALIZATION 20
#define MAX_FIXED_CONTACT_NUMBER 15
#define MAX_FIXED_QUALIFICATION 50

typedef struct doctor
{
    int doctorId;
    char doctorName[MAX_FIXED_DOCTOR_NAME];
    char doctorSpecialization[MAX_FIXED_SPECIALIZATION];
    float doctorConsultationFee;
    char doctorContactNumber[MAX_FIXED_CONTACT_NUMBER];
    int doctorExperience;
    char doctorQualification[MAX_FIXED_QUALIFICATION];
    char doctorStatus;
    struct doctor *next;
} doctor;



enum DoctorManagementMenu
{
    ADD_DOCTOR = 1,
    UPDATE_DOCTOR_DETAILS,
    DISPLAY_AVAILABLE_DOCTORS,
    SEARCH_DOCTOR_BY_ID,
    SEARCH_DOCTOR_BY_SPECIALIZATION,
    SORT_DOCTORS_BY_ID,
    DELETE_DOCTOR_BY_ID,
    DISPLAY_DELETED_DOCTORS_RECORD,
    EXIT_DOCTOR_MANAGEMENT
} ;

enum DoctorUpdateDetail
{
    UPDATE_DOCTOR_NAME=1,
    UPDATE_DOCTOR_SPECIALIZATION,
    UPDATE_DOCTOR_CONSULTATION_FEE,
    UPDATE_DOCTOR_CONTACT_NUMBER,
    UPDATE_DOCTOR_EXPERIENCE,
    UPDATE_DOCTOR_QUALIFICATION
};


void loginAsDoctorManagementUser();
void loadDoctorDataFromFile();
void addDoctor();
void updateDoctorDetails();
void displayDoctorDetails();
doctor* searchByDoctorId();
void searchByDoctorSpecialization();
void sortDoctorsById();
void insertDoctorSortedByName();
void deleteDoctorById();

#endif // DOCTOR_H
