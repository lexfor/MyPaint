#pragma once
#include "CMyPaintFigure.h"
class CMyPaintTriangle :
	public CMyPaintFigure
{
private:
	CPoint triangleCoordinates_[3];
	CPoint triangleCenter_;
	CPoint tempTriangle_[3];
	enum class clickCountEnum_ { firstClick, secondClick, thirdClick }clickCount_;
public:
	CMyPaintTriangle();
	CMyPaintTriangle(unsigned int, CString, int, COLORREF, int, COLORREF, int, CPoint, CPoint, CPoint);
	void setCoordinates(CPoint, bool)override;
	void findCenterCoordinates();
	bool ifThisFigure(CPoint)override;
	void move(CPoint*)override;
	void rotate(CPoint*, bool)override;
	void normalize()override;
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
	void changeOtherCoordinates()override;
	void draw(HDC)override;
	void tempDraw(HDC)override;
	void select(HDC)override;
	void scrollFigure(CPoint);
	std::vector<CPoint> getLeftCoordinate()override;
	std::vector<CPoint> getBottomCoordinate()override;
	std::vector<CPoint> getRightCoordinate()override;
	std::vector<CPoint> getTopCoordinate()override;
	std::vector<LONG> getMaxMinX();
	std::vector<LONG> getMaxMinY();
};

