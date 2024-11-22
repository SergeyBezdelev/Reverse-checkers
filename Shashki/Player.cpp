#include "stdafx.h"
#include "Player.h"
#include <time.h>
#include <algorithm>

using namespace std ;

Player::Player(Game * Agame){
	game=Agame ;
	srand(time(NULL)) ;
}

// Оценочная функция состояния доски 
int Player::getStepWeight(Game* stepgame) {
	return stepgame->countHuman() - stepgame->countComp();
}

// Функция минимакса для хода компьютера или человека
// Рекурсия переключает знак вычисления хода и передает порядок хода от человека к компьютеру
ONESTEP Player::algMinMax(Game* stepgame, bool comp) {
	return alphaBeta(stepgame, comp, 2, -99, 99);
}
// Функция Aльфа-Бетта
ONESTEP Player::alphaBeta(Game* stepgame, bool comp, int depth, int alpha, int beta) {
	if (depth == 0 || stepgame->isGameOver(false, comp ? STEP::sComp : STEP::sHuman)) {
		ONESTEP result = ONESTEP(0, 0, 0, 0);      //проверяет, достигнута ли заданная глубина поиска или игра завершена. 
		result.weight = getStepWeight(stepgame);   //Если это условие выполняется, метод возвращает структуру ONESTEP с оценкой хода
		return result;
	}

	ONESTEP bestStep;  //хранит лучший ход
	if (comp) {
		bestStep.weight = INT_MIN;
		vector<ONESTEP>* steps = stepgame->buildSteps(true);
		for (int i = 0; i < steps->size(); i++) {    //цикл по всем доступным ходам из вектора steps.
			ONESTEP os = steps->at(i);               //Для каждого хода создается копия игры, выполняется ход
			Game* gn = stepgame->createClone();
			gn->moveObject(os.xs, os.ys, os.xf, os.yf);
			ONESTEP nextStep = alphaBeta(gn, false, depth - 1, alpha, beta); // рекурсивно вызывается метод
			nextStep.weight *= -1;    // Инвертируем оценку хода для компьютера
			steps->at(i).weight = nextStep.weight;
			delete gn;

			bestStep = (nextStep.weight > bestStep.weight) ? steps->at(i) : bestStep;//Если оценка текущего хода лучше, чем
			alpha = (alpha > bestStep.weight) ? alpha : bestStep.weight;             //лучший ход до этого, обновляем значение
			if (beta <= alpha) {      //если условие отсечения выполняется, цикл прерывается.
				break;
			}
		}
	}
	else {
		bestStep.weight = INT_MAX;
		vector<ONESTEP>* steps = stepgame->buildSteps(false);
		for (int i = 0; i < steps->size(); i++) {
			ONESTEP os = steps->at(i);
			Game* gn = stepgame->createClone();
			gn->moveObject(os.xs, os.ys, os.xf, os.yf);
			ONESTEP nextStep = alphaBeta(gn, true, depth - 1, alpha, beta);
			nextStep.weight *= -1; // Инвертируем оценку хода для игрока
			steps->at(i).weight = nextStep.weight;
			delete gn;

			bestStep = (nextStep.weight < bestStep.weight) ? steps->at(i) : bestStep;
			beta = (beta < bestStep.weight) ? beta : bestStep.weight;
			if (beta <= alpha) {
				break;
			}
		}
	}
	return bestStep;
}

// Процедура выполнения хода на основе минимакса компьютером
bool Player::makeStep() {
	vector<ONESTEP> * steps = game->buildSteps(true) ;
	if (steps->size() > 0) {		                       //есть ли хотя бы один доступный ход в векторе steps
		ONESTEP step = algMinMax(game, true);		       //Выполняется перемещение объекта на игровом поле 
		game->moveObject(step.xs,step.ys,step.xf,step.yf); //с координатами step.xs, step.ys на позицию step.xf, step.yf.
		return true ;
	}
	else
		return false ;
}