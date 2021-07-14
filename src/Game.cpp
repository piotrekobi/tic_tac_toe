#include "Game.hpp"
#include <iostream>
#include <math.h>
#include <vector>

#define BOARD_WIDTH 900
#define BOARD_HEIGHT 900
#define BOARD_PADDING 35
#define LINE_LENGTH (BOARD_WIDTH - BOARD_PADDING * 2)
#define LINE_THICKNESS 20
#define CIRCLE_RADIUS 100
#define CROSS_LINE_LENGTH 282
#define SHAPE_WIDTH 10

void Game::initVariables()
{
    this->window = nullptr;
    this->currentPlayer = 1;
    this->winner = 0;
    this->numTurn = 1;
    srand(time(0));
}

void Game::initWindow()
{
    this->videoMode.width = 900;
    this->videoMode.height = 900;
    this->window = new sf::RenderWindow(this->videoMode, "Tic Tac Toe", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(30);
}

void Game::drawShape(int x, int y, int player)
{
    if (player == 0)
        return;

    else if (player == 1)
    {
        sf::CircleShape circle(CIRCLE_RADIUS);
        circle.setFillColor(sf::Color::White);
        circle.setOutlineThickness(SHAPE_WIDTH);
        circle.setOutlineColor(sf::Color::Black);
        circle.setPosition(x * BOARD_WIDTH / 3 + sqrt(2) * BOARD_PADDING,
                           y * BOARD_WIDTH / 3 + sqrt(2) * BOARD_PADDING);
        this->window->draw(circle);
    }
    else
    {
        sf::RectangleShape crossLines[2];
        for (auto &line : crossLines)
        {
            line.setSize(sf::Vector2f(CROSS_LINE_LENGTH, SHAPE_WIDTH));
            line.setFillColor(sf::Color::Black);
            line.setPosition(x * BOARD_WIDTH / 3 + sqrt(2) * BOARD_PADDING,
                             y * BOARD_WIDTH / 3 + sqrt(2) * BOARD_PADDING);
        }
        crossLines[0].rotate(45);
        crossLines[1].rotate(-45);
        crossLines[1].move(0, sqrt(2) * CROSS_LINE_LENGTH / 2);
        this->window->draw(crossLines[0]);
        this->window->draw(crossLines[1]);
    }
}

Game::Game(int humanPlayer) : humanPlayer(humanPlayer)
{
    this->initVariables();
    this->initWindow();

    this->gameBoard = new Board();
    this->renderBoard();
}

Game::~Game()
{
    delete this->window;
}

bool Game::running()
{
    return this->window->isOpen();
}

void Game::pollEvents()
{
    while (this->window->pollEvent(this->ev))
    {
        switch (ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;

        default:
            break;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) and this->currentPlayer == this->humanPlayer)
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
            if (mousePosition.x >= 0 && mousePosition.x <= BOARD_WIDTH && mousePosition.y >= 0 &&
                mousePosition.y <= BOARD_HEIGHT)
            {
                int x, y;
                x = mousePosition.x / (BOARD_WIDTH / 3);
                y = mousePosition.y / (BOARD_HEIGHT / 3);
                if (this->gameBoard->freeSpot(x, y))
                {
                    this->gameBoard->addMove(this->currentPlayer, x, y);
                    this->nextTurn();
                }
            }
        }
    }
}

void Game::makeRandomMove()
{
    std::vector<std::vector<int>> freeSpots = this->gameBoard->getFreeSpots();
    int index = rand() % freeSpots.size();
    this->gameBoard->addMove(this->currentPlayer, freeSpots.at(index).at(0), freeSpots.at(index).at(1));
}

void Game::makeRandomMove(std::vector<std::vector<int>> moves)
{
    int index = rand() % moves.size();
    this->gameBoard->addMove(currentPlayer, moves.at(index).at(0), moves.at(index).at(1));
}

void Game::makeBestMove()
{
    std::vector<std::vector<int>> bestSpots;
    int bestScore = (humanPlayer == 1) ? 2 : -2;

    std::vector<std::vector<int>> freeSpots = gameBoard->getFreeSpots();
    for (const auto &spot : freeSpots)
    {
        gameBoard->addMove(currentPlayer, spot.at(0), spot.at(1));
        int score = minimax(0, humanPlayer == 1, -2, 2);
        gameBoard->cancelMove(spot.at(0), spot.at(1));

        bool comparison = (humanPlayer == 1) ? score < bestScore : score > bestScore;
        if (comparison)
        {
            bestSpots.clear();
            bestScore = score;
            bestSpots.push_back(spot);
        }
        else if (score == bestScore)
            bestSpots.push_back(spot);
    }
    makeRandomMove(bestSpots);
}

int Game::getScore(int depth, bool isMaximizing, int alpha, int beta)
{
    int bestScore = (isMaximizing) ? -2 : 2;
    int player = (isMaximizing) ? 1 : 2;
    std::vector<std::vector<int>> freeSpots = this->gameBoard->getFreeSpots();
    for (const auto &spot : freeSpots)
    {
        this->gameBoard->addMove(player, spot.at(0), spot.at(1));
        int score = minimax(depth + 1, !isMaximizing, alpha, beta);
        this->gameBoard->cancelMove(spot.at(0), spot.at(1));
        if (isMaximizing)
        {
            if (score > bestScore)
                bestScore = score;
            if (bestScore > alpha)
                alpha = bestScore;
        }
        else
        {
            if (score < bestScore)
                bestScore = score;
            if (bestScore < beta)
                beta = bestScore;
        }

        if (alpha >= beta)
            break;
    }
    return bestScore;
}

int Game::minimax(int depth, bool isMaximizing, int alpha, int beta)
{
    int winner = this->gameBoard->checkWinner();
    if (winner != 0)
    {
        switch (winner)
        {
        case 1:
            return 1;
            break;
        case 2:
            return -1;
            break;
        case 3:
            return 0;
            break;
        }
    }
    return getScore(depth, isMaximizing, alpha, beta);
}

void Game::nextTurn()
{
    this->currentPlayer = (this->currentPlayer % 2) + 1;
    this->numTurn++;
    this->winner = this->gameBoard->checkWinner();
    this->renderBoard();
}
void Game::update()
{
    this->pollEvents();
    if (this->currentPlayer != humanPlayer)
    {
        if (numTurn == 1)
            makeRandomMove();
        else
            makeBestMove();
        this->nextTurn();
    }
}

void Game::renderBoard()
{
    this->window->clear(sf::Color::White);
    for (const auto &line : this->gameBoard->boardLines)
    {
        this->window->draw(line);
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            drawShape(i, j, this->gameBoard->getBoardPlayer(i, j));
        }
    }

    if (this->winner != 0)
    {
        switch (this->winner)
        {
        case 1:
            std::cout << "Circle wins!" << std::endl;
            break;
        case 2:
            std::cout << "Cross wins!" << std::endl;
            break;
        case 3:
            std::cout << "Tie" << std::endl;
            break;
        }
        this->window->display();
        sf::sleep(sf::seconds(1));
        this->window->close();
        exit(0);
    }
    this->window->display();
}

// void Game::render()
// {

// }
