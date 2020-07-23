#pragma once
#include "CMyPaintFigure.h"
class CMyPaintEllipse :
	public CMyPaintFigure
{
private:
	CPoint ellipseCoordinates_[4];
	CPoint ellipseCenter_;
	CPoint tempEllipse_[4];
public:
	CMyPaintEllipse();
	CMyPaintEllipse(unsigned int, CString, int, COLORREF, int, COLORREF, int, CRect);
	void setCoordinates(CPoint, bool)override;
	void findCenterCoordinates();
	bool ifThisFigure(CPoint point)override;
	void move(CPoint*)override;
	void rotate(CPoint*, bool)override;
	void normalize()override;
	void changeOtherCoordinates()override;
	bool edit(CPoint, bool)override;
	void makeTempCoordinatesNull()override;
	CPoint getCenterCoordinates()override;
	CPoint findConnectionCoordinates(CPoint, int)override;
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
	void draw(HDC)override;
	void tempDraw(HDC)override;
	void select(HDC)override;
	void scrollFigure(CPoint)override;
	std::vector<CPoint> getLeftCoordinate()override;
	std::vector<CPoint> getBottomCoordinate()override;
	std::vector<CPoint> getRightCoordinate()override;
	std::vector<CPoint> getTopCoordinate()override;
	std::vector<LONG> getMaxMinX()override;
	std::vector<LONG> getMaxMinY()override;
};

