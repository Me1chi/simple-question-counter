#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREENWIDTH 1200.0
#define SCREENHEIGHT 675.0
#define BLANKSPACESIZE 30.0
#define STANDARDBUTTONCURVE 0.3
#define BUTTONLINETHICKNESSDIV 400.0
#define SHADOWCOLOR (Color){40, 40, 40, 180}

typedef struct {

    int right_answers;
    int medium_answers;
    int wrong_answers;
    int time_spent; //in seconds

} SUBJECT;

//functions signatures (beggining)
void user_subjects_rw(char mode, int *subjects, SUBJECT** subject_vector);

int button(Vector2 position, Vector2 size, Color button_color, char* text, bool clickable);

void push_coord_right(Vector2* coordinates, float distance);

int question_button_kit(Vector2* initial_button_pos, float big_size, float small_size, Color button_color, int number_to_display);

void wont_be_negative(int* number);

void draw_centralized_text(Vector2 button_position, Vector2 button_size, float font_size, char* text, Color text_color); //meant to use inside a button

//end of functions signatures

int main() {

    //subjects vector variables
    SUBJECT* user_subjects;
    int user_subjects_counter = 0;

    //reads the stored user subjects array. If it doesn't exist, it will be created in the 1st use 
    user_subjects_rw('r', &user_subjects_counter, &user_subjects);

    //buttons drawing control (for the counter)
    float big_button_size = (SCREENWIDTH - 4*BLANKSPACESIZE)/3;

    float small_button_size = (big_button_size - BLANKSPACESIZE/2)/2;

    float timer_size_x = SCREENWIDTH - 2*BLANKSPACESIZE;
    float timer_size_y = SCREENHEIGHT - ((3/2)*BLANKSPACESIZE + big_button_size + small_button_size);
    Vector2 timer_size_vec = {timer_size_x, timer_size_y};

    Vector2 button_drawing_pos;

    //target fps = 60!!!
    SetTargetFPS(60);

    //window initializing
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Question counter");


    SUBJECT user_test = {0};


    while (!WindowShouldClose()) {

        //button position reset
        button_drawing_pos = (Vector2){BLANKSPACESIZE, BLANKSPACESIZE};

        BeginDrawing();

        ClearBackground(DARKGRAY);

        button(button_drawing_pos, timer_size_vec, GRAY, //esqueci de dar o git pull entao falta o formatador)
        button_drawing_pos.y += timer_size_y;

        user_test.wrong_answers += question_button_kit(&button_drawing_pos, big_button_size, small_button_size, RED, user_test.wrong_answers);
        wont_be_negative(&user_test.wrong_answers);

        user_test.medium_answers += question_button_kit(&button_drawing_pos, big_button_size, small_button_size, YELLOW, user_test.medium_answers);
        wont_be_negative(&user_test.medium_answers);

        user_test.right_answers += question_button_kit(&button_drawing_pos, big_button_size, small_button_size, GREEN, user_test.right_answers);
        wont_be_negative(&user_test.right_answers);

        EndDrawing();
    }

    user_subjects_rw('w', &user_subjects_counter, &user_subjects);

    //window closing 
    CloseWindow();

    //frees the subjects vector
    if (user_subjects_counter > 0)
        free(user_subjects);

    return 0;

}

void user_subjects_rw(char mode, int *subjects, SUBJECT** subject_vector) {

    FILE *fileptr;

    switch (mode) {
        case 'r':
            if ((fileptr = fopen("user_subjects.bin", "rb")) != NULL) {
                SUBJECT read = {0};

                while(!feof(fileptr)) {
                    
                    //allocates memory at runtime to store the user subjects
                    if (fread(&read, sizeof(read), 1, fileptr) == 1) {
                        if (*subjects == 0)
                            *subject_vector = malloc(sizeof(SUBJECT));
                        else 
                            *subject_vector = (SUBJECT *)realloc(*subject_vector, (*subjects)*sizeof(SUBJECT));

                        (*subject_vector)[*subjects] = read;

                        (*subjects)++;
                    }

                }

            } else
                printf("File doesn't exist yet or reading error!!\n");
            break;


        case 'w':
            if ((fileptr = fopen("user_subjects.bin", "wb")) != NULL) {
                
                if (fwrite(*subject_vector, sizeof(SUBJECT), *subjects, fileptr) != *subjects)
                    printf("Error in file writing!\n");

            } else
                printf("Error in file writing!\n");


            break;
    }

}

int button(Vector2 position, Vector2 size, Color button_color, char* text, bool clickable) {
// the logic action here, i.e, the changing in the value of a subject, will be done in the main loop
// using the returned value. 

    float font_size = SCREENHEIGHT/5.0;

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
    if (CheckCollisionPointRec(mouse_pointer, button) && clickable) {
        hovering = true;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            click = 1;
    }

    //button lines drawing 
    DrawRectangleRounded(button, STANDARDBUTTONCURVE, 20, BLACK);
    
    //float delta = button.width/100.0;
    float delta = SCREENWIDTH/BUTTONLINETHICKNESSDIV;

    button.x += delta;
    button.y += delta;
    button.width -= 2*delta;
    button.height -= 2*delta;

    //button fill drawing
    DrawRectangleRounded(button, STANDARDBUTTONCURVE, 20, button_color);

    //button text drawing
    draw_centralized_text(position, size, font_size, text, BLACK);

    //button highlight drawing
    if (hovering) {
        DrawRectangleRounded(button, STANDARDBUTTONCURVE, 20, SHADOWCOLOR);
    }

    return click;
}

void push_coord_right(Vector2* coordinates, float distance) {

    coordinates->x += distance;

}

int question_button_kit(Vector2* initial_button_pos, float big_size, float small_size, Color button_color, int number_to_display) {

    //variables assigning
    int count = 0;

    char number_text[14] = {0};

    Vector2 big_button_position = *initial_button_pos;

    Vector2 small_button_position = {initial_button_pos->x, 
    initial_button_pos->y + big_size + BLANKSPACESIZE/2};

    Vector2 big_size_vec = {big_size, big_size};

    Vector2 small_size_vec = {small_size, small_size};

    sprintf(number_text, "%d", number_to_display);

    //drawing and counting part
    button(big_button_position, big_size_vec, button_color, number_text, false);

    count += -button(small_button_position, small_size_vec, button_color, "-", true); //minus the return
        
    push_coord_right(&small_button_position, small_size + BLANKSPACESIZE/2);

    count += button(small_button_position, small_size_vec, button_color, "+", true);

    push_coord_right(initial_button_pos, big_size + BLANKSPACESIZE);

    return count;
}

void wont_be_negative(int* number) {
    if ((*number) < 0)
        *number = 0;
}

void draw_centralized_text(Vector2 button_position, Vector2 button_size, float font_size, char* text, Color text_color) {

    Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 2);

    button_position.x += button_size.x/2 - text_size.x/2.0;
    button_position.y += button_size.y/2 - text_size.y/2.0;

    DrawTextEx(GetFontDefault(), text, button_position, font_size, 2, text_color);

}