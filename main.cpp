#include <SFML/Window.hpp>
#include <common.h>

//CONSTANTS

enum snakeDir_t
{
    snakeDirError = 0;

    snakeDirLeft = -2;
    snakeDirDown = -1;
    //It is made intentionally that opposite directions are mutually opposite
    snakeDirUp = 1;
    snakeDirRight = 2;
};

enum { snakeInitBodyPosArraySize = 4 };

enum { snakeMaxFieldSizeX = 20 };
enum { snakeMaxFieldSizeY = 20 };

enum { reallocFactor = 2 };

enum
{
    snakeGameWin,
    snakeGameLose,
    snakeGameNotEnd,
};

//Point struct

struct Point
{
    size_t x;
    size_t y;
}

bool arePointsEqual(Point point1, Point point2)
{
    return (point1.x == point2.x) && (point1.y == point2.y);
}


/*
     O 0                                       20
    0  + - - - - - - - - - - - - - - - - - - - ->X
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
    20 | □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ □ 
       V
       Y
*/

//snakeGame_t, snake_t and snakeFruit_t structs

struct snakeFruit_t
{
    Point fruitPos;
};

struct snake_t
{
    size_t len;
    Point* bodyPos;
    size_t bodyPosSize;

    Point headPos;
    snakeDir_t dir;
};

struct snakeGame_t
{
    //Snake itself
    snake_t snake;

    //Field parameters
    Point bottomRightCorner;
    
    //Current fruit
    snakeFruit_t fruit;
};

//Functions for Snake Game

void snakeCtor(snakeGame_t* snakeGame Point snakeStartPos)
{
    snakeAssert(snakeGame);
     
    snakeGame->snake->len = 1;

    snakeGame->snake->bodyPos = (Point*)calloc(snakeInitBodyPosArraySize ? snakeInitBodyPosArraySize : 1, sizeof(*snakeGame->snake->bodyPos));
    snakeGame->snake->bodyPosSize = snakeInitBodyPosArraySize;

    snakeGame->snake->headPos.x = snakeStartPos.x;
    snakeGame->snake->headPos.y = snakeStartPos.y;
}


int snakeReallocBodyPos(snakeGame_t* snakeGame) 
{
    snakeAssert(snakeGame);
    assert(snakeGame->snake->bodyPos);
    assert(snakeGame->snake->bodyPosSize);
    //Reallocate memory
    ALLOC(snakeGame->snake->bodyPos = (Point*)realloc(snakeGame->snake->bodyPos, snakeGame->snake->bodyPosSize * REALLOC_FACTOR));
    free(snakeGame->snake->bodyPos);
    /*
    //Initialize memory
    for (int i = snakeGame->snake->bodyPosSize; i < snakeGame->snake->bodyPosSize * REALLOC_FACTOR; i++)
    {
        snakeGame->snake->bodyPos[i].x = snakeGame->snake->bodyPos[i].y = 0;
    };
    */
    snakeGame->snake->bodyPosSize *= REALLOC_FACTOR;

    return 0;
}

//is considered to be copy-pasted in some code sections
#define moveSnakeSection(sectionPos, snakeDir) \
{ \
    switch (snakeDir) \
        case snakeDirUp: \
        {
            sectionPos.y -= 1; \
        }; break; \
        case snakeDirDown: \
        { \
            sectionPos.y += 1; \
        }; break; \
        case snakeDirLeft: \
        { \
            sectionPos.x -= 1; \
        }; break; \
        case snakeDirRight: \
        { \
            sectionPos.x += 1; \
        }; break; \
}

int snakeMove(snakeGame_t* snakeGame, snakeDir_t dir)
{
    snakeAssert(snakeGame);
    assert(snakeGame->snake->dir != -dir); //<  Snake can't be moved in the opposite direction
    
    snakeGame->snake->dir = dir;
    
    snakeGame->snake->bodyPos[0] = snakeGame->snake->headPos;

    for (size_t i = 1; i < snakeGame->snake->len - 1/*snakeGame->snake->len considers head, and this loop doesn't move head*/, i++)
    {
        snakeGame->snake->bodyPos[i] = snakeGame->snake->bodyPos[i - 1];
    };

    moveSnakeSection(snakeGame->snake->headPos, dir); 

    return isEndOfSnakeGame(snakeGame);
}

#undef moveSnakeSection

void snakeDtor(snakeGame_t* snakeGame)
{
    snakeAssert(snakeGame);

    //Fill snake with poison values
    for (size_t i = 0; i < snakeGame->snake->bodyPosSize; i++)
    {
        snakeGame->snake->bodyPos[i].x = snakeGame->snake->bodyPos[i].y = 0;
    };
    snakeGame->snake->dir = snakeDirError;
    snakeGame->snake->headPos.x = snakeGame->snake->headPos.y = 0;
    snakeGame->snake->len = snakeGame->snake->bodyPosSize = 0;

    //Free allocated memory
    free(snakeGame->snake->bodyPos); //< free(a) is a macro declared in common.h
}

bool snakeOk(snakeGame_t* snakeGame)
{
    bool ok =    (snakeGame) &&
                 (snakeGame->snake->bodyPos) &&
                 (snakeGame->snake->headPos.x < snakeMaxFieldSizeX) &&// (snakeGame->snake->headPos.x >= 0) <- no need, 'cause headPos.x is unsigned
                 (snakeGame->snake->headPos.y < snakeMaxFieldSizeY) &&
                 (snakeGame->snake->len <= snakeGame->snake->bodyPosSize) &&
                 (snakeGame->snake->bodyPosSize < snakeMaxFieldSizeX * snakeMaxFieldSizeX) &&
                 (snakeGame->snake->dir !=snakeDirError);
    
    if (!ok)
        return true; //return 1, because it is NOT OK.

    for (size_t i = 0; i < snakeGame->snake->len - 1; i++)
    {
        ok &= (snakeGame->snake->bodyPos.x < snakeMaxFieldSizeX) && (snakeGame->snake->bodyPos.y < snakeMaxFieldSizeY);
    };

    return !ok; //Return 0 if OK, and 1 if NOT OK
}

bool isSnakeSelfIntersected(snakeGame_t* snakeGame)
{
    snakeAssert(snakeGame);

    for (size_t i = 0; i < snakeGame->snake->len - 1; i++)
    {
        if (arePointsEqual(snakeGame->snake->headPos, snakeGame->snake->bodyPos[i]))
            return true;
    }

    return false;
}

bool isSnakeSmashedIntoTheWall(snakeGame_t* snakeGame)
{
    snakeAssert(snakeGame);

    return (snakeGame->snake->headPos.x == snakeGame->snake->fieldSizeX) || (snakeGame->snake->headPos.y == snakeGame->snake->fieldSizeY);
}

bool isSnakeFieldFilled(snakeGame_t* snakeGame)
{
    snakeAssert(snakeGame);

    size_t fieldSizeX = snakeGame->snake->bottomRightCorner.x + 1;
    size_t fieldSizeY = snakeGame->snake->bottomRightCorner.y + 1;

    size_t area = fieldSizeX * fieldSizeY;

    return (snakeGame->snake->len == area);
}

int isEndOfSnakeGame(snakeGame_t* snakeGame)
{
    snakeAssert(snakeGame);
    assert((!isFieldFilled(snakeGame)) || (!isSnakeSelfIntersected(snakeGame)));

    if  (isFieldFilled(snakeGame))
    {
        return snakeGameWin; 
    }; else

    if ( (isSnakeSelfIntersected(snakeGame)) || (isSnakeSmashedIntoTheWall(snakeGame)) )
    {
        return snakeGameLose; 
    }; else
    return snakeGameNotEnd;
}

int main()
{
    sf::Window window(sf::VideoMode(800, 600), "My window", sf::Style::Fullscreen);

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}
