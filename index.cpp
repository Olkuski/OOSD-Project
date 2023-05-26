#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>

class Avatar
{
public:
    int wide = 30;
    int high = 60;

    void display(sf::RenderWindow &window)
    {
        sf::RectangleShape avatarShape(sf::Vector2f(wide, high));
        avatarShape.setOrigin(wide / 2, high / 2);
        avatarShape.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        avatarShape.setFillColor(sf::Color(255, 0, 0));
        window.draw(avatarShape);

        sf::ConvexShape rocketShape;
        rocketShape.setPointCount(4);
        rocketShape.setPoint(0, sf::Vector2f(-20, -30));
        rocketShape.setPoint(1, sf::Vector2f(0, -60));
        rocketShape.setPoint(2, sf::Vector2f(20, -30));
        rocketShape.setPoint(3, sf::Vector2f(0, 0));
        rocketShape.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        rocketShape.setFillColor(sf::Color(255, 0, 0));
        window.draw(rocketShape);
    }
};

class Enemy
{
public:
    float size;
    float x;
    float y;
    int score;

    void spawn()
    {
        size = rand() % 51 + 30; // diameter
        x = rand() % 481 + 10;   // random position x
        y = 0;
    }

    void display(sf::RenderWindow &window)
    {
        sf::CircleShape enemyShape(size / 2);
        enemyShape.setPosition(x - size / 2, y - size / 2);
        enemyShape.setFillColor(sf::Color(165, 42, 42));
        window.draw(enemyShape);
    }

    void move()
    {
        y = y + 0.1; /* - (size / 10) + (score / 5);*/
        if (x > 500 || y > 500)
        {
            spawn();
        }
    }
};

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    sf::RenderWindow window(sf::VideoMode(500, 500), "Dodger");

    Enemy enemies[4];
    Avatar avatar;
    sf::Font font;
    font.loadFromFile("Roboto-Bold.ttf");
    int stage = 1;
    int score = 0;

    for (int i = 0; i < 4; i++)
    {
        enemies[i].spawn();
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && stage == 1)
                stage = 2;

            if (event.type == sf::Event::MouseButtonPressed && stage == 3)
                window.close();
        }

        window.clear(sf::Color::Black);

        if (stage == 1)
        {
            sf::Text titleText;
            titleText.setFont(font);
            titleText.setCharacterSize(60);
            titleText.setFillColor(sf::Color::White);
            titleText.setString("Dodger");
            titleText.setPosition(window.getSize().x / 2 - titleText.getGlobalBounds().width / 2, window.getSize().y / 2 - 100);
            window.draw(titleText);

            sf::Text infoText;
            infoText.setFont(font);
            infoText.setCharacterSize(20);
            infoText.setFillColor(sf::Color::White);
            infoText.setString("Avoid the asteroids with your rocket!");
            infoText.setPosition(window.getSize().x / 2 - infoText.getGlobalBounds().width / 2, window.getSize().y / 2 - 30);
            window.draw(infoText);

            sf::Text startText;
            startText.setFont(font);
            startText.setCharacterSize(20);
            startText.setFillColor(sf::Color::White);
            startText.setString("Click the screen to start!");
            startText.setPosition(window.getSize().x / 2 - startText.getGlobalBounds().width / 2, window.getSize().y / 2 - 10);
            window.draw(startText);
        }
        else if (stage == 2)
        {
            score = static_cast<int>(std::floor(static_cast<float>(clock()) / 1000.0f));
            sf::Text scoreText;
            scoreText.setFont(font);
            scoreText.setCharacterSize(30);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setString("Score: " + std::to_string(score));
            scoreText.setPosition(10, 30);
            window.draw(scoreText);

            avatar.display(window);

            for (int i = 0; i < 4; i++)
            {
                enemies[i].display(window);
                enemies[i].move();
            }

            for (int i = 0; i < 4; i++)
            {
                float testX = enemies[i].x;
                float testY = enemies[i].y;

                if (enemies[i].x > sf::Mouse::getPosition(window).x + avatar.wide / 2)
                {
                    testX = sf::Mouse::getPosition(window).x + avatar.wide / 3;
                }
                else if (enemies[i].x < sf::Mouse::getPosition(window).x - avatar.wide / 2)
                {
                    testX = sf::Mouse::getPosition(window).x - avatar.wide / 3;
                }

                if (enemies[i].y > sf::Mouse::getPosition(window).y + avatar.high / 2)
                {
                    testY = sf::Mouse::getPosition(window).y + avatar.high / 3;
                }
                else if (enemies[i].y < sf::Mouse::getPosition(window).y - avatar.high / 2)
                {
                    testY = sf::Mouse::getPosition(window).y - avatar.high / 3;
                }

                float distX = enemies[i].x - testX;
                float distY = enemies[i].y - testY;
                float distance = std::sqrt((distX * distX) + (distY * distY));

                if (distance < enemies[i].size / 2)
                {
                    stage = 3;
                }
            }
        }
        else if (stage == 3)
        {
            int totalScore = score;
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setCharacterSize(20);
            gameOverText.setFillColor(sf::Color::White);
            gameOverText.setString("Score: " + std::to_string(totalScore));
            gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, window.getSize().y / 2 - 30);
            window.draw(gameOverText);

            sf::Text quitText;
            quitText.setFont(font);
            quitText.setCharacterSize(20);
            quitText.setFillColor(sf::Color::White);
            quitText.setString("Click the screen to quit the app!");
            quitText.setPosition(window.getSize().x / 2 - quitText.getGlobalBounds().width / 2, window.getSize().y / 2 - 10);
            window.draw(quitText);
        }

        window.display();
    }

    return 0;
}