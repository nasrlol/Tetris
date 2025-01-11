#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// GAME SETTINGS
#define FPS 30
#define FONT_SIZE 20

#define TETROMINO_SIZE 4

#define CYAN_I      (Color) { 0, 255, 255, 255 }
#define YELLOW_O    (Color) { 255, 255, 102, 255 }
#define PURPLE_T    (Color) { 138, 43, 226, 255 }
#define GREEN_S     (Color) { 34, 139, 34, 255 }
#define RED_Z       (Color) { 255, 69, 0, 255 }
#define BLUE_J      (Color) { 70, 130, 180, 255 }
#define ORANGE_L    (Color) { 255, 140, 0, 255 }

// Window and Grid Settings
static const int SCREEN_WIDTH = 400;
static const int SCREEN_HEIGHT = 600;
static const int COLUMNS = 10;
static const int ROWS = 20;
static const int CELL_WIDTH = SCREEN_WIDTH / COLUMNS;
static const int CELL_HEIGHT = SCREEN_HEIGHT / ROWS;

static int SCORE = 0;

typedef struct
{
    int blocks[TETROMINO_SIZE][TETROMINO_SIZE];
    Color color;
    int x; 
    int y; 

} TETROMINO;

TETROMINO TETROMINOES[7] = {
    // Making the tetrominoes in an array design
    {
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
            CYAN_I, 0, 0},

    {
        {
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
            YELLOW_O, 0, 0},
    {
        {
            {0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
            PURPLE_T, 0, 0},
    {
        {
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
            GREEN_S, 0, 0},
    {
        {
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
            RED_Z, 0, 0},
    {
        {
            {1, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
            BLUE_J, 0, 0},
    {
        {
            {0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
            ORANGE_L, 0, 0},
};

void DRAW_GRID_BACKGROUND()
{
    for (int i = 0; i <= COLUMNS; i++)
    {
        DrawLine(i * CELL_WIDTH, 0, i * CELL_WIDTH, SCREEN_HEIGHT, GRAY);
    }
    for (int j = 0; j <= ROWS; j++)
    {
        DrawLine(0, j * CELL_HEIGHT, SCREEN_WIDTH, j * CELL_HEIGHT, GRAY);
    }
}

int **CREATE_TETROMINOS_GRID(int rows, int columns)
{
    int **array = malloc(rows * sizeof(int *));
    if (!array)
        return NULL;

    for (int i = 0; i < rows; i++)
    {
        array[i] = calloc(columns, sizeof(int));
        if (!array[i])
        {
            for (int j = 0; j < i; j++)
                free(array[j]);
            free(array);
            return NULL;
        }
    }
    return array;
}

// detect if the active tetromino is hitting another tetromino
int COLLISION_DETECTION(TETROMINO *ACTIVE_TETROMINO, int **GRID)
{
    for (int y = 0; y < TETROMINO_SIZE; y++) {
        for (int x = 0; x < TETROMINO_SIZE; x++) {
            if (ACTIVE_TETROMINO->blocks[y][x] == 1) {
                int GRID_X = ACTIVE_TETROMINO->x + x;
                int GRID_Y = ACTIVE_TETROMINO->y + y;

                if (GRID_X < 0 || GRID_X >= COLUMNS || GRID_Y >= ROWS) {
                    // COLLISION
                    return 1; 
                }

                if (GRID_X >= 0 && GRID[GRID_Y][GRID_X] == 1) {
                    // COLLISION
                    return 1; 
                }
            }
        }
    }

    // NO COLLISION
    return 0;
}

void FREE_GRID(int **array, int rows)
{
    for (int i = 0; i < rows; i++)
        free(array[i]);
    free(array);
}

void DRAW_TETROMINO(TETROMINO *tetromino)
{
    for (int y = 0; y < TETROMINO_SIZE; y++)
    {
        for (int x = 0; x < TETROMINO_SIZE; x++)
        {
            if (tetromino->blocks[y][x] == 1)
            {
                DrawRectangle(
                        (tetromino->x + x) * CELL_WIDTH,
                        (tetromino->y + y) * CELL_HEIGHT,
                        CELL_WIDTH, CELL_HEIGHT,
                        tetromino->color);
            }
        }
    }
}

TETROMINO SPAWN_TETROMINO()
{
    int RANDOM_INDEX = GetRandomValue(0, 6);
    TETROMINO tetromino = TETROMINOES[RANDOM_INDEX];
    tetromino.x = COLUMNS / 2 - 2;
    tetromino.y = 0;
    return tetromino;
}

void MOVE_TETROMINO(TETROMINO *tetromino, int **GRID)
{
    if (!IsKeyDown(KEY_LEFT_SHIFT) && COLLISION_DETECTION(tetromino, GRID) == 0) 
    {
        if (IsKeyDown(KEY_RIGHT) && tetromino->x + TETROMINO_SIZE < COLUMNS)
            tetromino->x++;
        if (IsKeyDown(KEY_LEFT) && tetromino->x > 0)
            tetromino->x--;
        if (IsKeyDown(KEY_DOWN) && tetromino->y < ROWS - 2 )
            tetromino->y++;
    }
}

void SAVE_TETROMINO(TETROMINO *tetromino, int **GRID)
{
    for (int y = 0; y < TETROMINO_SIZE; y++)
    {
        for (int x = 0; x < TETROMINO_SIZE; x++)
        {
            if (tetromino->blocks[y][x] == 1)
            {
                int GRIDX = tetromino->x + x;
                int GRIDY = tetromino->y + y;
                if (GRIDX < COLUMNS && GRIDY >= 0 && GRIDY < ROWS)
                    GRID[GRIDY][GRIDX] = 1;
            }
        }
    }
}

void DRAW_SAVED_TETROMINO(int **GRID, TETROMINO *tetromino)
{
    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLUMNS; x++)
        {
            if (GRID[y][x] == 1)
                DrawRectangle((x * CELL_WIDTH) , (y * CELL_HEIGHT) , CELL_WIDTH, CELL_HEIGHT, tetromino->color);
        }
    }
}

void DRAW_STATS(TETROMINO *tetromino)
{
    char SCORE_TEXT[16];
    sprintf(SCORE_TEXT, "SCORE: %d", SCORE);
    DrawText(SCORE_TEXT, 10, 10, FONT_SIZE, BLUE);

    char CURRENT_Y_POSITION[16], CURRENT_X_POSITION[16];
    sprintf(CURRENT_Y_POSITION, "Y: %d", tetromino->y);
    sprintf(CURRENT_X_POSITION, "X: %d", tetromino->x); 
    DrawText(CURRENT_Y_POSITION, 10, 30, FONT_SIZE, BLUE);
    DrawText(CURRENT_X_POSITION, 10, 50, FONT_SIZE, BLUE);
}

void CHECK_FULL_LINE(int **GRID)
{
    int LAST_LINE_Y = ROWS - 2;
    bool FULL_LINE = true;

    for (int x = 0; x <= COLUMNS; x++)
    {
        if (GRID[LAST_LINE_Y][x] == 0)
        {
            FULL_LINE = false;
        }
                
    }
    if (!FULL_LINE){
        for (int x = 0; x <= COLUMNS; x++)
        {
            GRID[LAST_LINE_Y][x] = GRID[LAST_LINE_Y][0];
        }
    } 
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris");
    SetTargetFPS(FPS);

    TETROMINO ACTIVE_TETROMINO = SPAWN_TETROMINO();
    int **GRID = CREATE_TETROMINOS_GRID(ROWS, COLUMNS);
    if (!GRID)
    {
        printf("FAILED TO ALLOCATE MEMORY FOR THE GRID");
        CloseWindow();
        return 1;
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DRAW_GRID_BACKGROUND();
        DRAW_SAVED_TETROMINO(GRID, &ACTIVE_TETROMINO);
        DRAW_TETROMINO(&ACTIVE_TETROMINO);
        DRAW_STATS(&ACTIVE_TETROMINO);

        MOVE_TETROMINO(&ACTIVE_TETROMINO, GRID);

        if (ACTIVE_TETROMINO.y == ROWS - 2 && COLLISION_DETECTION(&ACTIVE_TETROMINO,GRID) == 0)
        {
            SCORE++;
            SAVE_TETROMINO(&ACTIVE_TETROMINO, GRID);
            ACTIVE_TETROMINO = SPAWN_TETROMINO();
        }

        EndDrawing();
    }

    FREE_GRID(GRID, ROWS);
    CloseWindow();
    return 0;
}
