#include "stdafx.h"
#include "Game.h"

Game::Game(){
	// �������� ������
	cells = new CELL*[Size()] ;
	for (int x=0; x<Size(); x++) 
		cells[x]= new CELL[Size()] ;

	// ��������� ������� � �����������
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

bool Game::moveObject(int xs, int ys, int xf, int yf) {  //����������� ������
	if (abs(xs-xf)>1) {           // ������ ����� ������ - ������ ������
		int xo = xs+(xf-xs)/2;
		int yo = ys+(yf-ys)/2;
		if (isCellEnemyFor(xo,yo,xs,ys)) cells[xo][yo]=cEmpty;
	}
	cells[xf][yf]=cells[xs][ys];  // ����������� ������
	cells[xs][ys]=cEmpty ;
	return true ;
}

int Game::Size() {    // ������ ����
	return 8 ;
}

bool Game::isValidCell(int x, int y) {  // ��������� �� ���� (��������� ������ ������)
	return (x+y) % 2 == 1 ;
}

bool Game::isCellEnemyFor(int xtest, int ytest, int x,int y) {  // �������� �� ������ ��������� �� ��������� 
	if (getCell(xtest, ytest) == cEmpty) return false;
	if (getCell(xtest, ytest) == cComp) return (getCell(x, y) == cHuman);
	if (getCell(xtest, ytest) == cHuman) return (getCell(x, y) == cComp);
}

bool Game::isCellMy(int xtest, int ytest, int x,int y) {  // �������� �� ������ ����������� �� ���������
	if (getCell(xtest, ytest) == cEmpty) return false;
	if (getCell(xtest, ytest) == cComp) return (getCell(x, y) == cComp);
	if (getCell(xtest, ytest) == cHuman) return (getCell(x, y) == cHuman);
}	

bool Game::isValidStep(int xs, int ys, int xf, int yf) {   // ��������� �� ���
	// ������������ ������
	if (!isValidCell(xf,yf)) return false ;
	// �������� ������
	if (getCell(xf,yf)!=cEmpty) return false ;

	// �������� ����������� ���� �� �����
	int dy = (getCell(xs, ys) == cHuman);
	if (getCell(xs, ys) == cHuman) 
		dy = -1;
	else 
		dy = 1;

	// ���� �������� �� ���� ������ - ��
	if ((xf==xs+1)&&(yf==ys+dy)) return true ;
	if ((xf==xs-1)&&(yf==ys+dy)) return true ;

	// ����� �������� ��� �������� ������
	return isGrabStep(xs, ys, xf, yf);
	
}

bool Game::isGrabStep(int xs, int ys, int xf, int yf) {   // �������� �� ��� - ����� ������
	// ������������ ������
	if (!isValidCell(xf, yf)) return false;
	// �������� ������
	if (getCell(xf, yf) != cEmpty) return false;
			
	for (int dy = -1; dy <= 1; dy += 2) {
		// ���� ��������� ����� ���� ������ � ������� ������ ��������� - ��
		if ((xf == xs + 2) && (yf == ys + 2 * dy) && (isCellEnemyFor(xs + 1, ys + dy, xs, ys))) return true;
		if ((xf == xs - 2) && (yf == ys + 2 * dy) && (isCellEnemyFor(xs - 1, ys + dy, xs, ys))) return true;
	}

	return false;
}

int Game::countHuman() {  // ����� ����� ������
	int cnt=0 ;
	for (int xs=0; xs<Size(); xs++)
		for (int ys=0; ys<Size(); ys++) 
			if (isCellHuman(xs,ys)) cnt++ ;
	return cnt ;
}

int Game::countComp() {  // ����� ����� ����������
	int cnt=0 ;
	for (int xs=0; xs<Size(); xs++)
		for (int ys=0; ys<Size(); ys++) 
			if (isCellComp(xs,ys)) cnt++ ;
	return cnt ;
}	

bool Game::testWin(bool * win, STEP step) {    // �������� ������
	vector<ONESTEP> * steps = buildSteps(step) ;
	// ���� ��� �����, �� ������� �� ����������
	if (steps->size()==0) {
		*win = countHuman() < countComp();
		return true ;
	}
		
	// ����������� ������
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

bool Game::isCellComp(int x,int y) {   // �������� �� ������ ������������� ������
	return getCell(x,y)==cComp ;
}

bool Game::isCellHuman(int x,int y) {  // �������� �� ������ ������ ������
	return getCell(x,y)==cHuman ;
}

bool Game::isExistGrabStep(bool iscomp) {  //���������� �� ��� ������ �� �����
	// ���������� ��� ���������� �����, ���� ������� ��� ������ - True, ���� ��� - False	
	for (int xs = 0; xs < Size(); xs++)
		for (int ys = 0; ys < Size(); ys++)
			if (((isCellHuman(xs, ys)) && (!iscomp)) ||
				((isCellComp(xs, ys)) && (iscomp)))
				for (int xf = 0; xf < Size(); xf++)
					for (int yf = 0; yf < Size(); yf++)
						if (getCell(xf, yf) == cEmpty)
							// ���� ������ ��� �������� ����� ������, �� �������
							if (isGrabStep(xs, ys, xf, yf))
								return true;
	return false;
}

vector<ONESTEP>* Game::buildSteps(bool iscomp) {   //C����� ������ ���� ��������� �����
	vector<ONESTEP> * steps = new vector<ONESTEP>() ;
	// ���������, ���������� �� ���� ������
	bool existgrab = isExistGrabStep(iscomp);

	// ���������� ��� ���������� �����, �������� ������ ����������	
	for (int xs=0; xs<Size(); xs++)
		for (int ys=0; ys<Size(); ys++)
			if ( ((isCellHuman(xs,ys))&&(!iscomp))||
				  ((isCellComp(xs,ys))&&(iscomp)) )
				for (int xf=0; xf<Size(); xf++)
					for (int yf=0; yf<Size(); yf++)
						if (getCell(xf, yf) == cEmpty) {
							// ��� ������� ����� ������, ��� ����������� - ��������� ������ ��
							if (existgrab) {
								if (isGrabStep(xs, ys, xf, yf))
									steps->push_back(ONESTEP(xs, ys, xf, yf));
							}
							else {
								// ����� ��� ��������� ����
								if (isValidStep(xs, ys, xf, yf))
									steps->push_back(ONESTEP(xs, ys, xf, yf));
							}
						}
	
	return steps ;
}

CELL Game::getCell(int x, int y) {  // �������� ������
	return cells[x][y] ;
}

Game * Game::createClone() {        // ����������� �����
	Game * gn = new Game() ;
	for (int xs=0; xs<Size(); xs++)
		for (int ys=0; ys<Size(); ys++)
			gn->cells[xs][ys]=cells[xs][ys] ;
	return gn ;
}