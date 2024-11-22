#include "stdafx.h"
#include "Game.h"

Game::Game(){
	// Создание клеток
	cells = new CELL*[Size()] ;
	for (int x=0; x<Size(); x++) 
		cells[x]= new CELL[Size()] ;

	// Стартовая очистка и расстановка
	for (int x=0; x<Size(); x++) 
		for (int y=0; y<Size(); y++) 
			cells[x][y]=cEmpty ;

	for (int y=0; y<=2; y++)
		for (int x=(y+1) % 2; x<Size(); x+=2)
			cells[x][y]=cComp ;
		
	for (int y=Size()-1; y>=Size()-3; y--)
		for (int x=(y+1) % 2; x<Size(); x+=2)
			cells[x][y]=cHuman ;
}

bool Game::moveObject(int xs, int ys, int xf, int yf) {  //Перемещение фигуры
	if (abs(xs-xf)>1) {           // Прыжок через клетку - взятие фигуры
		int xo = xs+(xf-xs)/2;
		int yo = ys+(yf-ys)/2;
		if (isCellEnemyFor(xo,yo,xs,ys)) cells[xo][yo]=cEmpty;
	}
	cells[xf][yf]=cells[xs][ys];  // Перемещение фигуры
	cells[xs][ys]=cEmpty ;
	return true ;
}

int Game::Size() {    // Размер поля
	return 8 ;
}

bool Game::isValidCell(int x, int y) {  // Корректно ли поле (разрешены черные клетки)
	return (x+y) % 2 == 1 ;
}

bool Game::isCellEnemyFor(int xtest, int ytest, int x,int y) {  // Является ли клетка вражеской по отношению 
	if (getCell(xtest, ytest) == cEmpty) return false;
	if (getCell(xtest, ytest) == cComp) return (getCell(x, y) == cHuman);
	if (getCell(xtest, ytest) == cHuman) return (getCell(x, y) == cComp);
}

bool Game::isCellMy(int xtest, int ytest, int x,int y) {  // Является ли клетка собственной по отношению
	if (getCell(xtest, ytest) == cEmpty) return false;
	if (getCell(xtest, ytest) == cComp) return (getCell(x, y) == cComp);
	if (getCell(xtest, ytest) == cHuman) return (getCell(x, y) == cHuman);
}	

bool Game::isValidStep(int xs, int ys, int xf, int yf) {   // Корректен ли ход
	// Некорректная клетка
	if (!isValidCell(xf,yf)) return false ;
	// Непустая клетка
	if (getCell(xf,yf)!=cEmpty) return false ;

	// Выбираем направление хода по доске
	int dy = (getCell(xs, ys) == cHuman);
	if (getCell(xs, ys) == cHuman) 
		dy = -1;
	else 
		dy = 1;

	// Если смещение на одну клетку - ок
	if ((xf==xs+1)&&(yf==ys+dy)) return true ;
	if ((xf==xs-1)&&(yf==ys+dy)) return true ;

	// Далее проверка уже варианта взятия
	return isGrabStep(xs, ys, xf, yf);
	
}

bool Game::isGrabStep(int xs, int ys, int xf, int yf) {   // Является ли ход - ходом взятия
	// Некорректная клетка
	if (!isValidCell(xf, yf)) return false;
	// Непустая клетка
	if (getCell(xf, yf) != cEmpty) return false;
			
	for (int dy = -1; dy <= 1; dy += 2) {
		// Если сместился через одну клетку и ближняя клетка вражеская - ок
		if ((xf == xs + 2) && (yf == ys + 2 * dy) && (isCellEnemyFor(xs + 1, ys + dy, xs, ys))) return true;
		if ((xf == xs - 2) && (yf == ys + 2 * dy) && (isCellEnemyFor(xs - 1, ys + dy, xs, ys))) return true;
	}

	return false;
}

int Game::countHuman() {  // Число фигур игрока
	int cnt=0 ;
	for (int xs=0; xs<Size(); xs++)
		for (int ys=0; ys<Size(); ys++) 
			if (isCellHuman(xs,ys)) cnt++ ;
	return cnt ;
}

int Game::countComp() {  // Число фигур компьютера
	int cnt=0 ;
	for (int xs=0; xs<Size(); xs++)
		for (int ys=0; ys<Size(); ys++) 
			if (isCellComp(xs,ys)) cnt++ ;
	return cnt ;
}	

bool Game::testWin(bool * win, STEP step) {    // Проверка победы
	vector<ONESTEP> * steps = buildSteps(step) ;
	// Если нет ходов, то подсчет по количеству
	if (steps->size()==0) {
		*win = countHuman() < countComp();
		return true ;
	}
		
	// Закончились фигуры
	if (countHuman()==0) {
		*win=true ;
		return true ;
	}

	if (countComp()==0) {
		*win=false ;
		return true ;
	}

	return false ;
}

bool Game::isGameOver(bool* win, STEP step) {
	return testWin(win, step);
}

bool Game::isCellComp(int x,int y) {   // Является ли клетка компьютерного игрока
	return getCell(x,y)==cComp ;
}

bool Game::isCellHuman(int x,int y) {  // Является ли клетка своего игрока
	return getCell(x,y)==cHuman ;
}

bool Game::isExistGrabStep(bool iscomp) {  //Существует ли ход взятия на доске
	// Перебираем все комбинации ходов, если находит что съесть - True, если нет - False	
	for (int xs = 0; xs < Size(); xs++)
		for (int ys = 0; ys < Size(); ys++)
			if (((isCellHuman(xs, ys)) && (!iscomp)) ||
				((isCellComp(xs, ys)) && (iscomp)))
				for (int xf = 0; xf < Size(); xf++)
					for (int yf = 0; yf < Size(); yf++)
						if (getCell(xf, yf) == cEmpty)
							// Если данный ход является ходом взятия, то возврат
							if (isGrabStep(xs, ys, xf, yf))
								return true;
	return false;
}

vector<ONESTEP>* Game::buildSteps(bool iscomp) {   //Cтроит список всех возможнвх ходов
	vector<ONESTEP> * steps = new vector<ONESTEP>() ;
	// Проверяем, существуют ли ходы взятия
	bool existgrab = isExistGrabStep(iscomp);

	// Перебираем все комбинации ходов, оставляя только корректные	
	for (int xs=0; xs<Size(); xs++)
		for (int ys=0; ys<Size(); ys++)
			if ( ((isCellHuman(xs,ys))&&(!iscomp))||
				  ((isCellComp(xs,ys))&&(iscomp)) )
				for (int xf=0; xf<Size(); xf++)
					for (int yf=0; yf<Size(); yf++)
						if (getCell(xf, yf) == cEmpty) {
							// При наличии ходов взятия, они обязательны - добавляем только их
							if (existgrab) {
								if (isGrabStep(xs, ys, xf, yf))
									steps->push_back(ONESTEP(xs, ys, xf, yf));
							}
							else {
								// Иначе все возможные ходы
								if (isValidStep(xs, ys, xf, yf))
									steps->push_back(ONESTEP(xs, ys, xf, yf));
							}
						}
	
	return steps ;
}

CELL Game::getCell(int x, int y) {  // Получить ячейку
	return cells[x][y] ;
}

Game * Game::createClone() {        // Клонировать доску
	Game * gn = new Game() ;
	for (int xs=0; xs<Size(); xs++)
		for (int ys=0; ys<Size(); ys++)
			gn->cells[xs][ys]=cells[xs][ys] ;
	return gn ;
}