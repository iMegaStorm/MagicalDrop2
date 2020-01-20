//Magical Drop II Game

//Header Files
#include <iostream>
#include <SFML/Graphics.hpp>

//Compiler Directives
using namespace std;
using namespace sf;

//Global Variables
//Game Window Variables...
Vector2f window_CurrentDimensions(500,600);
Vector2f window_PerspectiveDimensions(500,600);
RenderWindow window(VideoMode(window_CurrentDimensions.x,window_CurrentDimensions.y),"Magical Drop II",Style::Titlebar);

void SpawnBall(int BallColour, int BallPositionX, int BallPositionY, Sprite BallColours[4]){
	//Accessing the ball colour
	Sprite BallSprite = BallColours[BallColour - 2];
	BallSprite.setPosition(BallPositionX *50, BallPositionY *50);
	//BallSprite.setPosition(BallPositionX * window.getSize().x / 12, BallPositionY * window.getSize().x / 10);
	//BallSprite.setScale(Vector2f(BallSpriteCurrentScaleX,BallSpriteCurrentScaleY));
	window.draw(BallSprite);
}

int main()
{
	//Local Variables
	//Event Variables
	Event event;
	//Sprite Variables
	Texture Background, Jester, BlueBubble, YellowBubble, GreenBubble, RedBubble;
	//Font Variable
	Font ArialFont;
	Text text;
	float ArialCurrentScaleX = 1.2f,ArialCurrentScaleY = 1.2f;
	Vector2f ArialLocation(100,300);
	bool DeathFont = false;
	float Arial2CurrentScaleX = 1.2f,Arial2CurrentScaleY = 1.2f;
	Vector2f Arial2Location(100,300);
	//Background Image Variables
	float BackgroundCurrentScaleX = 1.0f,BackgroundCurrentScaleY = 1.0f;
	float BackgroundWidth,BackgroundHeight;
	Vector2f BackgroundLocation(0,0);
	//Jester Sprite Variables
	float JesterWidth, JesterHeight;
	Vector2f JesterLocation (15,568);
	bool JesterKilled = false;
	//Start/Stop Game Variables...
	bool ResetGame = false;
	bool StartGame = false;
	//Ball Sprite
	float BallSpriteSpeed = 0.5f;
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
	sf::Text Arial;
	Arial.setScale(sf::Vector2f(ArialCurrentScaleX,ArialCurrentScaleY));
	Arial.setFont(ArialFont);
	Arial.setPosition(ArialLocation.x, ArialLocation.y);

	sf::Text Arial2;
	Arial2.setScale(sf::Vector2f(Arial2CurrentScaleX,Arial2CurrentScaleY));
	Arial2.setFont(ArialFont);
	Arial2.setPosition(Arial2Location.x, Arial2Location.y);
	
	//Sprite BallSprite(int BallColours);
	Sprite BallColours[4] = {BlueBubbleSprite, RedBubbleSprite, YellowBubbleSprite, GreenBubbleSprite};

	//Array
	srand(time(NULL));
	int PlayField[12][10], i, j;
	for(i = 0; i<12; i++) 
	{
		for(j = 0; j<10; j++)
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
				if(j != JesterLocation.y) 
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



















	while (window.isOpen()) //The Game Window Loop
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
			window.close();
			}
			else if (event.type == Event::KeyPressed) //Reading keyboard keys
			{
				if(ResetGame)
				{
				if (event.key.code == 71) //Enum SFML Input Manager's Code
					{
						if (JesterLocation.x >= 32)
							{
							JesterLocation.x = JesterLocation.x - 50;
							}
					}
					if(event.key.code == 72)
					{
						if(JesterLocation.x < 465)
							{
							JesterLocation.x = JesterLocation.x + 50;
							}
					}
				}
					JesterSprite.setPosition(JesterLocation.x, JesterLocation.y);
			if (event.key.code == 58) //This Is NOT The ASCII Code of The Key Pressed, But The "Enum" SFML Input Manager's Code...
				{
				ResetGame = true;
				StartGame = true;
				}	
			}
		}
					window.clear();
					window.draw(BackgroundImage);
					window.draw(JesterSprite);
		
					if(JesterKilled) //If the Jester dies then do the following.
					{	
						Vector2f JesterLocation (250,560);
						//JesterSprite.setPosition(JesterLocation.x,JesterLocation.y);
						DeathFont = true;
						JesterKilled = false;
						ResetGame = false;
					} 

					if(!StartGame)
						{
						window.draw(Arial2);
						Arial2.setString("Press Enter to start!");
						}
					if(!ResetGame && DeathFont)
						{
						window.draw(Arial);
						Arial.setString("    GAME OVER! \nHit enter to restart!");
						}

			for(i = 0; i<12; i++) 
			{
				for(j = 0; j<10; j++) 
				{
					if(PlayField[i][j] != 0) 
						{
						SpawnBall (PlayField[i][j], j, i, BallColours);
						}
				}
			}
					window.display();
	}
		return 0;	
}

