#include "Game.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    int humanPlayer = 1;
    if (argc > 1)
    {
        humanPlayer = atoi(argv[1]);
    }
    Game game(humanPlayer);

    while (game.running())
    {
        game.update();
    }

    return 0;
}