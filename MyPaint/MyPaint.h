
// MyPaint.h: основной файл заголовка для приложения MyPaint
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CMyPaintApp:
// Сведения о реализации этого класса: MyPaint.cpp
//

class CMyPaintApp : public CWinApp
{
public:
	CMyPaintApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMyPaintApp theApp;
