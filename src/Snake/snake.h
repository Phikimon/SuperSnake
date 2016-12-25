#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#include "/home/philipp/Documents/Ded/lib/common.h"
#include "../Point/point.h"

//SNAKE ERRORS

  /**
    *                                    'SNAKEERR' TYPE DETALIZATION 
    *
    * +--- ~~ ---+---------------------+-----------------+--------------------+-----------------------+
    * |   ....   |     'snakeGame'     |    'snake'      |     'bodyPos'      | 'headPosWithinBounds' | 
    * +--- ~~ ---+---------------------+-----------------+--------------------+-----------------------+
    * |          |   field size is     |                 |                    |                       |
    * |   ....   |     less than       | snake direction | bodyPos_array_size |     snakeLength <     |
    * |          | 'snakeMaxFieldSize' |    is valid     |    < fieldSize     |   bodyPos_array_size  | 
    * +--- ~~ ---+---------------------+-----------------+--------------------+-----------------------+
    *
    * +--------------------------+----------------------------------+-------------------+-----------------------+
    * | 'lenLessThanBodyPosSize' | 'bodyPosSizeLessThanFieldSquare' |  'snakeDirValid'  | 'bodyPosWithinBounds' |
    * +--------------------------+----------------------------------+-------------------+-----------------------+
    * |                          |                                  |                   |                       |
    * |      headPos point       |          bodyPos pointer         |   snake pointer   |    snakeGame pointer  |
    * |     is within field      |             not null             |      not null     |       not null        |
    * +--------------------------+----------------------------------+-------------------+-----------------------+
    */

enum SNAKEERR_t
{
#define SNAKEERR_DEF(err) \
    SNAKEERR_##err,

#include "snakeOk.h"

#undef SNAKEERR_DEF
};

int snakeErrorPrint(SNAKEERR_t err);

int snakeErrorPrint(SNAKEERR_t err)
{

#define SNAKEERR_DEF(error) \
        case SNAKEERR_##error: \
        { \
            printf("'%s'", #error); \
            return 0; \
        }; break;

    switch (err)
    {
#include "snakeOk.h"
        default:
        {
            printf("Unknown error");
            return -1;
        }; break;

    }

#undef SNAKEERR_DEF
}

//CONSTANTS

enum { snakeInitBodyPosArraySize = 4 };

enum { snakeMaxFieldSizeX = 20 };
enum { snakeMaxFieldSizeY = 20 };

enum { REALLOC_FACTOR = 2 };

#define snakeNameAndVersion "SuperSnake v1.0beta"

//My types

enum SnakeDir
{
    snakeDirError = 0,

    snakeDirLeft = -2,
    snakeDirDown = -1,
    //It is made intentionally that opposite directions are mutually opposite
    snakeDirUp = 1,
    snakeDirRight = 2,
};

enum SnakeGameStatus
{
    snakeGameWin,
    snakeGameLose,
    snakeGameNotEnd,
};

struct SnakeFruit
{
    Point fruitPos;
};

struct Snake
{
    size_t len;
    Point* bodyPos;
    size_t bodyPosSize;

    Point headPos;
    SnakeDir dir;
};

struct SnakeGame
{
    //Snake itself
    Snake snake;

    //Field parameters
    Point fieldCorner;
     /* 
         O   0          X
           + - - - - - ->
        0  | □ □ □ □ x 
           | □ □ □ □ x 
           | □ □ □ □ x 
           | x x x x X <- fieldCorner point
         Y V
     */

    //Current fruit
    SnakeFruit fruit;
};

//Functions for Snake Game

/*
 * Constructor for snake game instance
 */
void snakeCtor(SnakeGame* snakeGame, Point fieldSize, Point snakeStartPos);

/*
 * Destructor for snake game instance
 */
void snakeDtor(SnakeGame* snakeGame);

/*
 * Reallocate memory for body positions array.
 * New size = Old Size * REALLOC_FACTOR.
 */
int snakeReallocBodyPos(SnakeGame* snakeGame);

/*
 * Change snake directory(can be called between two snake moves)
 */
void snakeChangeDir(SnakeGame* snakeGame, SnakeDir dir);

/*
 * Change *sectionPos point coordinated according to the snakeDir
 */
void moveSnakeSection(Point* sectionPos, SnakeDir snakeDir);

/*
 * Move the whole snake according to snake.dir
 */
int snakeMove(SnakeGame* snakeGame);

/*
 * Check if snake bited itself
 */
bool isSnakeSelfIntersected(SnakeGame* snakeGame);

/*
 * Check if snake smashed into the wall
 */
bool isSnakeSmashedIntoTheWall(SnakeGame* snakeGame);

/*
 * Check if snake Filled the whole play field
 */
bool isSnakeFieldFilled(SnakeGame* snakeGame);

/*
 * Returns SnakeGameStatus.
 */
int isEndOfSnakeGame(SnakeGame* snakeGame);

#define snakeAssert(snakeGame) \
{ \
    assert(snakeGame); \
    size_t state = snakeOk(snakeGame); \
    if (state) \
    { \
       throw(state); \
    } \
}

#include "snake.cpp"

#undef snakeAssert

#endif // SNAKE_H_INCLUDED
