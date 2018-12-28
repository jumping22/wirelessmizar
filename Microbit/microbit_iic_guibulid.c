#include "microbit_iic_guibulid.h"
#include "delay.h"
#include "i2c_STX.h"
#include "EndDevice.h"
#include "multi_button.h"
#include "GUI.h"

#include "WM.h"

#include "CHECKBOX.h"
#include "FRAMEWIN.h"
#include "PROGBAR.h"
#include "TEXT.h"
#include "BUTTON.h"
#include "SLIDER.h"
#include "HEADER.h"
#include "GRAPH.h"
#include "ICONVIEW.h"
#include "LISTVIEW.h"
#include "TREEVIEW.h"




extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect, "armfly", 0, 0, 0, 320, 240, FRAMEWIN_CF_MOVEABLE, 0 },
	{ BUTTON_CreateIndirect, "蓝宙电子", GUI_ID_BUTTON0, 0, 0, 200, 33, 0, 0 },
	{ TEXT_CreateIndirect, "蓝宙电", GUI_ID_TEXT0, 0, 100, 200, 33, 0, 0 },
//	{ TEXT_CreateIndirect, "蓝宙电电子", GUI_ID_TEXT1, 5, 40, 250, 50, 0, 0 },
//	{ TEXT_CreateIndirect, "蓝宙", GUI_ID_TEXT2, 5, 100, 360, 90, 0, 0 },
//	{ TEXT_CreateIndirect, "蓝", GUI_ID_TEXT3, 5, 220, 144, 144, 0, 0 },
//	{ TEXT_CreateIndirect, "蓝", GUI_ID_TEXT4, 205, 230, 144, 144, 0, 0 },
//	{ TEXT_CreateIndirect, "蓝", GUI_ID_TEXT5, 405, 230, 144, 144, 0, 0 },
//	{ TEXT_CreateIndirect, "蓝", GUI_ID_TEXT6, 605, 230, 144, 144, 0, 0 }
};

/*
*********************************************************************************************************
*  函 数 名: PaintDialog
*  功能说明: 对话框重绘函数
*  形 参：pMsg 消息指针
*  返 回 值: 无
*********************************************************************************************************
*/
void PaintDialog(WM_MESSAGE * pMsg)
{
	// WM_HWIN hWin = pMsg->hWin;
}

/*
*********************************************************************************************************
*  函 数 名: 对话框初始化
*  功能说明: 对话框初始化
*  形 参: pMsg 消息指针
*  返 回 值: 无
*********************************************************************************************************
*/
void InitDialog(WM_MESSAGE * pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	//
	// 配置 FrameWin
	//
	//FRAMEWIN_SetFont(hWin, &GUI_Font32B_ASCII);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_VCENTER | GUI_TA_CENTER);
	FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
	FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
	FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
	FRAMEWIN_SetTitleHeight(hWin, 35);
	/* 外部的 7 种字体在文件控件 TEXT 和按钮控件 BUTTON 中都使用了，具体调用方法如下，
	跟使用 emWin 自带的字体是一样的。*/
	//
	// 按钮的字体是 4 倍抗锯齿，144 点阵
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_BUTTON0), &GUI_FontFontSong);
	//
	// 分别用 16 点阵，32 点阵和 72 点阵字体显示 安富莱电子 五个字。
	//
	TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_RED);
	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT0), &GUI_FontFontSong);
//	TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_GREEN);
//	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT1), &GUI_FontFontSong);
//	TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT2), GUI_BLUE);
//	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT2), &GUI_FontFontSong);
//	//
//	// 分别用 144 点阵汉字，144 点阵的扩展模式且 4 倍抗锯齿汉字，144 点阵的 4 倍抗锯齿汉字和
//	// 144 点阵的扩展模式且带边框汉字。
//	//
//	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT3), &GUI_FontFontSong);
//	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT4), &GUI_FontFontSong);
//	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT5), &GUI_FontFontSong);
//	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT6), &GUI_FontFontSong);
}

/*
*********************************************************************************************************
*  函 数 名: _cbCallback
*  功能说明: 对话框回调函数
*  形 参: pMsg 消息指针
*  返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
	int NCode, Id;
	WM_HWIN hWin = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		PaintDialog(pMsg);
		break;
	case WM_INIT_DIALOG:
		InitDialog(pMsg);
		break;
	case WM_KEY:
		switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key)
		{
		case GUI_KEY_ESCAPE:
			GUI_EndDialog(hWin, 1);
			break;
		case GUI_KEY_ENTER:
			GUI_EndDialog(hWin, 0);
			break;
		}
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id)
		{
		case GUI_ID_OK:
			if (NCode == WM_NOTIFICATION_RELEASED)
				GUI_EndDialog(hWin, 0);
			break;
		case GUI_ID_CANCEL:
			if (NCode == WM_NOTIFICATION_RELEASED)
				GUI_EndDialog(hWin, 0);
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}





void MainTask(void)
{
	//GUI_Init();
	
	WM_MULTIBUF_Enable(1);
	/* 使能 UTF-8 编码，这个是第 3 步中所说的问题，不必限制一定要放在这个位置，使用外部字体之前调用了即可 */
	GUI_UC_SetEncodeUTF8();

	/* 调用此函数会自动的刷新桌面窗口 */
	WM_SetDesktopColor(GUI_WHITE);
	/* 创建对话框 */
	GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);

	while (1)
	{
		GUI_Delay(10);
	}
}




