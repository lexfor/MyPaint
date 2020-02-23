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
	CMyPaintRect( unsigned int, CString, int, COLORREF, int, COLORREF, int, CRect);
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
	void changeOtherCoordinates()override;
	bool edit(CPoint,bool)override;
	void makeTempCoordinatesNull()override;
	CPoint getCenterCoordinates()override;
	CPoint findConnectionCoordinates(CPoint,int)override;
	int findConnectionPlace(int)override;
	std::vector<int> getConnectionID()override;
	void deleteConnection(int)override;
	void properties(std::vector<int>, std::vector<CString>)override;
	CPoint getFirstCoordinate()override;
	CPoint getSecondCoordinate()override;
	CPoint getThirdCoordinate()override;
	int getFigureType()override;
	void setFirstCoordinate(CPoint)override;
	void setSecondCoordinate(CPoint)override;
	void setThirdCoordinate(CPoint)override;
	void drawInMemory(HDC)override;
};

