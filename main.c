#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREENWIDTH 1200.0
#define SCREENHEIGHT 675.0

#define STANDARDFONTSIZE 20.0
#define BLANKSPACESIZE 30.0
#define STANDARDBUTTONCURVE 0.3
#define BUTTONLINETHICKNESSDIV 400.0
#define BUTTONLENGTHOVERFONT 1.608

#define SHADOWCOLOR (Color){40, 40, 40, 180}
#define SOFTRED (Color){255, 105, 97, 255}
#define FULLRED (Color){255, 0, 0, 255}
#define SOFTYELLOW (Color){253, 253, 150, 255}
#define FULLYELLOW (Color){255, 255, 0, 255}
#define SOFTGREEN (Color){119, 221, 119, 255}
#define FULLGREEN (Color){0, 255, 0, 255}


typedef struct {

    char name[200];
    int right_answers;
    int medium_answers;
    int wrong_answers;
    int time_spent; //in seconds
    bool selected;

} SUBJECT;

//functions signatures (beggining)
void user_subjects_rw(char mode, int *subjects, int *array_size, SUBJECT** subject_vector);

int button(Vector2 position, Vector2 size, Color button_color, char* text, bool clickable);

void push_coord_right(Vector2* coordinates, float distance);

void push_coord_left(Vector2* coordinates, float distance);

int question_button_kit(Vector2* initial_button_pos, float big_size, float small_size, Color button_color, int number_to_display);

void wont_be_negative(int* number);

void draw_centralized_text(Vector2 button_position, Vector2 button_size, float font_size, char* text, Color text_color); //meant to use inside a button

void format_timer(int time_seconds, char* formatted_timer);

int subject_selected(SUBJECT *subject_array, int subject_counter);

void questions_done_display(Vector2 initial_position, Vector2 size, float big_size, SUBJECT subject, Color button_color);

int show_subject(Vector2* initial_position, float font_size, SUBJECT to_be_shown, Color color, float screen_limit);

void show_all_subjects(Vector2 initial_position, float font_size, SUBJECT **subject_array, int *subject_counter, int* array_size, Color color, float screen_limit, bool delete_on);

void delete_and_reorganize_subject(SUBJECT **subject_array, int *subject_counter, int *array_size, int position_to_delete);

void add_a_subject(Vector2 initial_position, float font_size, SUBJECT **subject_array, int *subject_counter, int *array_size, Color popup_color);

bool input_array_writer(char *array, int *position, Vector2 button_position, float button_size, Color color);

//end of functions signatures

int main() {

    //subjects vector variables
    SUBJECT* user_subjects;
    int user_subjects_counter = 0;
    int subject_index = -1;
    int subject_array_size = 0;

    //reads the stored user subjects array. If it doesn't exist, it will be created in the 1st use
    user_subjects_rw('r', &user_subjects_counter, &subject_array_size, &user_subjects);


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


    //main menu font size
    float menu_font_size = SCREENHEIGHT/25;

    //this is the size of the button space in the main menu;
    float button_reserved_space_size = SCREENWIDTH/10;
    bool delete_on = false;

    char delete_string[8] = "Deletar";

    Vector2 delete_button_pos = {SCREENWIDTH - 2*MeasureTextEx(GetFontDefault(), delete_string, menu_font_size, 1).x - BLANKSPACESIZE, SCREENHEIGHT - 2*menu_font_size - BLANKSPACESIZE};
    Vector2 delete_button_size = {2*MeasureTextEx(GetFontDefault(), delete_string, menu_font_size, 1).x, 2*menu_font_size};

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

            if (delete_on) {
                delete_on = button(delete_button_pos, delete_button_size, RED, delete_string, true) == 1 ? false : true;
            } else {
                delete_on = (button(delete_button_pos, delete_button_size, GRAY, delete_string, true) == 1 ? true : false);
            }

            show_all_subjects(button_drawing_pos, menu_font_size, &user_subjects, &user_subjects_counter, &subject_array_size, GRAY, button_reserved_space_size, delete_on);

        }

        EndDrawing();

    }

    user_subjects_rw('w', &user_subjects_counter, &subject_array_size, &user_subjects);

    //window closing
    CloseWindow();

    //frees the subjects vector
    if (user_subjects_counter > 0) {
        free(user_subjects);
    }

    return 0;

}

void user_subjects_rw(char mode, int *subjects, int *array_size, SUBJECT** subject_vector) {

    FILE *fileptr;

    switch (mode) {
        case 'r':
            if ((fileptr = fopen("user_subjects.bin", "rb")) != NULL) {
                SUBJECT read = {0};

                //allocates memory at runtime to store the user subjects
                while(fread(&read, sizeof(read), 1, fileptr) == 1) {

                    if (*subjects == 0) {
                        *array_size = 2;
                        *subject_vector = malloc((*array_size)*sizeof(SUBJECT));
                    } else if (*subjects >= *array_size) {
                        (*array_size) *= 2;
                        *subject_vector = (SUBJECT *)realloc(*subject_vector, (*array_size)*sizeof(SUBJECT));
                    }

                    (*subject_vector)[*subjects] = read;

                    (*subjects)++;

                }

            } else
                printf("File doesn't exist yet or reading error!!\n");
            break;


        case 'w':
            if ((fileptr = fopen("user_subjects.bin", "wb")) != NULL) {

                if (fwrite(*subject_vector, sizeof(SUBJECT), *subjects, fileptr) != *subjects)
                    printf("Error in file opening -> writing!\n");

            } else
                printf("Error in file writing -> writing!\n");


            break;
    }
}

int button(Vector2 position, Vector2 size, Color button_color, char* text, bool clickable) {
    // the logic action here, i.e, the changing in the value of a subject, will be done in the main loop
    // using the returned value.

    float font_size = size.y/BUTTONLENGTHOVERFONT;

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

    button_position.x += button_size.x/2.0 - text_size.x/2.0;
    button_position.y += button_size.y/2.0 - text_size.y/2.0;

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

int show_subject(Vector2* initial_position, float font_size, SUBJECT to_be_shown, Color color, float screen_limit) {
    float text_length = MeasureText(to_be_shown.name, font_size);
    int click = 0;
    Vector2 button_size = {2*text_length, 2*font_size};
    Vector2 drawing_pos = *initial_position;

    char wrong_string[12], medium_string[12], right_string[12];

    sprintf(wrong_string, "%d", to_be_shown.wrong_answers);
    sprintf(medium_string, "%d", to_be_shown.medium_answers);
    sprintf(right_string, "%d", to_be_shown.right_answers);

    //makes sure the button fits the screen
    if (drawing_pos.x + button_size.x + BLANKSPACESIZE > SCREENWIDTH - screen_limit) {
        drawing_pos.x = BLANKSPACESIZE;
        drawing_pos.y += button_size.y + BLANKSPACESIZE/2;
    }

    Rectangle button_rect = {
        drawing_pos.x,
        drawing_pos.y,
        button_size.x,
        button_size.y
    };

    click = button(drawing_pos, button_size, color, to_be_shown.name, true);

    *initial_position = drawing_pos;
    initial_position->x += button_size.x + BLANKSPACESIZE;

    button_size.x /= 3;

    //hovers a little diferent
    if (CheckCollisionPointRec(GetMousePosition(), button_rect)) {
        button(drawing_pos, button_size, SOFTRED, wrong_string, false);
        push_coord_right(&drawing_pos, button_size.x);
        button(drawing_pos, button_size, SOFTYELLOW, medium_string, false);
        push_coord_right(&drawing_pos, button_size.x);
        button(drawing_pos, button_size, SOFTGREEN, right_string, false);
    }

    return click;
}

void show_all_subjects(Vector2 initial_position, float font_size, SUBJECT **subject_array, int *subject_counter, int* array_size, Color color, float screen_limit, bool delete_on) {

    int index_to_use = -1;

    for (int i = 0; i < (*subject_counter); i++) {
        if (show_subject(&initial_position, font_size, (*subject_array)[i], color, screen_limit) == 1) {
            index_to_use = i;
        }
    }

    if (initial_position.x + MeasureText("Adicionar", font_size) + BLANKSPACESIZE > SCREENWIDTH - screen_limit) {
        initial_position.x = BLANKSPACESIZE;
        initial_position.y += font_size*2 + BLANKSPACESIZE/2;
    }

    if (button(initial_position, (Vector2){2*MeasureText("Adicionar", font_size), 2*font_size}, GRAY, "Adicionar", true))
        add_a_subject(initial_position, font_size, subject_array, subject_counter, array_size, color);

    if (index_to_use > -1) {
        if(delete_on) {
            delete_and_reorganize_subject(subject_array, subject_counter, array_size, index_to_use);
        } else {
            (*subject_array)[index_to_use].selected = true;
        }
    }
}

void delete_and_reorganize_subject(SUBJECT **subject_array, int *subject_counter, int *array_size, int position_to_delete) {
    for (int i = position_to_delete; i < (*subject_counter); i++) {
        if (i < *subject_counter - 1)
            (*subject_array)[i] = (*subject_array)[i + 1];
    }

    if ((*subject_counter)-- < 1) {
        free(*subject_array);
    } else if (*subject_counter < *array_size/2) {
        *array_size /= 2;
        *subject_array = realloc(*subject_array, (*array_size)*sizeof(SUBJECT));
    }
}

//show subject DONE
//show all the subjects DONE
//prompt a message
//add subject
//remove subject(toggle) if you click a subject it will be deleted
//reorder the subject array and subtract 1 from the subject counter



void add_a_subject(Vector2 initial_position, float font_size, SUBJECT **subject_array, int *subject_counter, int *array_size, Color popup_color) {

    char input_buffer[200] = {0};
    bool name_confirmed = false;
    int letter_position = 0;

    SUBJECT new_subject = {
        "",
        0,
        0,
        0,
        0,
        false
    };

    Vector2 subject_size = {2*font_size, 2*font_size};

    EndDrawing();

    while (!name_confirmed) {

        BeginDrawing();

        ClearBackground(DARKGRAY);

        if (subject_size.x < 2*MeasureText(input_buffer, font_size)) {
            subject_size.x = 2*MeasureText(input_buffer, font_size);
        }

        button(initial_position, subject_size, popup_color, input_buffer, false);

        if (input_array_writer(input_buffer, &letter_position, (Vector2){initial_position.x + subject_size.x, initial_position.y}, subject_size.y, popup_color)) {
            name_confirmed = true;
        }

        EndDrawing();
    }

    strcpy(new_subject.name, input_buffer);

    if (*subject_counter > 0 && (*array_size) <= (*subject_counter) + 1) {
        (*array_size) *= 2;
        (*subject_array) = realloc(*subject_array, (*array_size)*sizeof(SUBJECT));
    } else if (*subject_counter <= 0) {
        (*subject_array) = malloc(2*sizeof(SUBJECT));
        (*array_size) = 2;
    }

    (*subject_array)[*subject_counter] = new_subject;

    (*subject_counter)++;

    BeginDrawing();
}

bool input_array_writer(char *array, int *position, Vector2 button_position, float button_size, Color color) {
    char letter_input;

    Vector2 button_size_vec = {button_size, button_size};

    button_position.x = SCREENWIDTH - button_size - BLANKSPACESIZE;
    button_position.y = SCREENHEIGHT - button_size - BLANKSPACESIZE;

    letter_input = GetKeyPressed();

    if (letter_input >= 32 && letter_input <= 125 && *position < 200 && letter_input != KEY_EQUAL) {

        array[*position] = letter_input;

        array[*position+1] = '\0';

        (*position)++;
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (*position != 0)
            (*position)--;
        array[*position] = '\0';
    }


    return button(button_position, button_size_vec, color, "+", true);
}
