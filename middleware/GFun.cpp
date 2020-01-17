#include "StdAfx.h"


DWORD GetChatColor(BYTE bIdx)
{
	DWORD dwClr = 0;

	switch ( bIdx )
	{
	case _CHAT_COLOR1:		dwClr = RGB(  0,   0,   0);		break;
	case _CHAT_COLOR2:		dwClr = RGB( 10,  10,  10);		break;
	case _CHAT_COLOR3:		dwClr = RGB(255, 255, 255);		break;
	case _CHAT_COLOR4:		dwClr = RGB(255,   0,   0);		break;
	case _CHAT_COLOR5:		dwClr = RGB(  0, 255,   0);		break;
	case _CHAT_COLOR6:		dwClr = RGB(  0,   0, 255);		break;
	case _CHAT_COLOR7:		dwClr = RGB(255, 255,   0);		break;
	case _CHAT_COLOR8:		dwClr = RGB(255, 128,   0);		break;
	}

	return dwClr;
}
