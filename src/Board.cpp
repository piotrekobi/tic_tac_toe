#include "Board.hpp"
#include <vector>

#define BOARD_WIDTH 900
#define BOARD_HEIGHT 900
#define BOARD_PADDING 35
#define LINE_LENGTH (BOARD_WIDTH - BOARD_PADDING * 2)
#define LINE_THICKNESS 20

Board::Board()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            this->boardArray[i][j] = 0;
    }

    this->initBoard();
}

void Board::initBoard()
{
    for (int i = 0; i < 2; i++)
    {
        this->boardLines[i].setSize(sf::Vector2f(LINE_LENGTH, LINE_THICKNESS));
        this->boardLines[i].setFillColor(sf::Color::Black);
        this->boardLines[i].setPosition(BOARD_PADDING, (i + 1) * (BOARD_WIDTH / 3) - (LINE_THICKNESS / 2));
    }

    for (int i = 2; i < 4; i++)
    {
        this->boardLines[i].setSize(sf::Vector2f(LINE_THICKNESS, LINE_LENGTH));
        this->boardLines[i].setFillColor(sf::Color::Black);
        this->boardLines[i].setPosition((i - 1) * (BOARD_WIDTH / 3) - (LINE_THICKNESS / 2), BOARD_PADDING);
    }
}

void Board::addMove(int player, int x, int y)
{
    this->boardArray[x][y] = player;
}

void Board::cancelMove(int x, int y)
{
    this->boardArray[x][y] = 0;
}

bool Board::freeSpot(int x, int y)
{
    return this->boardArray[x][y] == 0;
}

std::vector<std::vector<int>> Board::getFreeSpots()
{
    std::vector<std::vector<int>> freeSpots;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (boardArray[i][j] == 0)
                freeSpots.push_back({i, j});
        }
    }
    return freeSpots;
}

int Board::checkWinner()
{
    for (int i = 0; i < 3; i++)
    {
        if (boardArray[i][0] != 0 and boardArray[i][0] == boardArray[i][1] and boardArray[i][1] == boardArray[i][2])
            return boardArray[i][0];
        if (boardArray[0][i] != 0 and boardArray[0][i] == boardArray[1][i] and boardArray[1][i] == boardArray[2][i])
            return boardArray[0][i];
    }
    if (boardArray[1][1] != 0 and boardArray[0][0] == boardArray[1][1] and boardArray[1][1] == boardArray[2][2])
        return boardArray[1][1];
    if (boardArray[1][1] != 0 and boardArray[0][2] == boardArray[1][1] and boardArray[1][1] == boardArray[2][0])
        return boardArray[1][1];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (boardArray[i][j] == 0)
                return 0;
        }
    }

    return 3;
}

int Board::getBoardPlayer(int x, int y)
{
    return this->boardArray[x][y];
}
