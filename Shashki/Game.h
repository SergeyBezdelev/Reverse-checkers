#pragma once

#include <vector>
using namespace std ;

enum CELL { cHuman,cComp,cEmpty };   // Перечисление типов ячеек
enum STEP { sHuman,sComp };          // Перечисление типов ходов

// Структура записи о ходе - от, до, вес хода
struct ONESTEP {
	int xs ;
	int ys ;
	int xf ;
	int yf ;
	int weight ;
	ONESTEP() {
		xs=0;
		ys=0;
		xf=0;
		yf = 0;
		weight = 0;
	}
	ONESTEP(int Axs,int Ays, int Axf, int Ayf) {
		xs=Axs ;
		ys=Ays ;
		xf=Axf ;
		yf=Ayf ;
		weight = 0 ;
	}
} ;

// Сравнение ходов по весу
class CompareStep{
public:
	bool operator()(const ONESTEP& s1, const ONESTEP& s2) const {
		return s1.weight > s2.weight;
	}
};

// Основной класс игры
class Game {
private:
	CELL ** cells ;     // Ячейки
public:
	Game();
	static int Size();  // Размер поля
	static bool isValidCell(int x, int y);   // Корректно ли поле (разрешены черные клетки)
	bool isValidStep(int xs, int ys, int xf, int yf);	// Корректен ли ход
	bool isGrabStep(int xs, int ys, int xf, int yf);	// Является ли ход - ходом взятия
	bool isExistGrabStep(bool iscomp);	     // Существует ли ход взятия на доске
	CELL getCell(int x, int y);	             // Получить ячейку
	bool isCellEnemyFor(int xtest, int ytest, int x,int y);	// Является ли клетка вражеской по отношению 
	bool isCellMy(int xtest, int ytest, int x,int y);       // Является ли клетка собственной по отношению
	bool isCellComp(int x,int y);	// Является ли клетка компьютерного игрока
	bool isCellHuman(int x,int y);	// Является ли клетка своего игрока
	int countHuman();   // Число фигур игрока
	int countComp();	// Число фигур компьютера
	bool moveObject(int xs, int ys, int xf, int yf);  // Передвинуть фигуру
	bool testWin(bool * win, STEP step) ;             // Проверка победы
	bool isGameOver(bool* win, STEP step);
	vector<ONESTEP>* buildSteps(bool iscomp) ;        // Построить список возможных ходов
	Game * createClone() ;	// Клонировать доску
};