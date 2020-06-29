#include <iostream>
#include <Windows.h>
#include <ctime>
#include <vector>
/*
System���� action�� blocks�� ����ͼ� �ڼ��Ѱ� �ð�
main�� ���ӿ� ���� �������� �͸� �õ��� ����.

*/
/*
��ϰ� �ٴ��� �迭���� �ٸ� ���ڷ� ������ ������
�ƴϸ� ���� ���� �� �ٴ��̻���� üũ�ϰ� �� ������ ������  <-�ϴ� �̰ɷ� ����

/////////������ ����


////////////���� �����°Ŷ� ���� Nź�� �������� ���� ->�ð����� �ұ� lin
*******��**********


///////////��line���� ����°� ����Ʈ�� ���� ����     ���࿡ ������ �ٵ��� Clear�� ���� ��ĳ�ؾ���?
*********Ŭ����*******************

////��� ������ �� �� �ȿŰ����� ��
***************Nearüũ�� ��, ��ϴ��� ��ĭ������ ���� �ľ��ϴ� �ɷ� �ٲ� �������� �װ� �κ������θ� �Ǿ��־���.*********
Ư�� ��� Ư�� ���� Ư�� Ÿ�ֿ̹� �ű�� ��ĭ ���󰡴°�
       -> Z�� Z���� �� �ؿ����� ��ĭ�κ� �ؿ��� ��ĭ�Ǵµ� �� ���� �ٷ� �� ��ϵ��� �ǹض��� ��ĭ �ƴ� �κ����� �̵��� ��,
	   -> L�̶� L���� �ǹض����� ���κ� 2ĭ�� ���η� ����µ�, �� �������� ��� �̵���Ű�� ���ʿ� �ִ� �ٴں���� ������
	   ==>>??Ȥ�� ��� �׸� �� 3�� �ƴѺκ��� ' '�� ó���߳�?, �̰� �ƴ϶�� ������ ����?
	   -*************������ ��ĭ ���� �� ���ǿ��� ���� 1ĭ Ȥ�� 2ĭ�� üũ�ϴ� �� �������� �ذ��***************


���� ó��//��! 
****�Էµ��Դ��� �ƴ������� ���Դٸ� ��� ���Դ����� ������ ������ ���ش�**********


����� ��� �����ӵ��� �ʹ� ������ ������ �����󰡼� ���� ����� �������� ����
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
////////space bar�� ������, d�� ������ ������, 4(<-)�� ����, 6(->)�� ������ 



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
			//���ӽ���
		}
		else if (menu_choice == 1)
		{
			drawInfo();
			//���۹� �� ��������
		}
		else if (menu_choice == 2)
		{
			//���ӳ��̵�����
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









//0�ܰ�� �ƹ� �͵� ���� ����, 1�ܰ�� �ؿ��ٰ� ������ ��Ƴ��簡 �׸��� �ܰ� �ö� ���� ������ ����� ���� �ð� ���Ŀ� Ư�� ��ġ�� �������� �� ���簡 ����.
//
void gloop()////���� �ܰ� ���׷��̵嵵 ������ ����.
{
	Action key;
	key.set_ingame(true);
	//�⺻ ����â
	
	

	cout << "___________________|_________|____________________|________|____________________";
	cout << "                   |         |     |        |     |        |                    ";
	cout << "                   |         |     |  ��    |     |        |                    ";
	cout << "                   |         |     |������  |     |        |                    ";
	cout << "                   |         |     |        |     |        |                    ";
	cout << "@@@@@@@@@@@@@@@@@@@|@@@@@@@@@|@@@@@@@@@@@@@@@@@@@@|@@@@@@@@|@@@@@@@@@@@@@@@@@@@@";
	
	system("cls");
	System s;
	drawUI(s);
	

	//////�Է¹ް� ���º�ȭ �׸��� �Է¹ް� .... until objects are gotten
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
	/////////////////�� ����� ������


	
	B.Create(whatblock, place, de_size_xy);

	

	while (starttime+runtimelevel>=endtime)
	{
		B.Create(mpt_whatblock, mpt_place, mpt_de_size_xy);
		
		Reservation(mpt_whatblock, mpt_de_size_xy);


		Falling(s, key, starttime, runtimelevel,whatblock, place, de_size_xy);//////Falling���κ��� ���� �޾Ƽ� �ٴڿ� �ε����� �ݺ���Ű�°ŷ�?
		Reschange(whatblock, place, de_size_xy, mpt_whatblock, mpt_place, mpt_de_size_xy);
		Sleep(50);///////////////////////////////////////////////////////////////////////�� �����ӵ�
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
			drawFail(key);/////////////////////���⼭�� �������� ���� ���� ����. ���� ��� ������ϰų� �� �������� ���ư��ų�
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
	cout << "<<numlock ���� ��, �� �Ʒ� ����Ű�� �޴� �̵�>>";
	gotoxy(29, 19);
	cout << "<<������ �����̽���>>";

	int x = 30;
	int y = 22;

	gotoxy(x - 2, y);
	cout << "> �� ��  �� ��";
	gotoxy(x, y + 2);
	cout << "�� �� ��  ��  �� �� �� ��";
	gotoxy(x, y + 4);
	cout << "�� ��  �� �� ��  �� ��";
	gotoxy(x, y + 6);
	cout << "�� �� �� ��" << endl << endl << endl << endl;

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
			return (y - Menu_Y_default) / 2; ////////////////////////0�� ���ӽ���, 1�� ���۹� �� ��������, 2�� ���� ���̵� ����, 3�� ��������
		}
	}
}

void drawInfo()
{
	system("cls");

	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	cout << "                                   �� �� ��" << endl << endl << endl << endl;
	cout << "                               (numlock�� Ǯ��)" << endl << endl;
	cout << "                            �� : <-(4)   �� : ->(6)" << endl << endl;
	cout << "                     �� �� �� �� : Ű �� ��  �� �� �� ��(2)" << endl << endl;
	cout << "                         �� �� ȸ �� : �� �� �� ��  ��" << endl << endl << endl << endl << endl << endl;

	cout << "                              �� �� �� : �� �� ��" << endl;
	cout << "                            dltkdgus3275@gmail.com" << endl << endl << endl;

	cout << "       �� �� ȭ �� �� ��  �� �� �� �� �� �� �� �� ��  �� ��  �� �� �� ��";

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
	
	//�̰� UI������ ���Ӱ��� ������ ��  �ϴ� ��ġ������ ���� ���� ��������.
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

	///// �ؿ� �ٴ� �����
	for (int j = 30; j < 50; j++)
	{
		s.UI[37][j] = 2;
		s.UI[38][j] = 2;
		s.UI[39][j] = 2;
	}


	/////���� �� ��� UI
	for (int i = 7; i < 11; i++)
	{
		for (int j = 60; j < 68; j++)
		{
			s.UI[i][j] = 1;
		}
	}

	/// �ܰ���
	for (int i = 15; i < 19; i++)
	{
		for (int j = 60; j < 70; j++)
		{
			s.UI[i][j] = 1;
		}
	}
	


	///ǥ����

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
				cout << "��";
				j++;
			}

		}
	}
	setColor(white, black);
	gotoxy(60, 16);
	cout << "�����ð�";
}
void drawFail(Action& key)
{
	key.set_ingame(false);
	//�ܰ� �����
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

	//����������  ���� 54ĭ ���� 8ĭ, x:13~, y:16~
	
	for (int i = 0; i < 8; i++)
	{

		for (int j = 0; j < 54; j++)
		{
			gotoxy(13 + j, 16 + i);
			cout << " ";
		}
	}
	
	//�۾��� ���� 32ĭ ���� 6ĭ ������ ���ڳ����� 2ĭ, �糡���� 2ĭ �� ���ڴ� ����7ĭ���̸� �ɵ�
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
	cout << "          �� �� ȭ �� �� ��  �� �� �� �� ��";
	gotoxy(13, 26);
	cout << "            �� �� �� �� �� ��  �� �� �� ��";
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
	//�ܰ� �����
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
		//����������  ���� 54ĭ ���� 8ĭ, x:13~, y:16~
		for (int i = 0; i < 8; i++)
	{
			for (int j = 0; j < 54; j++)
		{
			gotoxy(13 + j, 16 + i);
			cout << " ";
		}
	}
		//�۾��� ���� 32ĭ ���� 6ĭ ������ ���ڳ����� 2ĭ, �糡���� 2ĭ �� ���ڴ� ����7ĭ���̸� �ɵ�
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
	cout << "          �� �� ȭ �� �� ��  �� �� �� �� ��";
	gotoxy(13, 26);
	cout << "            �� �� �� �� �� ��  �� �� �� ��";
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
	////���� 5���� Ŀ�� ����� ����.
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

void Reservation(int whatblock[][8], int size_xy[2])///7~10, 60~70 �� y�� x��
{
	//�����
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

	///�׸���
	for (int i = size_xy[1] - 1; i >= 0; i--)/////////////////////////////������ ���� �׸��ٴ°� �ڳ�
	{
		for (int j = 0; j < size_xy[0]; j++)
		{
			gotoxy(j + 61, 10 - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��  ->y��ǥ���� ��� ���� �Ʒ��ڸ��ǰ�->��� ���������� �׸��°�
			if (whatblock[3 - i][j] == 3 && whatblock[3 - i][j + 1] == 3)
			{
				///////���� �����ִ� (�⺻ ���̵� ������ ���̵�) ������ �ǰ��ؼ� �׸���
				setColor(cyan, black);
				cout << "��";
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
				cout << "��";
				j++;
			}
		}
	}
	*/
}





bool at_end(int whatblock[][8], int x, int y, System& s, int myself = 0)/////cri�� �� �������ٸ� �ִ°� �ƴϴϱ� �迭�� �޵簡 �������� ����簡 �ؾ���
{                                                              //////////////////�迭�� ���� �� �̹� �ִ� whatblock�� �̿��غ���. 
	bool isend = false;                                                 /////////////////whatblock���� x����� �̵��� ������ ���� �ٴڰ� ����� �ָ� ���°ž� y���� �ʿ��ϳ�
	                                                          ///////////�ƴϸ� �� ����� x������̵��ϸ鼭 ���� �Ʒ��κ��� ��ĭ �Ʒ��� �ٴ��� �ִ����� Ȯ������
	                         ////////////////////////////////  y�� �⺻���� ����� ���� �Ʒ��� �ִ� ���� y��ǥ��
	
	for (int i = 3; i >= 0; i--)   //����� �ٴں��� �Ⱦ�� 
	{
		for (int j = 0; j < 7; j++)////////////////////������ <8������ ���⼭�� ������ ���δ� 2ĭ ������ �����ϴϱ� 7������ �ص� �� �ű�� <8�� ���� �� [][8]=�̻��� �� �� Ȥ�� 0�� ���ɼ� ����7
		{
			if (whatblock[i][j] == 3 && whatblock[i][j+1]==3)
			{
				if (s.UI[y-(3-i)+1-myself][x+j] == 2 &&s.UI[y-(3-i)+1-myself][x+j+1]==2)//////////i��ŭ �����ϱ� �ٴ��� y-0�̰� �� ���� y-1�̴ϱ�
				{
					isend = true;
					return isend;
				}
				
			}
		}
	}
	return isend;
															 											  
															  
															  
															  
	


}

// �������� �� ���� ����� �����ϰ� ����߸���.
void Falling(System& s, Action& key, int starttime, int runtimelevel, int whatblock[][8], int place, int de_size_xy[2])/////////////n ���°ž�?   �̰� �׳� ���ο� gloop���� �Ʒ������� �ڵ����� �����ϴ°ɷ� �ٲ�?
{



	// ����
	
	int size_xy[2];

	////������ ���� ->����Ʈ���� ���ܳ������ ������Ͽ� �ִ°� �����ұ��
	size_xy[0] = de_size_xy[0];
	size_xy[1] = de_size_xy[1];


	////////////////////////2�����迭����°� ���
	int** theBlock = new int* [size_xy[1]];

	for (int i = 0; i < size_xy[1]; i++)
	{
		theBlock[i] = new int[size_xy[0]];
	}

	// ��ġ


	////������ �ʿ��� ������ ������µ� ���� ��ϸ��� ���̽� ���� �ʿ䰡 �ֳ�?
	//// ��� �� �����ؼ� ����� �� �Ʒ��ٹۿ� �Ⱥ��̴µ� ���̸Ӱ� ȸ�������� ��쵵 �����ؾ���. �ƴϸ� �� ó�� ������ �� ���� ����� wholebody�� �����ֵ簡->���� ��߳�����
	/// ���غ��� ��� ũ�⸸ŭ underinterface���� ���� �ٲٴ°ž�

	//Turn�ϸ� ���簡 �ٲ�� �Ŵ� �׳� ���縦 ��������.

	//30~49 column������ ����â
	//����� 3�� ��, ���� 2ĭ�� ��ǥ��

	//�������� �߰��� ������� ���Ե� �����ؾ��� 
	///////��� �ʱⰪ�� Ŭ�������� �������� ����� �������� ��ȭ�� ���� ���⼭ �����߰ڴ�

	int x = 30, y = 0;



	//�ʱ�� ǥ���ϰ�
	x += place;
	
	setColor(cyan, black);
	for (int j = 0; j < size_xy[0]; j++)
	{
		
		if (whatblock[3][j] == 3 && whatblock[3][j + 1] == 3)
		{
			///////���� �����ִ� (�⺻ ���̵� ������ ���̵�) ������ �ǰ��ؼ� �׸���
			
			gotoxy(j + x, 0);
			cout << "��";
			++j;
		}
	}
	

	while (1)
	{
		gotoxy(60, 18);
		cout << starttime+runtimelevel-time(NULL)<<" ��";
		//Sleep(500);//////////////////////////////////////////////��� �ϰ� �ӵ� ����////���� �ƴϾ�...
		int start = clock();
		int end = 0;
		
		while (end < start + 100)///////////////��� �ϰ��ӵ� ����
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
		//////////////////����
		/*/
		start = clock();
		end = 0;
		while (end < start+50)///////////////�ӵ� ����
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
			//���ְ� //////���� ��, ����� (2)�� �Ⱦ��ָ� �ǰڳ�

			for (int i = size_xy[1]-1; i >= 0; i--)
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					//gotoxy(j + x, y - i + 5);
					//cout << i;
					if (y - i >= 0 && s.UI[y-i][j+x]!=2)/////////////////////////////////end���� �� ���Ÿ� ���� ..�ִٴ� ���ɼ�
					{
						gotoxy(j + x, y - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��
						cout << ' ';

					}


				}

			}

			
			
			y++;
			for (int i = size_xy[1]-1; i >=0; i--)/////////////////////////////������ ���� �׸��ٴ°� �ڳ�
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					if (y - i >= 0)
					{
						gotoxy(j + x, y - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��  ->y��ǥ���� ��� ���� �Ʒ��ڸ��ǰ�->��� ���������� �׸��°�
						if (whatblock[3-i][j] == 3 && whatblock[3-i][j + 1] == 3)
						{
							///////���� �����ִ� (�⺻ ���̵� ������ ���̵�) ������ �ǰ��ؼ� �׸���
							
							cout << "��";
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
						
						gotoxy(j + x, y - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��
						if (whatblock[3-i][j] == 3 && whatblock[3-i][j + 1] == 3)
						{
							///////���� �����ִ� (�⺻ ���̵� ������ ���̵�) ������ �ǰ��ؼ� �׸���

							cout << "��";
							s.UI[y - i][j + x] = 2;
							s.UI[y - i][j+1 + x] = 2;////////���� 2ĭ�̴ϱ�
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
	for (int i = 30; i < 49; i++)   /////�� ���� ����
	{
		if (s.UI[0][i] == 2)
		{
			return true;
		}
	}
	return false;
}

				////col�� y��	�� ������ col[i]���� i�� �� ��	col_size=37		/////////�׸��� ������ �ִ� 4������ Ŭ����ϱ�
void Clear(System& s, int lines[], int lines_size) ///////������ line���� clear�� ���� �� �Ʒ��� line���� Ŭ�����ϰ� �� ���� ����(clear�� ���ε� ��������)���� ������� 
{															////////���� ���� line���� Ŭ���� �� �� �� ���� ���ε� ����� �� ó�� Ŭ������ line�� ��ŭ ���ؼ� clear
	Sleep(300);

	///�����
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
	/////lineClear���� line�� �� line�ľ� ������ ����� �ƴµ�   
	///// ������°� x�� �������̳��� ��ġ�ָ� y�� ������°��ڳ�  �� �������� ����� �� x��ŭ ������ ������ ����θ� �ǳ�
	/////////������ �ִ� 2���� ������ ���� �� �ִ°Ŵ�
	
	//clear���� ã��
	
	
	vector<int> start;   // top�� y=0�� ������ �ǰ�  �׷��� �Ϲ�������
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

	///�������

	//int cleared_gap1 = start[0] - end[0]+1; ///////////���������������������������ƤѾƾƤ� 
	

	////�����Ƽ� �� 2���� ���̽��ۿ� ���°� �� �Ϲ�ȭ���� �ʰ� 2���� ������ ��. �ð� ������ �Ϲ�ȭ �ϵ簡
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
		//�Ʒ��� Ŭ����
		for (int i = 0; i < left_gap; i++)  //// 
		{
			for (int j = 30; j < 50; j++)
			{
				s.UI[start[0] - i][j] = s.UI[start[0] - i - (start[0] - end[0] + 1)][j];
			}
		}

		//���� ���� �� Ŭ����
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
	for (int i = 0; i < start.size(); i++) // start�� end����� ���ٴ� �����Ͽ�///
	{
		for(int k=end[i]-1;k>start[i+1];k--)
		{
				//////////////////////////////////////����1~��1 -> ��1 ���� ���� ������ 1ĭ�� ����� ���� ������ ������ 1ĭ�� ����
			int diff = start[i] - end[i]+1;
			for (int j = 30; j < 50; j++)
			{
				s.UI[k][j] = s.UI[k - diff][j];
			}
			
			if (start[i] == start.size() - 1)
			{
				for (int k = start[i]; k > start[i+1]; k--)///////////////���� ���� ������ �����
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
				cout << "��";
				j++;
			}

				
		}
	}
	


}

bool isLine(System& s, int lines[], int lines_size)   /////////col�� �μ��� 1�� y���� lineClear colsize = 37
{
	int i = 0;
	
	for (; i < lines_size; i++) /////////������ ũ��� 37
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
	bool isend = false;                                                 /////////////////whatblock���� x����� �̵��� ������ ���� �ٴڰ� ����� �ָ� ���°ž� y���� �ʿ��ϳ�
	                                                          ///////////�ƴϸ� �� ����� x������̵��ϸ鼭 ���� �Ʒ��κ��� ��ĭ �Ʒ��� �ٴ��� �ִ����� Ȯ������
	                         ////////////////////////////////  y�� �⺻���� ����� ���� �Ʒ��� �ִ� ���� y��ǥ��
	
	for (int i = 3; i >= 0; i--)   //����� �ٴں��� �Ⱦ�� 
	{
		for (int j = 0; j < 8; j++)
		{
			//����
			if (state == LEFT)
			{
				if (whatblock[i][0] ==  3)//|| )//whatblock[i][j] == 3 && whatblock[i][j+1]==3)
				{
					if (s.UI[y - (3 - i)][x - 1+myself] == 2)//////////i��ŭ �����ϱ� �ٴ��� y-0�̰� �� ���� y-1�̴ϱ�
					{
						isend = true;
						return isend;
					}
				}
				else if (whatblock[i][j] == 0 && whatblock[i][j + 1] == 3)
				{
					if (s.UI[y - (3 - i)][x +j- 1+myself] == 2)//////////i��ŭ �����ϱ� �ٴ��� y-0�̰� �� ���� y-1�̴ϱ�
					{
						isend = true;
						return isend;
					}
				}
			}
			//������
			else if (state == RIGHT)
			{
				if (whatblock[i][7]==3)// || (whatblock[i][j] == 3 && whatblock[i][j + 1] == 0))
				{
					if (s.UI[y - (3 - i)][x + 7 + 1-myself] == 2)//////////i��ŭ �����ϱ� �ٴ��� y-0�̰� �� ���� y-1�̴ϱ�
					{
						isend = true;
						return isend;
					}
				}
				else if (whatblock[i][j] == 3 && whatblock[i][j + 1] == 0)
				{
					if (s.UI[y - (3 - i)][x + j + 1-myself] == 2)//////////i��ŭ �����ϱ� �ٴ��� y-0�̰� �� ���� y-1�̴ϱ�
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

void Move(System &s,Action& key, int size_xy[2], int &x, int y, int whatblock[][8], int ctrl) ////x, y�� ����� �����ϴܿ� ��ġ
{       
                                                                             ////////Ư�� �ð����ȸ� �Է� �޴� �ý��� ��� �������� ���� 2������ ����
	/*int countClock = 0;
	while (1)//_kbhit())
	{
		
		int input = _kbhit(); // 1�̸� ������ 0�̸� �ȴ�����
		
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
			//���ְ�

			for (int i = size_xy[1] - 1; i >= 0; i--)
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					//gotoxy(j + x, y - i + 5);
					//cout << i;
					if (y - i >= 0 && s.UI[y - i][j + x] != 2)// && s.UI[y-i][j+x-1]!=2)////////////////////////////////
					{
						gotoxy(j + x, y - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��
						cout << ' ';

					}
				}

			}

			x -= 2;
			for (int i = size_xy[1] - 1; i >= 0; i--)/////////////////////////////������ ���� �׸��ٴ°� �ڳ�
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					if (y - i >= 0)
					{
						gotoxy(j + x, y - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��  ->y��ǥ���� ��� ���� �Ʒ��ڸ��ǰ�->��� ���������� �׸��°�
						if (whatblock[3 - i][j] == 3 && whatblock[3 - i][j + 1] == 3)
						{
							///////���� �����ִ� (�⺻ ���̵� ������ ���̵�) ������ �ǰ��ؼ� �׸���

							cout << "��";
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
			//���ְ�

			for (int i = size_xy[1] - 1; i >= 0; i--)
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					//gotoxy(j + x, y - i + 5);
					//cout << i;
					if (y - i >= 0 && s.UI[y - i][j + x] != 2 )//&& s.UI[y - i][j + x+1] != 2)
					{
						gotoxy(j + x, y - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��
						cout << ' ';

					}
				}

			}

			x += 2;
			for (int i = size_xy[1] - 1; i >= 0; i--)/////////////////////////////������ ���� �׸��ٴ°� �ڳ�
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					if (y - i >= 0)
					{
						gotoxy(j + x, y - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��  ->y��ǥ���� ��� ���� �Ʒ��ڸ��ǰ�->��� ���������� �׸��°�
						if (whatblock[3 - i][j] == 3 && whatblock[3 - i][j + 1] == 3)
						{
							///////���� �����ִ� (�⺻ ���̵� ������ ���̵�) ������ �ǰ��ؼ� �׸���

							cout << "��";
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

	if (ctrl == SPIN)////////////�ݽð�� ������  col�� 2�پ� ��� line 1�ٷ� �ٲ���ϴµ�
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

		////��� ��ĭ ������ x�̵��ϸ鼭 y�� ��� ã�� �����ϱ�
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
		


		if (!at_end(tmp, x, y, s, 1) && x+tmpsize_y-1<50)///tmpsize_y �� ���� ���� x���̴ϱ�, -1�� tmpsize�� ��ǥ�� �ƴ϶� ũ��ϱ�//&& !isNear(tmp, x, y, s, LEFT, 1) && !isNear(tmp, x, y, s, RIGHT, 1))
		{
			
			


			setColor(cyan, black);
			//���ְ� //////���� ��, ����� (2)�� �Ⱦ��ָ� �ǰڳ�

			for (int i = size_xy[1] - 1; i >= 0; i--)
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					//gotoxy(j + x, y - i + 5);
					//cout << i;
					if (y - i >= 0 && s.UI[y - i][j + x] != 2)/////////////////////////////////end���� �� ���Ÿ� ���� ..�ִٴ� ���ɼ�
					{
						gotoxy(j + x, y - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��
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

			
			for (int i = size_xy[1] - 1; i >= 0; i--)/////////////////////////////������ ���� �׸��ٴ°� �ڳ�
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					if (y - i >= 0)
					{
						gotoxy(j + x, y - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��  ->y��ǥ���� ��� ���� �Ʒ��ڸ��ǰ�->��� ���������� �׸��°�
						if (whatblock[3 - i][j] == 3 && whatblock[3 - i][j + 1] == 3)
						{
							///////���� �����ִ� (�⺻ ���̵� ������ ���̵�) ������ �ǰ��ؼ� �׸���

							cout << "��";
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
		if (!at_end(whatblock, x, y, s, -1)) ////////////���� �� ���θ�(0�̸�) ��ġ�� ��찡 ���ܼ� �� -1 (�� ��ĭ �� ����ִ� ���� �������� ���� �� �ֵ���)
		{
			setColor(cyan, black);
			//���ְ� //////���� ��, ����� (2)�� �Ⱦ��ָ� �ǰڳ�

			for (int i = size_xy[1] - 1; i >= 0; i--)
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					//gotoxy(j + x, y - i + 5);
					//cout << i;
					if (y - i >= 0 && s.UI[y - i][j + x] != 2)/////////////////////////////////end���� �� ���Ÿ� ���� ..�ִٴ� ���ɼ�
					{
						gotoxy(j + x, y - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��
						cout << ' ';

					}


				}

			}



			y+=2;
			for (int i = size_xy[1] - 1; i >= 0; i--)/////////////////////////////������ ���� �׸��ٴ°� �ڳ�
			{
				for (int j = 0; j < size_xy[0]; j++)
				{
					if (y - i >= 0)
					{
						gotoxy(j + x, y - i);/////y�� -1���� �������� 0���� �������� ���� �ϴ� 0���� �������� ��  ->y��ǥ���� ��� ���� �Ʒ��ڸ��ǰ�->��� ���������� �׸��°�
						if (whatblock[3 - i][j] == 3 && whatblock[3 - i][j + 1] == 3)
						{
							///////���� �����ִ� (�⺻ ���̵� ������ ���̵�) ������ �ǰ��ؼ� �׸���

							cout << "��";
							j++;
						}

					}


				}

			}


		}
	}
	
}