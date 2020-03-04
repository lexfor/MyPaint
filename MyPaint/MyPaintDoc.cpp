
// MyPaintDoc.cpp: реализация класса CMyPaintDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MyPaint.h"

#include <map>
#endif

#include "MyPaintDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMyPaintDoc

IMPLEMENT_DYNCREATE(CMyPaintDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyPaintDoc, CDocument)
	ON_COMMAND(ID_FILE_SEND_MAIL, &CMyPaintDoc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CMyPaintDoc::OnUpdateFileSendMail)
END_MESSAGE_MAP()


// Создание или уничтожение CMyPaintDoc

CMyPaintDoc::CMyPaintDoc() noexcept
{
	id_ = 0;
	penColor_ = RGB(0, 0, 0);
	brushColor_ = RGB(0, 0, 0);
	for (int i = 0; i < 16; i++) {
		customColors[i] = RGB(16 * i, 16 * i, 16 * i);
	}
	penWidth_ = 1;
	penStyle_ = PS_SOLID;
	brushStyle_ = 0;

}

CMyPaintDoc::~CMyPaintDoc()
{
}

BOOL CMyPaintDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}




// Сериализация CMyPaintDoc

void CMyPaintDoc::Serialize(CArchive& ar)
{
	std::map<int, CPoint> connectionCoordinate_;
	int figureSize_, connectionSize_, figureType_, id_, width_, penStyle_, brushStyle_, connectionType_, connectionPlace_[2], figureID_[2], connectionCoordinateSize_, key;
	COLORREF penColor_, brushColor_;
	CString name_;
	CPoint figureCoordinate[3], connectionCoordinate[2], point;

	if (ar.IsStoring())
	{
		ar << figure_.size();
		for (auto i = 0; i < figure_.size(); i++) {
			connectionCoordinate_ = figure_[i]->getConnectionCoordinates();
			ar << figure_[i]->getFigureType();
			ar << figure_[i]->getFirstCoordinate();
			ar << figure_[i]->getSecondCoordinate();
			ar << figure_[i]->getThirdCoordinate();
			ar << figure_[i]->getID();
			ar << figure_[i]->getName();
			ar << figure_[i]->getWidth();
			ar << figure_[i]->getPenColor();
			ar << figure_[i]->getPenStyle();
			ar << figure_[i]->getBrushStyle();
			ar << figure_[i]->getBrushColor();
			ar << connectionCoordinate_.size();
			for (auto it = connectionCoordinate_.begin(); it != connectionCoordinate_.end(); it++) {
				ar << it->first;
				ar << it->second;
			}
		}
		ar << connections_.size();
		for (auto i = 0; i < connections_.size(); i++) {
			ar << connections_[i].getConnectionType();
			ar << connections_[i].getFirstConnectionPlace();
			ar << connections_[i].getSecondConnectionPlace();
			ar << connections_[i].getFirstFigureID();
			ar << connections_[i].getSecondFigureID();
			ar << connections_[i].getFirstCoordinate();
			ar << connections_[i].getSecondCoordinate();
			ar << connections_[i].getID();
			ar << connections_[i].getName();
			ar << connections_[i].getWidth();
			ar << connections_[i].getColor();
			ar << connections_[i].getStyle();
		}
	}
	else
	{
		ar >> figureSize_;
		for (auto i = 0; i < figureSize_; i++) {
			CMyPaintFigure* ptr;
			ar >> figureType_;
			ar >> figureCoordinate[0];
			ar >> figureCoordinate[1];
			ar >> figureCoordinate[2];
			ar >> id_;
			ar >> name_;
			ar >> width_;
			ar >> penColor_;
			ar >> penStyle_;
			ar >> brushStyle_;
			ar >> brushColor_;
			switch (figureType_)
			{
			case 1:
				ptr = new CMyPaintEllipse;
				ptr->setFirstCoordinate(figureCoordinate[0]);
				ptr->setSecondCoordinate(figureCoordinate[1]);
				ptr->changeOtherCoordinates();
				ptr->setID(id_);
				ptr->setName(name_);
				ptr->setWidth(width_);
				ptr->setPenColor(penColor_);
				ptr->setPenStyle(penStyle_);
				ptr->setBrushStyle(brushStyle_);
				ptr->setBrushColor(brushColor_);
				ar >> connectionCoordinateSize_;
				for (auto j = 0; j < connectionCoordinateSize_; j++) {
					ar >> key;
					ar >> point;
					std::pair<int, CPoint> Pair(key, point);
					ptr->addConnectionCoordinate(Pair);
				}
				break;
			case 2:
				ptr = new CMyPaintRect;
				ptr->setFirstCoordinate(figureCoordinate[0]);
				ptr->setSecondCoordinate(figureCoordinate[1]);
				ptr->changeOtherCoordinates();
				ptr->setID(id_);
				ptr->setName(name_);
				ptr->setWidth(width_);
				ptr->setPenColor(penColor_);
				ptr->setPenStyle(penStyle_);
				ptr->setBrushStyle(brushStyle_);
				ptr->setBrushColor(brushColor_);
				ar >> connectionCoordinateSize_;
				for (auto j = 0; j < connectionCoordinateSize_; j++) {
					ar >> key;
					ar >> point;
					std::pair<int, CPoint> Pair(key, point);
					ptr->addConnectionCoordinate(Pair);
				}
				break;
			case 3:
				ptr = new CMyPaintTriangle;
				ptr->setFirstCoordinate(figureCoordinate[0]);
				ptr->setSecondCoordinate(figureCoordinate[1]);
				ptr->setThirdCoordinate(figureCoordinate[2]);
				ptr->setID(id_);
				ptr->setName(name_);
				ptr->setWidth(width_);
				ptr->setPenColor(penColor_);
				ptr->setPenStyle(penStyle_);
				ptr->setBrushStyle(brushStyle_);
				ptr->setBrushColor(brushColor_);
				ar >> connectionCoordinateSize_;
				for (auto j = 0; j < connectionCoordinateSize_; j++) {
					ar >> key;
					ar >> point;
					std::pair<int, CPoint> Pair(key, point);
					ptr->addConnectionCoordinate(Pair);
				}
				break;
			case 4:
				ptr = new CMyPaintLine;
				ptr->setFirstCoordinate(figureCoordinate[0]);
				ptr->setSecondCoordinate(figureCoordinate[1]);
				ptr->setID(id_);
				ptr->setName(name_);
				ptr->setWidth(width_);
				ptr->setPenColor(penColor_);
				ptr->setPenStyle(penStyle_);
				ptr->setBrushStyle(brushStyle_);
				ptr->setBrushColor(brushColor_);
				ar >> connectionCoordinateSize_;
				for (auto j = 0; j < connectionCoordinateSize_; j++) {
					ar >> key;
					ar >> point;
					std::pair<int, CPoint> Pair(key, point);
					ptr->addConnectionCoordinate(Pair);
				}
				break;
			default:
				break;
			}
			figure_.push_back(ptr);
		}
		ar >> connectionSize_;
		for (auto i = 0; i < connectionSize_; i++) {
			ar >> connectionType_;
			ar >> connectionPlace_[0];
			ar >> connectionPlace_[1];
			ar >> figureID_[0];
			ar >> figureID_[1];
			ar >> connectionCoordinate[0];
			ar >> connectionCoordinate[1];
			ar >> id_;
			ar >> name_;
			ar >> width_;
			ar >> penColor_;
			ar >> penStyle_;
			CMyPaintConnection con(id_, name_, width_, penColor_, penStyle_, connectionType_, connectionPlace_[0], figureID_[0], connectionCoordinate[0]);
			con.setSecondPlace(connectionPlace_[1]);
			con.setSecondFigureID(figureID_[1]);
			con.setSecondCoordinates(connectionCoordinate[1]);
			connections_.push_back(con);
		}
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CMyPaintDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CMyPaintDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CMyPaintDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CMyPaintDoc

#ifdef _DEBUG
void CMyPaintDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyPaintDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

int CMyPaintDoc::CreateEllipse(CPoint point) {
	CRect rectSize(point, point);
	CString str("Figure");
	str.Format(_T("Figure%i"), id_);
	CMyPaintFigure* ptr;
	ptr = new CMyPaintEllipse(id_, str, penWidth_, penColor_, penStyle_, brushColor_, brushStyle_, rectSize);
	figure_.push_back(ptr);
	id_++;
	return figure_.size() - 1;
}

int CMyPaintDoc::CreateRect(CPoint point) {
	CRect rectSize(point, point);
	CString str("Figure");
	str.Format(_T("Figure%i"), id_);
	CMyPaintFigure* ptr;
	ptr = new CMyPaintRect(id_, str, penWidth_, penColor_, penStyle_, brushColor_, brushStyle_, rectSize);
	figure_.push_back(ptr);
	id_++;
	return figure_.size() - 1;
}


int CMyPaintDoc::CreateTriangle(CPoint point) {
	CString str("Figure");
	str.Format(_T("Figure%i"), id_);
	CMyPaintFigure* ptr;
	ptr = new CMyPaintTriangle(id_, str, penWidth_, penColor_, penStyle_, brushColor_, brushStyle_, point, point, point);
	figure_.push_back(ptr);
	id_++;
	return figure_.size() - 1;
}

void CMyPaintDoc::setWidth(int width) {
	penWidth_ = width;
}

int CMyPaintDoc::getWidth() {
	return penWidth_;
}

void CMyPaintDoc::setPenStyle(int penStyle) {
	penStyle_ = penStyle;
}

int CMyPaintDoc::getPenStyle() {
	return penStyle_;
}

void CMyPaintDoc::setPenColor(COLORREF penColor) {
	penColor_ = penColor;
}

COLORREF CMyPaintDoc::getPenColor() {
	return penColor_;
}

void CMyPaintDoc::setBrushStyle(int brushStyle) {
	brushStyle_ = brushStyle;
}

int CMyPaintDoc::getBrushStyle() {
	return brushStyle_;
}

void CMyPaintDoc::setBrushColor(COLORREF brushColor) {
	brushColor_ = brushColor;
}

COLORREF CMyPaintDoc::getBrushColor() {
	return brushColor_;
}

int CMyPaintDoc::CreateLine(CPoint point) {
	CString str("Figure");
	str.Format(_T("Figure%i"), id_);
	CMyPaintFigure* ptr;
	ptr = new CMyPaintLine(id_, str, penWidth_, penColor_, penStyle_, point, point);
	figure_.push_back(ptr);
	id_++;
	return figure_.size() - 1;
}
// Команды CMyPaintDoc
