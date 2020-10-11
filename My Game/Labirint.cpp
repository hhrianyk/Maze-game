
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "prototaype.h"

class ENEMY;
float offsetX = 0, offsetY =0;///камера
const int height =5, width = 7;///початкові розміри
int resolutionX = 850;///Разрешение экрана монитора X
int resolutionY = 480;///Разрешение экрана монитора Y


int Volume = -00;///громкость музики
int level = 0;///уровень
int complexity = 2;///складнісь. лише парні числа 



bool deadend(int, int); // Вспомогательная функция, определяет тупики
void visual( ); // Изображение результата с помощью консольной графики
void mazemake(int height, int width); // Собственно алгоритм
void newLevel( );// новий уравень
void restatLevel(RenderWindow& window,  ENEMY enemy, ENEMY enemy2, ENEMY enemy3);

string** maze = new string* [height];
const string  wall = "#", pass = " ";

class PLAYER {

public:

	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float currentFrame;

	PLAYER(Texture& image)
	{
		sprite.setTexture(image);
		rect = FloatRect(2*16  , 1*16 , 10 ,10 );

		dx = dy = 0.1;
		currentFrame = 0;
	}


	void update(float time)
	{

		 rect.left += dx * time;
		 Collision(0);

		   if (!onGround) dy = dy + 0.0005 * time;
		  rect.top += dy * time;
		 onGround = false;
		 Collision(1);


		currentFrame += 0.005 * time;
		if (currentFrame > 6) currentFrame -= 6;

		if (dx > 0) sprite.setTextureRect(IntRect(40 * int(currentFrame), 244, 10  ,10  ));
		if (dx < 0) sprite.setTextureRect(IntRect(40 * int(currentFrame)+40, 244, 10   ,10  ));


		 sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

		dx = 0;
	}



	void Collision(int dir)
	{
		for (int i = rect.top / 16; i <( (rect.top + rect.height) / 16) ; i++)
			for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++)
			{
				if (maze[i][j] == "#") 
				{
					if ((dx > 0) && (dir == 0)) rect.left = j * 16 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 16 + 16;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 16 - rect.height;  dy = 0;   onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 16 + 16;   dy = 0; }
				}
				
				 

			}

	}
};

class ENEMY
{

public:
	float dx, dy;
	FloatRect rect;
	Sprite sprite;
	float currentFrame;
	bool life;


	void set(Texture& image, int x, int y)
	{
		sprite.setTexture(image);
		rect = FloatRect(x, y, 10, 10);

		dx = 0.05;
		dy = 0.05;
		currentFrame = 0;
		life = true;
	}

	void update(float time)
	{
		 rect.left += dx * time;
		
		  Collision(0); 
		  dy = dy + 0.00005 * time;
			rect.top  += dy * time;
		  
			 Collision(1);

		 


		currentFrame += time * 0.005;
		if (currentFrame > 2) currentFrame -= 2;

		sprite.setTextureRect(IntRect(18 * int(currentFrame), 0, 10, 10));
		if (!life) sprite.setTextureRect(IntRect(58, 0, 10, 10));


		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

	 

	}

	void Collision(int dir)
	{
		for (int i = rect.top / 16; i < ((rect.top + rect.height) / 16); i++)
			for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++)
				if (maze[i][j] == '#')
				{
					     if ((dx > 0) && (dir == 0)) {rect.left = j * 16 - rect.width; dx *= -1;}
			    	else if ((dx < 0) && (dir == 0)) {rect.left = j * 16 + 16;  dx *= -1;}

					     if ((dy > 0) && (dir == 1)) { rect.top = i * 16 - rect.height;  dy   =  0.01;   }
				    else if ((dy < 0) && (dir == 1)) { rect.top = i * 16 + 16;   dy   =- 0.01;}
				}
		
	}

};

int main() {
	
	srand((unsigned)time(NULL));
	for (int i = 0; i < height; i++)
		maze[i] = new string [width];

	
    mazemake(  height, width);
	visual( );

	RenderWindow window(VideoMode(resolutionX, resolutionY), "Alpha v.1.4");

	Texture t,t2;
	ENEMY  enemy, enemy2, enemy3;
	t.loadFromFile("Player 1.png");
	t2.loadFromFile("Player 2.png");
	enemy.set(t2, (width - 2) * 16, (height - 2) * 16);
	enemy2.set(t2, (width - 2) * 16, (1) * 16);
	enemy3.set(t2, (2) * 16, (height - 2) * 16);
	
	Sprite tile(t); 
	
	SoundBuffer buffer_Jump, buffer_dead;
	buffer_Jump.loadFromFile("Jump2.ogg");
	buffer_dead.loadFromFile("dead.wav");
	Sound sound_dead(buffer_dead);
	Sound sound_Jump(buffer_Jump);
	sound_dead.setVolume(Volume);
	sound_Jump.setVolume(Volume);


	Music music;
	music.openFromFile("music fon.ogg");
	music.setVolume(Volume);
	music.play();
	float currentFrame = 0;

	PLAYER p(t);

	Clock clock;

	RectangleShape rectangle(Vector2f(16, 16));

	while (window.isOpen())
	{
		
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 700;

		if (time > 20) time = 20;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A) )
		{p.dx = -0.1;		}

		if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)  )	
		{p.dx = 0.1;		}

		if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W) )
		{
			 {p.dy = -0.35;  sound_Jump.play(); p.onGround = false; Volume++;
			music.setVolume(Volume);
			sound_dead.setVolume(Volume);
			sound_Jump.setVolume(Volume);
			}}
		
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			p.dy = 0.10;
		}

		if (Keyboard::isKeyPressed(Keyboard::R))
		{
			    p.rect = FloatRect(2 * 16, 1 * 16, 10, 10);////стартова позиція гравця
				enemy.set(t2, (width + level * complexity - 2) * 16, (height + level * complexity - 2) * 16);////нова позиція ворога 1
				enemy2.set(t2, (width + level * complexity - 2) * 16, (1) * 16);////нова позиція ворога 2
				enemy3.set(t2, (2) * 16, (height + level * complexity - 2) * 16);////нова позиція ворога 3
				restatLevel(window,   enemy, enemy2, enemy3);
		}
		 
		p.update(time);
		enemy.update(time);
		enemy2.update(time);
		enemy3.update(time);

		if (p.rect.intersects(enemy.rect))
		{
			if (enemy.life) {
				if (p.dy > 0) {
					enemy.dx = 0; p.dy = -0.2; enemy.life = false; sound_dead.play();
				}
				else p.sprite.setColor(Color::Yellow), sound_dead.play();
			}
		}
		if (p.rect.intersects(enemy2.rect))
		{
			if (enemy2.life) {
				if (p.dy > 0) { enemy2.dx = 0; p.dy = -0.2; enemy2.life = false; sound_dead.play();
				}
				else p.sprite.setColor(Color::Blue), sound_dead.play();
			}
		}		
		if (p.rect.intersects(enemy3.rect))
		{
			if (enemy3.life) {
				if (p.dy > 0) { enemy3.dx = 0; p.dy = -0.2; enemy3.life = false; sound_dead.play();
				}
				else p.sprite.setColor(Color::Red), sound_dead.play();
			}
		}

		 if (p.rect.left > resolutionX/2) offsetX = p.rect.left - resolutionX / 2;
		 if (p.rect.top > resolutionY /2) offsetY = p.rect.top - resolutionY /2;
		 

		window.clear(Color::White);


		for (int i = 0; i < height + complexity * level; i++)
			for (int j = 0; j < width + complexity * level; j++)
			{


				if (maze[i][j] == "#") rectangle.setFillColor(Color::Black); 
				else if (maze[i][j] == " ") rectangle.setFillColor(Color::Green);

				rectangle.setPosition(j * 16 - offsetX, i * 16 - offsetY);
				window.draw(rectangle);
			}

 		window.draw(p.sprite);
		window.draw(enemy.sprite);
	    window.draw(enemy2.sprite); 
		window.draw(enemy3.sprite);
		window.display();
 
		if (enemy.life == false && enemy2.life == false && enemy3.life == false)
		{
			Texture over;
			 
		
			if (level == 10 )
			{
				over.loadFromFile("Game over.jpg");
				Sprite tile(over);
				window.draw(tile);
				window.display();
				Sleep(5000);
				window.close();
			}
			else {
				over.loadFromFile("next level.jpg");
				Sprite tile(over);
				window.draw(tile);
				window.display();
				Sleep(2000);
				newLevel();///побудова нового рівня
				enemy.life =  true, enemy2.life =  true, enemy3.life =  true;/// відновлення життя ворогів
				p.rect = FloatRect(2 * 16, 1 * 16, 10, 10);////стартова позиція гравця
				enemy.set(t2, (width+level* complexity - 2) * 16, (height+ level * complexity - 2) * 16);////нова позиція ворога 1
				enemy2.set(t2, (width +level * complexity - 2) * 16, (1) * 16);////нова позиція ворога 2
				enemy3.set(t2, (2) * 16, (height+level * complexity - 2) * 16);////нова позиція ворога 3
			}
		}

	}



	return 0;
}


bool deadend(int x, int y) {
	int a = 0;

	if (x != 1) {
		if (maze[y][x - 2] == pass)
			a += 1;
	}
	else a += 1;

	if (y != 1) {
		if (maze[y - 2][x] == pass)
			a += 1;
	}
	else a += 1;

	if (x != width + complexity * level - 2) {
		if (maze[y][x + 2] == pass)
			a += 1;
	}
	else a += 1;

	if (y != height + 2 * level - 2) {
		if (maze[y + 2][x] == pass)
			a += 1;
	}
	else a += 1;

	if (a == 4)
		return true;
	else
		return false;
}

 void visual( ) {
	for (int i = 0; i < height + complexity * level; i++) {
		for (int j = 0; j < width + complexity * level; j++)
		{
			if (maze[i][j] == "#")  cout << "# ";
			if (maze[i][j] == " ") cout << "  ";

			
		}cout << endl;
	}
 	cout << endl;
 }

void mazemake(int height,int width) {
	int x, y, c, a;
	bool b;

	for (int i = 0; i < height; i++) // Массив заполняется землей-ноликами
		for (int j = 0; j < width; j++)
			maze[i][j] = wall;

	x = 3; y = 3; a = 0; // Точка приземления крота и счетчик
	while (a < 10000) { 
		maze[y][x] = pass; a++;
		while (true) { // Бесконечный цикл, который прерывается только тупиком
			/*visual();
			system("cls");*/
			c = rand() % 4; // Напоминаю, что крот прорывает
			switch (c) {  // по две клетки в одном направлении за прыжок
			case 0: if (y != 1)
				if (maze[y - 2][x] == wall) { // Вверх
					maze[y - 1][x] = pass;
					maze[y - 2][x] = pass;
					y -= 2;
				}
				  break;
			case 1: if (y != height - 2)
				if (maze[y + 2][x] == wall) { // Вниз
					maze[y + 1][x] = pass;
					maze[y + 2][x] = pass;
					y += 2;
				}break;
			case 2: if (x != 1)
				if (maze[y][x - 2] == wall) { // Налево
					maze[y][x - 1] = pass;
					maze[y][x - 2] = pass;
					x -= 2;
				}break;
			case 3: if (x != width - 2)
				if (maze[y][x + 2] == wall) { // Направо
					maze[y][x + 1] = pass;
					maze[y][x + 2] = pass;
					x += 2;
				}break;
			}
			if (deadend(x, y))
				break;
			
			
		}

		if (deadend(x, y)) // Вытаскиваем крота из тупика
			do {
				x = 2 * (rand() % ((width - 1) / 2)) + 1;
				y = 2 * (rand() % ((height - 1) / 2)) + 1;
				
			} while (maze[y][x] != pass);
	
	} // На этом и все.
}

void newLevel( )
{
	for (int i = 0; i < height+ complexity * (level ); i++)
		delete[] maze[i];
	delete[] maze;
	level++;
	maze = new string * [height + complexity * level];
	for (int i = 0; i < height + complexity * level; i++)
		maze[i] = new string[width + complexity * level];
	mazemake(height + complexity * level, width + complexity * level);
    
	
	offsetX = 1;
	offsetY = 1;
	 

}

void restatLevel(RenderWindow& window, ENEMY enemy, ENEMY enemy2, ENEMY enemy3)
{
	Texture over;
	over.loadFromFile("restart.gif");
	Sprite tile(over);
	window.draw(tile);
	window.display();
	Sleep(2000);
	mazemake(height + complexity * level, width + complexity * level);
	enemy.life = true, enemy2.life = true, enemy3.life = true;/// відновлення життя ворогів



	offsetX = 1;
	offsetY = 1;
}