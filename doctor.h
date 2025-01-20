#ifndef DOCTOR_H
#define DOCTOR_H

typedef struct doctor
{
    int doctorId;
    char doctorName[50];
    char doctorSpecialization[20];
    float doctorConsultationFee;
    char doctorContactNumber[15];
    int doctorExperience;
    char doctorQualification[50];
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
