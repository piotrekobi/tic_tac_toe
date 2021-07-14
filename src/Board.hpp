#pragma once

#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

class Board
{
    int boardArray[3][3];
    void initBoard();

  public:
    sf::RectangleShape boardLines[4];
    Board();
    std::vector<std::vector<int>> getFreeSpots();
    int checkWinner();
    int getBoardPlayer(int x, int y);
    bool freeSpot(int x, int y);
    void addMove(int player, int x, int y);
    void cancelMove(int x, int y);
    // ~Board();
};