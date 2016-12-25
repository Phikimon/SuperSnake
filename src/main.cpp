#include <SFML/Window.hpp>

#include "Snake/snakeSFML.h"

void snakeUT()
{
    SnakeGame* snakeGame = (SnakeGame*)calloc(1, sizeof(*snakeGame));
    printf("# This is %s (c)Phil\n", snakeNameAndVersion);
    printf("# Enter fieldSizeX, fieldSizeY\n");

    size_t fieldSizeX = 0, fieldSizeY = 0;
    while (scanf ("%lu %lu", &fieldSizeX, &fieldSizeY) != 2)
    {
        printf ("# Input was incorrect\n");
        printf ("# Enter fieldSizeX, fieldSizeY\n");
        scanf ("%*[^\n]");
    }

    Point snakeStartPos = {fieldSizeX, fieldSizeY};
    snakeCtor(snakeGame, snakeStartPos);

    while (isEndOfSnakeGame(snakeGame) == snakeGameNotEnd)
    {
    }

};


int main()
{
/*
    try
    {
        snakeUT(); 
    } catch (SNAKEERR_t err)
    {
        printf("\nError catched in %s: line %d, function %s;\nError name: ", __FILE__, __LINE__, __func__);
        snakeErrorPrint(err);       
    }
*/


    return 0;
}
