#include "Game.h"

// Private functions
void Game::initVariables()
{
    this->window = nullptr;

    // Game logic
    this->health = 10;
    this->endGame = false;
    this->points = 0;
    this->enemySpawnTimerMax = 10.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 5;
    this->mouseHeld = false;
}

void Game::initWindow()
{
    this->videoMode.height = 600;
    this->videoMode.width = 800;
    this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);

    this-> window->setFramerateLimit(60);
}

void Game::initEnemies() {
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setFillColor(sf::Color::Cyan);
    // this->enemy.setOutlineColor(sf::Color::Green);
    // this->enemy.setOutlineThickness(1.f);


}

void Game::initFonts()
{
    // this->font.loadFromFile("C:\\Users\\user\\Desktop\\cmake sfml\\src\\fonts\\ComicSansMS.ttf");
    // std::cout << system("cd");

    this->font.loadFromFile("../../../src/fonts/ComicSansMS.ttf");
}

void Game::initText()
{
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(25);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NONE");
}

// Constructors / Destructors
Game::Game()
{
    this->initFonts();
    this->initText();
    this->initVariables();
    this->initWindow();
    this->initEnemies();
}

Game::~Game() {
    std::cout << "deconstructor runs";
    delete this->window;
}


// Accessors

const bool Game::running() const
{
    return this->window->isOpen();
}

const bool Game::getEndGame() const
{
    return this->endGame;
}

// Functions

void Game::spawnEnemy()
{
    /*
        @return void

        Spawns enemies and sets their types and positions randomly.
        - Sets a random positions.
        - Sets a random a color.
        - Adds enemy to the vector.
    */

   this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f
   );

    // Randomize enemy type
    int type = rand() % 5;

    switch(type)
    {
        case 0:
            this->enemy.setFillColor(sf::Color::Magenta);
            this->enemy.setSize(sf::Vector2f(10.f, 10.f));
            break;
        case 1:
            this->enemy.setFillColor(sf::Color::Blue);
            this->enemy.setSize(sf::Vector2f(30.f, 30.f));
            break;
        case 2:
            this->enemy.setFillColor(sf::Color::Yellow);
            this->enemy.setSize(sf::Vector2f(50.f, 50.f));
            break;
        case 3:
            this->enemy.setFillColor(sf::Color::Cyan);
            this->enemy.setSize(sf::Vector2f(50.f, 50.f));
            break;
        case 4:
            this->enemy.setFillColor(sf::Color::Green);
            this->enemy.setSize(sf::Vector2f(100.f, 100.f));
            break;
        default:
            this->enemy.setFillColor(sf::Color::Red);
            this->enemy.setSize(sf::Vector2f(400.f, 400.f));
            break;
    }

    // Spawn the enemy
   this->enemies.push_back(this->enemy);
}

// Event polling
void Game::pollEvents() {
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Escape)
                    this->window->close();
                break;
        }
    }
}

void Game::updateMousePositions()
{
    /*
        @return void

        updates the mouse positions:
        - Mouse positin relative to window (Vector2i)
    */

   this->mousePosWindow = sf::Mouse::getPosition(*this->window);
   this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);

    // Update mouse position
    // Relative to the screen
    // std::cout << "Mouse pos: " << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << std::endl;
    // Relative to the window
    // std::cout << "Mouse pos: "
    //           << sf::Mouse::getPosition(*this->window).x << " "
    //           << sf::Mouse::getPosition(*this->window).y << std::endl;
}

void Game::updateText()
{
    std::stringstream ss;

    ss << "Points: " << this->points << std::endl << "Health: " << this->health;

    this->uiText.setString(ss.str());
}

void Game::updateEnemies()
{
    /*
        @return void

        Updates the enemy spawn timer and spawns enemies
        when the total amount of enemies is smaller than the maximum.
        Moves the enemies downwards.
        Removes the nemies at the edge of the screen. // TODO
    */

    // Updating the timer for enemy spawning
    if(this->enemies.size() < this->maxEnemies)
    {
        if(this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            // Spawn the enemy and reset the timer
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else
            this->enemySpawnTimer += 1.f;
    }

    // Move and remove enemies past the bottom
    // for (auto &e : this->enemies)
    for (int i = this->enemies.size() - 1; i >= 0; i--)
    {
        // Move enemies
        this->enemies[i].move(0.f, 1.f);

        // Remove enemies past the bottom
        if (this->enemies[i].getPosition().y >  this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
        }
    }

    // Check if clicked upon
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (!this->mouseHeld)
        {
            // Prevent mouse holding being OP
            this->mouseHeld = true;

            bool deleted = false;
            for (size_t i = 0; i < this->enemies.size() && !deleted; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {
                    // Gain points
                    if (this->enemies[i].getFillColor() == sf::Color::Magenta)
                        this->points += 10;
                    else if (this->enemies[i].getFillColor() == sf::Color::Blue)
                        this->points += 5;
                    else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
                        this->points += 3;
                    else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
                        this->points += 2;
                    else if (this->enemies[i].getFillColor() == sf::Color::Green)
                        this->points += 1;

                    // Delete the clicked enemy
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);
                }
            }
        }
    }
    else
    {
        this->mouseHeld = false;
    }
}

void Game::renderEnemies(sf::RenderTarget& target)
{
    for (auto &e : this->enemies)
    {
        target.draw(e);
    }
}

void Game::update() {
    this->pollEvents();

    if (!this->endGame)
    {
        this->updateMousePositions();

        this->updateText();

        this->updateEnemies();
    }

    // End game condition
    if (this->health <= 0)
        this->endGame = true;
}

void Game::renderText(sf::RenderTarget &target)
{
    target.draw(this->uiText);
}

void Game::render() {
    /*
        @return void

        - clear old frame
        - render objects
        - display frame in window

        Renders the game objects.
    */

    this->window->clear();

    // Draw game objects
    this->renderEnemies(*this->window);

    this->renderText(*this->window);

    this->window->display();
}
