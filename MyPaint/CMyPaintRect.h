#pragma once
#include "CMyPaintFigure.h"
class CMyPaintRect :
	public CMyPaintFigure
{
private:
	CPoint rectCoordinates_[4]; 
	CPoint rectCenter_;
	CPoint tempRect_[4];
public:
	CMyPaintRect();
	CMyPaintRect(CClientDC& , unsigned int, CString, int, COLORREF, int, COLORREF, int, unsigned int, CRect);
	void draw(CClientDC&,HWND&)override;
	void setCoordinates(CPoint,bool)override;
	void findCenterCoordinates();
	CRect getCoordinates()override;
	bool ifThisFigure(CPoint)override;
	void move(CPoint *)override;
	void rotate(CPoint*,bool)override;
	void normalize()override;
	void drawTempFigure(CClientDC&, HWND&)override;
	CRect getTempCoordinates()override;
	void ChangeOtherCoordinates();
	bool edit(CPoint,bool)override;
	void makeTempCoordinatesNull()override;
	CPoint getCenterCoordinates()override;
	CPoint findConnectionCoordinates(CPoint,int)override;
	int findConnectionPlace(int)override;
	std::vector<int> getConnectionID()override;
	void deleteConnection(int)override;
	void properties(std::vector<int>, std::vector<CString>)override;
};

