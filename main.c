#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREENWIDTH 1200.0
#define SCREENHEIGHT 675.0

typedef struct {

    int right_answers;
    int medium_answers;
    int wrong_answers;
    int time_spent; //in seconds

} SUBJECT;

void user_subjects_rw(char mode, int *subjects, SUBJECT* subject_vector);

int main() {

    //subjects vector variables
    SUBJECT* user_subjects;
    int user_subjects_counter = 0;

    user_subjects_rw('r', &user_subjects_counter, user_subjects);

    //window initializing
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Question counter");


    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(DARKGRAY);

        DrawRectangle(SCREENWIDTH/2, SCREENHEIGHT/2, 160, 90, BLUE);

        EndDrawing();
    }

    user_subjects_rw('w', &user_subjects_counter, user_subjects);

    //window closing 
    CloseWindow();

    //frees the subjects vector
    if (user_subjects_counter > 0)
        free(user_subjects);

    return 0;

}

void user_subjects_rw(char mode, int *subjects, SUBJECT* subject_vector) {

    FILE *fileptr;

    switch (mode) {
        case 'r':
            if ((fileptr = fopen("user_subjects.bin", "rb")) != NULL) {
                SUBJECT read = {0};

                while(!feof(fileptr)) {
                    
                    if (fread(&read, sizeof(read), 1, fileptr) == 1) {
                        if (*subjects == 0)
                            subject_vector = malloc(sizeof(SUBJECT));
                        else 
                            subject_vector = (SUBJECT *)realloc(subject_vector, (*subjects)*sizeof(SUBJECT));

                        subject_vector[*subjects] = read;

                        (*subjects)++;
                    }

                }

            } else
                printf("File doesn't exist yet or reading error!!\n");
            break;


        case 'w':
            if ((fileptr = fopen("user_subjects.bin", "wb")) != NULL) {
                
                if (fwrite(subject_vector, sizeof(SUBJECT), *subjects, fileptr) != *subjects)
                    printf("Error in file writing!\n");

            } else
                printf("Error in file writing!\n");


            break;
    }

}