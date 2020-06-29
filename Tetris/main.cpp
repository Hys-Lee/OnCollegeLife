#include <iostream>
#include <Windows.h>
#include <ctime>
#include <vector>
/*
System에서 action과 blocks를 갖고와서 자세한걸 맡고
main은 게임에 대해 개괄적인 것만 맡도록 하자.

*/
/*
블록과 바닥을 배열에서 다른 숫자로 설정할 것인지
아니면 한줄 지울 때 바닥이상부터 체크하게 할 것인지 정하자  <-일단 이걸로 가쟈

/////////다음블럭 예고


////////////빨리 내리는거랑 언제 N탄이 끝나는지 설정 ->시간으로 할까 lin
*******끝**********


///////////한line차면 지우는거 이펙트랑 같이 구현     만약에 떨어진 줄들이 Clear될 때는 어캐해야함?
*********클리어*******************

////블록 옆으로 제 때 안옮겨지는 거
***************Near체크할 때, 블록단위 한칸단위로 옆을 파악하는 걸로 바꿈 이전에는 그게 부분적으로만 되어있었음.*********
특정 블록 특정 방향 특정 타이밍에 옮기면 반칸 날라가는거
       -> Z랑 Z역은 맨 밑에라인 빈칸부분 밑에가 반칸되는데 그 때는 바로 이 블록들이 맨밑라인 빈칸 아닌 부분으로 이동할 때,
	   -> L이랑 L역은 맨밑라인의 윗부분 2칸이 세로로 비었는데, 이 빈쪽으로 블록 이동시키면 빈쪽에 있던 바닥블록이 없어짐
	   ==>>??혹시 블록 그릴 때 3이 아닌부분은 ' '로 처리했냐?, 이게 아니라면 원인이 뭘까?
	   -*************옆으로 빈칸 만들 때 조건에서 옆에 1칸 혹은 2칸을 체크하는 걸 지웠더니 해결됨***************


스핀 처리//끝! 
****입력들어왔는지 아닌지에서 들어왔다면 어떻게 들어왔는지로 나눠서 딜레이 없앤다**********


참고로 블록 생성속도가 너무 빠르면 랜덤이 못따라가서 같은 블록이 연속으로 나옴
*/

enum color {
	black, blue, green, cyan, red, purple, brown, lightgray, darkgray, lightblue, lightgreen, lightcyan, lightred, lightpurple, yellow, white
};




#include "blocks.h"
#include "action.h"
#include "System.h"

#define Menu_Y_default 22


void gloop();

void drawTitle();
int drawMenu();
void drawInfo();
void drawUI(System&);
void drawFail(Action& );
void drawClear(Action&);

void init();
void gotoxy(int, int);
void setColor(int, int);

void Reschange(int[][8],int&, int[2], int[][8], int&, int[2]);
void Reservation(int[][8], int[2]);
bool at_end(int[][8], int x, int y, System& s, int myself);
void Falling(System&, Action&, int, int, int[][8], int, int[2]);
bool isFail(System&);
void Clear(System&, int[], int);
bool isLine(System&, int[], int);
bool isNear(int whatblock[][8], int x, int y, System s, int state, int myself);
void Move(System&, Action& , int [2], int &, int, int [][8],int);
void Spin(System&, Action&, int [][8], int, int, int [2], int);
void Downfast(System &, Action& , int [][8], int ,int&, int[2], int);

using namespace std;
////////space bar를 돌리기, d는 빠르게 내리기, 4(<-)는 왼쪽, 6(->)은 오른쪽 



int main()
{
	init();
	while (1)
	{
		drawTitle();
		int menu_choice = drawMenu();
		if (menu_choice == 0)
		{
			gloop();
			//게임시작
		}
		else if (menu_choice == 1)
		{
			drawInfo();
			//조작법 및 게임정보
		}
		else if (menu_choice == 2)
		{
			//게임난이도조정
		}
		else if (menu_choice == 3)
		{
			return 0;
		}
		setColor(white, black);
		system("cls");
	}	
	return 0;

}









//0단계는 아무 것도 없는 상태, 1단계는 밑에다가 뭔가를 깔아놓든가 그리고 단계 올라갈 수록 뭔가를 깔던가 일정 시간 이후에 특정 위치에 랜덤으로 블럭 놓든가 하자.
//
void gloop()////게임 단계 업그레이드도 상정해 놓자.
{
	Action key;
	key.set_ingame(true);
	//기본 게임창
	
	

	cout << "___________________|_________|____________________|________|____________________";
	cout << "                   |         |     |        |     |        |                    ";
	cout << "                   |         |     |  ㅁ    |     |        |                    ";
	cout << "                   |         |     |ㅁㅁㅁ  |     |        |                    ";
	cout << "                   |         |     |        |     |        |                    ";
	cout << "@@@@@@@@@@@@@@@@@@@|@@@@@@@@@|@@@@@@@@@@@@@@@@@@@@|@@@@@@@@|@@@@@@@@@@@@@@@@@@@@";
	
	system("cls");
	System s;
	drawUI(s);
	

	//////입력받고 상태변화 그리고 입력받고 .... until objects are gotten
	int starttime = time(NULL);
	int endtime = 0;
	int runtimelevel = 180;

	
	blocks B;
	int mpt_whatblock[8][8];
	int mpt_place = 0;
	int mpt_de_size_xy[2];
	int whatblock[8][8];
	int place = 0;
	int de_size_xy[2];
	/////////////////걍 블록을 가져와


	
	B.Create(whatblock, place, de_size_xy);

	

	while (starttime+runtimelevel>=endtime)
	{
		B.Create(mpt_whatblock, mpt_place, mpt_de_size_xy);
		
		Reservation(mpt_whatblock, mpt_de_size_xy);


		Falling(s, key, starttime, runtimelevel,whatblock, place, de_size_xy);//////Falling으로부터 값을 받아서 바닥에 부딪히면 반복시키는거로?
		Reschange(whatblock, place, de_size_xy, mpt_whatblock, mpt_place, mpt_de_size_xy);
		Sleep(50);///////////////////////////////////////////////////////////////////////블럭 생성속도
		int lines[37];
		int lines_size = 37;
		for (int i = 0; i < 37; i++)
			lines[i] = 0;
		if (isLine(s, lines, lines_size))
		{
			Clear(s, lines, lines_size);
			//isescaped = true;
			continue;
		}
		
		if (isFail(s))
		{
			drawFail(key);/////////////////////여기서도 선택지를 만들 수도 있지. 예를 들면 재시작하거나 걍 메인으로 돌아가거나
			break;
		}
		endtime = time(NULL);
		
		
	}
	if (starttime + runtimelevel <= endtime)
	{
		drawClear(key);
	}
	
	
}





void drawTitle()
{
	gotoxy(0, 10);
	cout << "          TTTTTT     EEEEEE    TTTTTT    RRRRR     IIIIII     SSSSS" << endl;
	cout << "            TT       E           TT      R    R      II      S" << endl;
	cout << "            TT       EEEEE       TT      RRRR        II       SSSS" << endl;
	cout << "            TT       E           TT      R   R       II           S" << endl;
	cout << "            TT       EEEEEE      TT      R    R    IIIIII    SSSSS" << endl;
}





int drawMenu()
{
	gotoxy(16, 18);
	cout << "<<numlock 해제 후, 위 아래 방향키로 메뉴 이동>>";
	gotoxy(29, 19);
	cout << "<<선택은 스페이스바>>";

	int x = 30;
	int y = 22;

	gotoxy(x - 2, y);
	cout << "> 게 임  시 작";
	gotoxy(x, y + 2);
	cout << "조 작 법  및  게 임 정 보";
	gotoxy(x, y + 4);
	cout << "시 작  난 이 도  설 정";
	gotoxy(x, y + 6);
	cout << "게 임 종 료" << endl << endl << endl << endl;

	while (1)
	{
		Action key;
		int n = key.KeyControl();
		switch (n)
		{
		case UP:
			if (y > Menu_Y_default)
			{
				gotoxy(x - 2, y);
				cout << ' ';
				gotoxy(x - 2, --(--y));
				cout << '>';
			}
			break;
		case DOWN:
			if (y < Menu_Y_default + 6)
			{
				gotoxy(x - 2, y);
				cout << ' ';
				gotoxy(x - 2, ++(++y));
				cout << '>';
			}
			break;
		case SELECT:
			return (y - Menu_Y_default) / 2; ////////////////////////0은 게임시작, 1은 조작법 및 게임정보, 2는 시작 난이도 변경, 3은 게임종료
		}
	}
}

void drawInfo()
{
	system("cls");

	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	cout << "                                   조 작 법" << endl << endl << endl << endl;
	cout << "                               (numlock을 풀고)" << endl << endl;
	cout << "                            좌 : <-(4)   우 : ->(6)" << endl << endl;
	cout << "                     빠 른 하 강 : 키 패 드  아 래 방 향(2)" << endl << endl;
	cout << "                         블 록 회 전 : 스 페 이 스  바" << endl << endl << endl << endl << endl << endl;

	cout << "                              제 작 자 : 이 상 현" << endl;
	cout << "                            dltkdgus3275@gmail.com" << endl << endl << endl;

	cout << "       이 전 화 면 으 로  돌 아 가 려 면 스 페 이 스  바 를  누 르 세 요";

	while (1)
	{
		Action key;
		if (key.KeyControl() == SELECT)
		{
			break;
		}

	}
}
void drawUI(System& s)
{
	
	//이건 UI공간과 게임공간 나누는 거  일단 합치지말고 따로 만들어서 가정하자.
	for (int i = 0; i < 40; i++)
	{
		int j = 0;
		for (; j < 30; j++)
		{
			s.UI[i][j] = 0;
		}
		for (; j < 50; j++)
		{
			s.UI[i][j] = 1;
		}
		for (; j < 80; j++)
		{
			s.UI[i][j] = 0;
		}

	}

	///// 밑에 바닥 만들기
	for (int j = 30; j < 50; j++)
	{
		s.UI[37][j] = 2;
		s.UI[38][j] = 2;
		s.UI[39][j] = 2;
	}


	/////다음 블럭 모양 UI
	for (int i = 7; i < 11; i++)
	{
		for (int j = 60; j < 68; j++)
		{
			s.UI[i][j] = 1;
		}
	}

	/// 단계판
	for (int i = 15; i < 19; i++)
	{
		for (int j = 60; j < 70; j++)
		{
			s.UI[i][j] = 1;
		}
	}
	


	///표현형

	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (s.UI[i][j] == 0)
			{
				setColor(lightgray, lightgray);
				cout << '#';

			}

			else if (s.UI[i][j] == 1)
			{
				setColor(white, black);
				cout << ' ';

			}

			else if (s.UI[i][j] == 2 && s.UI[i][j + 1] == 2)
			{
				setColor(white, black);
				cout << "■";
				j++;
			}

		}
	}
	setColor(white, black);
	gotoxy(60, 16);
	cout << "남은시간";
}
void drawFail(Action& key)
{
	key.set_ingame(false);
	//외각 만들기
	gotoxy(11, 15);
	for (int i = 0; i < 58; i++)
	{
		cout << "/";
	}
	for (int i = 1; i < 9; i++)
	{
		gotoxy(11, 15 + i);
		cout << "//";
		gotoxy(67, 15 + i);
		cout << "//";
	}
	gotoxy(11, 24);
	for (int i = 0; i < 58; i++)
	{
		cout << "/";
	}

	//빈공간만들기  가로 54칸 세로 8칸, x:13~, y:16~
	
	for (int i = 0; i < 8; i++)
	{

		for (int j = 0; j < 54; j++)
		{
			gotoxy(13 + j, 16 + i);
			cout << " ";
		}
	}
	
	//글씨는 가로 32칸 세로 6칸 쓸거임 글자끼리는 2칸, 양끝과는 2칸 띄어서 글자당 공간7칸씩이면 될듯
	gotoxy(14, 17);
	cout << "///////     /       ///    /        ///////  /////"<<endl;
	gotoxy(14, 18);
	cout << "/          / /       /     /        /        /    /" << endl;
	gotoxy(14, 19);
	cout << "/////     /   /      /     /        //////   /     /" << endl;
	gotoxy(14, 20);
	cout << "/        ///////     /     /        /        /     /" << endl;
	gotoxy(14, 21);
	cout << "/       /       /    /     /        /        /    /" << endl;
	gotoxy(14, 22);
	cout << "/      /         /  ///    ///////  ///////  /////" << endl;
	
	gotoxy(11, 25);
	cout << "//";
	gotoxy(67, 25);
	cout << "//";
	gotoxy(11, 26);
	cout << "//";
	gotoxy(67, 26);
	cout << "//";
	for (int j = 0; j < 54; j++)
	{
		gotoxy(13 + j, 25);
		cout << " ";
		gotoxy(13 + j, 26);
		cout << " ";
	}
	gotoxy(13, 25);
	cout << "          메 뉴 화 면 으 로  돌 아 가 려 면";
	gotoxy(13, 26);
	cout << "            스 페 이 스 바 를  누 르 세 요";
	gotoxy(11, 27);
	for (int i = 0; i < 58; i++)
	{
		cout << "/";
	}

	
	while (1)
	{
		int ctrl = key.KeyControl();
		if (ctrl == SELECT)
			break;
	}
	

}
void drawClear(Action& key)
{
	key.set_ingame(false);
	//외각 만들기
	gotoxy(11, 15);
	for (int i = 0; i < 58; i++)
	{
		cout << "/";
	}
	for (int i = 1; i < 9; i++)
	{
		gotoxy(11, 15 + i);
		cout << "//";
		gotoxy(67, 15 + i);
		cout << "//";
	}
		gotoxy(11, 24);
	for (int i = 0; i < 58; i++)
	{
		cout << "/";
	}
		//빈공간만들기  가로 54칸 세로 8칸, x:13~, y:16~
		for (int i = 0; i < 8; i++)
	{
			for (int j = 0; j < 54; j++)
		{
			gotoxy(13 + j, 16 + i);
			cout << " ";
		}
	}
		//글씨는 가로 32칸 세로 6칸 쓸거임 글자끼리는 2칸, 양끝과는 2칸 띄어서 글자당 공간7칸씩이면 될듯
	gotoxy(14, 17);
	cout << "     /////  /        ///////     /     /////   " << endl;
	gotoxy(14, 18);
	cout << "    /       /        /          / /    /    /" << endl;
	gotoxy(14, 19);
	cout << "   /        /        /////     /  /    /////" << endl;
	gotoxy(14, 20);
	cout << "   /        /        /        /////    /    /" << endl;
	gotoxy(14, 21);
	cout << "    /       /        /       /    /    /     /" << endl;
	gotoxy(14, 22);
	cout << "     /////  ///////  /////// /    /    /      /" << endl;
	
	gotoxy(11, 25);
	cout << "//";
	gotoxy(67, 25);
	cout << "//";
	gotoxy(11, 26);
	cout << "//";
	gotoxy(67, 26);
	cout << "//";
	for (int j = 0; j < 54; j++)
	{
		gotoxy(13 + j, 25);
		cout << " ";
		gotoxy(13 + j, 26);
		cout << " ";
	}
	gotoxy(13, 25);
	cout << "          메 뉴 화 면 으 로  돌 아 가 려 면";
	gotoxy(13, 26);
	cout << "            스 페 이 스 바 를  누 르 세 요";
	gotoxy(11, 27);
	for (int i = 0; i < 58; i++)
	{
		cout << "/";
	}

	while (1)
	{
		int ctrl = key.KeyControl();
		if (ctrl == SELECT)
			break;
	}

}



void init()
{
	system("mode con cols=80 lines=40 | title TETRIS");
	////밑의 5줄이 커서 지우는 거임.
	HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 10;
	SetConsoleCursorInfo(ConsoleHandle, &ConsoleCursor);
}

void gotoxy(int x, int y)
{
	HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(ConsoleHandle, pos);
}

void setColor(int forground, int background)
{
	HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int code = forground + background * 16;
	SetConsoleTextAttribute(ConsoleHandle, code);
}

void Reschange(int whatblock[][8], int& place, int size_xy[2], int mpt_whatblock[][8], int& mpt_place, int mpt_size_xy[2])
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			whatblock[i][j] = mpt_whatblock[i][j];
			mpt_whatblock[i][j] = 0;
		}
	}
	//
	place = mpt_place;
	//
	for(int i = 0; i < 2; i++)
	{
		size_xy[i] = mpt_size_xy[i];
	}
}

void Reservation(int whatblock[][8], int size_xy[2])///7~10, 60~70 이 y랑 x임
{
	//지우기
	for (int i = 0; i < 4; i++)
	{
		gotoxy(61, 7 + i);
		for (int j = 0; j < 8; j++)
		{
			setColor(white, black);
			cout << "  ";
			j++;
			
		}
	}

	///그리기
	for (int i = size_xy[1] - 1; i >= 0; i--)/////////////////////////////위에서 부터 그린다는거 자너
	{
		for (int j = 0; j < size_xy[0]; j++)
		{
			gotoxy(j + 61, 10 - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함  ->y좌표에서 블록 가장 아랫자리되게->블록 위에서부터 그리는것
			if (whatblock[3 - i][j] == 3 && whatblock[3 - i][j + 1] == 3)
			{
				///////내가 갖고있는 (기본 값이든 돌려진 값이든) 데이터 의거해서 그리기
				setColor(cyan, black);
				cout << "■";
				j++;
			}

		}

	}


	/*
	for (int i = 0; i < 4; i++)
	{
		gotoxy(61, 7 + i);
		for (int j = 0; j < 8; j++)
		{
			if (whatblock[i][7-j] == 3 && whatblock[i][7-(j+1)]==3)
			{
				setColor(cyan, black);
				cout << "■";
				j++;
			}
		}
	}
	*/
}





bool at_end(int whatblock[][8], int x, int y, System& s, int myself = 0)/////cri를 맨 마지막줄만 있는게 아니니까 배열로 받든가 각각으로 만들든가 해야지
{                                                              //////////////////배열로 받을 때 이미 있는 whatblock을 이용해보자. 
	bool isend = false;                                                 /////////////////whatblock에서 x축방향 이동할 때마다 가장 바닥과 가까운 애를 고르는거야 y값도 필요하네
	                                                          ///////////아니면 걍 블록의 x축방향이동하면서 가장 아래부분의 한칸 아래에 바닥이 있는지를 확인하자
	                         ////////////////////////////////  y는 기본값의 블록의 가장 아래에 있는 놈의 y좌표임
	
	for (int i = 3; i >= 0; i--)   //블록의 바닥부터 훑어본다 
	{
		for (int j = 0; j < 7; j++)////////////////////보통은 <8이지만 여기서는 어차피 가로는 2칸 단위로 생각하니까 7까지만 해도 됨 거기다 <8로 했을 때 [][8]=이상한 수 가 혹시 0일 가능성 방지7
		{
			if (whatblock[i][j] == 3 && whatblock[i][j+1]==3)
			{
				if (s.UI[y-(3-i)+1-myself][x+j] == 2 &&s.UI[y-(3-i)+1-myself][x+j+1]==2)//////////i만큼 뺄꺼니까 바닥은 y-0이고 그 위는 y-1이니까
				{
					isend = true;
					return isend;
				}
				
			}
		}
	}
	return isend;
															 											  
															  
															  
															  
	


}

// 랜덤으로 한 개의 블록을 생성하고 떨어뜨린다.
void Falling(System& s, Action& key, int starttime, int runtimelevel, int whatblock[][8], int place, int de_size_xy[2])/////////////n 쓰는거야?   이거 그냥 메인에 gloop에서 아래방향은 자동으로 설정하는걸로 바꿔?
{



	// 생성
	
	int size_xy[2];

	////사이즈 복사 ->디폴트값은 남겨놓을라고 헤더파일에 있는거 수정할까봐
	size_xy[0] = de_size_xy[0];
	size_xy[1] = de_size_xy[1];


	////////////////////////2차원배열만드는거 배워
	int** theBlock = new int* [size_xy[1]];

	for (int i = 0; i < size_xy[1]; i++)
	{
		theBlock[i] = new int[size_xy[0]];
	}

	// 배치


	////어차피 필요한 정보는 보내줬는데 굳이 블록마다 케이스 나눌 필요가 있나?
	//// 블록 막 생성해서 블록의 맨 아랫줄밖에 안보이는데 게이머가 회전시켰을 경우도 상정해야함. 아니면 걍 처음 생성될 때 부터 블록의 wholebody를 보여주든가->형평성 어긋나지만
	/// 기준부터 블록 크기만큼 underinterface에서 값을 바꾸는거야

	//Turn하면 현재가 바뀌는 거니 그냥 현재를 생각하자.

	//30~49 column까지가 게임창
	//블록은 3이 값, 가로 2칸이 ㅁ표현

	//떨어지는 중간에 사용자의 개입도 생각해야함 
	///////블록 초기값은 클래스에서 가져오고 사용자 개입의한 변화는 새로 여기서 만들어야겠당

	int x = 30, y = 0;



	//초기로 표현하고
	x += place;
	
	setColor(cyan, black);
	for (int j = 0; j < size_xy[0]; j++)
	{
		
		if (whatblock[3][j] == 3 && whatblock[3][j + 1] == 3)
		{
			///////내가 갖고있는 (기본 값이든 돌려진 값이든) 데이터 의거해서 그리기
			
			gotoxy(j + x, 0);
			cout << "■";
			++j;
		}
	}
	

	while (1)
	{
		gotoxy(60, 18);
		cout << starttime+runtimelevel-time(NULL)<<" 초";
		//Sleep(500);//////////////////////////////////////////////블록 하강 속도 조절////이젠 아니야...
		int start = clock();
		int end = 0;
		
		while (end < start + 100)///////////////블록 하강속도 조절
		{
			
			if (_kbhit())
			{
				Sleep(50);
				int ctrl = key.KeyControl();
				if (ctrl == LEFT || ctrl == RIGHT)
					Move(s, key, size_xy, x, y, whatblock, ctrl);
				else if (ctrl == SPIN)
					Spin(s, key, whatblock, x, y, size_xy, ctrl);
				else if (ctrl == DOWN)
					Downfast(s, key, whatblock, x, y, size_xy, ctrl);
			}/*
			else if (_kbhit())
			{
				//Sleep(50);
				Spin(s, key, whatblock, x, y, size_xy);
			}*/

				
			end = clock();
		}
		//////////////////스핀
		/*/
		start = clock();
		end = 0;
		while (end < start+50)///////////////속도 조절
		{

			if (_kbhit())
			{
				Sleep(50);
				Spin(s, key, whatblock, x, y, size_xy);
				
			}

			end = clock();
		}
		*/
		
		if (!at_end(whatblock,x ,y, s))
		{
			setColor(cyan, black);
			//없애고 //////없앨 때, 벽블록 (2)는 안없애면 되겠네

			for (int i = size_xy[1]-1; i >= 0; i--)
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					//gotoxy(j + x, y - i + 5);
					//cout << i;
					if (y - i >= 0 && s.UI[y-i][j+x]!=2)/////////////////////////////////end에서 다 못거를 수도 ..있다는 가능성
					{
						gotoxy(j + x, y - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함
						cout << ' ';

					}


				}

			}

			
			
			y++;
			for (int i = size_xy[1]-1; i >=0; i--)/////////////////////////////위에서 부터 그린다는거 자너
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					if (y - i >= 0)
					{
						gotoxy(j + x, y - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함  ->y좌표에서 블록 가장 아랫자리되게->블록 위에서부터 그리는것
						if (whatblock[3-i][j] == 3 && whatblock[3-i][j + 1] == 3)
						{
							///////내가 갖고있는 (기본 값이든 돌려진 값이든) 데이터 의거해서 그리기
							
							cout << "■";
							j++;
						}

					}


				}

			}
	

		}
			
		else
		{
			setColor(white, black);
			setColor(white, black);
			for (int i = size_xy[1]-1; i >= 0; i--)
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					if (y - i >= 0)
					{
						
						gotoxy(j + x, y - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함
						if (whatblock[3-i][j] == 3 && whatblock[3-i][j + 1] == 3)
						{
							///////내가 갖고있는 (기본 값이든 돌려진 값이든) 데이터 의거해서 그리기

							cout << "■";
							s.UI[y - i][j + x] = 2;
							s.UI[y - i][j+1 + x] = 2;////////ㅁ은 2칸이니까
							j++;
							
						}
						

					}


				}

			}
			break;
		}
		

	}
}

bool isFail(System& s)
{
	for (int i = 30; i < 49; i++)   /////블럭 가능 공간
	{
		if (s.UI[0][i] == 2)
		{
			return true;
		}
	}
	return false;
}

				////col은 y축	상 꽉찬곳 col[i]에서 i가 그 값	col_size=37		/////////그리고 어차피 최대 4개까지 클리어니까
void Clear(System& s, int lines[], int lines_size) ///////떨어진 line들이 clear될 때는 맨 아래의 line들을 클리어하고 그 위의 라인(clear될 라인들 직전까지)들을 끌어내리고 
{															////////이후 남은 line들을 클리어 한 후 그 위의 라인들 끌어내릴 때 처음 클리어한 line수 만큼 더해서 clear
	Sleep(300);

	///지우기
	for (int i = 0; i < lines_size; i++)
	{
		if (lines[i] == 1)
		{
			for (int j = 30; j < 50; j++)
			{
				s.UI[i][j] = 1;
				gotoxy(j, i);
				cout << ' ';
			}
		}
	}
	Sleep(100);
	/////lineClear시작 line과 끝 line파악 어차피 어딘지 아는뎅   
	///// 끌어내리는건 x의 라인차이나는 위치애를 y번 끌어내리는거자너  맨 마지막에 끌어내린 후 x만큼 라인을 맨위에 비워두면 되네
	/////////어차피 최대 2개가 떨어져 있을 수 있는거니
	
	//clear지점 찾기
	
	
	vector<int> start;   // top이 y=0쪽 방향이 되게  그래야 일반적이지
	vector<int> end;
	
	if (lines[lines_size-1] == 1)   ////36
	{
		start.push_back(lines_size-1);
	}
	
	for (int i = lines_size-1; i >= 0; i--)
	{
				
		if (lines[i] == 0 && lines[i - 1] == 1)
		{
			start.push_back(i-1);
		}

		
		if (lines[i] == 1 && lines[i - 1] == 0)
		{
			end.push_back(i);
		}
	}
	if (lines[0] == 1)
	{
		end.push_back(0);
	}

	///끌어내리기

	//int cleared_gap1 = start[0] - end[0]+1; ///////////으앙으앙으앙으앙으아응아으아ㅡ아아ㅡ 
	

	////귀찮아서 걍 2가지 케이스밖에 없는거 걍 일반화하지 않고 2개로 나눠서 함. 시간 남으면 일반화 하든가
	if (start.size() == 1 && end.size() == 1)
	{
		for (int i = start[0]; i >= (start[0] - end[0] + 1); i--)
		{
			for (int j = 30; j < 50; j++)
			{
				s.UI[i][j] = s.UI[i - (start[0] - end[0] + 1 )][j];
			}
		}
		for (int i = 0; i < (start[0] - end[0] + 1); i++)
		{
			for (int j = 30; j < 50; j++)
			{
				s.UI[i][j] = 1;
			}
		}
	}

	if (start.size() == 2 && end.size() == 2)
	{
		int left_gap = end[0] - start[1];
		//아랫줄 클리어
		for (int i = 0; i < left_gap; i++)  //// 
		{
			for (int j = 30; j < 50; j++)
			{
				s.UI[start[0] - i][j] = s.UI[start[0] - i - (start[0] - end[0] + 1)][j];
			}
		}

		//남은 위쪽 줄 클리어
		for (int i = start[0] - left_gap; i >= (start[0] - end[0] + 1 ) + (start[1] - end[1] + 1); i--)
		{
			for (int j = 30; j < 50; j++)
			{
				s.UI[i][j] = s.UI[i - (start[0] - end[0] + 1 ) - (start[1] - end[1] + 1)][j];
			}
		}
		for (int i = 0; i < (start[0] - end[0] + 1) + (start[1] - end[1] + 1); i++)
		{
			for (int j = 30; j < 50; j++)
			{
				s.UI[i][j] = 1;
			}
		}
	}
	
	

	/*


	///////////////////
	for (int i = 0; i < start.size(); i++) // start랑 end사이즈가 같다는 전재하에///
	{
		for(int k=end[i]-1;k>start[i+1];k--)
		{
				//////////////////////////////////////시작1~끝1 -> 끝1 이후 시작 전까지 1칸씩 땡기고 시작 없으면 끝까지 1칸씩 땡겨
			int diff = start[i] - end[i]+1;
			for (int j = 30; j < 50; j++)
			{
				s.UI[k][j] = s.UI[k - diff][j];
			}
			
			if (start[i] == start.size() - 1)
			{
				for (int k = start[i]; k > start[i+1]; k--)///////////////다음 시작 전까지 땡기기
				{
					
				}
				
			}
			
		}
	}
	
	*/

	for (int j = 30; j < 50; j++)
		s.UI[0][j] = 1;
	for (int k = 0; k < 37; k++)
	{
		for (int j = 30; j < 50; j++)
		{
			if (s.UI[k][j] == 1)
			{
				gotoxy(j, k);
				cout << ' ';
			}
			else if (s.UI[k][j] == 2)
			{
				gotoxy(j, k);
				cout << "■";
				j++;
			}

				
		}
	}
	


}

bool isLine(System& s, int lines[], int lines_size)   /////////col의 인수가 1인 y에서 lineClear colsize = 37
{
	int i = 0;
	
	for (; i < lines_size; i++) /////////조작할 크기는 37
	{
		bool line = true;
		for (int j = 30; j < 50; j++)
		{
			if (s.UI[i][j] != 2)
			{
				line = false;
				break;
			}
		}
		if (line == true)
		{
			lines[i] = 1;
		}
	}
	for (int k = 0; k < lines_size; k++)
	{
		if (lines[k] == 1)
			return true;
	}
	return false;
	

}



bool isNear(int whatblock[][8], int x, int y, System s, int state, int myself=0)
{
	bool isend = false;                                                 /////////////////whatblock에서 x축방향 이동할 때마다 가장 바닥과 가까운 애를 고르는거야 y값도 필요하네
	                                                          ///////////아니면 걍 블록의 x축방향이동하면서 가장 아래부분의 한칸 아래에 바닥이 있는지를 확인하자
	                         ////////////////////////////////  y는 기본값의 블록의 가장 아래에 있는 놈의 y좌표임
	
	for (int i = 3; i >= 0; i--)   //블록의 바닥부터 훑어본다 
	{
		for (int j = 0; j < 8; j++)
		{
			//왼쪽
			if (state == LEFT)
			{
				if (whatblock[i][0] ==  3)//|| )//whatblock[i][j] == 3 && whatblock[i][j+1]==3)
				{
					if (s.UI[y - (3 - i)][x - 1+myself] == 2)//////////i만큼 뺄꺼니까 바닥은 y-0이고 그 위는 y-1이니까
					{
						isend = true;
						return isend;
					}
				}
				else if (whatblock[i][j] == 0 && whatblock[i][j + 1] == 3)
				{
					if (s.UI[y - (3 - i)][x +j- 1+myself] == 2)//////////i만큼 뺄꺼니까 바닥은 y-0이고 그 위는 y-1이니까
					{
						isend = true;
						return isend;
					}
				}
			}
			//오른쪽
			else if (state == RIGHT)
			{
				if (whatblock[i][7]==3)// || (whatblock[i][j] == 3 && whatblock[i][j + 1] == 0))
				{
					if (s.UI[y - (3 - i)][x + 7 + 1-myself] == 2)//////////i만큼 뺄꺼니까 바닥은 y-0이고 그 위는 y-1이니까
					{
						isend = true;
						return isend;
					}
				}
				else if (whatblock[i][j] == 3 && whatblock[i][j + 1] == 0)
				{
					if (s.UI[y - (3 - i)][x + j + 1-myself] == 2)//////////i만큼 뺄꺼니까 바닥은 y-0이고 그 위는 y-1이니까
					{
						isend = true;
						return isend;
					}
				}
			}
		}
	}
	return isend;
}

void Move(System &s,Action& key, int size_xy[2], int &x, int y, int whatblock[][8], int ctrl) ////x, y는 블록의 최좌하단에 위치
{       
                                                                             ////////특정 시간동안만 입력 받는 시스템 블록 내려오기 전에 2번으로 하자
	/*int countClock = 0;
	while (1)//_kbhit())
	{
		
		int input = _kbhit(); // 1이면 누른거 0이면 안누른거
		
		if (input)
		{
			
		}
		else
		{
			break;
		}
		
	}
	
	*/
	
	int dir = ctrl;//key.KeyControl();
	if (dir == LEFT)
	{
		if (x > 30 && !isNear(whatblock, x, y, s, LEFT))
		{
			setColor(cyan, black);
			//없애고

			for (int i = size_xy[1] - 1; i >= 0; i--)
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					//gotoxy(j + x, y - i + 5);
					//cout << i;
					if (y - i >= 0 && s.UI[y - i][j + x] != 2)// && s.UI[y-i][j+x-1]!=2)////////////////////////////////
					{
						gotoxy(j + x, y - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함
						cout << ' ';

					}
				}

			}

			x -= 2;
			for (int i = size_xy[1] - 1; i >= 0; i--)/////////////////////////////위에서 부터 그린다는거 자너
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					if (y - i >= 0)
					{
						gotoxy(j + x, y - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함  ->y좌표에서 블록 가장 아랫자리되게->블록 위에서부터 그리는것
						if (whatblock[3 - i][j] == 3 && whatblock[3 - i][j + 1] == 3)
						{
							///////내가 갖고있는 (기본 값이든 돌려진 값이든) 데이터 의거해서 그리기

							cout << "■";
							j++;
						}

					}


				}

			}

		}

	}
	else if (dir == RIGHT)
	{
		if (x + size_xy[0] < 50 && !isNear(whatblock, x, y, s, RIGHT))
		{
			setColor(cyan, black);
			//없애고

			for (int i = size_xy[1] - 1; i >= 0; i--)
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					//gotoxy(j + x, y - i + 5);
					//cout << i;
					if (y - i >= 0 && s.UI[y - i][j + x] != 2 )//&& s.UI[y - i][j + x+1] != 2)
					{
						gotoxy(j + x, y - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함
						cout << ' ';

					}
				}

			}

			x += 2;
			for (int i = size_xy[1] - 1; i >= 0; i--)/////////////////////////////위에서 부터 그린다는거 자너
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					if (y - i >= 0)
					{
						gotoxy(j + x, y - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함  ->y좌표에서 블록 가장 아랫자리되게->블록 위에서부터 그리는것
						if (whatblock[3 - i][j] == 3 && whatblock[3 - i][j + 1] == 3)
						{
							///////내가 갖고있는 (기본 값이든 돌려진 값이든) 데이터 의거해서 그리기

							cout << "■";
							j++;
						}

					}
				}
			}
		}
	}
}


void Spin(System& s, Action& key, int whatblock[][8], int x, int y, int size_xy[2], int ctrl)
{
	//int ctrl = key.KeyControl();

	////////
	////////
	////////
	////////

	int tmp [4][8];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			tmp[i][j] = 0;
		}
	}

	if (ctrl == SPIN)////////////반시계로 돌리장  col은 2줄씩 끊어서 line 1줄로 바꿔야하는뎅
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (whatblock[i][j] == 3 && whatblock[i][j + 1] == 3)///////////j=0,1->i=0, j=2,3->i=1..../////i=0->j=0,1, i=1->j=2,3
				{
					tmp[3-(j / 2)][i * 2] = 3;
					tmp[3-(j / 2)][i * 2 + 1] = 3;
					j++;
				}
			}
		}

		////블록 한칸 단위로 x이동하면서 y줄 빈거 찾아 편집하기
		int nline = 0;

		for (int j = 0; j < 8; j++)
		{
			bool isnline = true;
			for (int i = 0; i < 4; i++)
			{
				if (tmp[i][j] != 0 || tmp[i][j + 1] != 0)
				{
					isnline = false;
				}
			}
			if (isnline == true)
			{
				nline++;
			}
			j++;
		}

		for (int j = 0; j < 8; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				if (nline * 2 + j < 8)
				{
					tmp[i][j] = tmp[i][j + nline * 2];
				}
				else
				{
					tmp[i][j] = 0;
				}

			}
		}
		int tmpsize_x = size_xy[0] / 2;
		int tmpsize_y = size_xy[1] * 2;
		


		if (!at_end(tmp, x, y, s, 1) && x+tmpsize_y-1<50)///tmpsize_y 가 돌린 후의 x길이니까, -1은 tmpsize가 좌표가 아니라 크기니까//&& !isNear(tmp, x, y, s, LEFT, 1) && !isNear(tmp, x, y, s, RIGHT, 1))
		{
			
			


			setColor(cyan, black);
			//없애고 //////없앨 때, 벽블록 (2)는 안없애면 되겠네

			for (int i = size_xy[1] - 1; i >= 0; i--)
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					//gotoxy(j + x, y - i + 5);
					//cout << i;
					if (y - i >= 0 && s.UI[y - i][j + x] != 2)/////////////////////////////////end에서 다 못거를 수도 ..있다는 가능성
					{
						gotoxy(j + x, y - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함
						cout << ' ';

					}


				}

			}

			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					whatblock[i][j] = tmp[i][j];
				}
			}
			size_xy[0] = tmpsize_y;
			size_xy[1] = tmpsize_x;

			
			for (int i = size_xy[1] - 1; i >= 0; i--)/////////////////////////////위에서 부터 그린다는거 자너
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					if (y - i >= 0)
					{
						gotoxy(j + x, y - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함  ->y좌표에서 블록 가장 아랫자리되게->블록 위에서부터 그리는것
						if (whatblock[3 - i][j] == 3 && whatblock[3 - i][j + 1] == 3)
						{
							///////내가 갖고있는 (기본 값이든 돌려진 값이든) 데이터 의거해서 그리기

							cout << "■";
							j++;
						}

					}


				}

			}


		}

	}
	

	
}
void Downfast(System & s,Action& key, int whatblock[][8], int x, int& y, int size_xy[2], int ctrl)
{
	if (ctrl == DOWN)
	{
		if (!at_end(whatblock, x, y, s, -1)) ////////////끝에 걍 놔두면(0이면) 겹치는 경우가 생겨서 걍 -1 (즉 한칸 위 띄워있는 상태 전까지만 내릴 수 있도록)
		{
			setColor(cyan, black);
			//없애고 //////없앨 때, 벽블록 (2)는 안없애면 되겠네

			for (int i = size_xy[1] - 1; i >= 0; i--)
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					//gotoxy(j + x, y - i + 5);
					//cout << i;
					if (y - i >= 0 && s.UI[y - i][j + x] != 2)/////////////////////////////////end에서 다 못거를 수도 ..있다는 가능성
					{
						gotoxy(j + x, y - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함
						cout << ' ';

					}


				}

			}



			y+=2;
			for (int i = size_xy[1] - 1; i >= 0; i--)/////////////////////////////위에서 부터 그린다는거 자너
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					if (y - i >= 0)
					{
						gotoxy(j + x, y - i);/////y를 -1부터 시작할지 0부터 시작할지 정해 일단 0부터 시작으로 함  ->y좌표에서 블록 가장 아랫자리되게->블록 위에서부터 그리는것
						if (whatblock[3 - i][j] == 3 && whatblock[3 - i][j + 1] == 3)
						{
							///////내가 갖고있는 (기본 값이든 돌려진 값이든) 데이터 의거해서 그리기

							cout << "■";
							j++;
						}

					}


				}

			}


		}
	}
	
}