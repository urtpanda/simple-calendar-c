#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>   // For SetConsoleTextAttribute()

// Function to set console text color
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Event Structure
typedef struct {
    char date[11];      // DD/MM/YYYY
    char description[100];
} Event;

// Function Declarations
int isLeapYear(int year);
int getDaysInMonth(int month, int year);
int getStartDay(int month, int year);
void displayCalendar(int month, int year);
void addEvent();
void viewEvents();
void deleteEvent();
void searchEvent();

// Check Leap Year
int isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

// Get total days in a month
int getDaysInMonth(int month, int year) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2: return isLeapYear(year) ? 29 : 28;
        default: return 0;
    }
}

// Calculate start day of month
int getStartDay(int month, int year) {
    int day = 1;
    int totalDays = 0;

    for (int y = 1900; y < year; y++) {
        totalDays += isLeapYear(y) ? 366 : 365;
    }
    for (int m = 1; m < month; m++) {
        totalDays += getDaysInMonth(m, year);
    }

    return (totalDays + day) % 7;
}

// Display Calendar with Colors
void displayCalendar(int month, int year) {
    int startDay = getStartDay(month, year);
    int days = getDaysInMonth(month, year);

    setColor(11);
    printf("\n        Calendar - %02d/%d\n", month, year);
    printf(" Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");
    setColor(7);

    // Print leading spaces
    for (int i = 0; i < startDay; i++) {
        printf("     ");
    }

    // Print all days
    for (int day = 1; day <= days; day++) {
        if ((startDay % 7) == 0) {
            setColor(9);  // Sunday Blue
        }
        else if ((startDay % 7) == 6) {
            setColor(12); // Saturday Red
        }
        else {
            setColor(10); // Weekdays Green
        }

        printf("%4d ", day);
        setColor(7);

        startDay++;
        if (startDay % 7 == 0) printf("\n");
    }
    printf("\n");
}

// Add Event
void addEvent() {
    Event e;
    FILE *file = fopen("events.txt", "a");
    if (!file) {
        setColor(12);
        printf("Error opening file!\n");
        setColor(7);
        return;
    }

    setColor(14);
    printf("Enter the event date (DD/MM/YYYY): ");
    setColor(7);
    scanf("%s", e.date);

    setColor(14);
    printf("Enter event description: ");
    setColor(7);
    getchar();
    fgets(e.description, 100, stdin);

    fprintf(file, "%s|%s", e.date, e.description);
    fclose(file);

    setColor(10);
    printf("Event added successfully!\n");
    setColor(7);
}

// View Events
void viewEvents() {
    Event e;
    FILE *file = fopen("events.txt", "r");
    if (!file) {
        setColor(12);
        printf("No events found!\n");
        setColor(7);
        return;
    }

    setColor(11);
    printf("\n----- Events List -----\n");
    setColor(7);

    while (fscanf(file, "%10[^|]|%99[^\n]\n", e.date, e.description) != EOF) {
        setColor(10);
        printf("Date: %s\n", e.date);
        setColor(7);
        printf("Event: %s\n", e.description);
    }
    fclose(file);
}

// Search Event by Date
void searchEvent() {
    char searchDate[11];
    Event e;
    int found = 0;

    setColor(14);
    printf("Enter the date to search (DD/MM/YYYY): ");
    setColor(7);
    scanf("%s", searchDate);

    FILE *file = fopen("events.txt", "r");
    if (!file) {
        setColor(12);
        printf("No events found!\n");
        setColor(7);
        return;
    }

    while (fscanf(file, "%10[^|]|%99[^\n]\n", e.date, e.description) != EOF) {
        if (strcmp(e.date, searchDate) == 0) {
            setColor(10);
            printf("Event found on %s: %s\n", e.date, e.description);
            setColor(7);
            found = 1;
        }
    }
    fclose(file);

    if (!found) {
        setColor(12);
        printf("No events found on this date.\n");
        setColor(7);
    }
}

// Delete Event by Date
void deleteEvent() {
    char deleteDate[11];
    Event e;
    int found = 0;

    setColor(14);
    printf("Enter the date of the event to delete (DD/MM/YYYY): ");
    setColor(7);
    scanf("%s", deleteDate);

    FILE *file = fopen("events.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!file || !temp) {
        setColor(12);
        printf("Error opening file!\n");
        setColor(7);
        return;
    }

    while (fscanf(file, "%10[^|]|%99[^\n]\n", e.date, e.description) != EOF) {
        if (strcmp(e.date, deleteDate) != 0) {
            fprintf(temp, "%s|%s\n", e.date, e.description);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(temp);

    remove("events.txt");
    rename("temp.txt", "events.txt");

    if (found) {
        setColor(10);
        printf("Event on %s deleted successfully!\n", deleteDate);
        setColor(7);
    } else {
        setColor(12);
        printf("No event found on this date.\n");
        setColor(7);
    }
}

// Main Function
int main() {
    int choice, month, year;

    do {
        setColor(10);
        printf("\n----------- Welcome to Simple Calendar Application -----------\n");
        setColor(7);

        printf("1. Add Event\n");
        printf("2. View Events\n");
        printf("3. Delete Event\n");
        printf("4. Search Event\n");
        printf("5. Display Calendar\n");
        printf("6. Exit\n");

        setColor(14);
        printf("Enter your choice: ");
        setColor(7);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addEvent();
                break;
            case 2:
                viewEvents();
                break;
            case 3:
                deleteEvent();
                break;
            case 4:
                searchEvent();
                break;
            case 5:
                setColor(14);
                printf("Enter the month and year (MM YYYY): ");
                setColor(7);
                scanf("%d %d", &month, &year);
                if (month < 1 || month > 12 || year < 1900) {
                    setColor(12);
                    printf("Invalid input! Month (1-12) and year (>= 1900).\n");
                    setColor(7);
                } else {
                    displayCalendar(month, year);
                }
                break;
            case 6:
                setColor(10);
                printf("Exiting... Thank you for using the application!\n");
                setColor(7);
                break;
            default:
                setColor(12);
                printf("Invalid choice! Please try again.\n");
                setColor(7);
        }
    } while (choice != 6);

    return 0;
}
