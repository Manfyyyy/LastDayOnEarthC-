#include "TXLib.h"
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <vector>
#include <ctime>


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
	HDC med;
	HDC water;
	
	int random_predmet = 0;
	bool itemPlaced = false;

	bool bool_water = false, bool_med = false;
	int k_water = 0, k_med = 0;

	const int SLOT_WIDTH = 90;
	const int SLOT_HEIGHT = 90;

	std::map<int, int> itemCount;

	std::vector<std::pair<int, int>> carSlots = {
		{550,  30}, {650,  30}, {750,  30}, {850,  30}, {950,  30},
		{550, 130}, {650, 130}, {750, 130}, {850, 130}, {950, 130},
		{550, 230}, {650, 230}, {750, 230}, {850, 230}, {950, 230}
	};

	std::vector<std::pair<int, int>> invSlots = {
		{50,  30}, {150,  30}, {250,  30}, {350,  30}, {450,  30},
		{50, 130}, {150, 130}, {250, 130}, {350, 130}, {450, 130},
		{50, 230}, {150, 230}, {250, 230}, {350, 230}, {450, 230}
	};

	

	std::map<int, std::pair<int, int>> inventory;

	std::pair<int, int> currentSlot = { 0, 0 };

	Inventar() {
		med = txLoadImage("textures/items/heal_texture.bmp");
		water = txLoadImage("textures/items/water_texture.bmp");

		if (!med || !water) {
			txMessageBox("Ошибка загрузки текстур!", "Ошибка", MB_OK | MB_ICONERROR);
		}

		itemCount[1] = 0;  // Вода
		itemCount[2] = 0;
	}

	std::pair<int, int> getRandomSlot() {
		int index = rand() % carSlots.size();
		return carSlots[index];
	}

	void open_car() {
		if (!itemPlaced) {
			random_predmet = rand() % 2 + 1;
			currentSlot = getRandomSlot();
			itemPlaced = true; 
		}

		txSetFillColor(TX_BLACK);
		txSetColor(TX_BLACK);

		// Разделительные линии
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

		int waterWidth = 37, waterHeight = 59;
		int medWidth = 32, medHeight = 27;

		float scaleX_water = static_cast<float>(SLOT_WIDTH - 10) / waterWidth;
		float scaleY_water = static_cast<float>(SLOT_HEIGHT - 10) / waterHeight;
		float scaleX_med = static_cast<float>(SLOT_WIDTH - 10) / medWidth;
		float scaleY_med = static_cast<float>(SLOT_HEIGHT - 10) / medHeight;

		float scaleWater = std::min(scaleX_water, scaleY_water);
		float scaleMed = std::min(scaleX_med, scaleY_med);

		int newWaterWidth = static_cast<int>(waterWidth * scaleWater);
		int newWaterHeight = static_cast<int>(waterHeight * scaleWater);
		int newMedWidth = static_cast<int>(medWidth * scaleMed);
		int newMedHeight = static_cast<int>(medHeight * scaleMed);

		int xWater = currentSlot.first + ((SLOT_WIDTH - newWaterWidth) / 2) + 5;
		int yWater = currentSlot.second + ((SLOT_HEIGHT - newWaterHeight) / 2) - 15;
		int xMed = currentSlot.first + ((SLOT_WIDTH - newMedWidth) / 2) + 5;
		int yMed = currentSlot.second + ((SLOT_HEIGHT - newMedHeight) / 2) - 8;

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 and txMouseX() >= xWater - 15 and txMouseX() <= xWater + newWaterWidth + 15 and txMouseY() >= yWater and txMouseY() <= yWater + newWaterHeight and random_predmet == 1) {
			addItemToInventory(1);
			itemPlaced = false;
		}

		else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 and txMouseX() >= xMed and txMouseX() <= xMed + newMedWidth and txMouseY() >= yMed and txMouseY() <= yMed + newMedHeight and random_predmet == 2) {
			addItemToInventory(2);
			itemPlaced = false;
		}

		if (random_predmet == 1 && water) {
			Win32::TransparentBlt(txDC(), xWater, yWater, newWaterWidth, newWaterHeight, water, 0, 0, 37, 59, TX_WHITE);
		}
		else if (random_predmet == 2 && med) {
			Win32::TransparentBlt(txDC(), xMed, yMed, newMedWidth, newMedHeight, med, 0, 0, 32, 27, TX_WHITE);
		}
	}

	void addItemToInventory(int itemType) {
		if (itemCount[itemType] == 0) {
			// Если предмета еще нет в инвентаре, находим первый свободный слот
			for (const auto& slot : invSlots) {
				bool occupied = false;
				for (const auto& pair : inventory) {
					if (pair.second == slot) {
						occupied = true;
						break;
					}
				}
				if (!occupied) {
					inventory[itemType] = slot;
					break;
				}
			}
		}
		itemCount[itemType]++;
	}
	
	
	
	void open_inventar() {
		txBegin();


		int waterWidth = 37, waterHeight = 59;
		int medWidth = 32, medHeight = 27;

		float scaleX_water = static_cast<float>(SLOT_WIDTH - 10) / waterWidth;
		float scaleY_water = static_cast<float>(SLOT_HEIGHT - 10) / waterHeight;
		float scaleX_med = static_cast<float>(SLOT_WIDTH - 10) / medWidth;
		float scaleY_med = static_cast<float>(SLOT_HEIGHT - 10) / medHeight;

		float scaleWater = std::min(scaleX_water, scaleY_water);
		float scaleMed = std::min(scaleX_med, scaleY_med);

		int newWaterWidth = static_cast<int>(waterWidth * scaleWater);
		int newWaterHeight = static_cast<int>(waterHeight * scaleWater);
		int newMedWidth = static_cast<int>(medWidth * scaleMed);
		int newMedHeight = static_cast<int>(medHeight * scaleMed);


		txSetFillColor(TX_BLACK);
		txSetColor(TX_BLACK);
		txRectangle(500, 0, 560, 360);
		txRectangle(0, 100, 540, 120);
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

		for (const auto& pair : inventory) {
			int itemType = pair.first;
			auto slot = pair.second;
			int x = slot.first;
			int y = slot.second;

			if (itemType == 1) {
				Win32::TransparentBlt(txDC(), x-15, y-10, newWaterWidth, newWaterHeight, water, 0, 0, 37, 59, TX_WHITE);
			}
			else if (itemType == 2) {
				Win32::TransparentBlt(txDC(), x-30, y+5, newMedWidth, newMedHeight, med, 0, 0, 32, 27, TX_WHITE);
			}

			txSelectFont("Arial", 20);
			txDrawText(x + 40, y + 60, x + 80, y + 80, std::to_string(itemCount[itemType]).c_str());
		}

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
	void cnhage_lan(string& language, string& language_text, string& start_text, string& about_me_text, string& settings_text, string& exit_text, string& back_text){
		
		
		if (language == "Русский"){
			language = "English";
			language_text = "Language: ";
			start_text = "Start";
			about_me_text = "About me";
			settings_text = "Settings";
			exit_text = "Exit";
			back_text = "Back";
		}
		
		else if (language == "English")
		{
			language = "Русский";
			language_text = "Язык: ";
			start_text = "Начать игру";
			about_me_text = "Об авторе";
			settings_text = "Настройки";
			exit_text = "Выход";
			back_text = "Вернуться";
		}
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
	bool prev_state = false;
	std::string language = "Русский";
	std::string language_text = "Язык: ";
	std::string start_text = "Начать игру";
	std::string about_me_text = "Об авторе";
	std::string settings_text = "Настройки";
	std::string exit_text = "Выход";
	std::string back_text = "Вернуться";


	Player blt;
	Background bg = { txLoadImage("textures/bg/mainbg.bmp")};
	Button start = { 0,300,150,40, W , B, start_text.c_str()};
	
	Button about = { 0, 400, 120, 40, W, B , about_me_text.c_str()};
	Button settings = { 0, 350, 150, 40, W, B , settings_text.c_str()};
	Button exit = { 0, 450, 120, 40, W, B , exit_text.c_str()};
	Button back = { 0 , 680 ,150 , 40 , W, B, back_text.c_str()};
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
		if (GetAsyncKeyState(0x45) && open_inv == true && !prev_state) {
			txSleep(100);
			open_inv = false;
		}
		if (!(GetAsyncKeyState(0x45) & 0x8000) && open_inv) {
			txSleep(50);
			prev_state = false;
		}

		if (GetAsyncKeyState(VK_ESCAPE) && pause) {
			txSleep(100);
			game = true;
			pause = false;
			
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
				}
				if (GetAsyncKeyState(0x45) && open_inv == false && !prev_state) {
					prev_state = true;
					txSleep(100);
					open_inv = true;
					open_car = false;
					invent.open_inventar();
				}
				

				if (GetAsyncKeyState(VK_ESCAPE) && !pause) {
					txSleep(100);
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
		
		if (bool_about) {
			txClear(); // Очищаем экран (делает фон белым)
			txSetColor(TX_BLACK);  // Устанавливаем цвет текста в чёрный
			if (language == "Русский")
			{
				txTextOut(0, 0, "Об авторе:");
				txTextOut(0, 30, "Если честно не знаю, что тут писать, но со мной можно связаться по телеграмму: @Manfy4");
			}
			else {
				txTextOut(0, 0, "About me:");
				txTextOut(0, 30, "To be honest, I don't know what to write here, but you can contact me by telegram: @Manfy4");
			}
			back.draw_b();
			if (back.pressed()) {
				bool_about = false;
				menu = true;
			}
		}
		
		if (bool_settings) {
			txClear();
			std::string language_button_text = language_text + language;
			Button languagge = { 0, 50, 200, 40, W, B, language_button_text.c_str() };
			languagge.draw_b();
			language = language.c_str();
			language_text = language_text.c_str();
			back.draw_b();
			if (languagge.pressed()) {
				txSleep(100);
				languagge.cnhage_lan(language, language_text, start_text, about_me_text, settings_text, exit_text, back_text);
				txClear();				
			}
			if (back.pressed()) {
				menu = true;
				bool_settings = false;
			}
		
		}


		if (menu) {
			/// Обо мне ///
			

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

			if (settings.pressed()) {
				
				menu = false;
				bool_settings = true;
				
				
			}

		}

		txEnd();
	}

	txDisableAutoPause();
	return 0;
}