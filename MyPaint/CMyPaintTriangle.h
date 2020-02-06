#pragma once
#include "CMyPaintFigure.h"
class CMyPaintTriangle :
	public CMyPaintFigure
{
private:
		CPoint triangleCoordinates_[3];
		CPoint triangleCenter_;
		CPoint tempTriangle_[3];
		enum class clickCountEnum_{firstClick,secondClick,thirdClick}clickCount_;
public:
	CMyPaintTriangle();
	CMyPaintTriangle( unsigned int, CString, int, COLORREF, int, COLORREF, int,CPoint,CPoint,CPoint);
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
	void changeOtherCoordinates()override;
};

