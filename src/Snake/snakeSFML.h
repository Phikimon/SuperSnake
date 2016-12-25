#include "Snake/snake.h"

SnakeGame* snakeGameInit()
{
    SnakeGame* snakeGame = NULL;
    ALLOC( snakeGame = (SnakeGame*)calloc(1, sizeof(*snakeGame)) );

    printf("# This is %s (c)Phil\n", snakeNameAndVersion);

    size_t fieldSizeX = 0, fieldSizeY = 0;
    size_t snakeStartPosX = 0, snakeStartPosY = 0;

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

    Point snakeStartPos  = { snakeStartPosX, snakeStartPosY };
    Point fieldSize      = { fieldSizeX    , fieldSizeY     };

    snakeCtor(snakeGame, fieldSize, snakeStartPos);

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

void snakeProcessEvent(sf::RenderWindow* window, sf::Event* event, snakeGame* snakeGame)
{
    assert(snake);

    switch (event.type)
    {
        case sf::Event::Closed:
        {
            window.close();
        }; break;

        case sf::Event::KeyEvent:
        {
            switch (event.KeyEvent.code)
            {
                case sf::Keyboard::Key::H:
                case sf::Keyboard::Key::J:
                case sf::Keyboard::Key::K:
                case sf::Keyboard::Key::L:
                {
                    snakeChangeDir(getSnakeDirFromChar(event.KeyEvent.code));
                }; break;
                case sf::Keyboard::Key::Escape:
                {
                    window.close();
                };
            };
        }; break;

        default: break;
    }
}

void snakeUT()
{
    //Init snakeGame == get field size from input
    SnakeGame* snakeGame = snakeGameInit();
    assert(snakeGame);

    //Init Window and Clock
    sf::RenderWindow window(sf::VideoMode(800, 600), snakeNameAndVersion);

    sf::Clock clock; clock.restart();

    //Main Loop
    while (window.isOpen())
    {
        sf::Event event;
       /* while*/ window.pollEvent(event);
        {
            snakeProcessEvent(&window, &event, snakeGame);
        }
        
        window.clear(sf::Color::Black);

        if ( clock.getElapsedTime().asMilliseconds >= 500 )
        {
            clock.restart();
            snakeMove(snakeGame);

            SnakeGameStatus status = snakeGameNotEnd;
            if (isEndOfSnakeGame(snakeGame) == snakeGameNotEnd)
            {
                //display that player won|losed
            }
        };

        window.display();
    }
};
