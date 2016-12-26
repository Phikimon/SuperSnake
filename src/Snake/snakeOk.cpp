
#define snakeOkAssert(smthToAssert, snakeError) \
    if ( !(smthToAssert) ) \
    { \
        /*If something went wrong, return, because may be e.g. (snakeGame == NULL) and I can't read from snakeGame->...*/ \
        throw(SNAKEERR_##snakeError); \
    } else

#define snakeOkAssertClose() {};

size_t snakeOk(SnakeGame* snakeGame)
{
    Point maxFieldCorner = { snakeMaxFieldSizeX, snakeMaxFieldSizeY };
    snakeOkAssert( isPointWithinBoundsAndZero(snakeGame->fieldCorner, maxFieldCorner)          ,  maxFieldCorner                 )

    snakeOkAssert( snakeGame                                                                   ,  snakeGame                      )
    snakeOkAssert( &snakeGame->snake                                                           ,  snake                          )
    snakeOkAssert( snakeGame->snake.bodyPos                                                    ,  bodyPos                        )
    snakeOkAssert( isPointWithinBoundsAndZero(snakeGame->snake.headPos, snakeGame->fieldCorner),  headPosWithinBounds            )
    snakeOkAssert( snakeGame->snake.len <= snakeGame->snake.bodyPosSize                        ,  lenLessThanBodyPosSize         )
    snakeOkAssert( snakeGame->snake.bodyPosSize <= snakeMaxFieldSizeX * snakeMaxFieldSizeY     ,  bodyPosSizeLessThanFieldSquare )
    snakeOkAssert( snakeGame->snake.dir != snakeDirError                                       ,  snakeDirValid                  )

    snakeOkAssertClose();

    for (size_t i = 0; i < snakeGame->snake.len - 1; i++)
    {
        if ( !isPointWithinBoundsAndZero (snakeGame->snake.bodyPos[i], snakeGame->fieldCorner) )
        {
            throw(SNAKEERR_bodyPosWithinBounds); 
        }
    };

    return 0;
}

#undef snakeOkAssertClose

#undef snakeOkAssert
