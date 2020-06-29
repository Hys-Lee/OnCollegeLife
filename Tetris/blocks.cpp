#include "blocks.h"



void blocks::Create(int whatblock[][8], int& place, int size_xy[2])     //////1: T, 2: Sq, 3: Z, 4: Z_r, 5: St, 6: L, 7: L_r  -> 어떤 블록인지, 어떤 위치인지
{
	srand(time(NULL));
	place = (rand() % 9)*2;////맨 왼쪽(0)부터 맨 오른쪽까지


	int wblock = rand() % 7 ;
	
	switch(wblock)
	{
	case T: /////////////////////// ㅏ 모양으로 떨어질꺼임
		for (int i = 0; i <8 ; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				whatblock[i][j] = Block_T[i][j];
			}
		}

		//8가지
		//맨 처음에 srand쓰면 그 다음부터 완벽한 random되는거야? ㅇㅇ
		
		
		

		size_xy[0] = size_T[0];
		size_xy[1] = size_T[1];
		break;
	case Sq:////////////////// ㅁ 모양으로 떨어질꺼임
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				whatblock[i][j] = Block_Sq[i][j];
			}
		}


		

		

		size_xy[0] = size_Sq[0];
		size_xy[1] = size_Sq[1];
		break;
	case Z:///////////////// 번개모양으로 떨어질거임
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				whatblock[i][j] = Block_Z[i][j];
			}
		}
		

		

		size_xy[0] = size_Z[0];
		size_xy[1] = size_Z[1];
		break;
	case Z_r:///////////////번개모양으로 떨어질거임
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				whatblock[i][j] = Block_Z_r[i][j];
			}
		}
		

		

		size_xy[0] = size_Z_r[0];
		size_xy[1] = size_Z_r[1];
		break;
	case St: ///////////////세로로 떨어질거임
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				whatblock[i][j] = Block_St[i][j];
			}
		}
		

		

		size_xy[0] = size_St[0];
		size_xy[1] = size_St[1];
		break;
	case L:///////////////// L 모양으로 떨어질거임 
		for (int i = 0; i <8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				whatblock[i][j] = Block_L[i][j];
			}
		}
		

		

		size_xy[0] = size_L[0];
		size_xy[1] = size_L[1];
		break;
	case L_r://///////////////L 좌우반전 모양으로 떨어질거임
		for (int i =0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				whatblock[i][j] = Block_L_r[i][j];
			}
		}
		

		

		size_xy[0] = size_L_r[0];
		size_xy[1] = size_L_r[1];
		break;
	}
}
