#pragma once

#include <vector>
using namespace std ;

enum CELL { cHuman,cComp,cEmpty };   // ������������ ����� �����
enum STEP { sHuman,sComp };          // ������������ ����� �����

// ��������� ������ � ���� - ��, ��, ��� ����
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

// ��������� ����� �� ����
class CompareStep{
public:
	bool operator()(const ONESTEP& s1, const ONESTEP& s2) const {
		return s1.weight > s2.weight;
	}
};

// �������� ����� ����
class Game {
private:
	CELL ** cells ;     // ������
public:
	Game();
	static int Size();  // ������ ����
	static bool isValidCell(int x, int y);   // ��������� �� ���� (��������� ������ ������)
	bool isValidStep(int xs, int ys, int xf, int yf);	// ��������� �� ���
	bool isGrabStep(int xs, int ys, int xf, int yf);	// �������� �� ��� - ����� ������
	bool isExistGrabStep(bool iscomp);	     // ���������� �� ��� ������ �� �����
	CELL getCell(int x, int y);	             // �������� ������
	bool isCellEnemyFor(int xtest, int ytest, int x,int y);	// �������� �� ������ ��������� �� ��������� 
	bool isCellMy(int xtest, int ytest, int x,int y);       // �������� �� ������ ����������� �� ���������
	bool isCellComp(int x,int y);	// �������� �� ������ ������������� ������
	bool isCellHuman(int x,int y);	// �������� �� ������ ������ ������
	int countHuman();   // ����� ����� ������
	int countComp();	// ����� ����� ����������
	bool moveObject(int xs, int ys, int xf, int yf);  // ����������� ������
	bool testWin(bool * win, STEP step) ;             // �������� ������
	bool isGameOver(bool* win, STEP step);
	vector<ONESTEP>* buildSteps(bool iscomp) ;        // ��������� ������ ��������� �����
	Game * createClone() ;	// ����������� �����
};