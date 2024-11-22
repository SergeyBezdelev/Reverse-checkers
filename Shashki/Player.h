#include "Game.h"

// ����� ������ ����������
class Player {
private:
	Game * game ;	// ������ �� ����
	// ������� ������ ���� ��� ���������-�������
	static int getStepWeight(Game* stepgame);
	// ������� ��������� ��� ���������-�������, � ������ ���� ����
	ONESTEP algMinMax(Game* stepgame, bool comp);
	// ������� A����-�����
	ONESTEP alphaBeta(Game* stepgame, bool comp, int depth, int alpha, int beta);
public:
	Player(Game * Agame);
	bool makeStep() ;   // ��������� ��� ������
};