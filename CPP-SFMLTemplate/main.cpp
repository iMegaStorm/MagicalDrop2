//Magical Drop II Game

//Header Files
#include <iostream>
#include <SFML/Graphics.hpp>

//Compiler Directives
using namespace std;
using namespace sf;

//Global Variables
//Game Window Variables...
RenderWindow window (VideoMode(800, 600),"Magical Drop II",Style::Close);

//Clock Variables
Clock MDClock;
int ClockReset = 0; //Reset ingame clock

//Game Variables
bool Row = false; //function to add a new row
int LineLimit = 12, ColumnLimit = 10;
int GameLostLines = 0;
bool GameLost;

//Game Screen Variables
	//Assigns a value for each screen
const int GAME_MENU_SCREEN = 0; //Main game screen
const int INSTRUCTIONS_MENU_SCREEN = 1; //Instructions Menu screen
const int LEVEL_1_SCREEN = 2; // Level 1 of the game
const int GAME_OVER_SCREEN = 3; //Game Over screen
int CURRENT_SCREEN = GAME_MENU_SCREEN; //Using current screen in order to switch to another screen

int JesterX = 0; //X poisiton of the jester
int JesterY = 11; //Y position of the jester
int CurrentValue = -1; //Current value that the jester is holding, -1 at the start so its either 1 of the colour of the balls or -1, this is so you can pick up different colour balls without throwing them first
int HeldBalls = 0; //How many balls the Jester is currently holding

int Score = 0; //ScoreBoard

bool StartGame = false; // starts the game if true, else it stops the game if false



//Method that shows the PlayField values with the location of the player

void drawDebugLayout(int PlayField[12][10], int jesterColumn) {
	for (int i = 0; i < 15; i++)
    {
        cout<<  endl;
    } 

	int i, j;
	for(i=0; i<12; i++)
	{
		for(j=0; j<10; j++)
		{
			if(i!=11)
			{
				cout<<PlayField[i][j]<<' ';
			}
			else if(i==11)
			{
				if(j!= jesterColumn)
				{
					cout<<PlayField[i][j]<<' ';
				}
				else
				{
					cout<<"1"<<' ';
				}
			}
		}
		cout<<'\n';
	}
	cout<<'\n';
}

void RecursivePull (int CurrentY, int CurrentX, int PlayField[12][10])
{
	if (PlayField[CurrentY-1][JesterX] == 0)
	{
		RecursivePull(CurrentY-1, JesterX, PlayField);
	}
	else
	{
		if(PlayField[CurrentY-1][JesterX] ==  CurrentValue || CurrentValue == -1)
		{
			CurrentValue = PlayField[CurrentY-1][JesterX];
			HeldBalls += 1;
			PlayField[CurrentY-1][JesterX] = 0;
			//cout << CurrentValue << " " << HeldBalls << endl;
		}
	}
}

void RecursiveThrow (int CurrentY, int CurrentX, int PlayField[12][10])
{
	if (HeldBalls > 0)
	{
		if (PlayField[CurrentY-1][JesterX] == 0)
		{
			RecursiveThrow(CurrentY-1, JesterX, PlayField);
		}
		else
		{
			PlayField[CurrentY][JesterX] = CurrentValue;
			//cout << PlayField[CurrentY-1][JesterX] << " " << PlayField[CurrentY][JesterX] << endl;
			HeldBalls -= 1;
			RecursiveThrow(CurrentY + 1, JesterX, PlayField);
		}
	}
}

int ValuesHeld;
 void RecursiveCheck (int CurrentY, int CurrentX, int PlayField[12][10])
 {
	 if (PlayField[CurrentY-1][JesterX] == 0)
		{
			RecursiveCheck(CurrentY-1, JesterX, PlayField);
		
		}
	 if (PlayField[CurrentY - 1][JesterX] == CurrentValue)
	 {
		 ValuesHeld++;
		 RecursiveCheck(CurrentY-1, JesterX, PlayField);
	 }
}

 void Delete (int CurrentY, int CurrentX, int PlayField[12][10])
 {
	 if (ValuesHeld >= 3)
	 {
		Score += ValuesHeld*50;
		while (PlayField[CurrentY][JesterX] == 0 || PlayField[CurrentY][JesterX] == CurrentValue)
		{
			PlayField[CurrentY][JesterX] = 0;
			CurrentY--;
		}
		 
	 }
 }
 
void SpawnBall(int BallColour, int BallPositionX, int BallPositionY, Sprite BallColours[4]){
	//Accessing the ball colour
	Sprite BallSprite = BallColours[BallColour - 2];
	BallSprite.setPosition(BallPositionX *50 +150, BallPositionY *50);
	//BallSprite.setPosition(BallPositionX * window.getSize().x / 12, BallPositionY * window.getSize().x / 10);
	//BallSprite.setScale(Vector2f(BallSpriteCurrentScaleX,BallSpriteCurrentScaleY));
	window.draw(BallSprite);
}

void newRow(int PlayField[12][10])
{
	for(int i = LineLimit - 1; i>0; i--)
	{
		for(int j=0; j<ColumnLimit; j++)
		{
			PlayField[i][j] = PlayField[i - 1][j];
		}
	}
	srand(time(NULL));
	for(int i=0; i<ColumnLimit; i++) 
	{
		int RandomNumber;
		do 
		{
			RandomNumber = rand() % 4 + 2;
		}
		while (RandomNumber == 0);
		PlayField[0][i] = RandomNumber;
	}

}

//checking if the game i lost - at least a ball is on the last row
bool CheckGameLost(int PlayField[12][10], int LineLimit, int ColumnLimit)
{
	bool lost = false;
	int i = 0;
	while(!lost && i<ColumnLimit) {
		if (PlayField[LineLimit - 1][i] != 0) {
			lost = true;
		}
		i++;
	}
	return lost;	
}

int main()
{
	//Local Variables
	//Event Variables
	Event event;
	//Sprite Variables
	Texture Background, Jester, BlueBubble, YellowBubble, GreenBubble, RedBubble, GameScreen, Instructions, PlayButton1, ExitButton1, GameOver;
	//Font Variable
	Font ArialFont;
	Text text;
	float ArialCurrentScaleX = 1.2f,ArialCurrentScaleY = 1.2f;
	Vector2f ArialLocation(250,300);
	bool DeathFont = false;
	float Arial2CurrentScaleX = 1.2f,Arial2CurrentScaleY = 1.2f;
	Vector2f Arial2Location(250,300);
	//Background Image Variables
	float BackgroundCurrentScaleX = 1.0f,BackgroundCurrentScaleY = 1.0f;
	float BackgroundWidth,BackgroundHeight;
	Vector2f BackgroundLocation(150,0);
	//Jester Sprite Variables
	float JesterWidth, JesterHeight;
	Vector2f JesterLocation (165, 568);

	Vector2i MouseCursorLocation(0,0);

	//Start, stop and reset Game Variables...
	bool ResetGame = false; // resets the game
	//float BallSpriteCurrentScaleX = 0.8f, BallSpriteCurrentScaleY = 0.8f;
	window.setFramerateLimit(60);

	if (!Background.loadFromFile("Assets/BackGround.png")) //Background image
		{
			cout << "Error 1: Failed to load background image" << "\n";
			system("pause");
		}
	Sprite BackgroundImage(Background);
	BackgroundImage.setScale(Vector2f(BackgroundCurrentScaleX,BackgroundCurrentScaleY));
	BackgroundWidth = BackgroundImage.getGlobalBounds().width;
	BackgroundHeight = BackgroundImage.getGlobalBounds().height;
	BackgroundImage.setPosition(BackgroundLocation.x,BackgroundLocation.y);

	if (!GameScreen.loadFromFile("Assets/GameScreen.png"))
		{
			cout << "Failed to load GameScreen image" << "\n";
			system("pause");
		}
	Sprite GameScreenMenu(GameScreen);

	if (!Instructions.loadFromFile("Assets/InstructionsMenu.png"))
		{
			cout << "Failed to load InstructionsMenu image" << "\n";
			system("pause");
		}
	Sprite InstructionsMenu(Instructions);

	if (!PlayButton1.loadFromFile("Assets/PlayButton.png"))
		{
			cout << "Failed to load PlayButton image" << "\n";
			system("pause");
		}
	Sprite PlayButton(PlayButton1);
	PlayButton.setOrigin(89.0/2,40.0/2);
	PlayButton.setPosition(400, 300);
	
	if (!ExitButton1.loadFromFile("Assets/ExitButton.png"))
		{
			cout << "Failed to load ExitButton image" << "\n";
			system("pause");
		}
	Sprite ExitButton(ExitButton1);
	ExitButton.setOrigin(82.0/2, 4.0/2);
	ExitButton.setPosition(400, 400);

	if (!GameOver.loadFromFile("Assets/GameOver.png"))
		{
			cout << "Failed to load ExitButton image" << "\n";
			system("pause");
		}
	Sprite GameOverMenu (GameOver);


	if (!Jester.loadFromFile("Assets/JesterFront.png")) //The "Main Character" Sprite
		{
			cout << "Error 2: Jester Image Failed To Load" << "\n";
			system("pause");
		}
	Sprite JesterSprite(Jester);
	JesterWidth = JesterSprite.getGlobalBounds().width;
	JesterHeight = JesterSprite.getGlobalBounds().height;
	JesterSprite.setPosition(JesterLocation.x, JesterLocation.y);
	JesterSprite.setOrigin(15.5f, 18.5f);

	if (!BlueBubble.loadFromFile("Assets/BlueBubble.png")) //Loading The "Blue Bubble" Sprite
		{
			cout << "Error 3: BlueBubble Image Failed To Load" << "\n";
			system("pause");
		}
	Sprite BlueBubbleSprite(BlueBubble);

	if (!YellowBubble.loadFromFile("Assets/YellowBubble.png")) //Loading The "Yellow Bubble" Sprite
		{
			cout << "Error 4: YellowBubble Image Failed To Load" << "\n";
			system("pause");
		}
	Sprite YellowBubbleSprite(YellowBubble);

	if (!RedBubble.loadFromFile("Assets/RedBubble.png")) //Loading The "Red Bubble" Sprite
		{
			cout << "Error 3: RedBubble Image Failed To Load" << "\n";
			system("pause");
		}
	Sprite RedBubbleSprite(RedBubble);

	if (!GreenBubble.loadFromFile("Assets/GreenBubble.png")) //Loading The "Green Bubble" Sprite
		{
			cout << "Error 3: GreenBubble Image Failed To Load" << "\n";
			system("pause");
		}
	Sprite GreenBubbleSprite(GreenBubble);

	if (!ArialFont.loadFromFile("Fonts/Arial/arial.ttf"))
		{
			cout << "Your font has failed to load" << "\n";
			system("pause");
		}

	Text Arial;
	Arial.setScale(Vector2f(Arial2CurrentScaleX,Arial2CurrentScaleY));
	Arial.setFont(ArialFont);
	Arial.setPosition(Arial2Location.x, Arial2Location.y);

	Text HighScore;
	HighScore.setCharacterSize(20);
	HighScore.setPosition (650, 10);
	HighScore.setFont(ArialFont);
	HighScore.setString("HighScore: ");

	//Sprite BallSprite(int BallColours);
	Sprite BallColours[4] = {BlueBubbleSprite, RedBubbleSprite, YellowBubbleSprite, GreenBubbleSprite};

	//Array
	srand(time(NULL));
	int PlayField[12][10];
	int i, j;
	for(i = 0; i<LineLimit; i++)
	{
		for(j = 0; j<ColumnLimit; j++)
		{
		if(i < 3)
			{
				PlayField[i][j] = rand() % 4 + 2;
			}
		else
			{
				PlayField[i][j] = 0;
			}
		}
	}

	drawDebugLayout(PlayField, (JesterLocation.x-150)/50);

	while (window.isOpen()) //The Game Window Loop
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) //Checks to see if the window is closed and then executes the code
			{
				window.close(); //closes the window
				break;
			}
			if (event.type == Event::MouseMoved) //Mouse Movement
				{
				MouseCursorLocation = Mouse::getPosition(window);
				}
			else if (event.type == Event::MouseButtonPressed)
				{
				if (event.mouseButton.button == Mouse::Left && CURRENT_SCREEN == GAME_MENU_SCREEN)
					{
					if ((MouseCursorLocation.x > PlayButton.getPosition().x - PlayButton.getGlobalBounds().width/2) && (MouseCursorLocation.x < (PlayButton.getPosition().x + PlayButton.getGlobalBounds().width/2)) && (MouseCursorLocation.y > PlayButton.getPosition().y - PlayButton.getGlobalBounds().height/2) && (MouseCursorLocation.y < (PlayButton.getPosition().y + PlayButton.getGlobalBounds().height/2)))
						{
							CURRENT_SCREEN = INSTRUCTIONS_MENU_SCREEN; //Changes the current screen if you click on the play button
							//cout << CURRENT_SCREEN;
						}
					if ((MouseCursorLocation.x > ExitButton.getPosition().x - ExitButton.getGlobalBounds().width/2) && (MouseCursorLocation.x < (ExitButton.getPosition().x + ExitButton.getGlobalBounds().width/2)) && (MouseCursorLocation.y > ExitButton.getPosition().y - ExitButton.getGlobalBounds().height/2) && (MouseCursorLocation.y < (ExitButton.getPosition().y + ExitButton.getGlobalBounds().height/2)))
						{
							window.close(); //Closes the window if you click on the exit button
						}
					}
				}
			else if (event.type == Event::KeyPressed) //Reading keyboard keys
			{
				if(StartGame && !GameLost)
				{
				if (Keyboard::isKeyPressed(Keyboard::Left)) //Checks for key presses 
					{
						if (JesterLocation.x > 250 - JesterWidth) //Moves left if the jester location is more than 250 on the x axis
							{
								JesterLocation.x = JesterLocation.x - 50;
								JesterX -= 1;
							}
						//cout << JesterX << endl;
						drawDebugLayout(PlayField, (JesterLocation.x-150)/50);
					}
					if(Keyboard::isKeyPressed(Keyboard::Right)) //Checks for key presses
					{
						if(JesterLocation.x < 650 -JesterWidth) //Moves left if the jester location is less than 650 on the x axis
							{
								JesterLocation.x = JesterLocation.x + 50;
								JesterX += 1;
							}
						//cout << JesterX << endl;
						drawDebugLayout(PlayField, (JesterLocation.x-150)/50);
					}
					if(Keyboard::isKeyPressed(Keyboard::Up)) //Checks for key presses
					{
							RecursiveThrow(JesterY, JesterX, PlayField);
							ValuesHeld = 0;
							RecursiveCheck(JesterY, JesterX, PlayField);
							Delete(JesterY, JesterX, PlayField);
							CurrentValue = -1;
					}
					if(Keyboard::isKeyPressed(Keyboard::Down)) //Checks for key presses
					{
						RecursivePull(JesterY, JesterX, PlayField);
					}
				}
				JesterSprite.setPosition(JesterLocation.x, JesterLocation.y);

			if (Keyboard::isKeyPressed(Keyboard::Return)) 
				{
					ResetGame = true;
					StartGame = true;
					MDClock.restart();
				}
			}
		}
		window.clear(Color(150,150,150));

		if (CURRENT_SCREEN == GAME_MENU_SCREEN)
			{
				window.draw(GameScreenMenu);
				window.draw(PlayButton);
				window.draw(ExitButton);
			}
		else if (CURRENT_SCREEN == INSTRUCTIONS_MENU_SCREEN)
			{
				window.draw(InstructionsMenu);
				if (Keyboard::isKeyPressed(Keyboard::Space))
				{
					CURRENT_SCREEN = LEVEL_1_SCREEN;
				}
			}
		else if (CURRENT_SCREEN == LEVEL_1_SCREEN)
			{
				window.draw(BackgroundImage);
				if (StartGame)
				{
					
					//getting the seconds
					int sec = (int) MDClock.getElapsedTime().asSeconds();
					//add a row at every secToAdd seconds
					int secToAdd = 5;
					//adding a row only if the game started
					if(sec % secToAdd == secToAdd - 1 && StartGame && !GameLost)
					{
						newRow(PlayField);
						MDClock.restart();
					}
				}
			for(i = 0; i<LineLimit; i++) 
			{
				for(j = 0; j<ColumnLimit; j++)
				{
					if(PlayField[i][j] != 0)
						{
						SpawnBall (PlayField[i][j], j, i, BallColours);
						}
				}
			}
			//check if at least one ball is on the last row - game ends
			if (CheckGameLost(PlayField, LineLimit, ColumnLimit) && !GameLost)
				{
					GameLostLines = 0;
					GameLost = true;
					CURRENT_SCREEN = GAME_OVER_SCREEN;
				}
			if(!StartGame)
				{
					Arial.setString("Press Enter to start!");
					window.draw(Arial);
				}
				window.draw(JesterSprite);
				HighScore.setString("HighScore: " + to_string(Score));
				window.draw(HighScore);
			}	
		else if (CURRENT_SCREEN == GAME_OVER_SCREEN)
		{
			window.draw(GameOverMenu);
			if (Keyboard::isKeyPressed(Keyboard::Return))
			{
			srand(time(NULL));
			int i, j;
			for(i = 0; i<LineLimit; i++)
			{
				for(j = 0; j<ColumnLimit; j++)
				{
				if(i < 3)
					{
						PlayField[i][j] = rand() % 4 + 2;
					}
				else
					{
						PlayField[i][j] = 0;
					}
				}
			}

			for(i = 0; i<LineLimit; i++) 
			{
				for(j = 0; j<ColumnLimit; j++)
				{
					if(PlayField[i][j] != 0)
						{
						SpawnBall (PlayField[i][j], j, i, BallColours);
						}
				}
			}
				Score = 0;
				GameLost = false;
				StartGame = false;
				CURRENT_SCREEN = LEVEL_1_SCREEN;
			}
		}
		window.display();
	}
		return 0;
}

