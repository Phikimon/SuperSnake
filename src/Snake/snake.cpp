
//Functions for Snake Game

#include "snakeOk.cpp"

void snakeCtor(SnakeGame* snakeGame, Point fieldSize, Point snakeStartPos)
{
    snakeAssert(snakeGame);
     
    snakeGame->fieldCorner = fieldSize;

    snakeGame->snake.len = 1;

    snakeGame->snake.bodyPos = (Point*)calloc(snakeInitBodyPosArraySize ? snakeInitBodyPosArraySize : 1, sizeof(*snakeGame->snake.bodyPos));
    snakeGame->snake.bodyPosSize = snakeInitBodyPosArraySize;

    snakeGame->snake.headPos.x = snakeStartPos.x;
    snakeGame->snake.headPos.y = snakeStartPos.y;
}

void snakeDtor(SnakeGame* snakeGame)
{
    snakeAssert(snakeGame);

    //Fill snake with poison values
    for (size_t i = 0; i < snakeGame->snake.bodyPosSize; i++)
    {
        snakeGame->snake.bodyPos[i].x = snakeGame->snake.bodyPos[i].y = 0;
    };
    snakeGame->snake.dir = snakeDirError;
    snakeGame->snake.headPos.x = snakeGame->snake.headPos.y = 0;
    snakeGame->snake.len = snakeGame->snake.bodyPosSize = 0;

    //Free allocated memory
    free(snakeGame->snake.bodyPos); //< free(a) is a macro declared in common.h
}

int snakeReallocBodyPos(SnakeGame* snakeGame) 
{
    snakeAssert(snakeGame);
    assert(snakeGame->snake.bodyPos);
    assert(snakeGame->snake.bodyPosSize);
    //Reallocate memory
    ALLOC(snakeGame->snake.bodyPos = (Point*)realloc(snakeGame->snake.bodyPos, snakeGame->snake.bodyPosSize * REALLOC_FACTOR));
    free(snakeGame->snake.bodyPos);
    /*
    //Initialize memory
    for (int i = snakeGame->snake.bodyPosSize; i < snakeGame->snake.bodyPosSize * REALLOC_FACTOR; i++)
    {
        snakeGame->snake.bodyPos[i].x = snakeGame->snake.bodyPos[i].y = 0;
    };
    */
    snakeGame->snake.bodyPosSize *= REALLOC_FACTOR;

    return 0;
}

//is considered to be copy-pasted in some code sections
void moveSnakeSection(Point* sectionPos, SnakeDir snakeDir)
{
    assert(sectionPos);

    switch (snakeDir)
    {
        case snakeDirUp:
        {
            sectionPos->y -= 1;
        }; break;
        case snakeDirDown:
        {
            sectionPos->y += 1;
        }; break;
        case snakeDirLeft:
        {
            sectionPos->x -= 1;
        }; break;
        case snakeDirRight:
        {
            sectionPos->x += 1;
        }; break;

        default:
        case snakeDirError:
        {
            throw SNAKEERR_WRGARGMOVSEC;
        }; break;
    };
}

void snakeChangeDir(SnakeGame* snakeGame, SnakeDir dir)
{
    snakeAssert(snakeGame);
    if (snakeGame->snake.dir != -dir) //<  Snake can't be moved in the opposite direction
        snakeGame->snake.dir = dir;
}

int snakeMove(SnakeGame* snakeGame)
{
    snakeAssert(snakeGame);
    
    snakeGame->snake.bodyPos[0] = snakeGame->snake.headPos;

    //snakeGame->snake.len considers head, and this loop doesn't move head
    //                                            |
    //                                            V
    for (size_t i = 1; i < snakeGame->snake.len - 1; i++)
    {
        snakeGame->snake.bodyPos[i] = snakeGame->snake.bodyPos[i - 1];
    };

    moveSnakeSection(&snakeGame->snake.headPos, snakeGame->snake.dir); 

    return isEndOfSnakeGame(snakeGame);
}

bool isSnakeSelfIntersected(SnakeGame* snakeGame)
{
    snakeAssert(snakeGame);

    for (size_t i = 0; i < snakeGame->snake.len - 1; i++)
    {
        if (arePointsEqual(snakeGame->snake.headPos, snakeGame->snake.bodyPos[i]))
            return true;
    }

    return false;
}

bool isSnakeSmashedIntoTheWall(SnakeGame* snakeGame)
{
    snakeAssert(snakeGame);

    return (snakeGame->snake.headPos.x == snakeGame->fieldCorner.x) || (snakeGame->snake.headPos.y == snakeGame->fieldCorner.y);
}

bool isSnakeFieldFilled(SnakeGame* snakeGame)
{
    snakeAssert(snakeGame);

    size_t area = snakeGame->fieldCorner.x * snakeGame->fieldCorner.y;

    return (snakeGame->snake.len == area);
}

int isEndOfSnakeGame(SnakeGame* snakeGame)
{
    snakeAssert(snakeGame);
    assert((!isSnakeFieldFilled(snakeGame)) || (!isSnakeSelfIntersected(snakeGame)));

    if  (isSnakeFieldFilled(snakeGame))
    {
        return snakeGameWin; 
    } else

    if ( (isSnakeSelfIntersected(snakeGame)) || (isSnakeSmashedIntoTheWall(snakeGame)) )
    {
        return snakeGameLose; 
    } else

    return snakeGameNotEnd;
}
