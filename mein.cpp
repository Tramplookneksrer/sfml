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

float offsetX=0, offsetY=0;

const int H = 11;
const int W = 40;





String TileMap[H] = {
	
	
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                      B",
"B           BB     BB   B B BB B  B    B",
"B           BB     BB   BBB B  B  B    B",
"B                       B B BB BB BB   B",
"B                                      B",
"B                                      B",
"B                                      B",
"B            BBBBBBB      BBBB         B",
"BBB        BB       BB   B     B       B",                                                                                                       
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
};

/*class LifeBar
{
public:
	Image image;
	Texture t;
	Sprite s;
	int max;
	RectangleShape bar;
 
 
	void draw(RenderWindow &window){
		Vector2f center = window.getView().getCenter();
		Vector2f size = window.getView().getSize();
 
		s.setPosition(center.x - size.x / 2 + 10, center.y - size.y / 2 + 10);
		bar.setPosition(center.x - size.x / 2 + 14, center.y - size.y / 2 + 14);
	}
 
};*/






class PLAYER{
public:
float dx,dy;
FloatRect rect;
bool onGround;
Sprite sprite;
float currentFrame;

    PLAYER(Texture &image){
    sprite.setTexture(image);
    rect = FloatRect(33,235,30,50);
    	
    dx=dy=0.1;
    currentFrame = 0;	
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
		
		if (dx>0) sprite.setTextureRect(IntRect(40*int(currentFrame),244,40,50));
	    if (dx<0) sprite.setTextureRect(IntRect(40*int(currentFrame)+40,244,-40,50));
		
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
		dx = 0;
	}
	

	void Collision(int dir)
   {
     for (int i = rect.top/32 ; i<(rect.top+rect.height)/32; i++)
	  for (int j = rect.left/32; j<(rect.left+rect.width)/32; j++)
		{ 
	  	 if (TileMap[i][j]=='B') 
		   { 
	        if ((dx>0) && (dir==0)) rect.left =  j*32 -  rect.width; 
		    if ((dx<0) && (dir==0)) rect.left =  j*32+ 32;
			if ((dy>0) && (dir==1))  {rect.top = i*32 -  rect.height;  dy=0;   onGround=true;}
			if ((dy<0) && (dir==1))  {rect.top = i*32 + 32;   dy=0;}
		   }	         	
    	}
   }
};

void menu(RenderWindow & window,bool & end)
{
    Texture Texturemenu1,Texturemenu2,menuBackground;
    Texturemenu1.loadFromFile("1.png");
    Texturemenu2.loadFromFile("2.png");
    menuBackground.loadFromFile("f2.jpg");
    Sprite menu1(Texturemenu1), menu2(Texturemenu2), fon(menuBackground);
    bool isMenu = 1;
    int menNum = 0;
    menu1.setScale(0.5,0.5);
    menu2.setScale(0.5, 0.5);
    menu1.setPosition(500,200);
    menu2.setPosition(500, 400);
    fon.setPosition(0, 0);

while (isMenu)
{
	menu1.setColor(Color::White);
    menu2.setColor(Color::White);
    menu1.setColor(Color::White);
    menu2.setColor(Color::White);
    if (IntRect(600, 200, 150, 150).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menNum = 1; }
    if (IntRect(600, 400, 150, 150).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Red); menNum = 2; }
    if (Mouse::isButtonPressed(Mouse::Left))
    {
        if (menNum == 1) { isMenu = false; }
        if (menNum == 2) { end = 1; break; }
    }
    window.draw(fon);
    window.draw(menu1);
    window.draw(menu2);
    window.display();
}
}



int main(){
	bool end = 0;
    RenderWindow window(VideoMode(1200, 800),"Sword Art",Style::Default);//ðàçìåð îêíà
    menu(window,end);
    
	if (end == true) { return 0; }
    bool isMove = false;
    float dX = 0;//êîððåêòèðîâêà íàæàòèÿ ïî x
	   float dY = 0;//êîððåêòèðîâêà íàæàòèÿ ïî y
    Image sun_i;
    Texture sun_t;
    Sprite sun_sp;

    sun_i.loadFromFile("sun.png");
    sun_i.createMaskFromColor(Color(50,50,50));
    sun_t.loadFromImage(sun_i);
    sun_sp.setTexture(sun_t);
    sun_sp.setPosition(380, 90);
    
	Texture t;
	t.loadFromFile("fang.png");
	float currentFrame=0;

    PLAYER p(t);//èãðîê
    
    //ñîõäà¸ì îáúåêò çâóêà
    SoundBuffer buffer;
    buffer.loadFromFile("xz.ogg");
    Sound sound(buffer);
    sound.setVolume(30);
    
    //ñîçäà¸ì îáúåêò ìóçûêè
	Music music;
	music.openFromFile("music.ogg");
	music.play();
	music.setLoop(true);
	music.setVolume(20);	
    
    
	Clock clock;
    RectangleShape rectangle( Vector2f(32,32));
	//óïðàâëåíèå îêíîì èãðû
	while (window.isOpen()){
		
    	float time = clock.getElapsedTime().asMilliseconds();
    	clock.restart();
    	time = time/0.99;
    	
        Event event;
        while (window.pollEvent(event))
        {
        	if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))//çàêðûòèå îêíà ïðè ïîìîùè êðåñòèêà èëè esc
        	window.close();
		}
			
		if (Mouse::isButtonPressed(Mouse::Left))
		{
		    p.dx = -0.15,5; 
		}
		//ïîëó÷àåì ãëîáàëüíóþ ïîçèöèþ ìûøè
		sf :: Vector2i position = sf :: Mouse :: getPosition ();
		//ïîëîæåíèå ìûøè îòíîñèòåëüíî îêíà
		//sf::Mouse::setPosition(sf::Vector2i(100, 200), window);
       
		//äâèæåíèå âëåâî
		if (Keyboard::isKeyPressed(Keyboard::A)){
			p.dx = -0.15,5; 
		}
		//äâèæåíèå âïðàâî
		if (Keyboard::isKeyPressed(Keyboard::D)){
			p.dx = 0.15,5;
		}
		//ïðûæîê
		if (Keyboard::isKeyPressed(Keyboard::W)){
			if (p.onGround) {p.dy = -0.40; p.onGround = false; sound.play();}
		}
		p.update(time);
		if (p.rect.left>600) offsetX = p.rect.left - 600;
        offsetY = p.rect.top - 400
		;
        window.clear(Color::White);//Color::Black
         for (int i=0; i<H; i++)
			 for (int j=0; j<W ; j++)
				{ 
				  if (TileMap[i][j]=='B') rectangle.setFillColor(Color::Red);

				  if (TileMap[i][j]==' ') continue;

		          rectangle.setPosition(j*32-offsetX,i*32 - offsetY) ; 
		          window.draw(rectangle);
	       	 }	  	 
		window.draw(p.sprite);
        window.draw(sun_sp);
        window.display();
    }
    return 0;
}
