#pragma once
#include "CMyPaintFigure.h"
class CMyPaintLine :
	public CMyPaintFigure
{
private:
	CPoint lineCoordinates_[2];
	CPoint lineCenter_;
	CPoint tempLineCoordinates_[2];
public:
	CMyPaintLine();
	CMyPaintLine(unsigned int, CString,int, COLORREF,int ,CPoint*);
	void draw(CClientDC&, HWND&)override;
	void setCoordinates(CPoint, bool)override;
	void findCenterCoordinates();
	CRect getCoordinates()override;
	bool ifThisFigure(CPoint point)override;
	void move(CPoint*)override;
	void rotate(CPoint*, bool)override;
	void normalize()override;
	void drawTempFigure(CClientDC&, HWND&)override;
	CRect getTempCoordinates()override;
	void changeOtherCoordinates()override;
	bool edit(CPoint, bool)override;
	void makeTempCoordinatesNull()override;
	CPoint getCenterCoordinates()override;
	void properties(std::vector<int>, std::vector<CString>)override;
	CPoint getFirstCoordinate()override;
	CPoint getSecondCoordinate()override;
	CPoint getThirdCoordinate()override;
	int getFigureType()override;
	void setFirstCoordinate(CPoint)override;
	void setSecondCoordinate(CPoint)override;
	void setThirdCoordinate(CPoint)override;
};

