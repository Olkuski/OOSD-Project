#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <cmath>
#include <iostream>

class GameObject
{
public:
    float x;
    float y;

    virtual void display(sf::RenderWindow &window) = 0; // function for displaying the avatar and the enemy
    virtual void move() = 0;                            // Function for movement of the avatar and the enemy
};

class Avatar : public GameObject
{
public:
    int wide = 30;
    int high = 60;
    sf::Sprite avatarSprite;
    sf::Texture avatarTexture;

    Avatar()
    {
        if (!avatarTexture.loadFromFile("rocket.png")) // loads the avatar from the image png file
        {
            throw std::runtime_error("Failed to load rocket.png file for avatar!");
        }
        avatarSprite.setTexture(avatarTexture);
    }

    // sets the cursor to the middle of the image
    void display(sf::RenderWindow &window) override
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2u textureSize = avatarTexture.getSize();

        sf::Vector2f spritePosition(
            std::max(-100, mousePosition.x - static_cast<int>(textureSize.x) / 2),
            mousePosition.y - textureSize.y / 2);

        avatarSprite.setPosition(spritePosition);
        window.draw(avatarSprite);
    }

    void move() override {}
};

class Enemy : public GameObject
{
public:
    float size; // size variable to determine the size of the asteroids (enemies)

    void spawn()
    {
        size = rand() % 51 + 30; // Generates random size between 30 and 80 for the enemy
        x = rand() % 481 + 10;   // Generates random x-axis position between 10 and 490 for the enemy
        y = 0;                   // Starts the enemies at the top of the window
    }

    void display(sf::RenderWindow &window) override
    {
        sf::CircleShape enemyShape(size / 2);
        enemyShape.setPosition(x - size / 2, y - size / 2); // Sets the position of the enemy shape
        enemyShape.setFillColor(sf::Color(165, 42, 42));    // Sets the color of the enemy shape
        window.draw(enemyShape);                            // Draws the enemy shape to the window
    }

    void move() override
    {
        y = y + 0.1;            // Increments the y-axis coordinate of the enemies to make them move down the window
        if (x > 500 || y > 500) // If the enemy goes out of the window's bounds
        {
            spawn(); // Respawns the enemies at the top with new random positions and sizes
        }
    }
};

int main()
{
    sf::RenderWindow window;

    try
    {
        window.create(sf::VideoMode(500, 500), "Dodger"); // Creates the game window with size 500x500 and title "Dodger"

        Enemy *enemies[4]; // Creates an array of 4 Enemy pointers
        Avatar avatar;
        sf::Font font;
        font.loadFromFile("Roboto-Bold.ttf"); // Loads the "roboto bold" font from the file
        int stage = 1;                        // Stage variable
        int score = 0;                        // Score variable

        for (int i = 0; i < 4; i++)
        {
            enemies[i] = new Enemy(); // Dynamically allocate memory for each Enemy object
            enemies[i]->spawn();      // Spawns the enemies at the beginning of the game
        }

        while (window.isOpen()) // Game loop
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close(); // Closes the game window if the close button is clicked

                if (event.type == sf::Event::MouseButtonPressed && stage == 1)
                    stage = 2; // Moves to the next stage when the mouse button is pressed in stage 1

                if (event.type == sf::Event::MouseButtonPressed && stage == 3)
                    window.close(); // Closes the game window when the mouse button is pressed in stage 3
            }

            window.clear(sf::Color::Black); // Clears the window with black color

            if (stage == 1)
            {
                // Stage 1: Displays the title and instructions
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
                // Stage 2: Gameplay
                score = static_cast<int>(std::floor(static_cast<float>(clock()) / 1000.0f));
                sf::Text scoreText;
                scoreText.setFont(font);
                scoreText.setCharacterSize(30);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setString("Score: " + std::to_string(score));
                scoreText.setPosition(10, 30);
                window.draw(scoreText);

                avatar.display(window); // Display the avatar (rocket) on the window

                for (int i = 0; i < 4; i++)
                {
                    enemies[i]->display(window); // Displays the enemies on the window
                    enemies[i]->move();          // Moves the enemies
                }

                // Collision detection algorithm between enemies and the avatar
                for (int i = 0; i < 4; i++)
                {
                    float testX = enemies[i]->x;
                    float testY = enemies[i]->y;

                    // checks if one of the sides of the rocket (avatar) is touching the asteroid (enemy)
                    if (enemies[i]->x > sf::Mouse::getPosition(window).x + avatar.wide / 2)
                    {
                        testX = sf::Mouse::getPosition(window).x + avatar.wide / 3;
                    }
                    else if (enemies[i]->x < sf::Mouse::getPosition(window).x - avatar.wide / 2)
                    {
                        testX = sf::Mouse::getPosition(window).x - avatar.wide / 3;
                    }

                    // checks if the top or bottom of the rocket (avatar) is touching the asteroid (enemy)
                    if (enemies[i]->y > sf::Mouse::getPosition(window).y + avatar.high / 2)
                    {
                        testY = sf::Mouse::getPosition(window).y + avatar.high / 3;
                    }
                    else if (enemies[i]->y < sf::Mouse::getPosition(window).y - avatar.high / 2)
                    {
                        testY = sf::Mouse::getPosition(window).y - avatar.high / 3;
                    }

                    // checks the distance between the center of the rocket (avatar) and the asteroid (enemy)
                    float distance = std::sqrt(std::pow(enemies[i]->x - testX, 2) + std::pow(enemies[i]->y - testY, 2));

                    if (distance < enemies[i]->size / 2) // If collision occurs
                    {
                        stage = 3; // Move to the next stage
                    }
                }
            }
            else if (stage == 3)
            {
                // Stage 3: Game over
                sf::Text gameOverText;
                gameOverText.setFont(font);
                gameOverText.setCharacterSize(60);
                gameOverText.setFillColor(sf::Color::White);
                gameOverText.setString("Game Over!");
                gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, window.getSize().y / 2 - 100);
                window.draw(gameOverText);

                // The display of the final score
                sf::Text finalScoreText;
                finalScoreText.setFont(font);
                finalScoreText.setCharacterSize(30);
                finalScoreText.setFillColor(sf::Color::White);
                finalScoreText.setString("Final Score: " + std::to_string(score));
                finalScoreText.setPosition(window.getSize().x / 2 - finalScoreText.getGlobalBounds().width / 2, window.getSize().y / 2 - 30);
                window.draw(finalScoreText);

                // The display the "Click the screen to quit!" message
                sf::Text quitText;
                quitText.setFont(font);
                quitText.setCharacterSize(20);
                quitText.setFillColor(sf::Color::White);
                quitText.setString("Click the screen to quit!");
                quitText.setPosition(window.getSize().x / 2 - quitText.getGlobalBounds().width / 2, window.getSize().y / 2 + 10);
                window.draw(quitText);
            }

            window.display(); // Updates the window display
        }

        for (int i = 0; i < 4; i++)
        {
            delete enemies[i]; // Deallocate memory for each Enemy object
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}