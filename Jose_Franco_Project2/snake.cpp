//
// Created by jfran on 12/5/2023.
//
#include <iostream>
#include <SDL.h>
#include <chrono>
#include <fstream>
#include <vector>
#include <algorithm>
#include <deque>



struct LandMine
{
    int x;
    int y;
};
enum Direction
{
    down,
    left,
    right,
    up
};
/**
 * Saves the game stats
 * @param numberOfApples // Saves the most amount of apples eaten
 * @param gamesPlayed // Saves the number of games played
 * @param bigSnakeSize // Saves the biggest snake size
 */
void saveGameStats (int numberOfApples, int gamesPlayed, int bigSnakeSize)
{
    std::ofstream File ("SnakeStats.txt");

    if (File.is_open())
    {
        File << "Most Apples Throated: " << numberOfApples << std::endl;
        File << "Number of Games Played: " << gamesPlayed << std::endl;
        File << "Biggest Snake: " << bigSnakeSize << std::endl;

        File.close();
    }
    else
    {
        std::cerr << "Unable to open file " << std::endl;
    }
}

void playSnake()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    auto window = SDL_CreateWindow("Jose's Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
    auto render = SDL_CreateRenderer(window, -1, 0);

    SDL_Event event;
    std::deque<SDL_Rect> recQue;
    int snakeSize = 1;

    std::vector<SDL_Rect> appleGuys;

    std::vector<LandMine> landMine;




    srand(static_cast<unsigned>(time(nullptr)));

    // Land Mine Generator

    for (int i = 0; i < 20; i++)
    {
        landMine.emplace_back(rand() % 100 *10, rand() % 100 *10);
    }




    bool running = true;

    int direction = 0;
    SDL_Rect snakeHead {500,500,10,10};

    //Apple Generator

    for (int i = 0; i < 100; i++)
    {
        appleGuys.emplace_back(rand() % 100 * 10, rand()%100*10, 10, 10);
    }

    int realAppleFan = 0;
    int numberOfApples = 0;
    int gamesPlayed = 0;
    int bigSnakeSize = 0;


    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_DOWN)
                {
                    direction = down;
                }
                if (event.key.keysym.sym == SDLK_UP)
                {
                    direction = up;
                }
                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    direction = left;
                }
                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    direction = right;
                }
            }
        }
        // Colliding with the Wall
        if (snakeHead.x < 0 || snakeHead.x >= 1000 || snakeHead.y < 0 || snakeHead.y >= 1000)
        {
            direction = 0;

            gamesPlayed++;
            numberOfApples = std::max(numberOfApples, realAppleFan);
            bigSnakeSize = std::max(bigSnakeSize, snakeSize);

            std::cout << "Game Over! " << std::endl;
            std::cout << "Apples eaten in this game: " << realAppleFan << std::endl;
            std::cout << "Most apples eaten in a single game: " << numberOfApples << std::endl;
            std::cout << "Number of games played: " << gamesPlayed << std::endl;
            std::cout << "Largest Snake size: " << bigSnakeSize << std::endl;

            saveGameStats(numberOfApples, gamesPlayed, bigSnakeSize);

            snakeSize = 1;
            realAppleFan = 0;
            recQue.clear();
            snakeHead = {500, 500, 10, 10};

            SDL_Delay(2000);
            continue;

        }

        switch (direction)
        {
            case down:
                snakeHead.y += 10;
                break;
            case up:
                snakeHead.y -= 10;
                break;
            case left:
                snakeHead.x -= 10;
                break;
            case right:
                snakeHead.x += 10;
                break;
        }
        // Land Mine Collision
        std::for_each(landMine.begin(), landMine.end(), [&](const auto& mine)
        {
            if (snakeHead.x == mine.x && snakeHead.y == mine.y)
            {
                direction = 0;

                gamesPlayed++;
                numberOfApples = std::max(numberOfApples, realAppleFan);
                bigSnakeSize = std::max(bigSnakeSize, snakeSize);

                std::cout << "ur shit exploded lol  " << std::endl;
                std::cout << "Apples eaten in this game: " << realAppleFan << std::endl;
                std::cout << "Most apples eaten in a single game: " << numberOfApples << std::endl;
                std::cout << "Number of games played: " << gamesPlayed << std::endl;
                std::cout << "Largest Snake size: " << bigSnakeSize << std::endl;

                saveGameStats(numberOfApples, gamesPlayed, bigSnakeSize);
                snakeSize = 1;
                realAppleFan = 0;
                recQue.clear();
                snakeHead = {500, 500, 10, 10};


                SDL_Delay(2000);

                running = false;
            }

        });
        // Apple Collision
        std::for_each(appleGuys.begin(), appleGuys.end(), [&] (auto& apple)
        {
            if (snakeHead.x == apple.x && snakeHead.y == apple.y)
            {
                snakeSize+= 10;
                apple.x = -10;
                apple.y = -10;
                realAppleFan++;
            }
        });
        // Snake-Head Collision
        std::for_each(recQue.begin(), recQue.end(), [&](auto& snakeSeg)
        {
            if (snakeHead.x == snakeSeg.x && snakeHead.y == snakeSeg.y)
            {
                snakeSize = 1;
            }

        });

        recQue.push_front(snakeHead);

        while (recQue.size() > snakeSize)
        {
            recQue.pop_back();
        }

        std::cout<<"Apples eaten: " << realAppleFan << std::endl;


        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);


        // Renders the land mines
        SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
        std::for_each(landMine.begin(), landMine.end(), [&] (const auto& mine)
        {
            SDL_Rect rect {mine.x, mine.y, 10, 10};
            SDL_RenderFillRect(render, &rect);

        });


        // Renders the Snake
        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
        std::for_each(recQue.begin(), recQue.end(), [&](auto& snakeSeg)
        {
            SDL_RenderFillRect(render, &snakeSeg);

        });
        // Renders the apples
        SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
        std::for_each(appleGuys.begin(), appleGuys.end(), [&](auto& apple)
        {
            SDL_RenderFillRect(render, &apple);

        });
        SDL_RenderPresent(render);

        SDL_Delay(25);
    }
}
