#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "bill.h"
#include "patient.h"
#include "pharmacy.h"
#include "room.h"
#include "treatment.h"
#include "doctor.h"

#define BILL_FILE_NAME "bill.txt"
#define BILL_USER_ID "123"
#define BILL_USER_PASSWORD "123"

bill *billHead = NULL;
bill *billTemp;
bill *billNode;
FILE *fb;
int lastBillId = 0;

void loadBillDataFromFile()
{
    fb = fopen("bill.txt", "r+");
    if (fb == NULL)
    {
        fb = fopen("bill.txt", "w+");
        if (fb == NULL)
        {
            printf("Unable to open or create file.\n");
            exit(1);
        }
    }

    rewind(fb);
    char line[1024];
    while (fgets(line, sizeof(line), fb))
    {
        billNode = (bill *)malloc(sizeof(bill));
        if (sscanf(line, "%d,%49[^,],%99[^,],%f,%49[^,],%f,%49[^,],%f,%f,%19[^,],%f,%f,%14[^,],%c", &billNode->billId, billNode->patientName, billNode->treatmentName, &billNode->treatmentCost, billNode->doctorName, &billNode->consultationFee, billNode->medicineName, &billNode->medicineCost, &billNode->pharmacyFee, billNode->roomType, &billNode->roomFee, &billNode->totalBillAmount, billNode->billDate, &billNode->billStatus) == 14)
        {
            billNode->next = NULL;
            insertBillSortedById();
        } else
        {

            free(billNode);
        }
    }
    printf("Bill data loaded from file.\n");
}

void loginAsBillManagementUser()
{
    char userId[15];
    char userPass[15];
    while(true)
    {
        printf("User ID:\n");
        char id[15];
        if(scanf("%s",&id) ==1 && strlen(id) <=15)
        {
            strcpy(userId,id);
            break;
        }
        else
        {
            printf("Invalid User Id,enter valid user id with less than 15 characters \n");
        }

    }
    while(true)
    {
        printf("User Password:\n");
        char password[15];
        if(scanf("%s",&password) ==1 && strlen(password) <=15)
        {
            strcpy(userPass,password);
            break;
        }
        else
        {
            printf("Invalid User password,enter valid user id with less than 15 characters \n ");
        }

    }
    if (strcmp(userId, "123") == 0 && strcmp(userPass, "123") == 0)
    {
        loadBillDataFromFile();
        int option;
        while (1) {
            printf("\n--- Bill Management System ---\n");
            printf("1. Generate Bill\n2. Display Bills\n3. Delete by bill ID\n4. Display Deleted record\n5. search bill by patient Name\n6. Sort by bill ID\n7. Exit\n");
            printf("Enter your option: ");
            scanf("%d", &option);

            switch (option)
            {
            case GENERATE_BILL:
                generateBill();
                break;
            case DISPLAY_BILL_DETAILS:
                displayBillDetails();
                break;
            case DELETE_BILL_BY_ID:
                deleteBillById();
                break;
            case DISPLAY_DELETED_BILL_RECORDS:
                displayDeletedBillRecords();
                break;
            case SEARCH_BILL_BY_PATIENT_ID:
                searchBillByPatientName();
                break;
            case SORT_BILL_BY_BILL_ID:
                sortByBillId();
                break;
            case EXIT_BILL_MANAGEMENT:
                printf("Exiting bill management menu.\n");
                fclose(fb);
                return;
            default:
                printf("Invalid choice, try again.\n");
                break;
            }
        }
    }
    else
    {
        printf("Invalid User ID or Password!\n");
    }
    fclose(fb);
}

void generateBill()
{
    billNode = (bill *)malloc(sizeof(bill));
    if (!billNode)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    billNode->billId = ++lastBillId;
    printf("Generated Bill ID: %d\n", billNode->billId);

    loadPatientDataFromFile();
    patient *pTemp = searchByPatientId();
    strcpy(billNode->patientName, pTemp->patientName);

    loadTreatmentDataFromFile();
    treatment *tTemp = searchByTreatmentId();
    strcpy(billNode->treatmentName, tTemp->treatmentName);
    billNode->treatmentCost = tTemp->treatmentCost;

    loadDoctorDataFromFile();
    doctor *dTemp = searchByDoctorId();
    strcpy(billNode->doctorName, dTemp->doctorName);
    billNode->consultationFee = dTemp->doctorConsultationFee;

    loadPharmacyDataFromFile();
    pharmacy *mTemp = searchByMedicineId();
    strcpy(billNode->medicineName, mTemp->medicineName);
    billNode->medicineCost = mTemp->medicineCost;
    while(true)
    {
        int quantity = 0;
        int quan=0;
        printf("Enter medicine quantity provided to patient: ");
        if(scanf("%d", &quan)==0 && quan>=0)
        {
            quantity = quan;
            billNode->pharmacyFee = billNode->medicineCost * quantity;
            break;
        }
        else
        {
            printf("invalid quantity,Enter quantity greater than 0\n");
        }


    }


    loadRoomDataFromFile();
    room *rTemp = searchByRoomId();
    strcpy(billNode->roomType, rTemp->roomType);
    billNode->roomFee = rTemp->roomFee;

    printf("Enter Bill Date (DD-MM-YYYY): ");
    scanf(" %[^\n]", billNode->billDate);

    billNode->totalBillAmount = billNode->consultationFee + billNode->pharmacyFee + billNode->roomFee + billNode->treatmentCost;
    billNode->billStatus = 'A';
    billNode->next = NULL;

    insertBillSortedById();

    fseek(fb, 0, SEEK_END);
    fprintf(fb, "%5d,%-49s,%-99s,%.2f,%-14s,%.2f,%-49s,%.2f,%.2f,%-9s,%.2f,%.2f,%-14s,%c\n",
            billNode->billId, billNode->patientName, billNode->treatmentName, billNode->treatmentCost,
            billNode->doctorName, billNode->consultationFee, billNode->medicineName, billNode->medicineCost,
            billNode->pharmacyFee, billNode->roomType, billNode->roomFee, billNode->totalBillAmount,
            billNode->billDate, billNode->billStatus);

    printf("Bill added successfully and saved to file!\n");
    fflush(fb);
}

void insertBillSortedById()
{
    if (billHead == NULL || billHead->billId > billNode->billId)
    {
        billNode->next = billHead;
        billHead = billNode;
    }
    else
    {
        billTemp = billHead;
        while (billTemp->next != NULL && billTemp->next->billId < billNode->billId)
        {
            billTemp = billTemp->next;
        }
        billNode->next = billTemp->next;
        billTemp->next = billNode;
    }
}


void displayBillDetails()

{
    if (billHead == NULL)
    {
        printf("No available bills to display.\n");
        return;
    }
    int found=0;
    billTemp = billHead;
    while (billTemp != NULL)
    {
        if (billTemp->billStatus == 'A')
        {
            found =1;
            printf("\n*******************************BILL DETAILS*******************************\n");
            printf("Bill ID: %d\n", billTemp->billId);
            printf("Bill Date: %s\n\n", billTemp->billDate);
            printf("Patient Name: %s\n", billTemp->patientName);
            printf("\n----Treatment Details----\n");
            printf("Treatment Name: %s\n", billTemp->treatmentName);
            printf("TreatmentCost: %f\n",billTemp->treatmentCost);
            printf("Consulted Doctor : Dr.%s\n",billTemp->doctorName);
            printf("Consultation Fee: %.2f\n", billTemp->consultationFee);
            printf("\n----Medicine Details----\n");
            printf("Medicine Name : %s\n",billTemp->medicineName);
            printf("Medicine Cost :%f \n",billNode->medicineCost);
            printf("Pharmacy Fee: %.2f\n", billTemp->pharmacyFee);
            printf("\n----Room Details----\n");
            printf("Room Type : %s\n",billTemp->roomType);
            printf("Room Fee: %.2f\n", billTemp->roomFee);
            printf("\n_______________________________Total Amount: %.2f____________________________\n", billTemp->totalBillAmount);

        }
        billTemp = billTemp->next;
    }
    if(!found)
    {
        printf("No deleted bills found\n");
    }

}

void displayDeletedBillRecords()
{
    if(billHead == NULL)
    {
        printf("No bills available to display\n");
        return;
    }
    int found =0;
    billTemp = billHead;
    printf("Deleted bill Details\n");
    while(billTemp != NULL)
    {
        if(billTemp->billStatus == 'D')
        {
            found =1;
            printf("\n*******************************BILL DETAILS*******************************\n");
            printf("Bill ID: %d\n", billTemp->billId);
            printf("Bill Date: %s\n\n", billTemp->billDate);
            printf("Patient Name: %s\n", billTemp->patientName);
            printf("\n----Treatment Details----\n");
            printf("Treatment Name: %s\n", billTemp->treatmentName);
            printf("TreatmentCost: %f\n",billTemp->treatmentCost);
            printf("Consulted Doctor : Dr.%s\n",billTemp->doctorName);
            printf("Consultation Fee: %.2f\n", billTemp->consultationFee);
            printf("\n----Medicine Details----\n");
            printf("Medicine Name : %s\n",billTemp->medicineName);
            printf("Medicine Cost :%f \n",billNode->medicineCost);
            printf("Pharmacy Fee: %.2f\n", billTemp->pharmacyFee);
            printf("\n----Room Details----\n");
            printf("Room Type : %s\n",billTemp->roomType);
            printf("Room Fee: %.2f\n", billTemp->roomFee);
            printf("\n_______________________________Total Amount: %.2f____________________________\n", billTemp->totalBillAmount);

        }
        billTemp = billTemp->next;
    }
    if(!found)
    {
        printf("No deleted bills found\n");
    }
}

void deleteBillById()
{
    int id;
    int found =0;
    printf("Enter Bill ID to delete: ");
    scanf("%d", &id);

    billTemp = billHead;
    while (billTemp != NULL)
    {
        if (billTemp->billId == id)
        {
            found =1;
            billTemp->billStatus = 'D';
            rewind(fb);
            long position;
            char line[256];
            while (fgets(line, sizeof(line), fb))
            {
                int existingId;
                sscanf(line, "%d,", &existingId);
                if (existingId == id)
                {
                    position = ftell(fb) - strlen(line);
                    fseek(fb, position+295, SEEK_SET);
                    fprintf(fb, "%c", 'D');

                    printf("Bill with ID %d marked as deleted.\n", id);
                    fflush(fb);

                    break;
                }
            }
            return;
        }

        billTemp = billTemp->next;
    }
    if(!found)
    {
        printf("Bill with ID %d not found.\n", id);

    }

}
void searchBillByPatientName()
{
    int patientName;
    int found =0;
    printf("Enter Patient Name to search: ");
    scanf("%[^\n]", patientName);

    billTemp = billHead;
    while (billTemp != NULL)
    {
        if (strcasecmp(patientName, billTemp->patientName) == 0 && billTemp->billStatus == 'A')
        {
            found = 1;
            printf("--- Bill Found ---\n");
            printf("Bill ID: %d\n", billTemp->billId);
            printf("Bill Date: %s\n\n", billTemp->billDate);
            printf("Patient Name: %s\n", billTemp->patientName);
            printf("\n----Treatment Details----\n");
            printf("Treatment Name: %s\n", billTemp->treatmentName);
            printf("TreatmentCost: %f\n",billTemp->treatmentCost);
            printf("Consulted Doctor : Dr.%s\n",billTemp->doctorName);
            printf("Consultation Fee: %.2f\n", billTemp->consultationFee);
            printf("\n----Medicine Details----\n");
            printf("Medicine Name : %s\n",billTemp->medicineName);
            printf("Medicine Cost :%f \n",billNode->medicineCost);
            printf("Pharmacy Fee: %.2f\n", billTemp->pharmacyFee);
            printf("\n----Room Details----\n");
            printf("Room Type : %s\n",billTemp->roomType);
            printf("Room Fee: %.2f\n", billTemp->roomFee);
            printf("\n_______________________________Total Amount: %.2f____________________________\n", billTemp->totalBillAmount);

            printf("\n");
            return;
        }
        billTemp = billTemp->next;
    }
    if(!found)
    {
        printf("No bills found for Patient ID %d.\n", patientName);

    }
}


void sortByBillId()
{
    if (billHead == NULL)
    {
        printf("No bills found.\n");
        return;
    }

    bill *tempHead = NULL, *tempTail = NULL, *current = billHead;

    while (current != NULL)
    {
        bill *newNode = (bill *)malloc(sizeof(bill));
        if (!newNode)
        {
            printf("Memory allocation failed\n");
            return;
        }

        *newNode = *current;
        newNode->next = NULL;

        if (tempHead == NULL)
        {
            tempHead = newNode;
            tempTail = newNode;
        }
        else
        {
            tempTail->next = newNode;
            tempTail = newNode;
        }

        current = current->next;
    }

    bill *splitList(bill *head)
    {
        bill *slow = head, *fast = head->next;
        while (fast != NULL && fast->next != NULL)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        bill *middle = slow->next;
        slow->next = NULL;
        return middle;
    }

    bill *mergeLists(bill *left, bill *right)
    {
        bill dummy;
        bill *tail = &dummy;
        dummy.next = NULL;

        while (left != NULL && right != NULL)
        {
            if (left->billId < right->billId)
            {
                tail->next = left;
                left = left->next;
            }
            else
            {
                tail->next = right;
                right = right->next;
            }
            tail = tail->next;
        }
        tail->next = (left != NULL) ? left : right;
        return dummy.next;
    }

    bill *mergeSort(bill *head)
    {
        if (head == NULL || head->next == NULL)
        {
            return head;
        }

        bill *middle = splitList(head);
        bill *left = mergeSort(head);
        bill *right = mergeSort(middle);
        return mergeLists(left, right);
    }

    bill *sortedList = mergeSort(tempHead);

    printf("--- Bills Sorted by ID ---\n");
    bill *temp = sortedList;
    while (temp != NULL)
    {
        printf("Bill ID: %d\n", temp->billId);
        printf("Bill Date: %s\n\n", temp->billDate);
        printf("Patient Name: %s\n", temp->patientName);
        printf("Treatment Name: %s\n", temp->treatmentName);
        printf("TreatmentCost: %f\n",temp->treatmentCost);
        printf("Consulted Doctor : Dr.%s\n",temp->doctorName);
        printf("Consultation Fee: %.2f\n", temp->consultationFee);
        printf("Medicine Name : %s\n",temp->medicineName);
        printf("Medicine Cost :%f \n",temp->medicineCost);
        printf("Pharmacy Fee: %.2f\n", temp->pharmacyFee);
        printf("Room Type : %s\n",temp->roomType);
        printf("Room Fee: %.2f\n", temp->roomFee);
        printf("Total Amount: %.2f\n", billTemp->totalBillAmount);

        printf("\n");
        temp = temp->next;
    }

    while (sortedList != NULL)
    {
        bill *temp = sortedList;
        sortedList = sortedList->next;
        free(temp);
    }
}
