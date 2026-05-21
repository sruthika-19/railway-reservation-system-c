#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { MALE, FEMALE } Gender;

typedef struct 
{
    char name[50];
    int age;
    Gender gender;
    int coachType;
    int seatNo;
    char pnr[20];
    char source[30];
    char destination[30];
    char journeyDate[11]; // DD-MM-YYYY
} Passenger;

#define MAX_AC_SEAT 60
#define MAX_NONAC_SEAT 60
#define MAX_SEATER_SEAT 60
#define MAX_WAITING_LIST 10

int acCount = 0;
int nonAcCount = 0;
int seaterCount = 0;

int acWLCount = 0;
int nonAcWLCount = 0;
int seaterWLCount = 0;

int serialNo = 100;

Passenger acCoach[MAX_AC_SEAT];
Passenger nonAcCoach[MAX_NONAC_SEAT];
Passenger seaterCoach[MAX_SEATER_SEAT];

Passenger acWaitingList[MAX_WAITING_LIST];
Passenger nonAcWaitingList[MAX_WAITING_LIST];
Passenger seaterWaitingList[MAX_WAITING_LIST];

void booking();
void generatePNR(Passenger * p);
void addToWaitingList(int coachType, Passenger * p);
void checkAvailability();
void cancellation();
void shiftWaitingList(int coachType);
void initializeSystem();
void displayMenu();
void prepareChart();
void saveToFile(Passenger p, int seatNumber, char status[]);
void loadFromFile();                       
void updateFileAfterShift(Passenger p, int newSeat); 
void updateFileAfterCancellation(Passenger p); 
void saveChartToFile();
void loadSystemData();
void saveSystemData();

int main()
{
    initializeSystem();
    int choice;
    do 
    {
        displayMenu();
        printf("Enter Choice (1 - 6): ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            booking();
            break;

        case 2:
            checkAvailability();
            break;

        case 3:
            cancellation();
            break;

        case 4:
            prepareChart();
            break;

        case 5:
            remove("reservation.txt");
            remove("system.dat");

            acCount = nonAcCount = seaterCount = 0;
            acWLCount = nonAcWLCount = seaterWLCount = 0;
            serialNo = 100;

            printf("System Reset Successfully!\n");
            break;
        
        case 6:
            printf("Exiting System...\n");
            break;
        
        default:
            printf("Invalid Entry....Try Again!\n");
            break;
        }
    } while (choice != 6);

    return 0;
}

void displayMenu()
{
    printf("\n--- RAILWAY RESERVATION MENU ---\n");
    printf("\t1. Booking\n");
    printf("\t2. Check Availability\n");
    printf("\t3. Cancellation\n");
    printf("\t4. Prepare Chart\n");
    printf("\t5. Reset System\n");
    printf("\t6. Exit\n");

}

void prepareChart()
{
    int i;
    char *genderText;

    printf("\nTotal AC Tickets Booked: %d\n", acCount);
    printf("Total Non-AC Tickets Booked: %d\n", nonAcCount);
    printf("Total Seater Tickets Booked: %d\n", seaterCount);
    printf("Total AC Waiting List Tickets: %d\n", acWLCount);
    printf("Total Non-AC Waiting List Tickets: %d\n", nonAcWLCount);
    printf("Total Seater Waiting List Tickets: %d\n", seaterWLCount);

    printf("\nAC COACH RESERVATIONS\n");
    for (i = 0; i < acCount; i++)
    {
        genderText = (acCoach[i].gender == MALE) ? "Male" : "Female";
        printf("Name: %-30s\t, Gender: %-8s\t, Seat No: %-4d\t, Coach: AC\t, PNR: %-10s, From: %-15s, To: %-15s, Journey On: %-10s\n", acCoach[i].name, genderText, acCoach[i].seatNo, acCoach[i].pnr, acCoach[i].source, acCoach[i].destination, acCoach[i].journeyDate);
    }

    printf("\nNON-AC COACH RESERVATIONS\n");
    for (i = 0; i < nonAcCount; i++)
    {
        genderText = (nonAcCoach[i].gender == MALE) ? "Male" : "Female";
        printf("Name: %-30s\t, Gender: %-8s\t, Seat No: %-4d\t, Coach: Non-AC\t, PNR: %-10s, From: %-15s, To: %-15s, Journey On: %-10s\n", nonAcCoach[i].name, genderText, nonAcCoach[i].seatNo, nonAcCoach[i].pnr, nonAcCoach[i].source, nonAcCoach[i].destination, nonAcCoach[i].journeyDate);
    }

    printf("\nSEATER COACH RESERVATION\n");
    for (i = 0; i < seaterCount; i++)
    {
        genderText = (seaterCoach[i].gender==MALE) ? "Male" : "Female";
        printf("Name: %-30s\t, Gender: %-8s\t, Seat No: %-4d\t, Coach: Seater\t, PNR: %-10s, From: %-15s, To: %-15s, Journey On: %-10s\n", seaterCoach[i].name, genderText, seaterCoach[i].seatNo, seaterCoach[i].pnr, seaterCoach[i].source, seaterCoach[i].destination, seaterCoach[i].journeyDate);
    }

    printf("\nAC COACH WAITING LIST\n");
    for (i = 0; i < acWLCount; i++)
    {
        genderText = (acWaitingList[i].gender==MALE) ? "Male" : "Female";
        printf("Name: %-30s\t, Gender: %-8s\t, Status: Waiting\t, PNR: %-10s, From: %-15s, To: %-15s, Journey On: %-10s\n", acWaitingList[i].name, genderText, acWaitingList[i].pnr, acWaitingList[i].source ,acWaitingList[i].destination, acWaitingList[i].journeyDate);
    }

    printf("\nNON-AC COACH WAITING LIST\n");
    for (i = 0; i < nonAcWLCount; i++)
    {
        
        genderText = (nonAcWaitingList[i].gender==MALE) ? "Male" : "Female";
        printf("Name: %-30s\t, Gender: %-8s\t, Status: Waiting\t, PNR: %-10s, From: %-15s, To: %-15s, Journey On: %-10s\n", nonAcWaitingList[i].name, genderText, nonAcWaitingList[i].pnr, nonAcWaitingList[i].source, nonAcWaitingList[i].destination, nonAcWaitingList[i].journeyDate);
    }

    printf("\nSEATER COACH WAITING LIST\n");
    for (i = 0; i < seaterWLCount; i++)
    {
        genderText = (seaterWaitingList[i].gender==MALE) ? "Male" : "Female";
        printf("Name: %-30s\t, Gender: %-8s\t, Status: Waiting\t, PNR: %-10s, From: %-15s, To: %-15s, Journey On: %-10s\n", seaterWaitingList[i].name, genderText, seaterWaitingList[i].pnr, seaterWaitingList[i].source, seaterWaitingList[i].destination, seaterWaitingList[i].journeyDate);
    }
    printf("\n=================================================\n");
}

void initializeSystem()
{
    loadSystemData();
    loadFromFile();
    printf("System initialized.\n");
}

void saveChartToFile()
{
    FILE *fp = fopen("chart.txt", "w");
    if (fp == NULL) 
        return;

    fprintf(fp, "AC Waiting List:\n");
    for (int i = 0; i < acWLCount; i++)
        fprintf(fp, "%d, %s\n", i+1, acWaitingList[i].name);

    fprintf(fp, "\nNon-AC Waiting List:\n");
    for (int i = 0; i < nonAcWLCount; i++)
        fprintf(fp, "%d, %s\n", i+1, nonAcWaitingList[i].name);

    fprintf(fp, "\nSeater Waiting List:\n");
    for (int i = 0; i < seaterWLCount; i++)
        fprintf(fp, "%d, %s\n", i+1, seaterWaitingList[i].name);

    fclose(fp);
}

void loadSystemData()
{
    FILE *fp = fopen("system.dat", "rb"); 

    if (fp == NULL)
        return;

    fread(&acCount, sizeof(int), 1, fp);
    fread(&nonAcCount, sizeof(int), 1, fp);
    fread(&seaterCount, sizeof(int), 1, fp);
    fread(&acWLCount, sizeof(int), 1, fp);
    fread(&nonAcWLCount, sizeof(int), 1, fp);
    fread(&seaterWLCount, sizeof(int), 1, fp);
    fread(&serialNo, sizeof(int), 1, fp);

    fread(acCoach, sizeof(Passenger), acCount, fp);
    fread(nonAcCoach, sizeof(Passenger), nonAcCount, fp);
    fread(seaterCoach, sizeof(Passenger), seaterCount, fp);
    fread(acWaitingList, sizeof(Passenger), acWLCount, fp);
    fread(nonAcWaitingList, sizeof(Passenger), nonAcWLCount, fp);
    fread(seaterWaitingList, sizeof(Passenger), seaterWLCount, fp);

    fclose(fp);
}

void saveSystemData()
{
    FILE *fp = fopen("system.dat", "wb");
    if (fp == NULL) 
        return;

    fwrite(&acCount, sizeof(int), 1, fp);
    fwrite(&nonAcCount, sizeof(int), 1, fp);
    fwrite(&seaterCount, sizeof(int), 1, fp);
    fwrite(&acWLCount, sizeof(int), 1, fp);
    fwrite(&nonAcWLCount, sizeof(int), 1, fp);
    fwrite(&seaterWLCount, sizeof(int), 1, fp);
    fwrite(&serialNo, sizeof(int), 1, fp);

    fwrite(acCoach, sizeof(Passenger), acCount, fp);
    fwrite(nonAcCoach, sizeof(Passenger), nonAcCount, fp);
    fwrite(seaterCoach, sizeof(Passenger), seaterCount, fp);
    fwrite(acWaitingList, sizeof(Passenger), acWLCount, fp);
    fwrite(nonAcWaitingList, sizeof(Passenger), nonAcWLCount, fp);
    fwrite(seaterWaitingList, sizeof(Passenger), seaterWLCount, fp);

    fclose(fp);
}

void checkAvailability()
{
    printf("\n------ SEAT AVAILABILITY ------");
    printf("\n AC SEATS            : %d available", MAX_AC_SEAT - acCount);
    printf("\n NON AC SEATS        : %d available", MAX_NONAC_SEAT - nonAcCount);
    printf("\n SEATER SEATS        : %d available", MAX_SEATER_SEAT - seaterCount);
    printf("\n AC WAITING LIST     : %d available", MAX_WAITING_LIST - acWLCount);
    printf("\n NON-AC WAITING LIST : %d available", MAX_WAITING_LIST - nonAcWLCount);
    printf("\n SEATER WAITING LIST : %d available", MAX_WAITING_LIST - seaterWLCount);
}

void cancellation()
{
    char pnr[20];
    int found = 0;

    printf("\nEnter PNR to cancel: ");
    if (scanf("%19s", pnr) != 1 || strlen(pnr) == 0)
    {
        printf("Invalid input!\n");
        return;
    }

    for (int i = 0; i < acCount && !found; i++)
    {
        if (strcmp(acCoach[i].pnr, pnr) == 0)
        {
            Passenger p = acCoach[i];

            for (int j = i; j < acCount - 1; j++)
                acCoach[j] = acCoach[j + 1];

            acCount--;

            updateFileAfterCancellation(p);
            shiftWaitingList(1);
            // any error if
            printf("From       : %s\n", p.source);
            printf("To         : %s\n", p.destination);
            printf("Journey On : %s\n", p.journeyDate);
            printf("Cancelled AC seat!\n");

            found = 1;
        }
    }
    
    for (int i = 0; i < nonAcCount && !found; i++)
    {
        if (strcmp(nonAcCoach[i].pnr, pnr) == 0)
        {
            Passenger p = nonAcCoach[i];

            for (int j = i; j < nonAcCount - 1; j++)
                nonAcCoach[j] = nonAcCoach[j + 1];

            nonAcCount--;

            updateFileAfterCancellation(p);
            shiftWaitingList(2);
            printf("From       : %s\n", p.source);
            printf("To         : %s\n", p.destination);
            printf("Journey On : %s\n", p.journeyDate);
            printf("Cancelled Non-AC seat!\n");

            found = 1;
        }
    }
    
    for (int i = 0; i < seaterCount && !found; i++)
    {
        if (strcmp(seaterCoach[i].pnr, pnr) == 0)
        {
            Passenger p = seaterCoach[i];

            for (int j = i; j < seaterCount - 1; j++)
                seaterCoach[j] = seaterCoach[j + 1];

            seaterCount--;

            updateFileAfterCancellation(p);
            shiftWaitingList(3);
            printf("From       : %s\n", p.source);
            printf("To         : %s\n", p.destination);
            printf("Journey On : %s\n", p.journeyDate);
            printf("Cancelled Seater seat!\n");

            found = 1;
        }
    }

    if (!found)
        printf("PNR not found!\n");

    saveSystemData();
}

void shiftWaitingList(int coachType)
{
    Passenger p;

    if (coachType == 1)
    {
        if (acWLCount == 0 || acCount >= MAX_AC_SEAT) return;

        p = acWaitingList[0];

        for (int i = 0; i < acWLCount - 1; i++)
            acWaitingList[i] = acWaitingList[i + 1];

        acWLCount--;
        p.seatNo = acCount + 1;
        acCoach[acCount++] = p;

        updateFileAfterShift(p, p.seatNo);
    }
    else if (coachType == 2)
    {
        if (nonAcWLCount == 0 || nonAcCount >= MAX_NONAC_SEAT) return;

        p = nonAcWaitingList[0];

        for (int i = 0; i < nonAcWLCount - 1; i++)
            nonAcWaitingList[i] = nonAcWaitingList[i + 1];

        nonAcWLCount--;
        p.seatNo = nonAcCount + 1;
        nonAcCoach[nonAcCount++] = p;

        updateFileAfterShift(p, p.seatNo);
    }
    else if (coachType == 3)
    {
        if (seaterWLCount == 0 || seaterCount >= MAX_SEATER_SEAT) return;

        p = seaterWaitingList[0];

        for (int i = 0; i < seaterWLCount - 1; i++)
            seaterWaitingList[i] = seaterWaitingList[i + 1];

        seaterWLCount--;
        p.seatNo = seaterCount + 1;
        seaterCoach[seaterCount++] = p;

        updateFileAfterShift(p, p.seatNo);
    }
    else
    {
        return;
    }
    p.coachType = coachType;

    printf("\nWaiting-list passenger %s Promoted to confirmed seat.\n", p.name);
    saveSystemData();
}

void updateFileAfterCancellation(Passenger p)
{
    FILE *fp = fopen("reservation.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char line[256];
    int lineNo = 0;

    while(fgets(line, sizeof(line), fp))
    {
        if(lineNo++ == 0) 
        { 
            fputs(line, temp); 
            continue;
        } 

        char pnr[20], name[50], gender[10], status[20], source[20], destination[20], date[12];
        int age, coach, seat;

        if(sscanf(line,"%s\t%49[^\t]\t%d\t%s\t%d\t%d\t%29[^\t]\t%29[^\t]\t%11[^\t]\t%s",
        pnr, name, &age, gender, &coach, &seat,source, destination, date, status)==10)
        {
            if(strcmp(pnr, p.pnr)==0)
                strcpy(status, "CANCELLED");

            fprintf(temp,"%-10s\t%-50s\t%-4d\t%-8s\t%-5d\t%-5d\t%-15s\t%-15s\t%-12s\t%-10s\n",
                pnr, name, age, gender, coach, seat, source, destination, date, status);
        }
    }
    fclose(fp); fclose(temp);
    remove("reservation.txt");
    rename("temp.txt","reservation.txt");
}

void booking()
{
    int ticketCnt, cnt = 0, gen;

    do 
    {
        printf("Enter Number of Tickets to be Booked(1 - 3): ");
        
        if (scanf("%d", &ticketCnt) != 1) 
        {
            printf("Invalid input. Try again.\n");
            while(getchar() != '\n');

            ticketCnt = 0;
            continue;
        }

    } while (ticketCnt < 1 || ticketCnt > 3);
    
    while (cnt < ticketCnt)
    {
        Passenger p = {0};
        do 
        {
            printf("Enter Name: ");
            scanf(" %49[^\n]", p.name);

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

        } while (strlen(p.name) == 0);

        do {
            printf("Enter Age: ");
            if (scanf("%d", &p.age) != 1) 
            {
                printf("Invalid input. Try again.\n");

                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                p.age = 0;
                continue;
            }

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            
        } while (p.age <= 0 || p.age > 120);

        do 
        {
            printf("Enter Gender:\n");
            printf("\t1. Male\n");
            printf("\t2. Female\n");
            printf("Choose (1 or 2): ");

            if (scanf("%d", &gen) != 1)
            {
                printf("Invalid input. Try again.\n");

                while (getchar() != '\n');
                gen = 0;
                continue;
            }

            while (getchar() != '\n');

        } while (gen != 1 && gen != 2);

        p.gender = (gen == 1) ? MALE : FEMALE;

        printf("Enter Source Station: ");
        scanf(" %29[^\n]", p.source);

        printf("Enter Destination Station: ");
        scanf(" %29[^\n]", p.destination);

        printf("Enter Journey Date (DD-MM-YYYY): ");
        scanf(" %10s", p.journeyDate);

        do 
        {
            printf("Enter Coach Type:\n");
            printf("\t1. AC Coach\n");
            printf("\t2. NON-AC Coach\n");
            printf("\t3. SEATER Coach\n");

            if (scanf("%d", &p.coachType) != 1)
            {
                printf("Invalid input. Try again.\n");

                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                p.coachType = 0;
                continue;
            }

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

        } while (p.coachType < 1 || p.coachType > 3);

        int seatAllocated = 0;

        if (p.coachType == 1 && acCount < MAX_AC_SEAT)
        {
            p.seatNo = acCount + 1;
            generatePNR(&p);
            acCoach[acCount++] = p;
            saveToFile(p, p.seatNo, "CONFIRMED");
            seatAllocated = 1;
        }
        else if (p.coachType == 2 && nonAcCount < MAX_NONAC_SEAT)
        {
            p.seatNo = nonAcCount + 1;
            generatePNR(&p);
            nonAcCoach[nonAcCount++] = p;
            saveToFile(p, p.seatNo, "CONFIRMED");
            seatAllocated = 1;
        }
        else if (p.coachType == 3 && seaterCount < MAX_SEATER_SEAT)
        {
            p.seatNo = seaterCount + 1;
            generatePNR(&p);
            seaterCoach[seaterCount++] = p;
            saveToFile(p, p.seatNo, "CONFIRMED");
            seatAllocated = 1;
        }

        if (!seatAllocated)
        {           
            if ((p.coachType == 1 && acWLCount < MAX_WAITING_LIST) ||
               (p.coachType == 2 && nonAcWLCount < MAX_WAITING_LIST) ||
               (p.coachType == 3 && seaterWLCount < MAX_WAITING_LIST))
            {
                p.seatNo = -1;
                generatePNR(&p);

                addToWaitingList(p.coachType, &p);
                saveToFile(p, -1, "WAITING");

                printf("Added to Waiting List.\n");
            }

            else
            {
                int available = 0;
                printf("No Seats Available in Selected coach...\n");
                
                if (acCount < MAX_AC_SEAT)
                {
                    printf("Seats Available in AC Coach\n");
                    available = 1;
                }

                if (nonAcCount < MAX_NONAC_SEAT)
                {
                    printf("Seats Available in NON-AC Coach\n");
                    available = 1;
                }
                    
                if (seaterCount < MAX_SEATER_SEAT)
                {
                    printf("Seats Available in Seater Coach\n");
                    available = 1;
                }

                if (available)
                {
                    printf("Please Try Booking in Other Coaches\n");
                    cnt--;
                    continue;
                }
                    else
                    printf("Sorry... All Seats are Booked\n");

            }
        }
        
        cnt++;
    }
    saveSystemData();
}

void generatePNR(Passenger * p)
{
    char first = toupper(p->name[0]);
    char second = (strlen(p->name) > 1) ? toupper(p->name[1]) : 'X';

   snprintf(p->pnr, sizeof(p->pnr), "%c%c%d%d-%d",
        first,
        second,
        p->age,
        p->coachType,
        serialNo++); 
}

void addToWaitingList(int coachType, Passenger * p)
{
    if (coachType == 1 && acWLCount < MAX_WAITING_LIST)
    {
        acWaitingList[acWLCount++] = *p;
        printf("\nWaiting List Number: %d", acWLCount);
    }
    else if (coachType == 2 && nonAcWLCount < MAX_WAITING_LIST)
    {
        nonAcWaitingList[nonAcWLCount++] = *p;
        printf("\nWaiting List Number: %d", nonAcWLCount);
    }
    else if (coachType == 3 && seaterWLCount < MAX_WAITING_LIST)
    {
        seaterWaitingList[seaterWLCount++] = *p;
        printf("\nWaiting List Number: %d", seaterWLCount);
    }
}

void saveToFile(Passenger p, int seatNumber, char status[])
{
    FILE *fp = fopen("reservation.txt", "a+");
    
    if (fp == NULL)
    {
        fp = fopen("reservation.txt", "w");
        if (fp == NULL)
        {
            printf("\nError Opening File!");
            return;
        }
    }

    fseek(fp, 0, SEEK_END);
    if (ftell(fp)== 0)
    {
        fprintf(fp, "%-10s\t%-50s\t%-4s\t%-8s\t%-5s\t%-5s\t%-15s\t%-15s\t%-12s\t%-10s\n",
        "PNR", "NAME", "AGE", "GENDER", "COACH", "SEAT", "SOURCE", "DEST", "DATE", "STATUS");
    }

   fprintf(fp, "%-10s\t%-50s\t%-4d\t%-8s\t%-5d\t%-5d\t%-15s\t%-15s\t%-12s\t%-10s\n",
        p.pnr,
        p.name,
        p.age,
        (p.gender == MALE ? "MALE" : "FEMALE"),
        p.coachType,
        seatNumber,
        p.source,
        p.destination,
        p.journeyDate,
        status);

    fclose(fp);
}

void loadFromFile()
{
    FILE *fp = fopen("reservation.txt", "r");
    if (!fp) 
        return;

    char line[256];
    int lineNo = 0;

    while (fgets(line, sizeof(line), fp))
    {
        lineNo++;
        if (lineNo == 1) 
            continue; 

        Passenger p;
        char genderStr[10], status[20];
        int seat, coach;

        if (sscanf(line, "%s\t%49[^\t]\t%d\t%s\t%d\t%d\t%29[^\t]\t%29[^\t]\t%11[^\t]\t%s",
                p.pnr, p.name, &p.age, genderStr, &coach, &seat,  p.source, p.destination, p.journeyDate, status) == 10)
        {
            p.gender = strcmp(genderStr, "MALE") == 0 ? MALE : FEMALE;
            p.coachType = coach;
            p.seatNo = seat;

            if (strcmp(status, "CONFIRMED") == 0)
            {
                if (p.coachType == 1 && acCount < MAX_AC_SEAT) 
                    acCoach[acCount++] = p;
                else if (p.coachType == 2 && nonAcCount < MAX_NONAC_SEAT) 
                    nonAcCoach[nonAcCount++] = p;
                else if (p.coachType == 3 && seaterCount < MAX_SEATER_SEAT) 
                    seaterCoach[seaterCount++] = p;
            }
            else if (strcmp(status, "WAITING") == 0)
            {
                addToWaitingList(p.coachType, &p);
            }
        }
    }

    fclose(fp);
}

void updateFileAfterShift(Passenger p, int newSeat)
{
    FILE *fp = fopen("reservation.txt", "r");
    if (!fp) 
        return;

    FILE *temp = fopen("temp.txt", "w");
    if (!temp)
    {
        fclose(fp);
        return;
    }

    char line[256];
    int lineNo = 0;

    while (fgets(line, sizeof(line), fp))
    {
        lineNo++;
        if (lineNo == 1)
        {
            fputs(line, temp);
            continue;
        }

        char pnr[20], name[50], genderStr[10], status[20], source[20], destination[20], date[12];
        int age, seat, coach;

        if (sscanf(line, "%s\t%49[^\t]\t%d\t%s\t%d\t%d\t%29[^\t]\t%29[^\t]\t%11[^\t]\t%s",
           pnr, name, &age, genderStr, &coach, &seat, source, destination, date, status) == 10)
        {
            if (strcmp(pnr, p.pnr) == 0)
            {
                seat = newSeat;
                strcpy(status, "CONFIRMED");
            }

        fprintf(temp,"%-10s\t%-50s\t%-4d\t%-8s\t%-5d\t%-5d\t%-15s\t%-15s\t%-12s\t%-10s\n",
        pnr, name, age, genderStr, coach, seat, source, destination, date, status);

        }
    }

    fclose(fp);
    fclose(temp);
    remove("reservation.txt");
    rename("temp.txt", "reservation.txt");
}
