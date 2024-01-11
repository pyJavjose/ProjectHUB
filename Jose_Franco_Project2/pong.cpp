//
// Created by jfran on 12/6/2023.
//
#include "pong.h"
#include <chrono>

struct GameStats
{
    int scorePaddleOne = 0;
    int scorePaddleTwo = 0;

};
void statSaver (const GameStats& stats)
{
    std::ofstream File ("stats.txt");
    if (File.is_open())
    {
        File << "Player One's Score: " << stats.scorePaddleOne << std::endl;
        File << "Player Two's Score: " << stats.scorePaddleTwo << std::endl;
        File.close();
    }

}
GameStats loadStats()
{
    GameStats stats;

    std::ifstream File ("stats.txt");
    if (File.is_open())
    {
        File >> stats.scorePaddleOne >> stats.scorePaddleTwo;
        File.close();
    }
    return stats;
}
enum Buttons
{
    PaddleOneUp = 0,
    PaddleOneDown,
    PaddleTwoUp,
    PaddleTwoDown,
};
enum class typeOfCollision
{
    None,
    Top,
    Middle,
    Bottom,
    Left,
    Right
};

struct contactWithBall
{
    typeOfCollision type;
    float penetration;
};

class Ball
{
public:
    Ball()
            : x(0.0f), y(0.0f)
    {}
    Ball (float x, float y)
            : x(x), y(y)
    {}
    Ball operator+(Ball const& rhs)
    {
        return Ball(x + rhs.x, y + rhs.y);

    }
    Ball& operator+= (Ball const& rhs)
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }
    Ball operator*(float rhs)
    {
        return Ball(x * rhs, y * rhs);
    }
    float x, y;
};
class BigBall
{
public:
    BigBall(Ball position, Ball velocity)
            : position(position), velocity(velocity)
    {
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
        rect.w = BALL_WIDTH;
        rect.h = BALL_HEIGHT;
    }
    void update(float dt)
    {
        position += velocity * dt;
    }
    void draw (SDL_Renderer* renderer)
    {
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);

        SDL_RenderFillRect(renderer, &rect);
    }
    void paddleCollision (contactWithBall const& contact)
    {
        position.x += contact.penetration;
        velocity.x = -velocity.x;

        if (contact.type == typeOfCollision::Top)
        {
            velocity.y = -.75f * BALL_SPEED;
        }
        else if (contact.type == typeOfCollision::Bottom)
        {
            velocity.y = 0.75f * BALL_SPEED;

        }

    }
    void wallCollision (contactWithBall const& contact)
    {
        if ((contact.type == typeOfCollision::Top) || (contact.type == typeOfCollision::Bottom))
        {
            position.y += contact.penetration;
            velocity.y = -velocity.y;
        }
        else if (contact.type == typeOfCollision::Left)
        {
            position.x = WINDOW_WIDTH / 2.0f;
            position.y = WINDOW_HEIGHT / 2.0f;
            velocity.x = BALL_SPEED;
            velocity.y = 0.75f * BALL_SPEED;
        }
        else if (contact.type == typeOfCollision::Right)
        {
            position.x = WINDOW_WIDTH / 2.0f;
            position.y = WINDOW_HEIGHT / 2.0f;
            velocity.x = -BALL_SPEED;
            velocity.y = 0.75f * BALL_SPEED;
        }
    }
    Ball position;
    Ball velocity;
    SDL_Rect rect{};
};
class Paddle
{
public:
    Paddle(Ball position, Ball velocity)
            : position(position), velocity(velocity)
    {
        rect.x = static_cast<int>(position.x);
        rect.y = static_cast<int>(position.y);
        rect.w = PADDLE_WIDTH;
        rect.h = PADDLE_HEIGHT;
    }
    void update (float dt)
    {
        position += velocity * dt;
        if (position.y < 0)
        {
            position.y  = 0;
        }
        else if (position.y > (WINDOW_HEIGHT - PADDLE_HEIGHT))
        {
            position.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
        }
    }
    void draw (SDL_Renderer* renderer)
    {
        rect.y = static_cast<int>(position.y);

        SDL_RenderFillRect(renderer, &rect);
    }
    Ball position;
    Ball velocity;
    SDL_Rect rect{};
};
contactWithBall collidor (BigBall const& ball, Paddle const& paddle);
contactWithBall checkWallCollision (BigBall const& ball);
void playPong()
{
    GameStats gameStats = loadStats();



    const float PADDLE_SPEED = 1.0f;

    SDL_Init(SDL_INIT_VIDEO);



    SDL_Window* window = SDL_CreateWindow("Pong", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);


    BigBall ball
            (Ball(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f),
             Ball(BALL_SPEED, 0.0f));

    Paddle paddleOne (
            Ball(50.0f, WINDOW_HEIGHT / 2.0f),
            Ball(0.0f, 0.0f));

    Paddle paddleTwo (
            Ball(WINDOW_WIDTH - 50.0f, WINDOW_HEIGHT / 2.0f),
            Ball(0.0f, 0.0f));


    bool running = true;
    bool buttons[4] = {};

    float dt = 0.0f;

    while (running)
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                else if (event.key.keysym.sym == SDLK_w)
                {
                    buttons[Buttons::PaddleOneUp] = true;
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    buttons[Buttons::PaddleOneDown] = true;
                }
                else if
                        (event.key.keysym.sym == SDLK_UP)
                {
                    buttons[Buttons::PaddleTwoUp] = true;
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    buttons[Buttons::PaddleTwoDown] = true;
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_w)
                {
                    buttons[Buttons::PaddleOneUp] = false;
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    buttons[Buttons::PaddleOneDown] = false;
                }
                else if (event.key.keysym.sym == SDLK_UP)
                {
                    buttons[Buttons::PaddleTwoUp] = false;
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    buttons[Buttons::PaddleTwoDown] = false;
                }
            }
        }
        if (buttons[Buttons::PaddleOneUp])
        {
            paddleOne.velocity.y = -PADDLE_SPEED;
        }
        else if (buttons[Buttons::PaddleOneDown])
        {
            paddleOne.velocity.y = PADDLE_SPEED;
        }
        else
        {
            paddleOne.velocity.y = 0.0f;
        }

        if (buttons[Buttons::PaddleTwoUp])
        {
            paddleTwo.velocity.y = -PADDLE_SPEED;
        }
        else if (buttons[Buttons::PaddleTwoDown])
        {
            paddleTwo.velocity.y = PADDLE_SPEED;
        }
        else
        {
            paddleTwo.velocity.y = 0.0f;
        }

        paddleOne.update(dt);
        paddleTwo.update(dt);

        ball.update(dt);


        if(contactWithBall contact = collidor(ball, paddleOne);
                contact.type != typeOfCollision::None)
        {
            ball.paddleCollision(contact);
            gameStats.scorePaddleOne++;

        }
        else if (contact = collidor(ball, paddleTwo);
                contact.type != typeOfCollision::None)
        {
            ball.paddleCollision(contact);
            gameStats.scorePaddleTwo++;

        }
        else if (contact = checkWallCollision(ball);
                contact.type != typeOfCollision::None)
        {
            ball.wallCollision(contact);
        }


        SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        for (int y = 0; y < WINDOW_HEIGHT; y++)
        {
            if (y % 5)
            {
                SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, y);
            }
        }

        ball.draw(renderer);

        paddleOne.draw(renderer);
        paddleTwo.draw(renderer);

        SDL_RenderPresent(renderer);

        auto stopTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
    }
    statSaver(gameStats);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}
/**
 * Checks Collision with the faces of the paddles
 * @param ball
 * @param paddle
 * @return
 */
contactWithBall collidor (BigBall const& ball, Paddle const& paddle)
{
    float leftGuy= ball.position.x;
    float rightGuy = ball.position.x + BALL_WIDTH;
    float topGuy = ball.position.y;
    float bottomGuy = ball.position.y + BALL_HEIGHT;

    float leftPad = paddle.position.x;
    float rightPad = paddle.position.x + PADDLE_WIDTH;
    float topPad = paddle.position.y;
    float bottomPad = paddle.position.y + PADDLE_HEIGHT;

    contactWithBall contact{};

    if (leftGuy >= rightPad)
    {
        return contact;
    }
    if (rightGuy <= leftPad)
    {
        return contact;
    }
    if(topGuy >= bottomPad)
    {
        return contact;
    }
    if (bottomGuy <= topPad)
    {
        return contact;
    }
    float upperPaddle = bottomPad - (2.0f * PADDLE_HEIGHT / 3.0f);
    float middlePaddle = bottomPad - (PADDLE_HEIGHT / 3.0f);

    if (ball.velocity.x < 0 )
    {
        contact.penetration = rightPad - leftGuy;
    }
    else if (ball.velocity.x > 0 )
    {
        contact.penetration = leftPad - rightGuy;
    }
    if ((bottomGuy > topPad) && (bottomGuy < upperPaddle))
    {
        contact.type = typeOfCollision::Top;
    }
    if((bottomGuy > topPad) && (bottomGuy < upperPaddle))
    {
        contact.type = typeOfCollision::Middle;
    }
    else
    {
        contact.type = typeOfCollision::Bottom;
    }
    return contact;
}
/**
 * Checks to see what type of collision the ball had with the wall
 * @param ball
 * @return
 */
contactWithBall checkWallCollision (BigBall const& ball)
{
    float ballLeft = ball.position.x;
    float ballRight = ball.position.x + BALL_WIDTH;
    float ballTop = ball.position.y;
    float ballBottom = ball.position.y + BALL_HEIGHT;

    contactWithBall contact{};

    if (ballLeft < 0.0f)
    {
        contact.type = typeOfCollision::Left;
    }
    else if (ballRight > WINDOW_WIDTH)
    {
        contact.type = typeOfCollision::Right;
    }
    else if (ballTop < 0.0f)
    {
        contact.type = typeOfCollision::Top;
        contact.penetration = -ballTop;
    }
    else if (ballBottom > WINDOW_HEIGHT)
    {
        contact.type = typeOfCollision::Bottom;
        contact.penetration = WINDOW_HEIGHT - ballBottom;
    }

    return contact;
}
