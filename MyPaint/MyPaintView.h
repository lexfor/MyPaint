
// MyPaintView.h: интерфейс класса CMyPaintView
//
#include<vector>
#include "CMyPaintEllipse.h"
#include "CMyPaintRect.h"
#include "CMyPaintTriangle.h"
#include "CMyPaintConnection.h"

#pragma once


class CMyPaintView : public CView
{
protected: // создать только из сериализации
	CMyPaintView() noexcept;
	DECLARE_DYNCREATE(CMyPaintView)

// Атрибуты
public:
	CMyPaintDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	virtual ~CMyPaintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	//std::vector<CMyPaintFigure*> figure_;
	//std::vector<CMyPaintConnection>connections_;
	unsigned int figureID_;
	int current_;
	int currentConnection_;
	int clickCount_;
	bool rotation_;
	bool edit_;
	bool connect_;
	enum class actionFlagEnum{draw,move,rotate,edit,connect,nothing}actionFlag_;
	enum class figureDrawEnum{rectDraw,ellipseDraw,triangleDraw,nothing}figureDraw_;
	CPoint movePoint_[2];
	CPoint rotatePoint_[2];
	COLORREF currentPenColor_;
	COLORREF currentBrushColor_;
	COLORREF customColors[16];
	int penWidth_;
	int penStyle_;
	int brushStyle_;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	HCURSOR myCursorHand_;
	HCURSOR myCursorStandart_;
	BOOL cursor_;
public:
	void OnButtonRect();
	void OnButtonEllipse();
	void OnButtonTriangle();
	bool findFigure(CPoint );
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextmenuDelete();
	afx_msg void OnContextmenuRotate();
	afx_msg void OnContextmenuNormalize();
	afx_msg void OnContextmenuEdit();
	afx_msg void OnConnectLine();
	afx_msg void OnConnectRightarrow();
	afx_msg void OnConnectLeftarrow();
	afx_msg void OnConnectBiderectionalarrow();
	afx_msg void OnPenColor();
	afx_msg void OnBrushColor();
	afx_msg void OnWidth();
	afx_msg void OnPenStyle();
	afx_msg void OnBrushStyle();
	bool findConnection(CPoint);
	afx_msg void OnLinemenuDelete();
	afx_msg void OnContextmenuProperties();
	afx_msg void OnLinemenuProperties();
	afx_msg void OnListview();
};

#ifndef _DEBUG  // версия отладки в MyPaintView.cpp
inline CMyPaintDoc* CMyPaintView::GetDocument() const
   { return reinterpret_cast<CMyPaintDoc*>(m_pDocument); }
#endif

