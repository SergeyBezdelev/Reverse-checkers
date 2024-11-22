#include "Game.h"

enum CLICKRESULT { crOk, crFinish } ;   // Результат клика по доске

// Класс рисования
class GamePainter {
private:
	Game * game ;//Ссылка на игру
	// Размеры доски и ячейки
	int _w ;
	int _h ;
	int dw ;
	int dh ;
	int border ;
	POINT Selected ;         // Выбранная клетка
	void ClearSelected() ;	 // Очистка выбранной
	bool isSelected() const; // Проверка выбранной
public:
	GamePainter();
	void setGame(Game * Agame) ;         // Установить игру для рисования
	void drawTo(HDC hdc,int w, int h);   // Вывести игру на контекст
	CLICKRESULT doClick(int cx,int cy);  // Щелчок по доске
};