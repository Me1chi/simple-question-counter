#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREENWIDTH 1200.0
#define SCREENHEIGHT 675.0
#define BLANKSPACESIZE 20.0

typedef struct {

    int right_answers;
    int medium_answers;
    int wrong_answers;
    int time_spent; //in seconds

} SUBJECT;

//functions signatures (beggining)
void user_subjects_rw(char mode, int *subjects, SUBJECT* subject_vector);

int button_logic_drawing(Vector2 position, Vector2 size, Color button_color, char* text);

//end of functions signatures

int main() {

    //subjects vector variables
    SUBJECT* user_subjects;
    int user_subjects_counter = 0;

    //reads the stored user subjects array. If it doesn't exist, it will be created in the 1st use 
    user_subjects_rw('r', &user_subjects_counter, user_subjects);

    //window initializing
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Question counter");


    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(DARKGRAY);

        button_logic_drawing((Vector2){SCREENWIDTH/2 - 100, SCREENHEIGHT/2 - 100}, (Vector2) {200, 200}, RED, "Teste");

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
                    
                    //allocates memory at runtime to store the user subjects
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

int button_logic_drawing(Vector2 position, Vector2 size, Color button_color, char* text) {
// the logic action here, i.e, the changing in the value of a subject, will be done in the main loop
// using the returned value. 

    float font_size = SCREENHEIGHT/20;

    bool hovering = false;
    int click = 0;
    Vector2 mouse_pointer = GetMousePosition();

    Rectangle button = {
        position.x,
        position.y,
        size.x,
        size.y
    };

    //button highlight and click
    if (CheckCollisionPointRec(mouse_pointer, button)) {
        hovering = true;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            click = 1;
    }

    //button drawing 
    DrawRectangleRounded(button, 0.6, 20, button_color);
    
    DrawRectangleRoundedLines(button, 0.6, 20, 1, BLACK);

    Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 2);

    position.x += size.x/2 - text_size.x/2.0;
    position.y += size.y/2 - text_size.y/2.0;

    DrawTextEx(GetFontDefault(), text, position, font_size, 2, BLACK);
    
    return click;
}