#include <SFML/Window.hpp>

#include "Snake/snakeSFML.h"

int main()
{

    try
    {
        snakeUT(); 
    }
    catch (SNAKEERR_t err)
    {
        printf("\nError catched in %s: line %d, function %s;\nError name: ", __FILE__, __LINE__, __func__);
        snakeErrorPrint(err);       
        printf("\n");
    }
    catch (SNMLERR_t err)
    {
        printf("\nSNMLERR occured.\n");
    }

    return 0;
}
