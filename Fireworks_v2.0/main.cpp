#include <SFML/Graphics.hpp>
using namespace sf;

const float a = 0.05;	// Ускорение свободного падения
const int Xwin = 1000,	// Ширина экрана
		  Ywin = 650,	// Длина экрана
		  CountF = 20,	// Кол-во ракет
		  MaxP = 100,	// Макс кол-во партиклов
		  MinP = 20;	// Минимальное кол-во партиклов

RenderWindow window(sf::VideoMode(Xwin, Ywin), "FireworksSUKABLUAT", Style::Close);
CircleShape particle(1), rocket(3);

class Firework {
private:
	const int MinColor = 20;	// Минимальное значение цвета
public:
	Vector2f coor, v;	// Координата, скорость
	bool isBurst;		// Проверка на взрыв
	int CountP, alpha,	// Кол-во партиклов, прозрачность и продолжительность жизни
		color[3];		// Цвет 
	Firework() {
		update();
	}
	void update() {		// Задает начальные значения ракете
		coor.x = rand() % (Xwin - 5) + 5,
		coor.y = rand() % (Ywin / 2) + Ywin;
		v = {0, (float)(rand() % 5 + 5)};
		isBurst = false;
		CountP = rand() % (MaxP - MinP) + MinP;
		alpha = 255;
		color[0] = rand() % (255 - MinColor) + MinColor, 
		color[1] = rand() % (255 - MinColor) + MinColor,
		color[2] = rand() % (255 - MinColor) + MinColor;
	}	 
};

class Particle {
private:
	float RandDir[2] {-1, 1};	// Рандомизирует направление полета частиц
public:
	Vector2f coor, v, tail[5];	// Координата, скорость, хвост от партикла
	Particle() {		
		update();
	}
	void update() {		// Задает начальное значение партиклам
		v.x = (float)(rand()) / (RAND_MAX) * 2 * RandDir[rand() % 2];
		v.y = (float)(rand()) / (RAND_MAX) * 2 * RandDir[rand() % 2];
		for (int i = 0; i < 5; ++i) {
			tail[i] = { NULL, NULL };
		}
	}
};

int main() {
	window.setFramerateLimit(60);

	Firework f[CountF];
	Particle p[CountF][MaxP];

	Clock clock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {				
				window.close();
			}
		}

		float t = clock.restart().asMilliseconds();
		t /= 10;

		for (int i = 0; i < CountF; ++i) {
			if (f[i].alpha <= 0) {		// Условие выхода из взрыва частиц
				f[i].update();
			}
			else if (f[i].v.y <= 0 && f[i].isBurst == false) {	// Условие выхода из полета ракеты
				f[i].isBurst = true;
				for (int start = 0; start < f[i].CountP; ++start) {
					p[i][start].coor = f[i].coor;
					p[i][start].update();
				}
			}			

			if (f[i].isBurst == true) {		// Функция взрыва ракеты
				for (int j = 0; j < f[i].CountP; ++j) {
					p[i][j].coor += {(float)(p[i][j].v.x * t), (float)(p[i][j].v.y * t + a * t * t * 0.5)};
					p[i][j].v += {0, (float)(a * t)};
					particle.setFillColor(Color(f[i].color[0], f[i].color[1], f[i].color[2], f[i].alpha));
					particle.setPosition(p[i][j].coor);
					window.draw(particle);
					//// Реализация хвоста у частиц ////
					for (int k = 0; k < 4; ++k) {
						p[i][j].tail[k] = p[i][j].tail[k + 1];
						particle.setPosition(p[i][j].tail[k]);
						window.draw(particle);
					}
					p[i][j].tail[4] = p[i][j].coor;
					particle.setPosition(p[i][j].tail[4]);
					window.draw(particle);

				}
				f[i].alpha -= 5;
			}
			else {		// Функция полета ракеты
				f[i].coor -= {0, f[i].v.y * t};
				f[i].v -= {0, a * t};
				rocket.setFillColor(Color(f[i].color[0], f[i].color[1], f[i].color[2], f[i].alpha));
				rocket.setPosition(f[i].coor);
				window.draw(rocket);			
			}
		}
		window.display();
		window.clear();		
	}
}
