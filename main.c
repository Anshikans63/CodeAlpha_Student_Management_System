#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Student {
    int id;
    char name[50];
    int age;
};

void clearInputBuffer();
void pauseScreen();
int loadStudents(struct Student students[]);
void saveStudents(struct Student students[], int count);
void sortByID(struct Student students[], int count);
void sortByName(struct Student students[], int count);

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void displaySortedByName();
void countStudents();

void clearInputBuffer() {
    while (getchar() != '\n');
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
}

int loadStudents(struct Student students[]) {
    FILE *fp = fopen("students.dat", "rb");
    int count = 0;

    if (fp == NULL)
        return 0;

    while (fread(&students[count], sizeof(struct Student), 1, fp)) {
        count++;
        if (count >= MAX) break;
    }

    fclose(fp);
    return count;
}

void saveStudents(struct Student students[], int count) {
    FILE *fp = fopen("students.dat", "wb");

    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    fwrite(students, sizeof(struct Student), count, fp);
    fclose(fp);
}

void sortByID(struct Student students[], int count) {
    int i, j;
    struct Student temp;

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (students[i].id > students[j].id) {
                temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
}

void sortByName(struct Student students[], int count) {
    int i, j;
    struct Student temp;

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (strcmp(students[i].name, students[j].name) > 0) {
                temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
}

void addStudent() {
    struct Student students[MAX];
    struct Student newStudent;
    int count = loadStudents(students);
    int i;

    if (count >= MAX) {
        printf("Storage Full!\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &newStudent.id);
    clearInputBuffer();

    for (i = 0; i < count; i++) {
        if (students[i].id == newStudent.id) {
            printf("ID already exists!\n");
            return;
        }
    }

    printf("Enter Full Name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    newStudent.name[strcspn(newStudent.name, "\n")] = 0;

    printf("Enter Age: ");
    scanf("%d", &newStudent.age);
    clearInputBuffer();

    students[count] = newStudent;
    count++;

    sortByID(students, count);
    saveStudents(students, count);

    printf("Student added successfully.\n");
}

void displayStudents() {
    struct Student students[MAX];
    int count = loadStudents(students);
    int i;

    if (count == 0) {
        printf("No records found.\n");
        return;
    }

    sortByID(students, count);

    printf("\n-------------------------------------\n");
    printf("ID\tName\t\tAge\n");
    printf("-------------------------------------\n");

    for (i = 0; i < count; i++) {
        printf("%d\t%-15s\t%d\n",
               students[i].id,
               students[i].name,
               students[i].age);
    }
}

void searchStudent() {
    struct Student students[MAX];
    int count = loadStudents(students);
    int id, i, found = 0;

    if (count == 0) {
        printf("No records found.\n");
        return;
    }

    printf("Enter ID to search: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (i = 0; i < count; i++) {
        if (students[i].id == id) {
            printf("\nStudent Found\n");
            printf("ID: %d\n", students[i].id);
            printf("Name: %s\n", students[i].name);
            printf("Age: %d\n", students[i].age);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student not found.\n");
}

void updateStudent() {
    struct Student students[MAX];
    int count = loadStudents(students);
    int id, i, found = 0;

    if (count == 0) {
        printf("No records found.\n");
        return;
    }

    printf("Enter ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (i = 0; i < count; i++) {
        if (students[i].id == id) {
            printf("Enter New Name: ");
            fgets(students[i].name, sizeof(students[i].name), stdin);
            students[i].name[strcspn(students[i].name, "\n")] = 0;

            printf("Enter New Age: ");
            scanf("%d", &students[i].age);
            clearInputBuffer();

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student not found.\n");
        return;
    }

    sortByID(students, count);
    saveStudents(students, count);
    printf("Updated successfully.\n");
}

void deleteStudent() {
    struct Student students[MAX];
    int count = loadStudents(students);
    int id, i, j, found = 0;

    if (count == 0) {
        printf("No records found.\n");
        return;
    }

    printf("Enter ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();

    for (i = 0; i < count; i++) {
        if (students[i].id == id) {
            for (j = i; j < count - 1; j++) {
                students[j] = students[j + 1];
            }
            count--;
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student not found.\n");
        return;
    }

    saveStudents(students, count);
    printf("Deleted successfully.\n");
}

void displaySortedByName() {
    struct Student students[MAX];
    int count = loadStudents(students);
    int i;

    if (count == 0) {
        printf("No records found.\n");
        return;
    }

    sortByName(students, count);

    printf("\nSorted By Name\n");
    printf("-------------------------------------\n");

    for (i = 0; i < count; i++) {
        printf("%d\t%-15s\t%d\n",
               students[i].id,
               students[i].name,
               students[i].age);
    }
}

void countStudents() {
    struct Student students[MAX];
    int count = loadStudents(students);

    printf("Total Students: %d\n", count);
}

int main() {
    int choice;

    while (1) {
        system("cls");

        printf("===== STUDENT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students (Sorted by ID)\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Display Sorted by Name\n");
        printf("7. Total Students\n");
        printf("8. Exit\n");
        printf("Enter choice: ");

        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: displaySortedByName(); break;
            case 7: countStudents(); break;
            case 8: exit(0);
            default: printf("Invalid choice!\n");
        }

        pauseScreen();
    }
}