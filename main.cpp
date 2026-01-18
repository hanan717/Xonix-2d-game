#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

const int M = 25;
const int N = 40;

int grid[M][N] = {0};
int ts = 18; // tile size
int high = 105;

struct Enemy
{
    float x, y, dx, dy;

    Enemy()
    {
        int gx, gy;
        do
        {
            gx = 2 + rand() % (N - 4);
            gy = 2 + rand() % (M - 4);
        } while (grid[gy][gx] != 0);

        x = gx * ts;
        y = gy * ts;

        dx = rand() % 3 - 1.f;
        dy = rand() % 3 - 1.f;
    }

    void move()
    {
        x += dx;
        if (grid[int(y) / ts][int(x) / ts] == 1)
        {
            dx = -dx;
            x += dx;
        }
        y += dy;
        if (grid[int(y) / ts][int(x) / ts] == 1)
        {
            dy = -dy;
            y += dy;
        }
    }
};

void drop(int y, int x)
{
    if (grid[y][x] == 0)
        grid[y][x] = -1;
    if (grid[y - 1][x] == 0)
        drop(y - 1, x);
    if (grid[y + 1][x] == 0)
        drop(y + 1, x);
    if (grid[y][x - 1] == 0)
        drop(y, x - 1);
    if (grid[y][x + 1] == 0)
        drop(y, x + 1);
}

void Initial_Start_display(RenderWindow &window)
{
    Texture start_T;
    if (!start_T.loadFromFile("images/button.png"))
    {
        cout << "Error: Could not load start texture!" << endl;
        exit(-1);
    }

    Sprite start_S;
    start_S.setTexture(start_T);
    start_S.setScale(1.f, 1.f);
    start_S.setPosition(((N * ts) - start_S.getGlobalBounds().width) / 2.0,
                        ((M * ts + high) - start_S.getGlobalBounds().height) / 2.0);

    Texture backgroundTexture;
    !backgroundTexture.loadFromFile("images/B6.jpg");

    Sprite background(backgroundTexture);
    background.setScale(
        float(window.getSize().x) / background.getTexture()->getSize().x,
        float(window.getSize().y) / background.getTexture()->getSize().y);

    Font font;
    font.loadFromFile("Arial.ttf");

    Text text;
    text.setFont(font);
    text.setString("START");
    text.setStyle(Text::Bold);
    text.setCharacterSize(start_S.getGlobalBounds().height * 0.7f);
    text.setFillColor(Color::White);

    FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0, textBounds.top + textBounds.height / 2.0);
    text.setPosition(start_S.getPosition().x + start_S.getGlobalBounds().width / 2.0,
                     start_S.getPosition().y + start_S.getGlobalBounds().height / 2.0);

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                exit(0);
            }

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                FloatRect range = start_S.getGlobalBounds();
                if (range.contains(e.mouseButton.x, e.mouseButton.y))
                {
                    cout << " START Seleceted\n";
                    return;
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(start_S);
        window.draw(text);
        window.display();
    }
}

int DisplayPlayerMenu(RenderWindow &window)
{
    const string labels[2] = {"1 PLAYER", "2 PLAYER"};
    const int buttonCount = 2;
    const float spacing = 30.f;

    Texture button_T;
    if (!button_T.loadFromFile("images/button.png"))
    {
        cout << "Error: Could not load button texture!" << endl;
        exit(-1);
    }

    Texture backgroundTexture;
    !backgroundTexture.loadFromFile("images/B6.jpg");

    Sprite background(backgroundTexture);
    background.setScale(
        float(window.getSize().x) / background.getTexture()->getSize().x,
        float(window.getSize().y) / background.getTexture()->getSize().y);

    Font font;
    font.loadFromFile("Arial.ttf");

    Sprite buttons[buttonCount];
    Text texts[buttonCount];

    Vector2f scale(1.5f, 1.f);
    float buttonWidth = button_T.getSize().x * scale.x;
    float buttonHeight = button_T.getSize().y * scale.y;
    float totalHeight = buttonCount * buttonHeight + (buttonCount - 1) * spacing;

    float startX = (window.getSize().x - buttonWidth) / 2.0f;
    float startY = (window.getSize().y - totalHeight) / 2.0f;

    for (int i = 0; i < buttonCount; ++i)
    {
        buttons[i].setTexture(button_T);
        buttons[i].setScale(scale);
        buttons[i].setPosition(startX, startY + i * (buttonHeight + spacing));

        texts[i].setFont(font);
        texts[i].setString(labels[i]);
        texts[i].setStyle(Text::Bold);
        texts[i].setCharacterSize(buttonHeight * 0.5f);
        texts[i].setFillColor(Color::White);

        FloatRect textBounds = texts[i].getLocalBounds();
        texts[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        texts[i].setPosition(buttons[i].getPosition().x + buttonWidth / 2.0f,
                             buttons[i].getPosition().y + buttonHeight / 2.0f);
    }

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                exit(0);
            }

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                for (int i = 0; i < buttonCount; ++i)
                {
                    if (buttons[i].getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
                    {
                        if (labels[i] == "1 PLAYER")
                        {
                            cout << "1 Player Selected\n";
                            return 1;
                        }
                        if (labels[i] == "2 PLAYER")
                        {
                            cout << "2 Player Selected\n";
                            return 2;
                        }
                    }
                }
            }
        }

        window.clear(Color::Black);

        window.draw(background);
        for (int i = 0; i < buttonCount; ++i)
        {
            window.draw(buttons[i]);
            window.draw(texts[i]);
        }
        window.display();
    }

    return -1;
}

int DisplayDifficultyMenu(RenderWindow &window)
{
    const string labels[4] = {"EASY", "MEDIUM", "HARD", "CONTINUES"};
    const int buttonCount = 4;
    const float spacing = 20.f;

    Texture button_T;
    if (!button_T.loadFromFile("images/button.png"))
    {
        cout << "Error: Could not load button texture!" << endl;
        exit(-1);
    }

    Texture backgroundTexture;
    !backgroundTexture.loadFromFile("images/B6.jpg");

    Sprite background(backgroundTexture);
    background.setScale(
        float(window.getSize().x) / background.getTexture()->getSize().x,
        float(window.getSize().y) / background.getTexture()->getSize().y);

    Font font;
    font.loadFromFile("Arial.ttf");

    Sprite buttons[buttonCount];
    Text texts[buttonCount];

    Vector2f scale(1.5f, 1.f);
    float buttonWidth = button_T.getSize().x * scale.x;
    float buttonHeight = button_T.getSize().y * scale.y;
    float totalHeight = buttonCount * buttonHeight + (buttonCount - 1) * spacing;

    float startX = ((N * ts) - buttonWidth) / 2.0f;
    float startY = ((M * ts) - totalHeight) / 2.0f;

    for (int i = 0; i < buttonCount; ++i)
    {
        buttons[i].setTexture(button_T);
        buttons[i].setScale(scale);
        buttons[i].setPosition(startX, startY + i * (buttonHeight + spacing) + high / 2.0);

        texts[i].setFont(font);
        texts[i].setString(labels[i]);
        texts[i].setStyle(Text::Bold);
        texts[i].setCharacterSize(buttonHeight * 0.5f);
        texts[i].setFillColor(Color::White);

        FloatRect textBounds = texts[i].getLocalBounds();
        texts[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        texts[i].setPosition(buttons[i].getPosition().x + buttonWidth / 2.0f,
                             buttons[i].getPosition().y + buttonHeight / 2.0f);
    }

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                exit(0);
            }

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                for (int i = 0; i < buttonCount; ++i)
                {
                    if (buttons[i].getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
                    {
                        if (labels[i] == "EASY")
                        {
                            cout << "Easy Selected\n";

                            return 1;
                        }

                        if (labels[i] == "MEDIUM")
                        {
                            cout << "Medium Selected\n";
                            return 2;
                        }
                        if (labels[i] == "HARD")
                        {
                            cout << "Hard Selected\n";

                            return 3;
                        }
                        if (labels[i] == "CONTINUES")
                        {
                            cout << "Continues Selected\n";

                            return 4;
                        }
                    }
                }
            }
        }

        window.clear(Color::Black);

        window.draw(background);
        for (int i = 0; i < buttonCount; ++i)
        {
            window.draw(buttons[i]);
            window.draw(texts[i]);
        }
        window.display();
    }

    return -1;
}

int DisplayExitMenu(RenderWindow &window, int score1, int score2, int time)
{
    const string labels[3] = {"EXIT", "MAIN MENU","RETRY"};
    const int buttonCount = 3;
    const float spacing = 20.f;

    Texture button_T;
    if (!button_T.loadFromFile("images/button.png"))
    {
        cout << "Error: Could not load button texture!" << endl;
        exit(-1);
    }

    Texture backgroundTexture;
    !backgroundTexture.loadFromFile("images/B6.jpg");

    Sprite background(backgroundTexture);
    background.setScale(
        float(window.getSize().x) / background.getTexture()->getSize().x,
        float(window.getSize().y) / background.getTexture()->getSize().y);

    Font font;
    font.loadFromFile("Arial.ttf");

    Sprite buttons[buttonCount];
    Text texts[buttonCount];
    Text scoreText, winnerText, timeText;

    Vector2f scale(1.1f, 0.7f);
    float buttonWidth = button_T.getSize().x * scale.x;
    float buttonHeight = button_T.getSize().y * scale.y;
    float totalHeight = buttonCount * buttonHeight + (buttonCount - 1) * spacing;

    float startX = ((N * ts) - buttonWidth) / 2.0f;
    float startY = ((M * ts) - totalHeight) / 2.0f + high / 2.0f;

    for (int i = 0; i < buttonCount; ++i)
    {
        buttons[i].setTexture(button_T);
        buttons[i].setScale(scale);
        buttons[i].setPosition(startX, startY + i * (buttonHeight + spacing) + high / 2.0f);

        texts[i].setFont(font);
        texts[i].setString(labels[i]);
        texts[i].setStyle(Text::Bold);
        texts[i].setCharacterSize(buttonHeight * 0.5f);

        FloatRect textBounds = texts[i].getLocalBounds();
        texts[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        texts[i].setPosition(buttons[i].getPosition().x + buttonWidth / 2.0f,
                             buttons[i].getPosition().y + buttonHeight / 2.0f);
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::White);
    scoreText.setStyle(Text::Bold);
    scoreText.setString("Player 1: " + to_string(score1) + "    Player 2: " + to_string(score2));
    FloatRect sb = scoreText.getLocalBounds();
    scoreText.setOrigin(sb.left + sb.width / 2.0f, sb.top + sb.height / 2.0f);
    scoreText.setPosition(N * ts / 2.0f, 100.f + high / 2.0f);

    int minutes = time / 60;
    int seconds = time % 60;

    timeText.setFont(font);
    timeText.setCharacterSize(28);
    timeText.setFillColor(Color::Cyan);
    timeText.setStyle(Text::Bold);
    timeText.setString("Time Played: " + to_string(minutes) + " min " + to_string(seconds) + " sec");
    FloatRect tb = timeText.getLocalBounds();
    timeText.setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
    timeText.setPosition(N * ts / 2.0f, 140.f + high / 2.0f);

    winnerText.setFont(font);
    winnerText.setCharacterSize(40);
    winnerText.setStyle(Text::Bold);

    if (score1 > score2)
    {
        winnerText.setString("PLAYER 1 WINS!");
        winnerText.setFillColor(Color::Green);
    }
    else if (score2 > score1)
    {
        winnerText.setString("PLAYER 2 WINS!");
        winnerText.setFillColor(Color::Blue);
    }
    else
    {
        winnerText.setString("IT'S A DRAW!");
        winnerText.setFillColor(Color::Yellow);
    }

    FloatRect wb = winnerText.getLocalBounds();
    winnerText.setOrigin(wb.left + wb.width / 2.0f, wb.top + wb.height / 2.0f);
    winnerText.setPosition(N * ts / 2.0f, 40.f + high / 2.0f);

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                exit(0);

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                for (int i = 0; i < buttonCount; ++i)
                {
                    if (buttons[i].getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
                    {
                        if (labels[i] == "EXIT")
                            return 0;
                        if (labels[i] == "MAIN MENU")
                            return 1;
                        if(labels[i] == "RETRY")
                            return 2;
                    }
                }
            }
        }

        window.clear(Color::Black);
        window.draw(background);
        window.draw(winnerText);
        window.draw(scoreText);
        window.draw(timeText);
        for (int i = 0; i < buttonCount; ++i)
        {
            window.draw(buttons[i]);
            window.draw(texts[i]);
        }
        window.display();
    }

    return -1;
}

const int MAX_ENTRIES = 5;

void UpdateScoreboard(int playerNumber, int score, int timeSeconds)
{
    int scores[MAX_ENTRIES + 1] = {0};
    int times[MAX_ENTRIES + 1] = {0};
    string players[MAX_ENTRIES + 1];
    int count = 0;

    ifstream infile("scoreboard.txt");
    if (infile.is_open())
    {
        string label;
        infile >> label >> label >> label;
        while (infile >> players[count] >> label >> scores[count] >> label >> times[count])
        {
            count++;
            if (count == MAX_ENTRIES)
                break;
        }
        infile.close();
    }

    scores[count] = score;
    times[count] = timeSeconds;
    players[count] = "Player[" + to_string(playerNumber) + "]";
    count++;

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (scores[j] > scores[i] || (scores[j] == scores[i] && times[j] < times[i]))
            {
                swap(scores[i], scores[j]);
                swap(times[i], times[j]);
                swap(players[i], players[j]);
            }
        }
    }

    if (count > MAX_ENTRIES)
        count = MAX_ENTRIES;

    ofstream outfile("scoreboard.txt");
    outfile << "=== SCOREBOARD ===\n";
    for (int i = 0; i < count; i++)
        outfile << players[i] << " Score: " << scores[i] << " Time: " << times[i] << "\n";
}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(N * ts, M * ts + high), "Xonix Game!");
    window.setFramerateLimit(60);

    // Start Button
    Initial_Start_display(window);

MainMenu:
    // setting all the grid to 0
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = 0;

    // Display Buttons for ( 1-Player OR 2-Player )
    int player_1_or_2 = DisplayPlayerMenu(window);

    // Display Buttons for ( 4 types of Level Difficulity )
    int Level = DisplayDifficultyMenu(window);

    // Reloading pictures
    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    // Backgroung Picture
    Texture backgroundTexture;
    !backgroundTexture.loadFromFile("images/B7.jpg");
    Sprite background(backgroundTexture);
    background.setScale(
        float(window.getSize().x) / background.getTexture()->getSize().x,
        float(window.getSize().y) / background.getTexture()->getSize().y);

    // Backgroung Music
    Music music;
    if (!music.openFromFile("Music.ogg"))
        return -1;
    music.setLoop(true);
    music.play();

    // Sound Effect
    SoundBuffer SB;
    if (!SB.loadFromFile("Effect.wav"))
        return -1;
    Sound E1, E2;
    E1.setBuffer(SB);
    E2.setBuffer(SB);

    // Text Font
    Font font;
    font.loadFromFile("Arial.ttf");
    // Text for Score
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(25);
    scoreText.setFillColor(Color::White);
    scoreText.setStyle(Text::Bold);
    // Text for No of Moves
    Text moveText;
    moveText.setFont(font);
    moveText.setCharacterSize(18);
    moveText.setFillColor(Color::White);
    // Tet for Tiles count
    Text TileText;
    TileText.setFont(font);
    TileText.setCharacterSize(18);
    TileText.setFillColor(Color::White);

    Text PowerUpText;
    PowerUpText.setFont(font);
    PowerUpText.setCharacterSize(18);
    PowerUpText.setFillColor(Color::White);

    // Loading Spirits from Texture of Pictures loading previously
    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    // Enemy dynamic Array
    Enemy *a = nullptr;
    int enemyCount = Level * 2;
    if (Level == 4)
        enemyCount = 2;
    a = new Enemy[enemyCount];

    // Game = false if Game Over
    
    
    bool Game = true;

    // x1, y1 position of player 1
    // dx1, dy1 velocity of player 1
    int x1 = 0, y1 = 0, dx1 = 0, dy1 = 0;

    // x2, y2 position of player 2
    // dx2, dy2 velocity of player 2
    int x2 = N - 1, y2 = M - 1, dx2 = 0, dy2 = 0;

    int prex1 = 0, prey1 = 0;
    int prex2 = 0, prey2 = 0;

    // timer is used for calulating time change
    // delay is used to delay players movement for 0.07 seconds
    // enemyIncreaseTimer is used to incease speed of enemies after 20 seconds
    float timer = 0, delay = 0.07, enemyIncreaseTimer = 0;

    // Clock is used to claculate time
    Clock clock;

    // gameClock is used to calculate to game time to be later displyed in exit menu
    Clock gameClock;
    // gameTime is used to store thst time in seconds
    int gameTime = 0;

    // These are Scores , Moves counter
    int player1Score = 0, player2Score = 0;
    int movesCount1 = 0, movesCount2 = 0;
    int TilesCount1 = 0, TilesCount2 = 0;
    int PowerUpCount1 = 0, PowerUpCount2 = 0;
    bool PowerUpUsed1 = false, PowerUpUsed2 = false;
    float Enemy_Stop_Timer = 0;

    // Treshold is used for tiles score as said in pdf
    int threshold1 = 10, threshold2 = 10;
    // occurrence counter
    int occurrences1 = 0, occurrences2 = 0;

    // Meaning of Values of Grid
    // 0 = Empty
    // 1 = Blue Block
    // 2 = Green Block (PLayer_1's Tail)
    // 3 = Orage Block (PLayer_2's Tail)

    // Setting the boudary (1) of grid to close it
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;

    // Main Game Loop
  
    while (window.isOpen())
    {
        // calculating difference in time
        float deltaTime = clock.getElapsedTime().asSeconds();
        clock.restart();

        // updating the timer counter
        timer += deltaTime;              // for 0.07 seconds delay
        enemyIncreaseTimer += deltaTime; // for 20 seconds delay

        // calculating the total Game Time
        gameTime = gameClock.getElapsedTime().asSeconds();

        // Event is used to get input from keyboard or Mouse
        Event e;
        while (window.pollEvent(e))
        {
            // if 'X' Cross Botton in Top Right of windows is pressed in window
            if (e.type == Event::Closed)
                window.close();

            // if 'Esccape' key is pressed
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Escape)
                {
                    for (int i = 1; i < M - 1; i++)
                        for (int j = 1; j < N - 1; j++)
                            grid[i][j] = 0;

                    x1 = 0;
                    y1 = 0;
                    x2 = N - 1;
                    y2 = M - 1;
                    Game = true;
                }
        }

        // Player 1 Controlls
        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            dx1 = -1;
            dy1 = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            dx1 = 1;
            dy1 = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            dx1 = 0;
            dy1 = -1;
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            dx1 = 0;
            dy1 = 1;
        }
        // Player 1 pressing Q
        if (Keyboard::isKeyPressed(Keyboard::Q))
        {
            if (PowerUpCount1 > 0 && Enemy_Stop_Timer <= 0)
            {
                PowerUpCount1--;
                Enemy_Stop_Timer = 3.0f; // start 3 seconds
            }
        }

        if (player_1_or_2 == 2)
        {

            // Player 2 Controlls
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                dx2 = -1;
                dy2 = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                dx2 = 1;
                dy2 = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                dx2 = 0;
                dy2 = -1;
            }
            if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                dx2 = 0;
                dy2 = 1;
            }

            // Player 2 pressing P
            if (Keyboard::isKeyPressed(Keyboard::P))
            {
                if (PowerUpCount2 > 0 && Enemy_Stop_Timer <= 0)
                {
                    PowerUpCount2--;
                    Enemy_Stop_Timer = 3.0f; // freeze for 3 seconds
                }
            }
        }

        {
            static int nextPowerUpScore1 = 50;

            while (player1Score >= nextPowerUpScore1)
            {
                PowerUpCount1++;
                if (nextPowerUpScore1 == 50)
                    nextPowerUpScore1 = 70;
                else
                    nextPowerUpScore1 += 30;
            }
        }

        if (player_1_or_2 == 2)
        {
            static int nextPowerUpScore2 = 50;

            while (player2Score >= nextPowerUpScore2)
            {
                PowerUpCount2++;
                if (nextPowerUpScore2 == 50)
                    nextPowerUpScore2 = 70;
                else
                    nextPowerUpScore2 += 30;
            }
        }

        // 20 second timer
        // if will be used after every 20 seconds
        if (enemyIncreaseTimer >= 20.f)
        {
            if (Level == 4)
            {
                // if Level is Continues
                // then enmey Count will be incresed by 1 after every 20 seconds
                // and the sie of dynamic array wiil be incesed by 1
                Enemy *temp = new Enemy[enemyCount + 1];
                for (int i = 0; i < enemyCount; i++)
                    temp[i] = a[i];
                delete[] a;
                a = temp;
                enemyCount++;
            }

            // incresing the speed of enemies by 1.2 factor
            const float speedFactor = 1.2f;

            for (int i = 0; i < enemyCount; i++)
            {
                a[i].dx *= speedFactor;
                a[i].dy *= speedFactor;
            }
            enemyIncreaseTimer = 0;
        }

        // 0.07 second timer
        // if will be used after every 0.07 seconds
        if (timer > delay)
        {
            // Storing prevoius position of playe 1 nad player 2
            prex1 = x1;
            prey1 = y1;

            prex2 = x2;
            prey2 = y2;

            // changing position of player 1
            x1 += dx1;
            y1 += dy1;

            // changing position of player 2
            x2 += dx2;
            y2 += dy2;

            // if player 2 moves out of bounderies
            if (x1 < 0)
                x1 = 0;
            if (x1 > N - 1)
                x1 = N - 1;
            if (y1 < 0)
                y1 = 0;
            if (y1 > M - 1)
                y1 = M - 1;

            // if player 2 moves out of boundeies
            if (x2 < 0)
                x2 = 0;
            if (x2 > N - 1)
                x2 = N - 1;
            if (y2 < 0)
                y2 = 0;
            if (y2 > M - 1)
                y2 = M - 1;

            // if player1 comes at it own tail or at the tail of other player ( GAME OVER )
            if (grid[y1][x1] == 2 || grid[y1][x1] == 3)
                Game = false;

            // if player 1 comes at empty space it will be converted to 2 ( Green Tile)
            if (grid[y1][x1] == 0)
            {
                grid[y1][x1] = 2;
            }

            // if player2 comes at it own tail or at the tail of other player ( GAME OVER )
            if (grid[y2][x2] == 3 || grid[y2][x2] == 2)
                Game = false;

            // if player 2 comes at empty space it will be converted to 3 ( Orange Tile )
            if (grid[y2][x2] == 0)
            {
                grid[y2][x2] = 3;
            }

            if (grid[y1][x1] == 1)
            {
                // spped is agian zero
                dx1 = dy1 = 0;

                // to fill the empty tiles with blue tiles
                if (grid[prey1][prex1] == 2)
                {

                    // drop function is used to fill empty space with blue tiles
                    for (int i = 0; i < enemyCount; i++)
                        drop(a[i].y / ts, a[i].x / ts);

                    // couter to count the number of  empty tiles filled as blue tiles
                    int count = 0;

                    // filling the empty tiles (with no enemy inside them ( not deleared as -1) ) with blue tiles
                    for (int i = 0; i < M; i++)
                        for (int j = 0; j < N; j++)
                            if (grid[i][j] == -1)
                                grid[i][j] = 0;
                            else if (grid[i][j] == 0 || grid[i][j] == 2)
                            {
                                grid[i][j] = 1;
                                count++;
                            }

                    // for bonus score if the player has fiiled more then 10 tiles
                    if (count >= threshold1)
                    {
                        // occureces is used to count the number of times player has filled the empty tiles
                        if (occurrences1 > 5)
                        {
                            player1Score += 4 * count; // increasing score by factor 4
                        }
                        if (occurrences1 > 3)
                        {
                            player1Score += 2 * count; // increasing score by factor 2
                            threshold1 = 5;            // making treshold 5
                        }
                        else
                        {
                            player1Score += 2 * count; // increasing score by factor 2
                        }

                        occurrences1++; // increment occurences
                    }
                    else
                    {
                        player1Score += count; // simple score
                    }

                    if (count > 1)
                    {
                        // counting tiles filled by player 1
                        TilesCount1 += count;

                        // prodeces sound effect when the empty tiles are filled with blue tiles
                        E1.play();

                        // counter of moves
                        movesCount1++;
                    }
                }
            }

            // if player 2 comes back at the blue boxs
            // then he will fill the empty tiles between its tail

            if (player_1_or_2 == 2)
            {

                if (grid[y2][x2] == 1)
                {
                    // spped is agian zero
                    dx2 = dy2 = 0;

                    // to fill the empty tile with blue tiles
                    if (grid[prey2][prex2] == 3)
                    {

                        // drop function is used to fill empty space with blue tiles
                        for (int i = 0; i < enemyCount; i++)
                            drop(a[i].y / ts, a[i].x / ts);

                        // couter to count the number of  empty tiles filled as blue tiles
                        int count = 0;

                        // filling the empty tiles (with no enemy inside them ( not deleared as -1) ) with blue tiles
                        for (int i = 0; i < M; i++)
                            for (int j = 0; j < N; j++)
                                if (grid[i][j] == -1)
                                    grid[i][j] = 0;
                                else if (grid[i][j] == 0 || grid[i][j] == 3)
                                {
                                    grid[i][j] = 1;
                                    count++;
                                }

                        // for bonus score if the player has fiiled more then 10 tiles
                        if (count >= threshold2)
                        {

                            // occureces is used to count the number of times player has filled the empty tiles
                            if (occurrences2 > 5)
                            {
                                player2Score += 4 * count; // increasing score by factor 4
                            }
                            if (occurrences2 > 3)
                            {
                                player2Score += 2 * count; // increasing score by factor 2
                                threshold2 = 5;            // making treshold 5
                            }
                            else
                            {
                                player2Score += 2 * count; // increasing score by factor 2
                            }

                            occurrences2++; // increment occurences
                        }
                        else
                        {
                            player2Score += count; // simple score
                        }

                        if (count > 1)
                        {

                            // counting tiles filled by player 2
                            TilesCount2 += count;

                            // prodeces sound effect when the empty tiles are filled with blue tiles
                            E2.play();

                            // counter of moves
                            movesCount2++;
                        }
                    }
                }
            }

            // reset timer
            timer = 0;
        }

        // Decrement timer
        if (Enemy_Stop_Timer > 0)
        {
            Enemy_Stop_Timer -= deltaTime;
            if (Enemy_Stop_Timer < 0)
                Enemy_Stop_Timer = 0;
        }

        // Move enemies only if not frozen
        if (Enemy_Stop_Timer <= 0)
        {
            // moving (changing poistion) of every enimes
            for (int i = 0; i < enemyCount; i++)
                a[i].move();
        }

        // if player 1 comes back at the blue boxs
        // then he will fill the empty tiles between its tail

        // if anyone of the enimes touches player 1 or player 2 Tail
        // Game Over
        for (int i = 0; i < enemyCount; i++)
            if (grid[int(a[i].y) / ts][int(a[i].x) / ts] == 2 || grid[int(a[i].y) / ts][int(a[i].x) / ts] == 3)
                Game = false;

        // Erasing prevoiusly drawn objects
        window.clear();

        // drawing background
        window.draw(background);

        // diplaying tiles
        // Meaning of Values of Grid
        // 0 = Empty
        // 1 = Blue Block
        // 2 = Green Block (PLayer_1's Tail)
        // 3 = Orage Block (PLayer_2's Tail)

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (grid[i][j] == 0)
                    continue;
                if (grid[i][j] == 1)
                    sTile.setTextureRect(IntRect(0, 0, ts, ts));
                if (grid[i][j] == 2)
                    sTile.setTextureRect(IntRect(3 * ts, 0, ts, ts));
                if (grid[i][j] == 3)
                    sTile.setTextureRect(IntRect(4 * ts, 0, ts, ts));
                sTile.setPosition(j * ts, i * ts + high);
                window.draw(sTile);
            }

        // Drawing Red tile for the position of Player 1
        sTile.setTextureRect(IntRect(2 * ts, 0, ts, ts));
        sTile.setPosition(x1 * ts, y1 * ts + high);
        window.draw(sTile);

        if (player_1_or_2 == 2)
        {
            // Drawing Orange tile for the position of Player 2
            sTile.setTextureRect(IntRect(6 * ts, 0, ts, ts));
            sTile.setPosition(x2 * ts, y2 * ts + high);
            window.draw(sTile);
        }

        // enmey rotaiting by the factor 10
        sEnemy.rotate(10);

        // Drawing enimies
        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(a[i].x, a[i].y + high);
            window.draw(sEnemy);
        }

        // drawing Scores and Movement
        {
            FloatRect sb;
            string TextString;
            {
                TextString = "Player 1: " + to_string(player1Score);

                if (player_1_or_2 == 2)
                    TextString += " - Player 2: " + to_string(player2Score);

                scoreText.setString(TextString);
                FloatRect sb = scoreText.getLocalBounds();
                scoreText.setOrigin(sb.left + sb.width / 2.0f, sb.top + sb.height / 2.0f);
                scoreText.setPosition(N * ts / 2.0f, 15.f);
            }
            {
                TextString = "P1 Moves: " + to_string(movesCount1);

                if (player_1_or_2 == 2)
                    TextString += " - P2 Moves: " + to_string(movesCount2);

                moveText.setString(TextString);
                sb = moveText.getLocalBounds();
                moveText.setOrigin(sb.left + sb.width / 2.0f, sb.top + sb.height / 2.0f);
                moveText.setPosition(N * ts / 2.0f, 39.f);
            }
            {
                TextString = "P1 Tiles: " + to_string(TilesCount1);

                if (player_1_or_2 == 2)
                    TextString += " - P2 Tiles: " + to_string(TilesCount2);

                TileText.setString(TextString);
                sb = TileText.getLocalBounds();
                TileText.setOrigin(sb.left + sb.width / 2.0f, sb.top + sb.height / 2.0f);
                TileText.setPosition(N * ts / 2.0f, 64.f);
            }
            {
                TextString = "P1 PowerUps [Q]: " + to_string(PowerUpCount1);

                if (player_1_or_2 == 2)
                    TextString += " - P2 PowerUps [P]: " + to_string(PowerUpCount2);

                PowerUpText.setString(TextString);
                sb = PowerUpText.getLocalBounds();
                PowerUpText.setOrigin(sb.left + sb.width / 2.0f, sb.top + sb.height / 2.0f);
                PowerUpText.setPosition(N * ts / 2.0f, 90.f);
            }
        }

        window.draw(scoreText);
        window.draw(moveText);
        window.draw(TileText);
        window.draw(PowerUpText);

        // Game Over
        if (!Game)
            break;

        // diplay window
        window.display();
    }

    // filling of top 5 scores
    UpdateScoreboard(1, player1Score, gameTime);
    if (player_1_or_2 == 2)
        UpdateScoreboard(2, player2Score, gameTime);

    // pause music
    music.pause();

    // Exit Menu
    if (DisplayExitMenu(window, player1Score, player2Score, gameTime) == 1)
        goto MainMenu;
  
    delete[] a;
    return 0;
}
