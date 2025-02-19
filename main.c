#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREENWIDTH 1200.0
#define SCREENHEIGHT 675.0
#define STANDARDFONTSIZE 20.0
#define BLANKSPACESIZE 30.0
#define STANDARDBUTTONCURVE 0.3
#define BUTTONLINETHICKNESSDIV 400.0
#define SHADOWCOLOR (Color){40, 40, 40, 180}
#define FULLRED (Color){255, 0, 0, 255}
#define FULLYELLOW (Color){255, 255, 0, 255}
#define FULLGREEN (Color){0, 255, 0, 255}

typedef struct {

    char* name;
    int right_answers;
    int medium_answers;
    int wrong_answers;
    int time_spent; //in seconds
    bool selected;

} SUBJECT;

//functions signatures (beggining)
void user_subjects_rw(char mode, int *subjects, SUBJECT** subject_vector);

int button(Vector2 position, Vector2 size, Color button_color, char* text, bool clickable);

void push_coord_right(Vector2* coordinates, float distance);

void push_coord_left(Vector2* coordinates, float distance);

int question_button_kit(Vector2* initial_button_pos, float big_size, float small_size, Color button_color, int number_to_display);

void wont_be_negative(int* number);

void draw_centralized_text(Vector2 button_position, Vector2 button_size, float font_size, char* text, Color text_color); //meant to use inside a button

void format_timer(int time_seconds, char* formatted_timer);

int subject_selected(SUBJECT *subject_array, int subject_counter);

void questions_done_display(Vector2 initial_position, Vector2 size, float big_size, SUBJECT subject, Color button_color);

int show_subject(Vector2* initial_position, float font_size, SUBJECT to_be_shown, bool delete_on, Color color);
//end of functions signatures

int main() {

    //subjects vector variables
    SUBJECT* user_subjects;
    int user_subjects_counter = 0;
    int subject_index = -1;

    //reads the stored user subjects array. If it doesn't exist, it will be created in the 1st use
    user_subjects_rw('r', &user_subjects_counter, &user_subjects);


    //buttons drawing control (for a specific subject)
    float big_button_size = (SCREENWIDTH - 4*BLANKSPACESIZE)/3;

    float small_button_size = (big_button_size - BLANKSPACESIZE/2)/2;

    float timer_size_x = SCREENWIDTH - 2*BLANKSPACESIZE;
    float timer_size_y = SCREENHEIGHT - ((5.0/2)*BLANKSPACESIZE + big_button_size + small_button_size);
    Vector2 timer_size_vec = {timer_size_x, timer_size_y};

    char timer_string[12 + 1] = {0};

    Vector2 close_subject_size_vec = {timer_size_y, timer_size_y};

    Vector2 questions_done_size_vec = close_subject_size_vec;
    questions_done_size_vec.x *= 4;

    Vector2 button_drawing_pos;


    //target fps = 60!!!
    SetTargetFPS(60);

    //window initializing
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Question counter");

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(DARKGRAY);

        //button position reset
        button_drawing_pos = (Vector2){BLANKSPACESIZE, BLANKSPACESIZE/2};

        if ((subject_index = subject_selected(user_subjects, user_subjects_counter)) > -1) {

            //HERE SHOULD BE THE REAL TIMER *MODIFYLATER*
            format_timer(40000, timer_string);

            button(button_drawing_pos, timer_size_vec, GRAY, timer_string, false);

            if (button(button_drawing_pos, close_subject_size_vec, GRAY, "x", true)) {
                user_subjects[subject_index].selected = false;
            }

            questions_done_display(button_drawing_pos, questions_done_size_vec, big_button_size, user_subjects[subject_index], GRAY);

            button_drawing_pos.y += timer_size_y + BLANKSPACESIZE/2;

            user_subjects[subject_index].wrong_answers += question_button_kit(&button_drawing_pos, big_button_size, small_button_size, FULLRED, user_subjects[subject_index].wrong_answers);
            wont_be_negative(&user_subjects[subject_index].wrong_answers);

            user_subjects[subject_index].medium_answers += question_button_kit(&button_drawing_pos, big_button_size, small_button_size, FULLYELLOW, user_subjects[subject_index].medium_answers);
            wont_be_negative(&user_subjects[subject_index].medium_answers);

            user_subjects[subject_index].right_answers += question_button_kit(&button_drawing_pos, big_button_size, small_button_size, FULLGREEN, user_subjects[subject_index].right_answers);
            wont_be_negative(&user_subjects[subject_index].right_answers);

        } else {
            ///TEST TEST TEST
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);
            show_subject(&button_drawing_pos, SCREENWIDTH/50, (SUBJECT){"blue subject", 60, 60, 60, 120, false}, false, GRAY);



            ///TEST TEST TEST
        }

        EndDrawing();

    }

    user_subjects_rw('w', &user_subjects_counter, &user_subjects);

    //window closing
    CloseWindow();

    //frees the subjects vector
    if (user_subjects_counter > 0) {
        free(user_subjects);
    }

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

    float font_size = size.y/2.0;

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

void push_coord_left(Vector2* coordinates, float distance) {
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

void format_timer(int time_seconds, char* formatted_timer) {
    int time_minutes = 0;
    int time_hours = 0;

    char string_seconds[3] = {0};
    char string_minutes[3] = {0};
    char string_hours[3] = {0};


    time_minutes = time_seconds/60;
    time_seconds = time_seconds%60;

    time_hours = time_minutes/60;
    time_minutes = time_minutes%60;

    if (time_seconds >= 10)
        sprintf(string_seconds, "%d", time_seconds);
    else
        sprintf(string_seconds, "0%d", time_seconds);

    if (time_minutes >= 10)
        sprintf(string_minutes, "%d", time_minutes);
    else
        sprintf(string_minutes, "0%d", time_minutes);

    if (time_hours >= 10)
        sprintf(string_hours, "%d", time_hours);
    else
        sprintf(string_hours, "0%d", time_hours);

    sprintf(formatted_timer, "%s:%s:%s", string_hours, string_minutes, string_seconds);
}

int subject_selected(SUBJECT *subject_array, int subject_counter) {
    int selected = -1;

    for (int i = 0; i < subject_counter; i++) {
        if (subject_array[i].selected) {
            selected = i;
        }
    }

    return selected;
}

void questions_done_display(Vector2 initial_position, Vector2 size, float big_size, SUBJECT subject, Color button_color) {
    //must be used right after the close button logic!!!
    Vector2 drawing_pos = initial_position;
    drawing_pos.x += 2*(big_size + BLANKSPACESIZE) + (big_size - size.x);

    char string_to_display[30] = {0};

    sprintf(string_to_display, "Questões: %d", subject.wrong_answers + subject.medium_answers + subject.right_answers);

    button(drawing_pos, size, button_color, string_to_display, false);
}

int show_subject(Vector2* initial_position, float font_size, SUBJECT to_be_shown, bool delete_on, Color color) {
    float text_length = MeasureText(to_be_shown.name, font_size);
    int click = 0;
    Vector2 button_size = {2*text_length, 2*font_size};

    if (initial_position->x + button_size.x + BLANKSPACESIZE > SCREENWIDTH) {
        initial_position->x = BLANKSPACESIZE;
        initial_position->y += button_size.y + BLANKSPACESIZE/2;
    }

    click = button(*initial_position, button_size, color, to_be_shown.name, true);

    initial_position->x += button_size.x + BLANKSPACESIZE;

    return click;
}


//MAKE A HOVERING WHEN THE USER PASS THE MOUSE OVER A SUBJECT IT SHOWS THE ANSWERS
//show subject -> show all the subjects
//prompt a message -> add subject
//remove subject(toggle) if you click a subject it will be deleted
//reorder the subject array and subtract 1 from the subject counter
