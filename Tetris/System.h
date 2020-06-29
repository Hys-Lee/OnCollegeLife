#ifndef SYSTEM_H
#define SYSTEM_H



////col은 80, line은 40 사이즈임 윈도우 창은
// 양옆에 16칸씩 남기고 그 안에 구분선 1칸하고 그 안에 블록 놓일 수 있는 곳으로

/// 바닥이나 다른 블록에 닿고 n초 뒤에 고정되는거임., n초마다 내려오고.

/////맨 밑바닥하고 블록하고 처음에는 다르게 설정하고(표현만 같게?) 블록이 바닥이나 다른 블록에 닿으면 바닥과 같은 성질 되도록.



class System
{
protected:

public:
	System(){}
	int UI[40][81];//0은 바탕, 1은 가이드라인?구분선, 2는 바닥, 블록은 underinterface에서 없애고 overinterface에만 나타나고 바닥이나 타	블럭에 닿으면 underinterface그만큼 바꾸자.

	

	
	
	


	


	
};

#endif SYSTEM_H
