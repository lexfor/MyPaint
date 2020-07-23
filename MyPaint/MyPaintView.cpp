
// MyPaintView.cpp: реализация класса CMyPaintView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MyPaint.h"
#endif

#include "MyPaintDoc.h"
#include "MyPaintView.h"
#include "CMyPaintWidthDialog.h"
#include "CMyPaintPenStyleDialog.h"
#include "CMyPaintBrushStyleDialog.h"
#include "CMyPaintListViewDialog.h"
#include <afxdlgs.h>
#include "algorithm"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMyPaintView

IMPLEMENT_DYNCREATE(CMyPaintView, CScrollView)

BEGIN_MESSAGE_MAP(CMyPaintView, CScrollView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_RECT, &CMyPaintView::OnButtonRect)
	ON_COMMAND(ID_ELLIPSE, &CMyPaintView::OnButtonEllipse)
	ON_COMMAND(ID_TRIANGLE, &CMyPaintView::OnButtonTriangle)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_COMMAND(ID_CONTEXTMENU_DELETE, &CMyPaintView::OnContextmenuDelete)
	ON_COMMAND(ID_CONTEXTMENU_ROTATE, &CMyPaintView::OnContextmenuRotate)
	ON_COMMAND(ID_CONTEXTMENU_NORMALIZE, &CMyPaintView::OnContextmenuNormalize)
	ON_COMMAND(ID_CONTEXTMENU_EDIT, &CMyPaintView::OnContextmenuEdit)
	ON_COMMAND(ID_CONNECT_LINE, &CMyPaintView::OnConnectLine)
	ON_COMMAND(ID_CONNECT_RIGHTARROW, &CMyPaintView::OnConnectRightarrow)
	ON_COMMAND(ID_CONNECT_LEFTARROW, &CMyPaintView::OnConnectLeftarrow)
	ON_COMMAND(ID_CONNECT_BIDERECTIONALARROW, &CMyPaintView::OnConnectBiderectionalarrow)
	ON_COMMAND(ID_PEN_COLOR, &CMyPaintView::OnPenColor)
	ON_COMMAND(ID_BRUSH_COLOR, &CMyPaintView::OnBrushColor)
	ON_COMMAND(ID_WIDTH, &CMyPaintView::OnWidth)
	ON_COMMAND(ID_PEN_STYLE, &CMyPaintView::OnPenStyle)
	ON_COMMAND(ID_BRUSH_STYLE, &CMyPaintView::OnBrushStyle)
	ON_COMMAND(ID_LINEMENU_DELETE, &CMyPaintView::OnLinemenuDelete)
	ON_COMMAND(ID_CONTEXTMENU_PROPERTIES, &CMyPaintView::OnContextmenuProperties)
	ON_COMMAND(ID_LINEMENU_PROPERTIES, &CMyPaintView::OnLinemenuProperties)
	ON_COMMAND(ID_LISTVIEW, &CMyPaintView::OnListview)
	ON_COMMAND(ID_LINE, &CMyPaintView::OnLine)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_COMMAND(ID_CONTEXTMENU_CHECKCROSS, &CMyPaintView::OnContextmenuCheckcross)
END_MESSAGE_MAP()

// Создание или уничтожение CMyPaintView

CMyPaintView::CMyPaintView() noexcept
{
	current_ = 0;
	currentConnection_ = 0;
	figureID_ = 0;
	figureDraw_ = figureDrawEnum::nothing;
	actionFlag_ = actionFlagEnum::nothing;
	clickCount_ = 0;
	cursor_ = true;
	rotation_ = false;
	edit_ = false;
	connect_ = false;
	currentPenColor_ = RGB(0, 0, 0);
	currentBrushColor_ = RGB(0, 0, 0);
	for (int i = 0; i < 16; i++) {
		customColors[i] = RGB(16 * i, 16 * i, 16 * i);
	}
	penWidth_ = 1;
	penStyle_ = PS_SOLID;
	brushStyle_ = 0;
	previous_.x = 0;
	previous_.y = 0;
	next_.x = 0;
	next_.y = 0;
	docSize_ = GetTotalSize();
	isCheckCross = false;
}

CMyPaintView::~CMyPaintView()
{
}

BOOL CMyPaintView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Рисование CMyPaintView

void CMyPaintView::OnDraw(CDC* /*pDC*/)
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HDC dc = ::GetDC(m_hWnd);
	//CClientDC dc(this);
	for (size_t i = 0; i < pDoc->figure_.size(); i++) {
		if (i == current_ && rotation_) {
			continue;
		}
		pDoc->figure_[i]->draw(dc);
	}
	if (rotation_) {
		pDoc->figure_[current_]->tempDraw(dc);
	}
	for (size_t i = 0; i < pDoc->connections_.size(); i++) {
		pDoc->connections_[i].draw(dc);
	}
	::ReleaseDC(m_hWnd, dc);
}


// Печать CMyPaintView

BOOL CMyPaintView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CMyPaintView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CMyPaintView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// Диагностика CMyPaintView

#ifdef _DEBUG
void CMyPaintView::AssertValid() const
{
	CView::AssertValid();
}

void CMyPaintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyPaintDoc* CMyPaintView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyPaintDoc)));
	return (CMyPaintDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CMyPaintView


void CMyPaintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	try {
		if (point.x < 0 || point.y < 0) {
			throw 1;
		}

		CMyPaintDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		HDC dc = ::GetDC(m_hWnd);
		if (figureDraw_ != figureDrawEnum::nothing) {
			SetCapture();
			pDoc->setWidth(penWidth_);
			pDoc->setPenStyle(penStyle_);
			pDoc->setPenColor(currentPenColor_);
			pDoc->setBrushStyle(brushStyle_);
			pDoc->setBrushColor(currentBrushColor_);
			switch (figureDraw_)
			{
			case figureDrawEnum::rectDraw:
				current_ = pDoc->CreateRect(point);
				actionFlag_ = actionFlagEnum::draw;
				break;
			case figureDrawEnum::ellipseDraw:
				current_ = pDoc->CreateEllipse(point);
				actionFlag_ = actionFlagEnum::draw;
				break;
			case figureDrawEnum::triangleDraw:
				if (clickCount_ == 0) {
					current_ = pDoc->CreateTriangle(point);
					actionFlag_ = actionFlagEnum::draw;
				}
				if (clickCount_ == 2) {
					actionFlag_ = actionFlagEnum::nothing;
					ReleaseCapture();
					clickCount_ = 0;
				}
				pDoc->figure_[current_]->setCoordinates(point, true);
				break;
			case figureDrawEnum::lineDraw:
				current_ = pDoc->CreateLine(point);
				actionFlag_ = actionFlagEnum::draw;
			default:
				break;
			}

			if (current_ < 0) {
				throw 2;
			}

			figureID_++;
			pDoc->figure_[current_]->draw(dc);
		}
		if (actionFlag_ == actionFlagEnum::rotate) {
			pDoc->figure_[current_]->makeTempCoordinatesNull();
			pDoc->UpdateAllViews(nullptr);
			rotatePoint_[0] = point;
			rotation_ = true;
			return;
		}
		if (actionFlag_ == actionFlagEnum::edit) {
			edit_ = true;
			return;
		}
		if (actionFlag_ == actionFlagEnum::connect) {
			connect_ = true;
			return;
		}
		if (isCheckCross) {
			if (findFigure(point)) {
				if (current_ == crossFigure[0]) {
					AfxMessageBox(L"Данная фигура уже выбрана");
					return;
				}
				crossFigure[1] = current_;
				isCheckCross = false;
				if (checkCross())
				{
					AfxMessageBox(L"Данные фигуры пересекаются");
				}
				else {
					AfxMessageBox(L"Данные фигуры не пересекаются");
				}
			}
			return;
		}

		if (cursor_) {
			if (findFigure(point)) {
				movePoint_[0] = point;
				actionFlag_ = actionFlagEnum::move;
			}
		}
		::ReleaseDC(m_hWnd, dc);
	}
	catch (int a) {
		switch (a)
		{
		case 1:
			AfxMessageBox(L"Неверные координаты");
			break;
		case 2:
			AfxMessageBox(L"Неправильная фигура");
			actionFlag_ = actionFlagEnum::nothing;
			figureDraw_ = figureDrawEnum::nothing;
			break;
		default:
			break;
		}
	}
	CView::OnLButtonDown(nFlags, point);
}


void CMyPaintView::OnMouseMove(UINT nFlags, CPoint point)
{
	try {
		if (point.x < 0 || point.y < 0) {
			throw 1;
		}
		if (current_ < 0) {
			throw 2;
		}

		CMyPaintDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		HDC dc = ::GetDC(m_hWnd);

		if (actionFlag_ == actionFlagEnum::draw) {
			pDoc->UpdateAllViews(nullptr);
			pDoc->figure_[current_]->setCoordinates(point, false);
			pDoc->figure_[current_]->draw(dc);
		}

		if (actionFlag_ == actionFlagEnum::move) {
			movePoint_[1] = point;
			std::vector<int>nums;
			pDoc->UpdateAllViews(nullptr);
			pDoc->figure_[current_]->move(movePoint_);
			pDoc->figure_[current_]->draw(dc);
			nums = pDoc->figure_[current_]->getConnectionID();
			for (size_t i = 0; i < nums.size(); i++) {
				int key;
				for (size_t j = 0; j < pDoc->connections_.size(); j++) {
					if (pDoc->connections_[j].getID() == nums[i]) {
						key = j;
						break;
					}
				}
				pDoc->UpdateAllViews(nullptr);
				if (pDoc->connections_[key].getFirstFigureID() == pDoc->figure_[current_]->getID()) {
					pDoc->connections_[key].moveCoordinates(movePoint_, 0);
				}
				else {
					pDoc->connections_[key].moveCoordinates(movePoint_, 1);
				}
				pDoc->connections_[key].draw(dc);
			}
			movePoint_[0] = point;
		}

		if (actionFlag_ == actionFlagEnum::rotate && rotation_) {
			rotatePoint_[1] = point;
			std::vector<int>nums;
			pDoc->UpdateAllViews(nullptr);
			pDoc->figure_[current_]->rotate(rotatePoint_, false);
			pDoc->figure_[current_]->tempDraw(dc);
			nums = pDoc->figure_[current_]->getConnectionID();
		}

		if (actionFlag_ == actionFlagEnum::edit && edit_) {
			pDoc->UpdateAllViews(nullptr);
			pDoc->figure_[current_]->edit(point, false);
			pDoc->figure_[current_]->draw(dc);
		}
		::ReleaseDC(m_hWnd, dc);
	}
	catch (int a) {
		switch (a)
		{
		case 1:
			AfxMessageBox(L"Неверные координаты");
			break;
		case 2:
			AfxMessageBox(L"Неверная фигура");
			break;
		default:
			break;
		}
	}
	CView::OnMouseMove(nFlags, point);
}


void CMyPaintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	try {
		if (point.x < 0 || point.y < 0) {
			throw 1;
		}
		if (current_ < 0) {
			throw 2;
		}

		CMyPaintDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		HDC dc = ::GetDC(m_hWnd);
		if (actionFlag_ == actionFlagEnum::draw) {
			pDoc->UpdateAllViews(nullptr);
			pDoc->figure_[current_]->setCoordinates(point, false);
			pDoc->figure_[current_]->draw(dc);
			ReleaseCapture();
			if (figureDraw_ == figureDrawEnum::triangleDraw) {
				clickCount_++;
			}
			else {
				actionFlag_ = actionFlagEnum::nothing;
			}

		}
		if (actionFlag_ == actionFlagEnum::move) {
			movePoint_[1] = point;
			std::vector<int>nums;
			pDoc->UpdateAllViews(nullptr);
			pDoc->figure_[current_]->move(movePoint_);
			pDoc->figure_[current_]->draw(dc);

			nums = pDoc->figure_[current_]->getConnectionID();
			for (size_t i = 0; i < nums.size(); i++) {
				int key;
				for (size_t j = 0; j < pDoc->connections_.size(); j++) {
					if (pDoc->connections_[j].getID() == nums[i]) {
						key = j;
						break;
					}
				}
				pDoc->UpdateAllViews(nullptr);
				if (pDoc->connections_[key].getFirstFigureID() == pDoc->figure_[current_]->getID()) {
					pDoc->connections_[key].moveCoordinates(movePoint_, 0);
				}
				else {
					pDoc->connections_[key].moveCoordinates(movePoint_, 1);
				}
				pDoc->connections_[key].draw(dc);
			}
			actionFlag_ = actionFlagEnum::nothing;
		}
		if (actionFlag_ == actionFlagEnum::rotate && rotation_) {
			rotatePoint_[1] = point;
			std::vector<int>nums;
			pDoc->UpdateAllViews(nullptr);
			pDoc->figure_[current_]->rotate(rotatePoint_, true);
			pDoc->figure_[current_]->draw(dc);
			nums = pDoc->figure_[current_]->getConnectionID();
			for (size_t i = 0; i < nums.size(); i++) {
				int key;
				for (size_t j = 0; j < pDoc->connections_.size(); j++) {
					if (pDoc->connections_[j].getID() == nums[i]) {
						key = j;
						break;
					}
				}
				pDoc->UpdateAllViews(nullptr);
				if (pDoc->connections_[key].getFirstFigureID() == pDoc->figure_[current_]->getID()) {
					pDoc->connections_[key].setFirstCoordinates(pDoc->figure_[current_]->getConnectCoordinate(nums[i]));
				}
				else {
					pDoc->connections_[key].setSecondCoordinates(pDoc->figure_[current_]->getConnectCoordinate(nums[i]));
				}
				pDoc->connections_[key].draw(dc);
			}
			actionFlag_ = actionFlagEnum::nothing;
			rotation_ = false;
			pDoc->figure_[current_]->makeTempCoordinatesNull();
		}
		if (actionFlag_ == actionFlagEnum::edit && edit_) {
			pDoc->UpdateAllViews(nullptr);
			if (pDoc->figure_[current_]->edit(point, true)) {
				pDoc->figure_[current_]->draw(dc);
				actionFlag_ = actionFlagEnum::nothing;
				edit_ = false;
			}
			else {
				pDoc->figure_[current_]->draw(dc);
			}
		}
		if (actionFlag_ == actionFlagEnum::connect && connect_) {
			if (findFigure(point)) {
				if (pDoc->figure_[current_]->getID() == pDoc->connections_[currentConnection_].getFirstFigureID()) {
					pDoc->connections_.erase(pDoc->connections_.begin() + currentConnection_);
					actionFlag_ = actionFlagEnum::nothing;
					connect_ = false;
					return;
				}
				CPoint firstCenter;
				int num;
				for (size_t i = 0; i < pDoc->figure_.size(); i++) {
					if (pDoc->figure_[i]->checkID(pDoc->connections_[currentConnection_].getFirstFigureID())) {
						num = i;
					}
				}
				firstCenter = pDoc->figure_[num]->getCenterCoordinates();
				pDoc->connections_[currentConnection_].setSecondCoordinates(pDoc->figure_[current_]->findConnectionCoordinates(firstCenter, pDoc->connections_[currentConnection_].getID()));
				pDoc->connections_[currentConnection_].setSecondPlace(pDoc->figure_[current_]->findConnectionPlace(pDoc->connections_[currentConnection_].getID()));
				pDoc->connections_[currentConnection_].setSecondFigureID(pDoc->figure_[current_]->getID());
				pDoc->connections_[currentConnection_].setFirstCoordinates(pDoc->figure_[num]->findConnectionCoordinates(pDoc->figure_[current_]->getCenterCoordinates(), pDoc->connections_[currentConnection_].getID()));
				pDoc->connections_[currentConnection_].setFirstPlace(pDoc->figure_[num]->findConnectionPlace(pDoc->connections_[currentConnection_].getID()));
				pDoc->connections_[currentConnection_].draw(dc);
				currentConnection_++;
				actionFlag_ = actionFlagEnum::nothing;
				connect_ = false;
			}
		}
		::ReleaseDC(m_hWnd, dc);
		CView::OnLButtonUp(nFlags, point);
	}
	catch (int a) {
		switch (a)
		{
		case 1:
			AfxMessageBox(L"Неверные координаты");
			break;
		case 2:
			AfxMessageBox(L"Неверная фигура");
			break;
		default:
			break;
		}
	}
}


void CMyPaintView::OnButtonRect()
{
	figureDraw_ = figureDrawEnum::rectDraw;
	cursor_ = false;
}


void CMyPaintView::OnButtonEllipse()
{
	figureDraw_ = figureDrawEnum::ellipseDraw;
	cursor_ = false;
}


void CMyPaintView::OnButtonTriangle()
{
	figureDraw_ = figureDrawEnum::triangleDraw;
	cursor_ = false;
}


bool CMyPaintView::findFigure(CPoint point)
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	for (size_t i = 0; i < pDoc->figure_.size(); i++) {
		if (pDoc->figure_[i]->ifThisFigure(point)) {
			current_ = i;
			return true;
		}
	}
	return false;
}


BOOL CMyPaintView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (cursor_) {
		SetCursor(myCursorHand_);
	}
	else {
		SetCursor(myCursorStandart_);
	}
	return TRUE;
	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CMyPaintView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	HDC dc = ::GetDC(m_hWnd);
	CMenu menu, lineMenu;
	VERIFY(menu.LoadMenu(IDR_MENU1));
	VERIFY(lineMenu.LoadMenuW(IDR_MENU2));
	CMenu* pPopup = menu.GetSubMenu(0);
	CMenu* pPopupLine = lineMenu.GetSubMenu(0);
	cursor_ = true;
	figureDraw_ = figureDrawEnum::nothing;
	actionFlag_ = actionFlagEnum::nothing;
	if (findFigure(point)) {
		pDoc->figure_[current_]->select(dc);
		pPopup->TrackPopupMenuEx(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
		pPopup->DestroyMenu();
		menu.DestroyMenu();
	}
	else {
		if (findConnection(point)) {
			pPopupLine->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
			pPopupLine->DestroyMenu();
			lineMenu.DestroyMenu();
		}
	}
	pDoc->UpdateAllViews(nullptr);
	CView::OnRButtonUp(nFlags, point);
	::ReleaseDC(m_hWnd, dc);
}


int CMyPaintView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CMyPaintApp* pApp;
	pApp = (CMyPaintApp*)AfxGetApp();
	myCursorHand_ = pApp->LoadCursorW(IDC_CURSOR1);
	myCursorStandart_ = pApp->LoadCursorW(IDC_CURSOR2);
	return 0;
}


void CMyPaintView::OnContextmenuDelete()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	std::vector<int> nums;
	int deleteID = pDoc->figure_[current_]->getID();
	nums = pDoc->figure_[current_]->getConnectionID();
	pDoc->figure_.erase(pDoc->figure_.begin() + current_);
	current_ = 0;
	for (size_t i = 0; i < nums.size(); i++) {
		int num;
		for (size_t j = 0; j < pDoc->connections_.size(); j++) {
			if (nums[i] == pDoc->connections_[j].getID()) {
				num = j;
				break;
			}
		}
		if (pDoc->connections_[num].getFirstFigureID() != deleteID) {
			for (size_t j = 0; j < pDoc->figure_.size(); j++) {
				if (pDoc->figure_[j]->getID() == pDoc->connections_[num].getFirstFigureID()) {
					pDoc->figure_[j]->deleteConnection(nums[i]);
					pDoc->connections_.erase(pDoc->connections_.begin() + num);
					pDoc->UpdateAllViews(nullptr);
					break;
				}
			}
		}
		else {
			for (size_t j = 0; j < pDoc->figure_.size(); j++) {
				if (pDoc->figure_[j]->getID() == pDoc->connections_[num].getSecondFigureID()) {
					pDoc->figure_[j]->deleteConnection(nums[i]);
					pDoc->connections_.erase(pDoc->connections_.begin() + num);
					pDoc->UpdateAllViews(nullptr);
					break;
				}
			}
		}
	}
	currentConnection_ = 0;
	pDoc->UpdateAllViews(nullptr);

}


void CMyPaintView::OnContextmenuRotate()
{
	actionFlag_ = actionFlagEnum::rotate;
}


void CMyPaintView::OnContextmenuNormalize()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	HDC dc = ::GetDC(m_hWnd);
	std::vector<int>nums;
	pDoc->figure_[current_]->normalize();
	pDoc->UpdateAllViews(nullptr);
	pDoc->figure_[current_]->draw(dc);
	nums = pDoc->figure_[current_]->getConnectionID();
	for (size_t i = 0; i < nums.size(); i++) {
		int key;
		for (size_t j = 0; j < pDoc->connections_.size(); j++) {
			if (pDoc->connections_[j].getID() == nums[i]) {
				key = j;
				break;
			}
		}
		pDoc->UpdateAllViews(nullptr);
		if (pDoc->connections_[key].getFirstFigureID() == pDoc->figure_[current_]->getID()) {
			pDoc->connections_[key].setFirstCoordinates(pDoc->figure_[current_]->getConnectCoordinate(nums[i]));
		}
		else {
			pDoc->connections_[key].setSecondCoordinates(pDoc->figure_[current_]->getConnectCoordinate(nums[i]));
		}
		pDoc->connections_[key].draw(dc);
	}
	::ReleaseDC(m_hWnd, dc);
}


void CMyPaintView::OnContextmenuEdit()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	actionFlag_ = actionFlagEnum::edit;
	std::vector<int> nums;
	nums = pDoc->figure_[current_]->getConnectionID();
	for (size_t i = 0; i < nums.size(); i++) {
		int num;
		for (size_t j = 0; j < pDoc->connections_.size(); j++) {
			if (nums[i] == pDoc->connections_[j].getID()) {
				num = j;
				break;
			}
		}
		if (pDoc->connections_[num].getFirstFigureID() != pDoc->figure_[current_]->getID()) {
			for (size_t j = 0; j < pDoc->figure_.size(); j++) {
				if (pDoc->figure_[j]->getID() == pDoc->connections_[num].getFirstFigureID()) {
					pDoc->figure_[j]->deleteConnection(nums[i]);
					pDoc->connections_.erase(pDoc->connections_.begin() + num);
					pDoc->UpdateAllViews(nullptr);
					break;
				}
			}
		}
		else {
			for (size_t j = 0; j < pDoc->figure_.size(); j++) {
				if (pDoc->figure_[j]->getID() == pDoc->connections_[num].getSecondFigureID()) {
					pDoc->figure_[j]->deleteConnection(nums[i]);
					pDoc->connections_.erase(pDoc->connections_.begin() + num);
					pDoc->UpdateAllViews(nullptr);
					break;
				}
			}
		}
	}
}


void CMyPaintView::OnConnectLine()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CPoint point;
	CString str;
	str.Format(_T("Line%i"), figureID_);
	CMyPaintConnection connection_(figureID_++, str, penWidth_, currentPenColor_, penStyle_, 1, 0, pDoc->figure_[current_]->getID(), point);
	pDoc->connections_.push_back(connection_);
	actionFlag_ = actionFlagEnum::connect;
}


void CMyPaintView::OnConnectRightarrow()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CPoint point;
	CString str;
	str.Format(_T("Right arrow%i"), figureID_);
	CMyPaintConnection connection_(figureID_++, str, penWidth_, currentPenColor_, penStyle_, 2, 0, pDoc->figure_[current_]->getID(), point);
	pDoc->connections_.push_back(connection_);
	actionFlag_ = actionFlagEnum::connect;
}


void CMyPaintView::OnConnectLeftarrow()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CPoint point;
	CString str;
	str.Format(_T("Left arrow%i"), figureID_);
	CMyPaintConnection connection_(figureID_++, str, penWidth_, currentPenColor_, penStyle_, 3, 0, pDoc->figure_[current_]->getID(), point);
	pDoc->connections_.push_back(connection_);
	actionFlag_ = actionFlagEnum::connect;
}


void CMyPaintView::OnConnectBiderectionalarrow()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CPoint point;
	CString str;
	str.Format(_T("Biderectional arrow%i"), figureID_);
	CMyPaintConnection connection_(figureID_++, str, penWidth_, currentPenColor_, penStyle_, 4, 0, pDoc->figure_[current_]->getID(), point);
	pDoc->connections_.push_back(connection_);
	actionFlag_ = actionFlagEnum::connect;
}


void CMyPaintView::OnPenColor()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CColorDialog dlg;
	currentPenColor_ = pDoc->getPenColor();
	dlg.m_cc.rgbResult = currentPenColor_;
	dlg.m_cc.lpCustColors = customColors;
	dlg.DoModal();
	currentPenColor_ = dlg.m_cc.rgbResult;
	pDoc->setPenColor(currentPenColor_);
}


void CMyPaintView::OnBrushColor()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CColorDialog dlg;
	currentBrushColor_ = pDoc->getBrushColor();
	dlg.m_cc.rgbResult = currentBrushColor_;
	dlg.m_cc.lpCustColors = customColors;
	dlg.DoModal();
	currentBrushColor_ = dlg.m_cc.rgbResult;
	pDoc->setBrushColor(currentBrushColor_);
}


void CMyPaintView::OnWidth()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CMyPaintWidthDialog dlg;
	penWidth_ = pDoc->getWidth();
	dlg.setCurrentWidth(penWidth_);
	dlg.DoModal();
	penWidth_ = dlg.getCurrentWidth();
	if (penWidth_ > 1 && penStyle_ > 0) {
		AfxMessageBox(L"Данная ширина не совместима с выбранным стилем");
		penStyle_ = 0;
		pDoc->setPenStyle(penStyle_);
	}
	pDoc->setWidth(penWidth_);
}


void CMyPaintView::OnPenStyle()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CMyPaintPenStyleDialog dlg;
	penStyle_ = pDoc->getPenStyle();
	dlg.setPenStyle(penStyle_);
	dlg.DoModal();
	penStyle_ = dlg.getPenStyle();
	if (penWidth_ > 1 && penStyle_ > 0) {
		AfxMessageBox(L"Данный стиль не совместим с выбранной шириной");
		penWidth_ = 1;
		pDoc->setWidth(penWidth_);
	}
	pDoc->setPenStyle(penStyle_);
}


void CMyPaintView::OnBrushStyle()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CMyPaintBrushStyleDialog dlg;
	brushStyle_ = pDoc->getBrushStyle();
	dlg.setBrushStyle(brushStyle_);
	dlg.DoModal();
	brushStyle_ = dlg.getBrushStyle();
	pDoc->setBrushStyle(brushStyle_);
}

bool CMyPaintView::findConnection(CPoint point) {
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	for (size_t i = 0; i < pDoc->connections_.size(); i++) {
		pDoc->connections_[i].ifThisConnection(point);
		currentConnection_ = i;
		return true;
	}
	return false;
}


void CMyPaintView::OnLinemenuDelete()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	for (size_t i = 0; i < pDoc->figure_.size(); i++) {
		if (pDoc->figure_[i]->getID() == pDoc->connections_[currentConnection_].getFirstFigureID()) {
			pDoc->figure_[i]->deleteConnection(pDoc->connections_[currentConnection_].getID());
			continue;
		}
		if (pDoc->figure_[i]->getID() == pDoc->connections_[currentConnection_].getSecondFigureID()) {
			pDoc->figure_[i]->deleteConnection(pDoc->connections_[currentConnection_].getID());
		}
	}
	pDoc->connections_.erase(pDoc->connections_.begin() + currentConnection_);
	pDoc->UpdateAllViews(nullptr);
}


void CMyPaintView::OnContextmenuProperties()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CClientDC dc(this);
	std::vector<int> ids;
	std::vector<CString>names;
	std::map<int, CPoint>nums;
	for (size_t i = 0; i < pDoc->figure_.size(); i++) {
		if (current_ == i) {
			continue;
		}
		ids.push_back(pDoc->figure_[i]->getID());
		names.push_back(pDoc->figure_[i]->getName());
	}
	for (size_t i = 0; i < pDoc->connections_.size(); i++) {
		if (currentConnection_ == i) {
			continue;
		}
		ids.push_back(pDoc->connections_[i].getID());
		names.push_back(pDoc->connections_[i].getName());
	}
	pDoc->figure_[current_]->properties(ids, names);
	pDoc->UpdateAllViews(nullptr);
	nums = pDoc->figure_[current_]->getConnectionCoordinates();
	for (auto i = 0; i < pDoc->connections_.size(); i++) {
		for (auto it = nums.begin(); it != nums.end(); it++) {
			if (pDoc->connections_[i].getID() == it->first) {
				if (pDoc->connections_[i].getFirstFigureID() == pDoc->figure_[current_]->getID()) {
					pDoc->connections_[i].setFirstCoordinates(it->second);
				}
				else {
					pDoc->connections_[i].setSecondCoordinates(it->second);
				}
				pDoc->UpdateAllViews(nullptr);
				pDoc->connections_[i].draw(dc);
			}
		}
	}
	pDoc->figure_[current_]->draw(dc);
}


void CMyPaintView::OnLinemenuProperties()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CClientDC dc(this);
	std::vector<int> ids;
	std::vector<CString>names;
	for (size_t i = 0; i < pDoc->figure_.size(); i++) {
		if (current_ == i) {
			continue;
		}
		ids.push_back(pDoc->figure_[i]->getID());
		names.push_back(pDoc->figure_[i]->getName());
	}
	for (size_t i = 0; i < pDoc->connections_.size(); i++) {
		if (currentConnection_ == i) {
			continue;
		}
		ids.push_back(pDoc->connections_[i].getID());
		names.push_back(pDoc->connections_[i].getName());
	}
	pDoc->connections_[currentConnection_].properties(ids, names);
	pDoc->UpdateAllViews(nullptr);
	pDoc->connections_[currentConnection_].draw(dc);
}



void CMyPaintView::OnListview()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CMyPaintListViewDialog dlg;
	dlg.getFigure(pDoc->figure_);
	dlg.getConnections(pDoc->connections_);
	dlg.DoModal();
}



void CMyPaintView::OnLine()
{
	figureDraw_ = figureDrawEnum::lineDraw;
	cursor_ = false;
}


BOOL CMyPaintView::OnEraseBkgnd(CDC* pDC)
{

	return 1;
}


void CMyPaintView::OnPaint()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect client; CPoint diff;
	::GetClientRect(m_hWnd, &client);
	next_ = GetScrollPosition();
	diff.x = previous_.x - next_.x;
	diff.y = previous_.y - next_.y;
	docSize_.cx += diff.x;
	docSize_.cy += diff.y;
	if (docSize_.cx < 1500) {
		docSize_.cx += 100;
		CSize size = GetTotalSize();
		size.cx += 100;
		SetScrollSizes(MM_TEXT, size, CSize(500, 500), CSize(50, 50));
	}
	if (docSize_.cy < 500) {
		docSize_.cy += 100;
		CSize size = GetTotalSize();
		size.cy += 100;
		SetScrollSizes(MM_TEXT, size, CSize(500, 500), CSize(50, 50));
	}
	PAINTSTRUCT ps;
	::BeginPaint(m_hWnd, &ps);
	HDC dc = ::GetDC(m_hWnd);
	back_buffer = ::CreateCompatibleDC(dc);
	HBITMAP hbmp = ::CreateCompatibleBitmap(dc, client.Width(), client.Height());
	::SelectObject(back_buffer, hbmp);
	::FillRect(back_buffer, &client, (HBRUSH)GetStockObject(WHITE_BRUSH));
	for (auto i = 0; i < pDoc->figure_.size(); i++) {
		pDoc->figure_[i]->scrollFigure(diff);
		if (i == current_ && rotation_) {
			continue;
		}
		pDoc->figure_[i]->draw(back_buffer);
	}
	if (rotation_) {
		pDoc->figure_[current_]->tempDraw(back_buffer);
	}
	for (size_t i = 0; i < pDoc->connections_.size(); i++) {
		pDoc->connections_[i].scrollConnection(diff);
		pDoc->connections_[i].draw(back_buffer);
	}
	::BitBlt(dc, 0, 0, client.Width(), client.Height(), back_buffer, 0, 0, SRCCOPY);
	::EndPaint(m_hWnd, &ps);
	::ReleaseDC(m_hWnd, back_buffer);
	::ReleaseDC(m_hWnd, dc);
	::DeleteObject(hbmp);
	::DeleteDC(back_buffer);
	previous_ = next_;
}


void CMyPaintView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();


	CSize DocSize(2000, 2000);
	SetScrollSizes(MM_TEXT, DocSize, CSize(500, 500), CSize(50, 50));
}


bool CMyPaintView::checkCross()
{
	CMyPaintDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int rightFigureID,leftFigureID, topFigureID, bottomFigureID;
	if (pDoc->figure_[crossFigure[0]]->getCenterCoordinates().x > pDoc->figure_[crossFigure[1]]->getCenterCoordinates().x) {
		rightFigureID = crossFigure[0];
		leftFigureID = crossFigure[1];
	}
	else {
		rightFigureID = crossFigure[1];
		leftFigureID = crossFigure[0];
	}
	if (pDoc->figure_[crossFigure[0]]->getCenterCoordinates().y > pDoc->figure_[crossFigure[1]]->getCenterCoordinates().y) {
		topFigureID = crossFigure[0];
		bottomFigureID = crossFigure[1];
	}
	else {
		topFigureID = crossFigure[1];
		bottomFigureID = crossFigure[0];
	}
	if (pDoc->figure_[crossFigure[0]]->ifThisFigure(pDoc->figure_[crossFigure[1]]->getCenterCoordinates())) {
		return true;
	}
	if (pDoc->figure_[crossFigure[1]]->ifThisFigure(pDoc->figure_[crossFigure[0]]->getCenterCoordinates())) {
		return true;
	}
	std::vector<CPoint> temp;
	temp = pDoc->figure_[rightFigureID]->getLeftCoordinate();
	for (auto i = 0; i < temp.size(); i++) {
		if (pDoc->figure_[leftFigureID]->ifThisFigure(temp[i])) {
			return true;
		}
	}
	temp = pDoc->figure_[leftFigureID]->getRightCoordinate();
	for (auto i = 0; i < temp.size(); i++) {
		if (pDoc->figure_[rightFigureID]->ifThisFigure(temp[i])) {
			return true;
		}
	}
	temp = pDoc->figure_[bottomFigureID]->getTopCoordinate();
	for (auto i = 0; i < temp.size(); i++) {
		if (pDoc->figure_[topFigureID]->ifThisFigure(temp[i])) {
			return true;
		}
	}
	temp = pDoc->figure_[topFigureID]->getBottomCoordinate();
	for (auto i = 0; i < temp.size(); i++) {
		if (pDoc->figure_[bottomFigureID]->ifThisFigure(temp[i])) {
			return true;
		}
	}
	std::vector<LONG> FirstMaxMinX = pDoc->figure_[crossFigure[0]]->getMaxMinX();
	std::vector<LONG> FirstMaxMinY = pDoc->figure_[crossFigure[0]]->getMaxMinY();
	std::vector<LONG> SecondMaxMinX = pDoc->figure_[crossFigure[1]]->getMaxMinX();
	std::vector<LONG> SecondMaxMinY = pDoc->figure_[crossFigure[1]]->getMaxMinY();
	if (FirstMaxMinX[0] > SecondMaxMinX[0] && FirstMaxMinX[1] < SecondMaxMinX[1] && FirstMaxMinY[0] < SecondMaxMinY[0] && FirstMaxMinY[1] > SecondMaxMinY[1]) {
		return true;
	}
	if (FirstMaxMinX[0] < SecondMaxMinX[0] && FirstMaxMinX[1] > SecondMaxMinX[1] && FirstMaxMinY[0] > SecondMaxMinY[0] && FirstMaxMinY[1] < SecondMaxMinY[1]) {
		return true;
	}
	return false;
}


void CMyPaintView::OnContextmenuCheckcross()
{
	crossFigure[0] = current_;
	isCheckCross = true;
}
