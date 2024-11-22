#include "Game.h"

// Класс игрока компьютера
class Player {
private:
	Game * game ;	// Ссылка на игру
	// Функция оценки хода для аргумента-функции
	static int getStepWeight(Game* stepgame);
	// Функция минимакса для аргумента-функции, с учетом типа хода
	ONESTEP algMinMax(Game* stepgame, bool comp);
	// Функция Aльфа-Бетта
	ONESTEP alphaBeta(Game* stepgame, bool comp, int depth, int alpha, int beta);
public:
	Player(Game * Agame);
	bool makeStep() ;   // Выполнить шаг игрока
};