#include <iostream>
#include <SDL.h>
#include <chrono>
#include <fstream>
#include <vector>
#include <algorithm>
#include <deque>


#include "snake.h"
#include "pong.h"



enum Games
{
    Pong = 1,
    Snake = 2,
    Quit = -1
};

void playSnake();
void playPong();


int main(int argc, char* argv[])
{

    std::cout << "Hello Welcome to my Pong and Snake games, type into the console which game you wish to play" << std::endl;
    std::cout << " (1) Pong" << std::endl;
    std::cout << " (2) Snake " << std::endl;
    std::cout << "Type -1 into the console if you wish to not play either " << std::endl;

    int userInput;
    std::cin >> userInput;

    switch (userInput)
    {
        case Pong:
        {
            playPong();

            break;
        }

        case Snake:
        {
            playSnake();

            break;
        }
        case Quit:
        {
            std::cout<< "Thank you for playing! " << std::endl;

            break;
        }
        default:
        {
            std::cout <<  "Invalid Selection! " << std::endl;

        }
    }
    return 0;
}

