#ifndef conio_H
#define conio_H
#include <conio.h>
#endif conio_H

////////space bar를 돌리기, d는 빠르게 내리기, 4(<-)는 왼쪽, 6(->)은 오른쪽 

#ifndef ACTION_H
#define ACTION_H

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define SPIN 4
#define SELECT 5



class Action
{
protected:
	bool ingame=false;
public:
	void set_ingame(bool ig);
	bool get_ingame();

	int KeyControl();

	void Turn();
	


};

#endif ACTION_H