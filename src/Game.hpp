#pragma once

#include "Board.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

class Game
{

    int currentPlayer;
    int humanPlayer;
    sf::RenderWindow *window;
    sf::Event ev;
    sf::VideoMode videoMode;
    Board *gameBoard;
    int winner;
    int numTurn;
    void initVariables();
    void initWindow();
    void nextTurn();
    void makeRandomMove();
    void makeRandomMove(std::vector<std::vector<int>> moves);
    void makeBestMove();
    void drawShape(int x, int y, int shape);
    int minimax(int depth, bool isMaximizing, int alpha, int beta);
    int getScore(int depth, bool isMaximizing, int alpha, int beta);

  public:
    Game(int humanPlayer);
    virtual ~Game();
    bool running();
    void pollEvents();
    void update();
    void renderBoard();
};