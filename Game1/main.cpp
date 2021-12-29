                                                                  /////////////////////////////// 
                                                                 //////////[SWORD ART]//////////
                                                                ///////////////////////////////
#pragma execution_character_set("utf-8")

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <unistd.h>
#include <vector>
#include <conio.h>
#include <ctime>
#include <string>


using namespace sf;
using namespace std;
int ground = 400;
struct pos {
    int x, y;
};

float offsetX=0, offsetY=0;
                                    ///////////////////////////////////////////////////////////////////////////
                                   ///if bugs are found in the "game", then these are not bugs but a feature//
                                  ///////////////////////////////////////////////////////////////////////////
const int H = 11;
const int W = 40;

String TileMap[H] = {

// 'B'	-	unbrackable wall
// '#'	-	brackable wall
// ' '	-	void(air)
	
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                      B",
"B          #BB     BB   B B BB B  B    B",
"B      #### BB     BB   BBB B  B  B    B",
"B      ####             B B BB BB BB   B",
"B     #                                B",
"B    #          ###                    B",
"B   #          ####                    B",
"B  #         BBBBBBB      BBBB         B",
"BBB    #   BB       BB   B   # B       B",                                                                                                       
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
};


int hitMap[2][H][W];	//hit map. [some variables][height][width]
						//some variables: 
						//	[0] - hits
						//	[1]	- max avable hp

class PLAYER{
public:
float dx,dy;
bool isleft;
FloatRect rect;
bool onGround;
Sprite sprite;
int hp, hit;	//to give DAMAGE change ONLY hit. DO NOT TOUCH hp
float currentFrame;

	PLAYER(Texture &image){
	sprite.setTexture(image);
	rect = FloatRect(33,235,30,50);

	dx=dy=0.1;
	currentFrame = 0;
	isleft = false;
	hp = 1000;		//it is a max avable health
	hit = 0;		//It is a health
	}
	void update(float time){
		
		rect.left +=dx * time;
		Collision(0);
		if (!onGround) dy = dy + 0.0015*time;
		
		rect.top += dy * time;
		onGround = false;
		Collision(1);
		currentFrame += 0.05*time;
		if (currentFrame >6) currentFrame-=6;
		
		if (dx > 0)
		{
			sprite.setTextureRect(IntRect(40 * int(currentFrame), 244, 40, 50));
			isleft = false;
		}
		if (dx < 0)
		{
			sprite.setTextureRect(IntRect(40 * int(currentFrame) + 40, 244, -40, 50));
			isleft = true;
		}
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
		dx = 0;
	}
	
	void Collision(int dir)
   {
	 for (int i = rect.top/32 ; i<(rect.top+rect.height)/32; i++)
	  for (int j = rect.left/32; j<(rect.left+rect.width)/32; j++)
		{ 
		 if (TileMap[i][j]!=' ')
		   { 
			if ((dx>0) && (dir==0)) rect.left =  j*32 -  rect.width; 
			if ((dx<0) && (dir==0)) rect.left =  j*32+ 32;
			if ((dy>0) && (dir==1))  {rect.top = i*32 -  rect.height;  dy=0;   onGround=true;}
			if ((dy<0) && (dir==1))  {rect.top = i*32 + 32;   dy=0;}
		   }	         	
		}
   }
};

class LifeBar{
private:
	RectangleShape hpSector;
	Color hitPoint_FULL;
	Color hitPoint_VOID;
public:
	
	LifeBar(){
		hpSector.setSize(Vector2f(1, 2));	//change 10 by another num to increase the hit points streak
		hitPoint_FULL = Color::Red;
		hitPoint_VOID = Color::Transparent;
	}
	void draw(RenderWindow& window)
	{
		//draw hitpoints a map
		for (int i = 0; i < H; i++)
			for (int j = 0, proc = 0; j < W; j++)
				if (hitMap[0][i][j] != 0)
				{
					proc = (100 * hitMap[0][i][j]) / (hitMap[1][i][j]);
					for (int k = 0; k < 32; k++)
					{
						hpSector.setFillColor(((k * 100) / 32 < proc) ? hitPoint_VOID : hitPoint_FULL);
						hpSector.setPosition((j * 32) + (30) - k - offsetX, (i * 32) - 10 - offsetY);
						window.draw(hpSector);
					}
				}
	}
	void hpPlayerDraw(RenderWindow& window, PLAYER& p)
	{
		int proc = (100 * p.hit) / p.hp;	//(100*hitMap[0][i][j])/(hitMap[1][i][j] * 1000)
		for (int k = 0; k < 32; k++)
		{
			hpSector.setFillColor(((k * 100) / 32 < proc) ? hitPoint_VOID : hitPoint_FULL);
			hpSector.setPosition((p.rect.left) + (30) - k - offsetX, (p.rect.top - (p.rect.height / 2)) + 32 - 10 - offsetY);
			window.draw(hpSector);
		}
		
	}
};

void menu(RenderWindow & window)
{
	Texture Texturemenu1,Texturemenu2,menuBackground;
	Texturemenu1.loadFromFile("1.png");
	Texturemenu2.loadFromFile("2.png");
	menuBackground.loadFromFile("q1.jpg");
	Sprite menu1(Texturemenu1), menu2(Texturemenu2), fon(menuBackground);
	int menNum = 0;
	bool end = false;
	menu1.setScale(0.5,0.5);
	menu2.setScale(0.5, 0.5);
	menu1.setPosition(500,200);
	menu2.setPosition(500, 400);
	fon.setPosition(0, 0);
	while (1)
	{
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		if (IntRect(600, 200, 150, 150).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menNum = 1; }
		if (IntRect(600, 400, 150, 150).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Red); menNum = 2; }
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menNum == 1) { break; }
			if (menNum == 2) { end = 1; break; }
		}
		window.draw(fon);
		window.draw(menu1);
		window.draw(menu2);
		window.display();
	}
	if (end == 1)
		window.close();
		
}
bool Delay(float milsec, Clock &clock){
    Time time = clock.getElapsedTime();
    if (time.asSeconds() > (milsec/1000)){
        clock.restart();
        return true;
    }else
    return false;
}
void Icon_Animation(RenderWindow &window)
	{
		Clock clock;
    	window.clear(Color(0,0,0,0));

        Image  c_i;  c_i.loadFromFile("doer1.png");
        Texture sfml_t, c_t;
        c_t.loadFromImage(c_i);
        Sprite sfml_s, c_s;
        c_s.setTexture(c_t);
        c_s.setPosition(0,0);
        int i = 5;
        while(i < 255)
        if(Delay(130,clock)){
            window.clear(Color(0,0,0,0));
            c_s.setColor(Color(255,255,255,i));
            window.draw(c_s);
            window.display();
            i+=25;
        }
        while(i > 5)
        if(Delay(130,clock)){
            window.clear(Color(0,0,0,0));
            c_s.setColor(Color(255,255,255,i));
            window.draw(c_s);
            window.display();
            i-=25;
        }
    }

int main(){
	bool end = 0;
	RenderWindow window(VideoMode(1200, 800),"Sword Art",Style::Default);//window management
	
	Icon_Animation(window);
	//window.setView(View(FloatRect(0.0f, 0.0f, 1920.0f, 1080.0f)));//you can uncomment, but it's better not to
	menu(window);
	
	
	//load a hitmap damage
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			hitMap[0][i][j] = 0;

	bool isMove = false;
	float dX = 0;//correction of movement by x
	float dY = 0;//correction of movement by y
	Image sun_i;
	Texture sun_t;
	Sprite sun_sp;

	sun_i.loadFromFile("sun.png");
	sun_i.createMaskFromColor(Color(50,50,50));
	sun_t.loadFromImage(sun_i);
	sun_sp.setTexture(sun_t);
	
	Texture t;
	t.loadFromFile("fang.png");
	float currentFrame=0;

	PLAYER p(t);//player(-_-)
	//connecting sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("hit1.ogg");
	Sound hit(hitBuffer);
	
	SoundBuffer buffer;
	buffer.loadFromFile("xz.ogg");
	Sound sound(buffer);
	sound.setVolume(30);
	
	
	//connecting music
	p.rect.left=pos.x - dx;
	p.rect.top=pos.y - dy;
	Music music;
	music.openFromFile("music.ogg");
	music.play();
	music.setLoop(true);
	music.setVolume(20);	
	
	LifeBar lb;//init LifeBar
    
	Clock clock;
	RectangleShape rectangle(Vector2f(32, 32));
	
	while (window.isOpen()){
		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		time = time/0.99;
		
		Vector2i pixelPos = Mouse::getPosition(window);//taking the cursor coord
		Vector2f pos = window.mapPixelToCoords(pixelPos);//we transfer them to the gaming ones (we move away from the coord window)
		
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)//exit to the cross or esc
				window.close();
			if (Keyboard::isKeyPressed(Keyboard::Escape))
				menu(window);
			if (event.type == Event::MouseButtonPressed)//if the mouse key is pressed
	            if (event.key.code == Mouse::Left)//namely the left
		            if (p.sprite.getGlobalBounds().contains(pos.x, pos.y))//and at the same time, the cursor coordinate falls into the sprite
			            {
		                    dX = pos.x - p.sprite.getPosition().x;//we make a difference between the cursor position and the sprite.to adjust the click
		                    dY = pos.y - p.sprite.getPosition().y;//the same for the player
		                    isMove = true;//we can move the sprite							
						}
		    if (event.type == Event::MouseButtonReleased)//if you released the key
                if (event.key.code == Mouse::Left) //namely the left
                    isMove = false; //then we can't move the sprite
                    p.sprite.setColor(Color::White);//and we give it the same color
		}
		if (isMove) {//if we can move
            p.sprite.setColor(Color::Green);//painting the sprite green
            p.sprite.setPosition(pos.x - dX, pos.y - dY);
}	
		
		//break wall
		if (Keyboard::isKeyPressed(Keyboard::E))
		{
			char breakable_wall = '#';	//the wall you can break
			int healh = 1000; //milli Seconds to break
			int drotate = (p.isleft) ? -1 : 1;
			int x = (int)((p.rect.left + p.rect.width/2) / 32) + drotate;	//player coords to breakable block coords
			int y = (int)((p.rect.top + (p.rect.height / 2)) / 32);
			if (TileMap[y][x] == breakable_wall)
			{
				hit.play();
				hitMap[1][y][x] = healh;	//write about his health
				if (hitMap[0][y][x] >= healh)
				{
					TileMap[y][x] = ' ';
					hitMap[0][y][x] = 0;	//reinit hits
				}
				else
					hitMap[0][y][x] += 1;	//1 hit
			}
		}
		
		
		sf :: Vector2i position = sf :: Mouse :: getPosition ();
		
		//moving to the left
		if (Keyboard::isKeyPressed(Keyboard::A)){
			p.dx = -0.15,5; 
		}
		//movement to the right
		if (Keyboard::isKeyPressed(Keyboard::D)){
			p.dx = 0.15,5;
		}
		//jump
		if (Keyboard::isKeyPressed(Keyboard::W)){
			if (p.onGround) {p.dy = -0.40; p.onGround = false; sound.play();}
		}
		p.update(time);
		if (p.rect.left>600) offsetX = p.rect.left - 600;
		offsetY = p.rect.top - 400;
		
		window.clear(Color::Black);//Color::Black(hell)/White(paradise)
		 for (int i=0; i<H; i++)
			 for (int j=0; j<W ; j++)
				{ 
				  if (TileMap[i][j]=='B') rectangle.setFillColor(Color::Red);
				  if (TileMap[i][j] == '#') rectangle.setFillColor(Color::Magenta);
				  if (TileMap[i][j]==' ') continue;

				  rectangle.setPosition(j*32-offsetX,i*32 - offsetY) ; 
				  window.draw(rectangle);
			 }
		lb.draw(window);
		
		lb.hpPlayerDraw(window, p);
		
		//window.setFramerateLimit(60);//fps(*__*)
		window.draw(p.sprite);
		sun_sp.setPosition((10*32) - offsetX, (-1*32) - offsetY - 300);
		window.draw(sun_sp);
		window.display();
	}
	return (0);
}
