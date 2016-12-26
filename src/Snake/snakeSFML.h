
#include "snake.h"

SnakeGame* snakeGameInit()
{
    SnakeGame* snakeGame = NULL;
    ALLOC( snakeGame = (SnakeGame*)calloc(1, sizeof(*snakeGame)) );

    printf("# This is %s (c)Phil\n", snakeNameAndVersion);

    size_t fieldSizeX = 0, fieldSizeY = 0;
    size_t snakeStartPosX = 0, snakeStartPosY = 0;
    size_t fruitsNumber = 0;

    printf("# Enter fieldSizeX, fieldSizeY\n");
    while (scanf ("%lu %lu", &fieldSizeX, &fieldSizeY) != 2)
    {
        printf ("# Input was incorrect\n");
        printf ("# Enter fieldSizeX, fieldSizeY\n");
        scanf ("%*[^\n]");
    }

    printf("\n# Enter snakeStartPosX, snakeStartPosY\n");
    while (scanf ("%lu %lu", &snakeStartPosX, &snakeStartPosY) != 2)
    {
        printf ("# Input was incorrect\n");
        printf ("# Enter snakeStartPosX, snakeStartPosY\n");
        scanf ("%*[^\n]");
    }

    printf("\n# Enter number of fruit photos uploaded\n");
    while (scanf ("%lu", &fruitsNumber) != 1)
    {
        printf ("# Input was incorrect\n");
        printf ("# Enter number of fruit photos uploaded\n");
        scanf ("%*[^\n]");
    }

    Point snakeStartPos  = { snakeStartPosX, snakeStartPosY };
    Point fieldSize      = { fieldSizeX    , fieldSizeY     };

    snakeCtor(snakeGame, fieldSize, snakeStartPos, fruitsNumber);

    return snakeGame;
}

SnakeDir getSnakeDirFromChar(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::Key::H:
        {
            return snakeDirLeft;
        }; break;
        case sf::Keyboard::Key::L:
        {
            return snakeDirRight;
        }; break;
        case sf::Keyboard::Key::J:
        {
            return snakeDirUp;
        }; break;
        case sf::Keyboard::Key::K:
        {
            return snakeDirDown;
        }; break;
        default:
        {
            return snakeDirError;
        }; break;
    }
};

void snakeProcessEvent(sf::RenderWindow* window, sf::Event* event, SnakeGame* snakeGame)
{
    snakeAssert(snakeGame);

    switch (event->type)
    {
        case sf::Event::Closed:
        {
            window->close();
        }; break;

        case sf::Event::KeyPressed:
        {
#define isSFKey(a) (event->key.code == sf::Keyboard::Key::a)
            if ( isSFKey(H) || isSFKey(J) || isSFKey(K) || isSFKey(L) ) 
            {
                snakeChangeDir(snakeGame, getSnakeDirFromChar(event->key.code));
            } else
            if isSFKey(Escape)
            {
                window->close();
            };
#undef isSFKey
        }; break;

        default: break;
    }
}



void snakeUT()
{
    //Init snakeGame == get field size and start snake point from input
    SnakeGame* snakeGame = snakeGameInit();
    assert(snakeGame);

    //Init Window and Clock
    sf::RenderWindow window(sf::VideoMode(snakeTileSize*(snakeGame->fieldCorner.x + 2), snakeTileSize*(snakeGame->fieldCorner.x + 2)), snakeNameAndVersion, sf::Style::Fullscreen);

    sf::Clock clock;

    //Draw background

    //Main Loop
    while (window.isOpen())
    {
        sf::Event event;
       /* while*/ window.pollEvent(event);
        {
            snakeProcessEvent(&window, &event, snakeGame);
        }
        
        window.clear(sf::Color::Black);
        
        //Make move
        if ( clock.getElapsedTime().asMilliseconds() >= snakeOneStepDuration )
        {
            clock.restart();
            snakeMove(snakeGame);
                
            SnakeGameStatus status = snakeGameNotEnd;
            if ( (status = isEndOfSnakeGame(snakeGame) ) != snakeGameNotEnd )
            {
                window.close();
                if (status == snakeGameWin)
                {
                    printf("You won.\n");
                } else
                {
                    printf("You losed. Try again!\n");
                }
            }
        };
        //Draw some stuff
        for (size_t i = 0; i < snakeGame->snake.len - 1; i++)
            window.draw(snakeGame->snake.bodySprite[i]);
        window.draw(snakeGame->fieldSprite);
        window.draw(snakeGame->snake.headSprite);

        window.display();
    }
};
