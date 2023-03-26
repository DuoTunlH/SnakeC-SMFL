#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <string>
using namespace std;
using namespace sf;

int N=15,M=15;
int size=32;
int w = size*N;
int h = size*M;
char direction = 'R';
int bodyParts = 2;
bool running = true; 
bool firstRun = true;
int score = 0;
Font font;

struct Snake { 
	int x,y;
}  s[100];

struct Fruit { 
	int x,y;
} f;

void newApple() {
	f.x=rand()%N;
	f.y=rand()%M;
}
void checkApple() {
	if ((s[0].x==f.x) && (s[0].y==f.y)) {
		bodyParts++;
		score++;
		newApple();
	}
}

void checkCollisions() {
	for (int i=1;i<bodyParts;i++) {
    	if (s[0].x==s[i].x && s[0].y==s[i].y)  
			running = false;
	}
	if (s[0].x>=N) running = false;
	if (s[0].x<0) running = false;
    if (s[0].y>=M) running = false;
	if (s[0].y<0) running = false;
}

void move() {
	for (int i=bodyParts;i>0;i--) {
		s[i].x=s[i-1].x; 
		s[i].y=s[i-1].y;
	}

    if (direction=='D') s[0].y+=1;      
    if (direction=='L') s[0].x-=1;        
    if (direction=='R') s[0].x+=1;         
    if (direction=='U') s[0].y-=1; 
}
void changeDirection() {
    if (Keyboard::isKeyPressed(Keyboard::Left)&&direction!='R') 
		direction='L';   
    if (Keyboard::isKeyPressed(Keyboard::Right)&&direction!='L') 
		direction='R';
    if (Keyboard::isKeyPressed(Keyboard::Up)&&direction!='D') 
		direction='U';
    if (Keyboard::isKeyPressed(Keyboard::Down)&&direction!='U') 
		direction='D';
}

void restart() {
	s[0].x = M/2;
	s[0].y = N/2;	
	score = 0;
	direction = 'R';
	bodyParts = 2;
	newApple();
	running = true;
}

Text startText() {
	Text StartText;
    StartText.setFont(font);
    StartText.setString("Press space to start");
    StartText.setCharacterSize(50);
    StartText.setFillColor(sf::Color::White);
    StartText.setPosition(w/2 - 50*4 ,h/2 - 50);
    return StartText;
}

Text gameOverText() {
	Text GameOver;
    GameOver.setFont(font);
    GameOver.setString("Game Over \nYour score is " + to_string(score) +"\nPress space to restart");
    GameOver.setCharacterSize(50);
    GameOver.setFillColor(sf::Color::White);
    GameOver.setPosition(w/2 - 50*4 ,h/2 - 100);
    return GameOver;
}

int main() {  
 
    srand(time(0));

    RenderWindow window(VideoMode(w, h), "Snake Game!");
    
    s[0].x = M/2;
	s[0].y = N/2;
	
    Texture snakeTexture,appleTexture;
   		snakeTexture.loadFromFile("resources/snake.png");
		appleTexture.loadFromFile("resources/apple.png");
    Sprite snakeSprite(snakeTexture);
    	snakeSprite.setScale(2,2);
    Sprite appleSprite(appleTexture);
    	appleSprite.setScale(2,2);

    Clock clock;
    float timer=0, delay=0.1;
	
   	newApple(); 

    font.loadFromFile("resources/times.ttf");
    
    Event e;
	//show start screen if it is the first run
   	while(firstRun) {	
		window.clear();
		window.draw(startText());
		window.display();
		if (Keyboard::isKeyPressed(Keyboard::Space))
			firstRun = false;
		while (window.pollEvent(e)) {
	        if (e.type == Event::Closed)      
	        window.close();
	    }	
	}
	
	window.clear();
	
    while (window.isOpen()) {
    	while(running) {
	        float time = clock.getElapsedTime().asSeconds();
	        clock.restart();
	        timer+=time;
	       
			changeDirection();
	
   	        if (timer>delay) {
				timer=0; 
				move();
				checkApple();
				checkCollisions();
			}
			
		    window.clear();
		    //draw snake
		    for (int i=0;i<bodyParts;i++) { 
				snakeSprite.setPosition(s[i].x*size, s[i].y*size);  
				window.draw(snakeSprite); 
			}
		   	//draw apple
		    appleSprite.setPosition(f.x*size, f.y*size);  
			window.draw(appleSprite);    
			
		    window.display();
		}
		
		window.clear();
		Text gameOver = gameOverText();
		//show game over screen
		while(!running) {
			window.clear();
			window.draw(gameOver);
			window.display();
			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				window.clear();
				restart();
			}	
			while (window.pollEvent(e)) {
		        if (e.type == Event::Closed)      
		        window.close();
		    }	
		}
    }
    
    return 0;
}
