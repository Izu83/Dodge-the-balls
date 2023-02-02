#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

int main()
{
    std::ifstream best_score_file{ "score.txt" };
    int best_score;
    best_score_file >> best_score;
    best_score_file.close();

    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(400, 400), "Ball Game");
    window.setFramerateLimit(60);

    sf::Texture background_texture;
    if (!background_texture.loadFromFile("background.png")) 
    {

        return 1;
    }
    sf::Sprite background(background_texture);

    sf::Image icon;
    if (!icon.loadFromFile("player.png"))
        return 1;
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("player.png"))
        return 1;

    sf::Sprite player;
    player.setTexture(playerTexture);
    player.setTextureRect(sf::IntRect(0, 0, 64, 64));
    player.setPosition(175, 325);

    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("enemy.png"))
        return 1;


    std::vector<sf::Sprite> obstacles;

    int score = 0;
    sf::Text scoreText;
    sf::Font font;
    font.loadFromFile("CascadiaCode.ttf");
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    sf::Text bestScoreText;
    bestScoreText.setFont(font);
    bestScoreText.setCharacterSize(20);
    bestScoreText.setFillColor(sf::Color::White);
    bestScoreText.setPosition(219, 375);
    bestScoreText.setString("Best score: " + std::to_string(best_score));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                if (score > best_score)
                {
                    std::ofstream best_score_file_out("score.txt");
                    best_score_file_out << score;

                }
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player.getPosition().x > 0)
            player.move(-5, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player.getPosition().x < 350)
            player.move(5, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            scoreText.setString("Closing Game!");
            scoreText.setCharacterSize(36);
            scoreText.setPosition(120, 200);
            window.draw(scoreText);
            window.display();
            sf::sleep(sf::seconds(2));
            if (score > best_score)
            {
                std::ofstream best_score_file_out("score.txt");
                best_score_file_out << score;

            }
            window.close();
        }

        if (rand() % 100 == 0)
        {
            sf::Sprite obstacle;
            obstacle.setTexture(enemyTexture);
            obstacle.setTextureRect(sf::IntRect(0, 0, 64, 64));
            obstacle.setPosition(rand() % 300, 0);
            obstacles.push_back(obstacle);
        }

        for (int i = 0; i < obstacles.size(); i++)
        {
            obstacles[i].move(0, 5);

            if (obstacles[i].getGlobalBounds().intersects(player.getGlobalBounds()))
            {
                scoreText.setString("Game Over!");
                scoreText.setCharacterSize(36);
                scoreText.setPosition(120, 200);
                window.draw(scoreText);
                window.display();
                sf::sleep(sf::seconds(2));
                if (score > best_score)
                {
                    std::ofstream best_score_file_out("score.txt");
                    best_score_file_out << score;

                }
                window.close();
            }

            if (obstacles[i].getPosition().y > 400)
            {
                obstacles.erase(obstacles.begin() + i);
                score++;

                scoreText.setString("Score: " + std::to_string(score));
            }
        }

        window.clear();
        window.draw(background);
        window.draw(bestScoreText);
        window.draw(scoreText);
        window.draw(player);
        for (int i = 0; i < obstacles.size(); i++)
            window.draw(obstacles[i]);
        window.display();
    }

    return 0;
}

