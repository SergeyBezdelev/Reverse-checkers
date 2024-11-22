#include "stdafx.h"
#include "GamePainter.h"

GamePainter::GamePainter(){               // Выбранная клетка
	game = NULL ;
	ClearSelected() ;
}

void GamePainter::ClearSelected() {       // Очистка выбранной
	Selected.x=-1 ;
	Selected.y=-1 ;
}

 bool  GamePainter::isSelected() const {  // Проверка выбранной
	return (Selected.x!=-1) ;
}

CLICKRESULT GamePainter::doClick(int cx,int cy) {  // Щелчок по доске
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
		// Проверяем ходы взятия
		bool existgrab = game->isExistGrabStep(false);
		// Если ходы взятия есть, то разрешаем лишь их, иначе все возможные
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

void GamePainter::drawTo(HDC hdc,int w, int h) {    // Вывести игру на контекст
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
			// Рисование клетки, если нужно - с фоном, но обязательно правильного цвета
			if (game->isValidCell(x,y)) SelectObject(hdc, brushGray); else SelectObject(hdc,OldBrush) ;
			if ((Selected.x==x)&&(Selected.y==y)) SelectObject(hdc,brushGreen) ;
			Rectangle(hdc,border+x*dw,border+y*dh,border+(x+1)*dw,border+(y+1)*dh) ;
			
			if (game!=NULL) {   // Вывод фигур
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

void GamePainter::setGame(Game * Agame) {    // Установить игру для рисования
	game = Agame ;
}