#include "stdafx.h"
#include "Player.h"
#include <time.h>
#include <algorithm>

using namespace std ;

Player::Player(Game * Agame){
	game=Agame ;
	srand(time(NULL)) ;
}

// ��������� ������� ��������� ����� 
int Player::getStepWeight(Game* stepgame) {
	return stepgame->countHuman() - stepgame->countComp();
}

// ������� ��������� ��� ���� ���������� ��� ��������
// �������� ����������� ���� ���������� ���� � �������� ������� ���� �� �������� � ����������
ONESTEP Player::algMinMax(Game* stepgame, bool comp) {
	return alphaBeta(stepgame, comp, 2, -99, 99);
}
// ������� A����-�����
ONESTEP Player::alphaBeta(Game* stepgame, bool comp, int depth, int alpha, int beta) {
	if (depth == 0 || stepgame->isGameOver(false, comp ? STEP::sComp : STEP::sHuman)) {
		ONESTEP result = ONESTEP(0, 0, 0, 0);      //���������, ���������� �� �������� ������� ������ ��� ���� ���������. 
		result.weight = getStepWeight(stepgame);   //���� ��� ������� �����������, ����� ���������� ��������� ONESTEP � ������� ����
		return result;
	}

	ONESTEP bestStep;  //������ ������ ���
	if (comp) {
		bestStep.weight = INT_MIN;
		vector<ONESTEP>* steps = stepgame->buildSteps(true);
		for (int i = 0; i < steps->size(); i++) {    //���� �� ���� ��������� ����� �� ������� steps.
			ONESTEP os = steps->at(i);               //��� ������� ���� ��������� ����� ����, ����������� ���
			Game* gn = stepgame->createClone();
			gn->moveObject(os.xs, os.ys, os.xf, os.yf);
			ONESTEP nextStep = alphaBeta(gn, false, depth - 1, alpha, beta); // ���������� ���������� �����
			nextStep.weight *= -1;    // ����������� ������ ���� ��� ����������
			steps->at(i).weight = nextStep.weight;
			delete gn;

			bestStep = (nextStep.weight > bestStep.weight) ? steps->at(i) : bestStep;//���� ������ �������� ���� �����, ���
			alpha = (alpha > bestStep.weight) ? alpha : bestStep.weight;             //������ ��� �� �����, ��������� ��������
			if (beta <= alpha) {      //���� ������� ��������� �����������, ���� �����������.
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
			nextStep.weight *= -1; // ����������� ������ ���� ��� ������
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

// ��������� ���������� ���� �� ������ ��������� �����������
bool Player::makeStep() {
	vector<ONESTEP> * steps = game->buildSteps(true) ;
	if (steps->size() > 0) {		                       //���� �� ���� �� ���� ��������� ��� � ������� steps
		ONESTEP step = algMinMax(game, true);		       //����������� ����������� ������� �� ������� ���� 
		game->moveObject(step.xs,step.ys,step.xf,step.yf); //� ������������ step.xs, step.ys �� ������� step.xf, step.yf.
		return true ;
	}
	else
		return false ;
}