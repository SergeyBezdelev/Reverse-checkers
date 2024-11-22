#include "stdafx.h"
#include "GamePainter.h"

GamePainter::GamePainter(){               // ��������� ������
	game = NULL ;
	ClearSelected() ;
}

void GamePainter::ClearSelected() {       // ������� ���������
	Selected.x=-1 ;
	Selected.y=-1 ;
}

 bool  GamePainter::isSelected() const {  // �������� ���������
	return (Selected.x!=-1) ;
}

CLICKRESULT GamePainter::doClick(int cx,int cy) {  // ������ �� �����
	POINT clicked;
	bool find = false ;

	for (int x=0; x<game->Size(); x++) 
		for (int y=0; y<game->Size(); y++) 
			if ((cx>border+x*dw)&&(cy>border+y*dh)&&(cx<border+(x+1)*dw)&&(cy<border+(y+1)*dh)) {
				clicked.x = x ;
				clicked.y = y ;
				find = true ;
			}

	if (!find) return crOk ;

	if (game->getCell(clicked.x,clicked.y)==cHuman) {
		Selected = clicked ;
		return crOk ;
	}

	if (isSelected()) {	
		// ��������� ���� ������
		bool existgrab = game->isExistGrabStep(false);
		// ���� ���� ������ ����, �� ��������� ���� ��, ����� ��� ���������
		bool z = existgrab ? game->isGrabStep(Selected.x, Selected.y, clicked.x, clicked.y) :
			game->isValidStep(Selected.x, Selected.y, clicked.x, clicked.y);
		if (z) {
			game->moveObject(Selected.x,Selected.y,clicked.x,clicked.y) ;
			ClearSelected() ;
			return crFinish ;
		}
		else 
			return crOk ;
	}
	return crOk ;
}

void GamePainter::drawTo(HDC hdc,int w, int h) {    // ������� ���� �� ��������
	_w = w ;
	_h = h ;

	border = 10 ;
	dw = (w-border*3)/game->Size() ;
	dh = (h-border*3-50)/game->Size() ;

	COLORREF gray = RGB(80,80,80) ;
	COLORREF black = RGB(0,0,0) ;
	COLORREF white = RGB(255,255,255) ;
	COLORREF green = RGB(100,255,100) ;

	HBRUSH brushGray = CreateSolidBrush(gray) ;
	HBRUSH brushBlack = CreateSolidBrush(black) ;
	HBRUSH brushWhite = CreateSolidBrush(white) ;
	HBRUSH brushGreen = CreateSolidBrush(green) ;
	
	HPEN pen = CreatePen(PS_SOLID, 1,gray) ;
	
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, brushGray);
	HPEN OldPen = (HPEN)SelectObject(hdc, pen);

	for (int x=0; x<game->Size(); x++) 
		for (int y=0; y<game->Size(); y++) {
			// ��������� ������, ���� ����� - � �����, �� ����������� ����������� �����
			if (game->isValidCell(x,y)) SelectObject(hdc, brushGray); else SelectObject(hdc,OldBrush) ;
			if ((Selected.x==x)&&(Selected.y==y)) SelectObject(hdc,brushGreen) ;
			Rectangle(hdc,border+x*dw,border+y*dh,border+(x+1)*dw,border+(y+1)*dh) ;
			
			if (game!=NULL) {   // ����� �����
				if (game->getCell(x,y)==cComp) {
					SelectObject(hdc,brushBlack) ;
					Ellipse(hdc,border+x*dw+3,border+y*dh+3,border+(x+1)*dw-3,border+(y+1)*dh-3) ;
				}
				if (game->getCell(x,y)==cHuman) {
					SelectObject(hdc,brushWhite) ;
					Ellipse(hdc,border+x*dw+3,border+y*dh+3,border+(x+1)*dw-3,border+(y+1)*dh-3) ;
				}				
			}
		}
	SelectObject(hdc,OldBrush) ;
	DeleteObject(brushGray) ;
	DeleteObject(brushBlack) ;
	DeleteObject(brushWhite) ;
	SelectObject(hdc,OldPen) ;
	DeleteObject(pen) ;	
}

void GamePainter::setGame(Game * Agame) {    // ���������� ���� ��� ���������
	game = Agame ;
}