
//Functions for Snake Game

#include "snakeOk.cpp"

void snakeCtor(SnakeGame* snakeGame, Point fieldSize, Point snakeStartPos, size_t fruitPhotosNumber)
{
    snakeGame->fieldCorner = fieldSize;

    snakeGame->snake.len = 1;
    
    size_t initArraysSize = snakeInitBodyPosArraySize ? snakeInitBodyPosArraySize : 1;

    snakeGame->snake.bodyPos = (Point*)calloc(initArraysSize, sizeof(*snakeGame->snake.bodyPos));
    snakeGame->snake.bodyPosSize = snakeInitBodyPosArraySize;

    snakeGame->snake.headPos.x = snakeStartPos.x;
    snakeGame->snake.headPos.y = snakeStartPos.y;
    
    snakeGame->snake.dir = snakeDirUp;
    
    if ( !snakeGame->snake.bodyTexture.loadFromFile("./Textures/SnakeBody.png") )
    {
        throw (SNMLERR_OPFILE);
    };

    if ( !snakeGame->snake.headTexture.loadFromFile("./Textures/SnakeHead.png") )
    {
        throw (SNMLERR_OPFILE);
    };

    if ( !snakeGame->fieldTexture.loadFromFile("./Textures/Earth.png") )
    {
        throw (SNMLERR_OPFILE);
    };

    snakeGame->fieldSprite.setTexture      ( snakeGame->fieldTexture      );
    snakeGame->snake.headSprite.setTexture ( snakeGame->snake.headTexture );
    
    snakeGame->fieldSprite.setPosition((float)snakeGame->fieldCorner.x*snakeTileSize/2, (float)snakeGame->fieldCorner.x*snakeTileSize/2);
    snakeGame->snake.headSprite.setPosition(posInField2posOnScreen(snakeGame->snake.headPos));

    ALLOC(snakeGame->snake.bodySprite = (sf::Sprite*)calloc(initArraysSize, sizeof(*snakeGame->snake.bodySprite)));
    ALLOC(snakeGame->fruitTexture = (sf::Texture*)calloc(fruitPhotosNumber, sizeof(*snakeGame->fruitTexture)));
    
    snakeGame->fruitTexturesNum = fruitPhotosNumber; 
    for (size_t i = 1; i <= fruitPhotosNumber; i++)
    {
        char fruitTextureName[MAX_LINE_LEN] = "";
        sprintf(fruitTextureName, "./Textures/Fruit %lu.png", i);

        if ( !snakeGame->fruitTexture[i].loadFromFile(fruitTextureName) )
        {
            throw (SNMLERR_OPFILE);
        };
    }

    generateNewFruit(snakeGame);
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

int reallocSnake(SnakeGame* snakeGame) 
{
    snakeAssert(snakeGame);
    assert(snakeGame->snake.bodyPos);
    assert(snakeGame->snake.bodySprite);
    assert(snakeGame->snake.bodyPosSize);
    //Reallocate memory
    ALLOC(snakeGame->snake.bodyPos = (Point*)realloc(snakeGame->snake.bodyPos, snakeGame->snake.bodyPosSize * REALLOC_FACTOR));
    ALLOC(snakeGame->snake.bodySprite = (sf::Sprite*)realloc(snakeGame->snake.bodySprite, snakeGame->snake.bodyPosSize * REALLOC_FACTOR));
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
    snakeGame->snake.bodySprite[0].setPosition(posInField2posOnScreen(snakeGame->snake.bodyPos[0]));

    while (snakeGame->snake.len >= snakeGame->snake.bodyPosSize)
    {
       reallocSnake(snakeGame);
    }

    for (size_t i = 1; i < snakeGame->snake.len; i++)
    {
        snakeGame->snake.bodyPos[i] = snakeGame->snake.bodyPos[i - 1];
        snakeGame->snake.bodySprite[i].setPosition(posInField2posOnScreen(snakeGame->snake.bodyPos[i]));
    };

    if (isFruitEaten(snakeGame))
    {
        snakeGame->snake.bodySprite[snakeGame->snake.len - 2].setTexture(snakeGame->snake.bodyTexture);
        snakeGame->snake.bodySprite[snakeGame->snake.len - 2].setPosition(posInField2posOnScreen(snakeGame->snake.bodyPos[snakeGame->snake.len-2]));
        generateNewFruit(snakeGame);
        snakeGame->snake.len++;
    }

    snakeGame->snake.headSprite.setPosition(posInField2posOnScreen(snakeGame->snake.headPos));
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

SnakeGameStatus isEndOfSnakeGame(SnakeGame* snakeGame)
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

bool isFruitEaten(SnakeGame* snakeGame)
{
    snakeAssert(snakeGame);

    return arePointsEqual(snakeGame->snake.headPos, snakeGame->fruit.fruitPos);
};

void generateNewFruit(SnakeGame* snakeGame)
{
   snakeAssert(snakeGame);

   //Destroy previous fruit

   //Create new fruit 
   srand(genSeed());
   snakeGame->fruit.fruitPos.x = rand() % snakeGame->fieldCorner.x; 

   srand(rand());
   snakeGame->fruit.fruitPos.y = rand() % snakeGame->fieldCorner.y; 
}

sf::Vector2f posInField2posOnScreen(Point inField)
{
    return sf::Vector2f(((float)inField.y + 1)*snakeTileSize, ((float)inField.y + 1)*snakeTileSize);         
};
