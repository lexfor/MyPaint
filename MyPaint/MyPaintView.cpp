
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
#include <afxdlgs.h>
#include "algorithm"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMyPaintView

IMPLEMENT_DYNCREATE(CMyPaintView, CView)

BEGIN_MESSAGE_MAP(CMyPaintView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_RECT,&CMyPaintView::OnButtonRect)
	ON_COMMAND(ID_ELLIPSE,&CMyPaintView::OnButtonEllipse)
	ON_COMMAND(ID_TRIANGLE,&CMyPaintView::OnButtonTriangle)
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
	CClientDC dc(this);
	for (size_t i = 0; i < figure_.size(); i++) {
		if (i == current_ && rotation_) {
			continue;
		}
		figure_[i]->draw(dc, m_hWnd);
	}
	if (rotation_) {
			figure_[current_]->drawTempFigure(dc, m_hWnd);
	}
	for (size_t i = 0; i < connections_.size(); i++) {
		connections_[i].draw(dc);
	}
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
	CClientDC dc(this);
	if (figureDraw_ != figureDrawEnum::nothing) {
		SetCapture();
		CRect rectSize(point, point);
		CString str("Figure");
		str.Format(_T("Figure%i"), figureID_);
		CMyPaintFigure* ptr;
		switch (figureDraw_)
		{
		case figureDrawEnum::rectDraw:
			ptr = new CMyPaintRect(dc, figureID_, str, penWidth_, currentPenColor_, penStyle_, currentBrushColor_, brushStyle_, 0, rectSize);
			figure_.push_back(ptr);
			current_ = figure_.size() - 1;
			actionFlag_ = actionFlagEnum::draw;
			break;
		case figureDrawEnum::ellipseDraw:
			ptr = new CMyPaintEllipse(dc, figureID_, str, penWidth_, currentPenColor_, penStyle_, currentBrushColor_, brushStyle_, 0, rectSize);
			figure_.push_back(ptr);
			current_ = figure_.size() - 1;
			actionFlag_ = actionFlagEnum::draw;
			break;
		case figureDrawEnum::triangleDraw:
			if (clickCount_ == 0) {
				ptr = new CMyPaintTriangle(dc, figureID_, str, penWidth_, currentPenColor_, penStyle_, currentBrushColor_, brushStyle_, 0, point, point, point);
				figure_.push_back(ptr);
				current_ = figure_.size() - 1;
				actionFlag_ = actionFlagEnum::draw;
			}
			if (clickCount_ == 2) {
				actionFlag_ = actionFlagEnum::nothing;
				clickCount_ = 0;
			}
			figure_[current_]->setCoordinates(point, true);
			break;
		default:
			break;
		}
		figureID_++;
		figure_[current_]->draw(dc, m_hWnd);
	}
	if (actionFlag_ == actionFlagEnum::rotate) {
		figure_[current_]->makeTempCoordinatesNull();
		InvalidateRect(figure_[current_]->getCoordinates(), true);
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
	if (cursor_) {
		if (findFigure(point)) {
			movePoint_[0] = point;
			actionFlag_ = actionFlagEnum::move;
		}
	}
	CView::OnLButtonDown(nFlags,point);
}


void CMyPaintView::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	if (actionFlag_ == actionFlagEnum::draw) {
		InvalidateRect(figure_[current_]->getCoordinates(), true);
		figure_[current_]->setCoordinates(point, false);
		figure_[current_]->draw(dc, m_hWnd);
	}
	if (actionFlag_ == actionFlagEnum::move) {
		movePoint_[1] = point;
		std::vector<int>nums;
		InvalidateRect(figure_[current_]->getCoordinates(), true);
		figure_[current_]->move(movePoint_);
		figure_[current_]->draw(dc, m_hWnd);
		nums = figure_[current_]->getConnectionID();
		for (size_t i = 0; i < nums.size(); i++) {
			int key;
			for (size_t j = 0; j < connections_.size(); j++) {
				if (connections_[j].getID() == nums[i]) {
					key = j;
					break;
				}
			}
			InvalidateRect(connections_[key].getInvalidRect());
			if (connections_[key].getFirstFigureID() == figure_[current_]->getID()) {
				connections_[key].moveCoordinates(movePoint_, 0);
			}
			else {
				connections_[key].moveCoordinates(movePoint_, 1);
			}
			connections_[key].draw(dc);
		}
		movePoint_[0] = point;
	}
	if (actionFlag_ == actionFlagEnum::rotate && rotation_) {
		rotatePoint_[1] = point;
		std::vector<int>nums;
		InvalidateRect(figure_[current_]->getTempCoordinates(), true);
		figure_[current_]->rotate(rotatePoint_,false);
		figure_[current_]->drawTempFigure(dc, m_hWnd);
		nums = figure_[current_]->getConnectionID();
	}
	if (actionFlag_ == actionFlagEnum::edit && edit_) {
		InvalidateRect(figure_[current_]->getCoordinates(), true);
		figure_[current_]->edit(point,false);
		figure_[current_]->draw(dc, m_hWnd);
	}
	CView::OnMouseMove(nFlags, point);
}


void CMyPaintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	if (actionFlag_ == actionFlagEnum::draw) {
		InvalidateRect(figure_[current_]->getCoordinates(), true);
		figure_[current_]->setCoordinates(point, false);
		figure_[current_]->draw(dc, m_hWnd);
		if (figureDraw_ == figureDrawEnum::triangleDraw) {
			clickCount_++;
		}
		else {
			actionFlag_ = actionFlagEnum::nothing;
		}
		ReleaseCapture();
	}
	if (actionFlag_ == actionFlagEnum::move) {
		movePoint_[1] = point;
		std::vector<int>nums;
		InvalidateRect(figure_[current_]->getCoordinates(), true);
		figure_[current_]->move(movePoint_);
		figure_[current_]->draw(dc, m_hWnd);
		
		nums = figure_[current_]->getConnectionID();
		for (size_t i = 0; i < nums.size(); i++) {
			int key;
			for (size_t j = 0; j < connections_.size(); j++) {
				if (connections_[j].getID() == nums[i]) {
					key = j;
					break;
				}
			}
			InvalidateRect(connections_[key].getInvalidRect());
			if (connections_[key].getFirstFigureID() == figure_[current_]->getID()) {
				connections_[key].moveCoordinates(movePoint_, 0);
			}
			else {
				connections_[key].moveCoordinates(movePoint_, 1 );
			}
			connections_[key].draw(dc);
		}
		actionFlag_ = actionFlagEnum::nothing;
	}
	if (actionFlag_ == actionFlagEnum::rotate && rotation_) {
		rotatePoint_[1] = point;
		std::vector<int>nums;
		InvalidateRect(figure_[current_]->getTempCoordinates(), true);
		figure_[current_]->rotate(rotatePoint_,true);
		figure_[current_]->draw(dc, m_hWnd);
		nums = figure_[current_]->getConnectionID();
		for (size_t i = 0; i < nums.size(); i++) {
			int key;
			for (size_t j = 0; j < connections_.size(); j++) {
				if (connections_[j].getID() == nums[i]) {
					key = j;
					break;
				}
			}
			InvalidateRect(connections_[key].getInvalidRect());
			if (connections_[key].getFirstFigureID() == figure_[current_]->getID()) {
				connections_[key].setFirstCoordinates(figure_[current_]->getConnectCoordinate(nums[i]));
			}
			else {
				connections_[key].setSecondCoordinates(figure_[current_]->getConnectCoordinate(nums[i]));
			}	
			connections_[key].draw(dc);
		}
		actionFlag_ = actionFlagEnum::nothing;
		rotation_ = false;
		figure_[current_]->makeTempCoordinatesNull();
	}
	if (actionFlag_ == actionFlagEnum::edit && edit_) {
		InvalidateRect(figure_[current_]->getCoordinates(), true);
		if (figure_[current_]->edit(point,true)) {
			figure_[current_]->draw(dc, m_hWnd);
			actionFlag_ = actionFlagEnum::nothing;
			edit_ = false;
		}
		else {
			figure_[current_]->draw(dc, m_hWnd);
		}
	}
	if (actionFlag_ == actionFlagEnum::connect && connect_) {
		if (findFigure(point)) {
			if (figure_[current_]->getID() == connections_[currentConnection_].getFirstFigureID()) {
				connections_.erase(connections_.begin() + currentConnection_);
				actionFlag_ = actionFlagEnum::nothing;
				connect_ = false;
				return;
			}
			CPoint firstCenter;
			int num;
			for (size_t i = 0; i < figure_.size(); i++) {
				if (figure_[i]->checkID(connections_[currentConnection_].getFirstFigureID())) {
					num = i;
				}
			}
			firstCenter = figure_[num]->getCenterCoordinates();
			connections_[currentConnection_].setSecondCoordinates(figure_[current_]->findConnectionCoordinates(firstCenter, connections_[currentConnection_].getID()));
			connections_[currentConnection_].setSecondPlace(figure_[current_]->findConnectionPlace(connections_[currentConnection_].getID()));
			connections_[currentConnection_].setSecondFigureID(figure_[current_]->getID());
			connections_[currentConnection_].setFirstCoordinates(figure_[num]->findConnectionCoordinates(figure_[current_]->getCenterCoordinates(), connections_[currentConnection_].getID()));
			connections_[currentConnection_].setFirstPlace(figure_[num]->findConnectionPlace(connections_[currentConnection_].getID()));
			connections_[currentConnection_].draw(dc);
			currentConnection_++;
			actionFlag_ = actionFlagEnum::nothing;
			connect_ = false;
		}
	}
	CView::OnLButtonUp(nFlags, point);
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
	for (size_t i = 0; i < figure_.size(); i++) {
		if (figure_[i]->ifThisFigure(point)) {
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
	CMenu menu,lineMenu;
	VERIFY(menu.LoadMenu(IDR_MENU1));
	VERIFY(lineMenu.LoadMenuW(IDR_MENU2));
	CMenu* pPopup = menu.GetSubMenu(0);
	CMenu* pPopupLine = lineMenu.GetSubMenu(0);
	cursor_ = true;
	figureDraw_ = figureDrawEnum::nothing;
	actionFlag_ = actionFlagEnum::nothing;
	if (findFigure(point)) {
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
	CView::OnRButtonUp(nFlags, point);
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
	CRect figureRect, connectionRect;
	std::vector<int> nums;
	int deleteID = figure_[current_]->getID();
	figureRect = figure_[current_]->getCoordinates();
	nums = figure_[current_]->getConnectionID();
	figure_.erase(figure_.begin() + current_);
	current_ = 0;
	for (size_t i = 0; i < nums.size(); i++) {
		int num;
		for (size_t j = 0; j < connections_.size(); j++) {
			if (nums[i] == connections_[j].getID()) {
				num = j;
				break;
			}
		}
		if (connections_[num].getFirstFigureID() != deleteID) {
			for (size_t j = 0; j < figure_.size(); j++) {
				if (figure_[j]->getID() == connections_[num].getFirstFigureID()) {
					figure_[j]->deleteConnection(nums[i]);
					connectionRect = connections_[num].getInvalidRect();
					connections_.erase(connections_.begin() + num);
					InvalidateRect(connectionRect, true);
					break;
				}
			}
		}
		else {
			for (size_t j = 0; j < figure_.size(); j++) {
				if (figure_[j]->getID() == connections_[num].getSecondFigureID()) {
					figure_[j]->deleteConnection(nums[i]);
					connectionRect = connections_[num].getInvalidRect();
					connections_.erase(connections_.begin() + num);
					InvalidateRect(connectionRect, true);
					break;
				}
			}
		}
	}
	currentConnection_ = 0;
	InvalidateRect(figureRect, true);

}


void CMyPaintView::OnContextmenuRotate()
{
	actionFlag_ = actionFlagEnum::rotate;
}


void CMyPaintView::OnContextmenuNormalize()
{
	CRect figureRect;
	CClientDC dc(this);
	std::vector<int>nums;
	figureRect = figure_[current_]->getCoordinates();
	figure_[current_]->normalize();
	InvalidateRect(figureRect, true);
	figure_[current_]->draw(dc, m_hWnd);
	nums = figure_[current_]->getConnectionID();
	for (size_t i = 0; i < nums.size(); i++) {
		int key;
		for (size_t j = 0; j < connections_.size(); j++) {
			if (connections_[j].getID() == nums[i]) {
				key = j;
				break;
			}
		} 
		InvalidateRect(connections_[key].getInvalidRect());
		if (connections_[key].getFirstFigureID() == figure_[current_]->getID()) {
			connections_[key].setFirstCoordinates(figure_[current_]->getConnectCoordinate(nums[i]));
		}
		else {
			connections_[key].setSecondCoordinates(figure_[current_]->getConnectCoordinate(nums[i]));
		}
		connections_[key].draw(dc);
	}
}


void CMyPaintView::OnContextmenuEdit()
{
	actionFlag_ = actionFlagEnum::edit;
	CRect  connectionRect;
	std::vector<int> nums;
	nums = figure_[current_]->getConnectionID();
	for (size_t i = 0; i < nums.size(); i++) {
		int num;
		for (size_t j = 0; j < connections_.size(); j++) {
			if (nums[i] == connections_[j].getID()) {
				num = j;
				break;
			}
		}
		if (connections_[num].getFirstFigureID() != figure_[current_]->getID()) {
			for (size_t j = 0; j < figure_.size(); j++) {
				if (figure_[j]->getID() == connections_[num].getFirstFigureID()) {
					figure_[j]->deleteConnection(nums[i]);
					connectionRect = connections_[num].getInvalidRect();
					connections_.erase(connections_.begin() + num);
					InvalidateRect(connectionRect, true);
					break;
				}
			}
		}
		else {
			for (size_t j = 0; j < figure_.size(); j++) {
				if (figure_[j]->getID() == connections_[num].getSecondFigureID()) {
					figure_[j]->deleteConnection(nums[i]);
					connectionRect = connections_[num].getInvalidRect();
					connections_.erase(connections_.begin() + num);
					InvalidateRect(connectionRect, true);
					break;
				}
			}
		}
	}
}


void CMyPaintView::OnConnectLine()
{
	CPoint point;
	CString str("Figure");
	str.Format(_T("Figure%i"), figureID_);
	CMyPaintConnection connection_(figureID_++,str,penWidth_,currentPenColor_,penStyle_,1,0,figure_[current_]->getID(),point);
	connections_.push_back(connection_);
	actionFlag_ = actionFlagEnum::connect;
}


void CMyPaintView::OnConnectRightarrow()
{
	CPoint point;
	CString str("Figure");
	str.Format(_T("Figure%i"), figureID_);
	CMyPaintConnection connection_(figureID_++, str, penWidth_, currentPenColor_, penStyle_, 2, 0, figure_[current_]->getID(), point);
	connections_.push_back(connection_);
	actionFlag_ = actionFlagEnum::connect;
}


void CMyPaintView::OnConnectLeftarrow()
{
	CPoint point;
	CString str("Figure");
	str.Format(_T("Figure%i"), figureID_);
	CMyPaintConnection connection_(figureID_++, str, penWidth_, currentPenColor_, penStyle_, 3, 0, figure_[current_]->getID(), point);
	connections_.push_back(connection_);
	actionFlag_ = actionFlagEnum::connect;
}


void CMyPaintView::OnConnectBiderectionalarrow()
{
	CPoint point;
	CString str("Figure");
	str.Format(_T("Figure%i"), figureID_);
	CMyPaintConnection connection_(figureID_++, str, penWidth_, currentPenColor_, penStyle_, 4, 0, figure_[current_]->getID(), point);
	connections_.push_back(connection_);
	actionFlag_ = actionFlagEnum::connect;
}


void CMyPaintView::OnPenColor()
{
	CColorDialog dlg;
	dlg.m_cc.rgbResult = currentPenColor_;
	dlg.m_cc.lpCustColors = customColors;
	dlg.DoModal();
	currentPenColor_ = dlg.m_cc.rgbResult;
	//Invalidate();
}


void CMyPaintView::OnBrushColor()
{
	CColorDialog dlg;
	dlg.m_cc.rgbResult = currentBrushColor_;
	dlg.m_cc.lpCustColors = customColors;
	dlg.DoModal();
	currentBrushColor_ = dlg.m_cc.rgbResult;
	//Invalidate();
}


void CMyPaintView::OnWidth()
{
	CMyPaintWidthDialog dlg;
	dlg.setCurrentWidth(penWidth_);
	dlg.DoModal();
	penWidth_ = dlg.getCurrentWidth();
	if (penWidth_ > 1 && penStyle_ > 0) {
		AfxMessageBox(L"Данная ширина не совместима с выбранным стилем");
		penStyle_ = 0;
	}
}


void CMyPaintView::OnPenStyle()
{
	CMyPaintPenStyleDialog dlg;
	dlg.setPenStyle(penStyle_);
	dlg.DoModal();
	penStyle_ = dlg.getPenStyle();
	if (penWidth_ > 1 && penStyle_ > 0) {
		AfxMessageBox(L"Данный стиль не совместим с выбранной шириной");
		penWidth_ = 1;
	}
}


void CMyPaintView::OnBrushStyle()
{
	CMyPaintBrushStyleDialog dlg;
	dlg.setBrushStyle(brushStyle_);
	dlg.DoModal();
	brushStyle_ = dlg.getBrushStyle();
}

bool CMyPaintView::findConnection(CPoint point) {
	for (size_t i = 0; i < connections_.size(); i++) {
		connections_[i].ifThisConnection(point);
		currentConnection_ = i;
		return true;
	}
	return false;
}


void CMyPaintView::OnLinemenuDelete()
{
	CRect rect;
	for (size_t i = 0; i < figure_.size(); i++) {
		if (figure_[i]->getID() == connections_[currentConnection_].getFirstFigureID()) {
			figure_[i]->deleteConnection(connections_[currentConnection_].getID());
			continue;
		}
		if (figure_[i]->getID() == connections_[currentConnection_].getSecondFigureID()) {
			figure_[i]->deleteConnection(connections_[currentConnection_].getID());
		}
	}
	rect = connections_[currentConnection_].getInvalidRect();
	connections_.erase(connections_.begin() + currentConnection_);
	InvalidateRect(rect, true);
}


void CMyPaintView::OnContextmenuProperties()
{
	CClientDC dc(this);
	std::vector<int> ids;
	std::vector<CString>names;
	std::map<int, CPoint>nums;
	CRect rect;
	for (size_t i = 0; i < figure_.size(); i++) {
		if (current_ == i) {
			continue;
		}
		ids.push_back(figure_[i]->getID());
		names.push_back(figure_[i]->getName());
	}
	for (size_t i = 0; i < connections_.size(); i++) {
		if (currentConnection_ == i) {
			continue;
		}
		ids.push_back(connections_[i].getID());
		names.push_back(connections_[i].getName());
	}
	rect = figure_[current_]->getCoordinates();
	figure_[current_]->properties(ids,names);
	InvalidateRect(rect, true);
	nums = figure_[current_]->getConnectionCoordinates();
	for (auto i = 0; i < connections_.size(); i++) {
		for (auto it = nums.begin(); it != nums.end(); it++) {
			if (connections_[i].getID() == it->first) {
				rect = connections_[i].getInvalidRect();
				if (connections_[i].getFirstFigureID() == figure_[current_]->getID()) {
					connections_[i].setFirstCoordinates(it->second);
				}
				else {
					connections_[i].setSecondCoordinates(it->second);
				}
				InvalidateRect(rect, true);
				connections_[i].draw(dc);
			}
		}
	}
	figure_[current_]->draw(dc, m_hWnd);
}


void CMyPaintView::OnLinemenuProperties()
{
	CClientDC dc(this);
	std::vector<int> ids;
	std::vector<CString>names;
	CRect rect;
	for (size_t i = 0; i < figure_.size(); i++) {
		if (current_ == i) {
			continue;
		}
		ids.push_back(figure_[i]->getID());
		names.push_back(figure_[i]->getName());
	}
	for (size_t i = 0; i < connections_.size(); i++) {
		if (currentConnection_ == i) {
			continue;
		}
		ids.push_back(connections_[i].getID());
		names.push_back(connections_[i].getName());
	}
	rect = connections_[currentConnection_].getInvalidRect();
	connections_[currentConnection_].properties(ids, names);
	InvalidateRect(rect, true);
	connections_[currentConnection_].draw(dc);
}

