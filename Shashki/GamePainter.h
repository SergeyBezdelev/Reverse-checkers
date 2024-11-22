#include "Game.h"

enum CLICKRESULT { crOk, crFinish } ;   // ��������� ����� �� �����

// ����� ���������
class GamePainter {
private:
	Game * game ;//������ �� ����
	// ������� ����� � ������
	int _w ;
	int _h ;
	int dw ;
	int dh ;
	int border ;
	POINT Selected ;         // ��������� ������
	void ClearSelected() ;	 // ������� ���������
	bool isSelected() const; // �������� ���������
public:
	GamePainter();
	void setGame(Game * Agame) ;         // ���������� ���� ��� ���������
	void drawTo(HDC hdc,int w, int h);   // ������� ���� �� ��������
	CLICKRESULT doClick(int cx,int cy);  // ������ �� �����
};