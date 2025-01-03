#include <stdio.h>
#include <stdlib.h>
#include "patient.h"
#include "doctor.h"
#include "staff.h"
#include "treatment.h"
#include "pharmacy.h"
#include "room.h"
#include "bill.h"
#include "menu.h"



void menu()
{
    while (1)
    {
        int choice;
        printf("____Administrative Menu____\n");
        printf("1. Login As Patient Management User\n");
        printf("2. Login As Doctor Management User\n");
        printf("3. Login As Staff Management User\n");
        printf("4. Login As Treatment and Price Management User\n");
        printf("5. Login As Pharmacy Management User\n");
        printf("6. Login As Room Management User\n");
        printf("7. Login As Bill Management User\n");
        printf("8. Exit Administrative Menu\n");
        printf("Enter the choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case LOGIN_AS_PATIENT_MANAGEMENT_USER:
            loginAsPatientManagementUser();
            break;
        case LOGIN_AS_DOCTOR_MANAGEMENT_USER:
            loginAsDoctorManagementUser();
            break;
        case LOGIN_AS_STAFF_MANAGEMENT_USER:
            loginAsStaffManagementUser();
            break;
        case LOGIN_AS_TREATMENT_AND_PRICE_MANAGEMENT_USER:
            loginAsTreatmentAndPriceManagementUser();
            break;
        case LOGIN_AS_PHARMACY_MANAGEMENT_USER:
            loginAsPharmacyManagementUser();
            break;
        case LOGIN_AS_ROOM_MANAGEMENT_USER:
            loginAsRoomManagementUser();
            break;
        case LOGIN_AS_BILL_MANAGEMENT_USER:
            loginAsBillManagementUser();
            break;
        case EXIT_FROM_ADMINISTRATIVE_MENU:
            printf("Exiting the administrative Menu\n");
            return;
        default:
            printf("Invalid choice\n");
            break;
        }
    }
}
