
// MyPaintDoc.h: интерфейс класса CMyPaintDoc 
//


#pragma once
#include <vector>
#include "CMyPaintEllipse.h"
#include "CMyPaintRect.h"
#include "CMyPaintTriangle.h"
#include "CMyPaintConnection.h"
#include "CMyPaintLine.h"

class CMyPaintDoc : public CDocument
{
protected: // создать только из сериализации
	CMyPaintDoc() noexcept;
	DECLARE_DYNCREATE(CMyPaintDoc)

	// Атрибуты
public:

	// Операции
public:

	// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// Реализация
public:
	virtual ~CMyPaintDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	void setWidth(int);
	int getWidth();
	void setPenStyle(int);
	int getPenStyle();
	void setPenColor(COLORREF);
	COLORREF getPenColor();
	void setBrushStyle(int);
	int getBrushStyle();
	void setBrushColor(COLORREF);
	COLORREF getBrushColor();
	int CreateEllipse(CPoint point);
	int CreateRect(CPoint point);
	int CreateTriangle(CPoint point);
	int CreateLine(CPoint point);
#endif

protected:

	// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	std::vector<CMyPaintFigure*>figure_;
	std::vector<CMyPaintConnection> connections_;
private:
	int id_;
	COLORREF penColor_;
	COLORREF brushColor_;
	COLORREF customColors[16];
	int penWidth_;
	int penStyle_;
	int brushStyle_;
};
