#include "TXLib.h"
#include <iostream>
#include <sstream>
#include <algorithm> 


///Статистика игрока///
class PlayerStat {
public:
	int hp;
	int hunger;
	int water;
	string name;
};
///////////////////////




struct Inventar {
	HDC med = txLoadImage("textures/items/heal_texture.bmp");
	HDC water = txLoadImage("textures/items/water_texture.bmp");
	int kolvo_predmetov;
	int random_predmet;

	void random_loot() {
		random_predmet = rand() % (2 - 1 + 1) + 1;
		if (random_predmet == 1)(cout << random_predmet);
		else (cout << random_predmet);
		
	}

	void open_car() {
		txSetFillColor(TX_BLACK);
		txSetColor(TX_BLACK);

		txRectangle(640, 0, 660, 360);
		txRectangle(740, 0, 760, 360);
		txRectangle(840, 0, 860, 360);
		txRectangle(940, 0, 960, 360);
		txRectangle(1040, 0, 1080, 360);

		txRectangle(540, 0, 1080, 20);

		txRectangle(540, 100, 1080, 120);
		txRectangle(540, 200, 1080, 220);
		txRectangle(540, 300, 1080, 360);

		txSetFillColor(TX_WHITE);
		txSetColor(TX_WHITE);
		Win32::TransparentBlt(txDC(), 500, 0, 200, 200, water, 0, 0, 200, 200, TX_WHITE);
	}

	void open_inventar() {
		txClear();
		
		txBegin();
		
		txSetFillColor(TX_BLACK);
		txSetColor(TX_BLACK);
		
		txRectangle(500,0,560,360);
		
		txRectangle(0, 100, 540,120);
		txRectangle(0, 200, 540, 220);
		txRectangle(0, 300, 540, 360);
		
		txRectangle(0, 0, 20, 360);
		txRectangle(0, 0, 540, 20);

		txRectangle(100, 0, 120, 360);
		txRectangle(200, 0, 220, 360);
		txRectangle(300, 0, 320, 360);
		txRectangle(400, 0, 420, 360);
		txRectangle(500, 0, 520, 360);


		txSetFillColor(TX_WHITE);
		txSetColor(TX_WHITE);
		txEnd();
	}
};





///Анимация бездействия///
struct PlayerAnimation {
	int x,y,cadr = 0;
	
	

	HDC idle[4], idleL[4];

	PlayerAnimation() {
		idle[0] = txLoadImage("textures/persons/Idle/1.bmp");
		idle[1] = txLoadImage("textures/persons/Idle/2.bmp");
		idle[2] = txLoadImage("textures/persons/Idle/3.bmp");
		idle[3] = txLoadImage("textures/persons/Idle/4.bmp");

		idleL[0] = txLoadImage("textures/persons/Idle/L1.bmp");
		idleL[1] = txLoadImage("textures/persons/Idle/L2.bmp");
		idleL[2] = txLoadImage("textures/persons/Idle/L3.bmp");
		idleL[3] = txLoadImage("textures/persons/Idle/L4.bmp");
	}

	void Idle(int x, int y, bool left, bool right, bool& open_inv) {
		HDC* currentIdle = right ? idle : left ? idleL : idle;
		int index = (cadr / 2) % 4;
		if (open_inv == false)
		{
			Win32::TransparentBlt(txDC(), x, y, 64, 64, currentIdle[index], 0, 0, 14, 15, TX_WHITE);
		}
		cadr = (cadr + 1) % 16;
	}
};
//////////////////////////



///Кнопки///
struct Button {
	int x;
	int y;
	int width;
	int heigh;
	int color;
	int color_text;
	const char* text;
	void draw_b() {
		txSetColor(color);
		txSetFillColor(color);
		Win32::RoundRect(txDC(), x, y, x + width, y + heigh, 30, 30);
		txSelectFont("Times New Roman", 30);
		txSetColor(color_text);
		txDrawText(x, y, x + width, y + heigh, text);
	}
	bool pressed() {
		return(txMouseX() > x && txMouseX() < x + width &&
			txMouseY() > y && txMouseY() < y + heigh &&
			txMouseButtons() == 1);
	}
};
////////////

struct GameBackground {
	HDC cars;
	HDC road;
	HDC houses_trees;
	HDC sky = txLoadImage("textures/bg/city/sky.bmp");
	HDC houses = txLoadImage("textures/bg/city/houses.bmp");
	int new_x_sky = 1;
	int old_x_sky = 0;
	int new_x_sky2 = 1;
	int old_x_sky2 = -1080;
	bool move_sky = true;

	int x = 0;
	int x2 = 1080;

	void draw(int location, int player_x, int player_y, bool& open_inv) {
		if (location == 1 and open_inv == false) {
			txBegin(); // Начинаем рисование

			txClear();

			if (move_sky) {
				if (old_x_sky >= 1080) old_x_sky = -1080;
				if (old_x_sky2 >= 1080) old_x_sky2 = -1080;

				Win32::TransparentBlt(txDC(), old_x_sky, 0, 1080, 720, sky, 0, 0, 1080, 720, TX_WHITE);
				Win32::TransparentBlt(txDC(), old_x_sky2, 0, 1080, 720, sky, 0, 0, 1080, 720, TX_WHITE);

				old_x_sky++;
				old_x_sky2++;
			}

			// Отрисовка объектов
			drawBackgroundLayers(player_x, player_y, open_inv);

			txEnd(); // Завершаем рисование
		}
	}
	void drawBackgroundLayers(int player_x, int player_y, bool& open_inv) {
		
		
		if (open_inv == false)

		{
			Win32::TransparentBlt(txDC(), x, 0, 1080, 720, houses_trees, 0, 0, 1080, 720, TX_WHITE);
			Win32::TransparentBlt(txDC(), x, 0, 1080, 720, houses, 0, 0, 1080, 720, TX_WHITE);
			Win32::TransparentBlt(txDC(), x, 0, 1080, 720, cars, 0, 0, 1080, 720, TX_WHITE);
			Win32::TransparentBlt(txDC(), x, 0, 1080, 720, road, 0, 0, 1080, 720, TX_WHITE);

			Win32::TransparentBlt(txDC(), x2, 0, 1080, 720, houses_trees, 0, 0, 1080, 720, TX_WHITE);
			Win32::TransparentBlt(txDC(), x2, 0, 1080, 720, houses, 0, 0, 1080, 720, TX_WHITE);
			Win32::TransparentBlt(txDC(), x2, 0, 1080, 720, cars, 0, 0, 1080, 720, TX_WHITE);
			Win32::TransparentBlt(txDC(), x2, 0, 1080, 720, road, 0, 0, 1080, 720, TX_WHITE);

			
		}
	}
	void animbg(bool right, bool left, int& x3)
	{
		if (right == true)
		{
			x--;
			x2--;
			x3--;
			right = false;
		}
		if (left == true)
		{
			x++;
			x2++;
			x3++;
			left = false;
		}
	}
};



///Задний фон///
struct Background {
	HDC image;
	void drawbg()
	{
		
		txBitBlt(txDC(), 0, 0, 1080, 720, image);
	}
};
////////////////

///Анимация передвижения///
struct Player {
	int x, y , cadr = 0;
	bool left, right = false;
	HDC walk[6], walkL[6];

	Player() {
		walk[0] = txLoadImage("textures/persons/Walk/1.bmp");
		walk[1] = txLoadImage("textures/persons/Walk/2.bmp");
		walk[2] = txLoadImage("textures/persons/Walk/3.bmp");
		walk[3] = txLoadImage("textures/persons/Walk/4.bmp");
		walk[4] = txLoadImage("textures/persons/Walk/5.bmp");
		walk[5] = txLoadImage("textures/persons/Walk/6.bmp");

		walkL[0] = txLoadImage("textures/persons/Walk/L1.bmp");
		walkL[1] = txLoadImage("textures/persons/Walk/L2.bmp");
		walkL[2] = txLoadImage("textures/persons/Walk/L3.bmp");
		walkL[3] = txLoadImage("textures/persons/Walk/L4.bmp");
		walkL[4] = txLoadImage("textures/persons/Walk/L5.bmp");
		walkL[5] = txLoadImage("textures/persons/Walk/L6.bmp");
	}

	void draw(int x, int y, bool left, bool right, bool& open_inv) {
		if (open_inv == false)
		{
			HDC* currentWalk = right ? walk : left ? walkL : walk;
			int index = (cadr / 2) % 4;

			Win32::TransparentBlt(txDC(), x, y, 64, 64, currentWalk[index], 0, 0, 14, 15, TX_WHITE);

			cadr = (cadr + 1) % 16;
		}
	}


};
///////////////////////////



int main() {
	txCreateWindow(1080, 720);
	txTextCursor(false);
	bool loading = true;
	int precent = 0;
	int w;
	int b;
	int W = w = TX_WHITE;
	bool running = true;
	int B = b = TX_BLACK;
	int sleep = 2;
	string loadd = "Загрузка программы ";
	int load_count = 1;
	bool game = false;
	bool bool_about = false;
	bool bool_settings = false;
	bool menu = true;
	int x = 1;
	int y = 550;
	bool move = false;
	bool right = false;
	bool left = false;
	bool down = false;
	bool up = false;
	bool pause = false;
	int location = 1;
	int mouse_x, mouse_y;
	int x3 = 250;
	int y1 = 200;
	bool open_inv = false;
	bool open_car = false;
	Player blt;
	Background bg = { txLoadImage("textures/bg/mainbg.bmp") };
	Button start = { 0,300,150,40, W , B, "Начать игру" };
	Button about = { 0, 400, 120, 40, W, B , "Об авторе" };
	Button settings = { 0, 350, 150, 40, W, B , "Настройки" };
	Button exit = { 0, 450, 120, 40, W, B , "Выход" };
	Button back = { 0 , 680 ,150 , 40 , W, B, "Вернуться" };
	PlayerAnimation anim;
	GameBackground gamebg = { txLoadImage("textures/bg/city/car_trees.bmp"), txLoadImage("textures/bg/city/road.bmp"), txLoadImage("textures/bg/city/houses&trees_bg.bmp") };
	Inventar invent;

	while (running) {
		// Очищаем экран перед отрисовкой
		txClear();
		txBegin();
		

		// Если в меню, то отображаем фон и кнопки
		if (menu) {
			bg.drawbg();
			start.draw_b();
			about.draw_b();
			settings.draw_b();
			exit.draw_b();
		}


		if (open_inv && GetAsyncKeyState(0x45) == false) {
			invent.open_inventar();
			if (open_car) (invent.open_car());
			
		}
		
		if (GetAsyncKeyState(0x45) && open_inv == true) {
			txSleep(50);
			open_inv = false;
			cout << open_inv;
		}

		// Если в игре, отрисовываем игру
		if (game) {
			mouse_x, mouse_y = txMouseX(), txMouseY();
			if (open_inv != true)
			{
				gamebg.draw(location, x, y, open_inv);

				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 and txMouseX() >= x3 and txMouseX() <= x3 + 380 and x < x3 + 420 and x + 64 > x3 - 40) {
					open_inv = true;
					open_car = true;
					invent.random_loot();
				}
				
				if (GetAsyncKeyState(0x45) && open_inv == false) {
					open_inv = true;
					open_car = false;
					invent.open_inventar();
				}

				if (GetAsyncKeyState(VK_ESCAPE) && !pause) {
					game = false;
					pause = true;
				}

				if (y > 550) y = 550;
				if (x < 0) x = 0;

				if (!move && y <= 550) {
					anim.Idle(x, y, left, right, open_inv);
					txSleep(50);
				}

				if (GetAsyncKeyState(0x57) & 0x8000 && location != 1) {
					y -= 5;
					up = true; down = false;
					blt.draw(x, y, left, right, open_inv);
					move = true;
					txSleep(10);
				}
				else if (GetAsyncKeyState(0x53) & 0x8000 && y <= 550) {
					y += 5;
					up = false; down = true;
					blt.draw(x, y, left, right, open_inv);
					move = true;
					txSleep(10);
				}
				else if (GetAsyncKeyState(0x41) & 0x8000 && x >= 1) {
					x -= 5;
					left = true;
					right = false;
					gamebg.animbg(right, left,x3);
					blt.draw(x, y, left, right, open_inv);
					move = true;
					txSleep(10);
				}
				else if (GetAsyncKeyState(0x44) & 0x8000) {
					x += 5;
					left = false;
					right = true;
					gamebg.animbg(right, left,x3);
					blt.draw(x, y, left, right, open_inv);
					move = true;
					txSleep(10);
				}
				else if (move) {
					anim.Idle(x, y, left, right, open_inv);
					move = false;
					txSleep(50);
				}
			}
		}
		if (menu) {
			/// Обо мне ///
			if (bool_about) {
				txSetFillColor(B);
				txSetColor(B);
				txRectangle(0, 0, 1080, 720);
				txSetColor(W);
				txTextOut(0, 0, "Об авторе:");
				txTextOut(0, 30, "Если честно, не знаю, что тут писать, но со мной можно связаться по телеграмму: @Manfy4");
				back.draw_b();
				if (back.pressed()) {
					bool_about = false;
					menu = true;
				}
			}

			/// Кнопка "Начать игру" ///
			if (start.pressed()) {
				menu = false;
				game = true;
			}

			/// Кнопка "Обо мне" ///
			if (about.pressed()) {
				menu = false;
				bool_about = true;
			}

			/// Кнопка "Выход" ///
			if (exit.pressed()) {
				running = false;
			}
		}

		txEnd();
	}

	txDisableAutoPause();
	return 0;
}