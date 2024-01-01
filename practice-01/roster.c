#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "roster.h"

char *strtrim(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

int add_student(FILE* in, FILE* out, char* students[], int current_students, int const max_students) {
    char name[101];
    fscanf(in, "%100[^\n]\n", name);
    char *trimmed_name = strtrim(name);
    
    fprintf(out, "Command: add %s\n\t", trimmed_name); // Added "Command:"

    if (current_students >= max_students) {
        fprintf(out, "Student %s not added. The class is already at capacity.\n", trimmed_name);
        return current_students;
    }
    
    students[current_students] = malloc(strlen(trimmed_name) + 1);
    strcpy(students[current_students], trimmed_name);
    current_students++;
    fprintf(out, "%s was added. %d spot(s) remain.\n", trimmed_name, max_students - current_students);
    return current_students;
}

void modify_name(FILE* in, FILE* out, char* students[], int current_students) {
    char old_name[101], new_name[101];
    fscanf(in, "%100[^:]:%100[^\n]\n", old_name, new_name);
    char *trimmed_old_name = strtrim(old_name);
    char *trimmed_new_name = strtrim(new_name);

    fprintf(out, "Command: modify %s:%s\n\t", trimmed_old_name, trimmed_new_name); // Added "Command:"
    
    for (int i = 0; i < current_students; i++) {
        if (strcmp(students[i], trimmed_old_name) == 0) {
            free(students[i]);  
            students[i] = malloc(strlen(trimmed_new_name) + 1);  
            strcpy(students[i], trimmed_new_name);  
            fprintf(out, "Student %s name modified to %s.\n", trimmed_old_name, trimmed_new_name);
            return;
        }
    }
    fprintf(out, "No student with name %s found.\n", trimmed_old_name);
}

int remove_student(FILE* in, FILE* out, char* students[], int current_students, int const max_students) {
    char name[101];
    fscanf(in, "%100[^\n]\n", name);
    char *trimmed_name = strtrim(name);

    fprintf(out, "Command: remove %s\n\t", trimmed_name); // Added "Command:"
    
    for (int i = 0; i < current_students; i++) {
        if (strcmp(students[i], trimmed_name) == 0) {
            free(students[i]);
            for (int j = i; j < current_students - 1; j++) {
                students[j] = students[j + 1];
            }
            current_students--;
            fprintf(out, "Student %s removed. The course now has %d seats remaining.\n", trimmed_name, max_students - current_students);
            return current_students;
        }
    }
    fprintf(out, "No student named %s was found to remove.\n", trimmed_name);
    return current_students;
}

void display_class(FILE* out, char* students[], int current_students) {
    fprintf(out, "Command: display\n\t"); // Added "Command:"
    fprintf(out, "Currently Enrolled:\n");
    for (int i = 0; i < current_students; i++) {
        fprintf(out, "\t\tStudent %d: %s\n", i + 1, students[i]);
    }
}

void delete_class(FILE* out, char* students[], int* current_students) {
    fprintf(out, "Command: delete\n"); // Added "Command:"
    for (int i = 0; i < *current_students; i++) {
        fprintf(out, "\t%s removed in class delete.\n", students[i]); // Adjusted format
        free(students[i]);
    }
    *current_students = 0;
    fprintf(out, "\tClass was restarted and reopened for enrollment.\n\n"); // Adjusted format
}
