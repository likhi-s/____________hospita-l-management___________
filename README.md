# ____________HOSPITAL MANAGEMENT___________

## Overview

The Hospital Management System is designed to manage various aspects of hospital operations, like Patient Management,Doctor Management,Staff Management,Pharmacy Management,Treatment Management,Room Management,Billing Management.

## Features

### 1. Patient Management
- Register, update, delete patient records.
- Search patients by ID or name.
- Display active or deleted patient records.
- Sort patients by ID.

### 2. Doctor Management
- Register, update, delete Doctor records.
- Search Doctor by ID or name.
- Display active or deleted Doctor records.
- Sort Doctor by ID.

### 3. Staff Management
- Register, update, delete staff records.
- Search staff by ID or name.
- Display active or deleted staff records.
- Sort staff by ID.

### 4. Pharmacy Management
- Register, update, delete medicine records.
- Search medicine by ID or name.
- Check medicine Stock.
- Display active or deleted medicine records.
- Sort medicine by ID.


### 5. Treatment Management
- Register, update, delete treatment records.
- Search treatment by ID or name.
- Display active or deleted treatment records.
- Sort treatment by ID.

  
### 6. Room Management
- Add, update, and delete room details.
- Search for rooms by ID or type.
- Check room availability.
- Display available rooms and sort them by ID.
  

### 7. Billing Management
- Generate and display patient bills.
- Delete and retrieve deleted bill records.
- Search bills by patient name.
- Sort bills by ID.

### Header Files
- **patient.h**: Contains structures and functions for patient management.
- **doctor.h**: Contains structures and functions related to doctor management.
- **staff.h**: Contains structures and functions related to staff management.
- **pharmacy.h**: Contains structures and functions related to pharmacy management.
- **treatment.h**: Contains structures and functions related to treatment management.
- **room.h**: Manages room details and availability.
- **bill.h**: Contains structures and functions related to billing management.

### Key Functions
- **Patient Management**:
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

-**Doctor Management**:
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

- **Staff Management**: 
void loginAsStaffManagementUser();
void addStaff();
void updateStaffDetails();
void displayStaffDetails();
void searchByStaffId();
void searchByStaffRole();
void sortByStaffId();
void deleteStaffById();

- **Pharmacy Management**:
void  loginAsPharmacyManagementUser();
void loadPharmacyDataFromFile();
void addMedicine();
void updateMedicineDetails();
void displayMedicineDetails();
pharmacy* searchByMedicineId();
void searchByMedicineName();
void checkMedicineStock();
void sortByMedicineId();
void deleteMedicineById();

- **Treatment Management**:
void loginAsTreatmentAndPriceManagementUser();
void loadTreatmentDataFromFile();
void addTreatment();
void updateTreatmentDetails();
void displayTreatment();
treatment* searchByTreatmentId();
void searchByTreatmentName();
void sortTreatmentById();
void deleteTreatmentById();

- **Room Managemnet**:
void loginAsRoomManagementUser();
void addRoom();
void updateRoomDetails();
void displayRoomDetails();
room* searchByRoomId();
void searchByRoomType();
void checkAvailability();
void sortByRoomId();
void deleteRoomById();

- **Billing Management**:
void loginAsBillManagementUser();
void generateBill();
void displayBillDetails();
void deleteBillById();
void displayDeletedBillRecords();
void searchBillByPatientName();
void sortByBillId();
void insertBillSortedById();


## How to Use

1. Clone the repository or copy the source files to your local system.
2. Compile the project using a C compiler (e.g., GCC):
   gcc main.c -o hospital_management
