#include "action.h"

void Action:: set_ingame(bool ig)
{
	ingame = ig; 
}
bool Action::get_ingame() 
{
	return ingame; 
}
int Action:: KeyControl()
{
	char temp = _getch();
	if (temp == 228 || temp == 0)
	{
		temp = _getch();
		if (temp == 75)
		{
			return LEFT;
		}
		else if (temp == 77)
		{
			return RIGHT;
		}
		else if (temp == 72)
		{
			return UP;
		}
		else if (temp == 80)
		{
			return DOWN;
		}
	}
	else if (temp == ' ')
	{
		if (ingame == true)
			return SPIN;
		else
			return SELECT;
	}

}
