#pragma once
#include <vector>
class CMyPaintConnection
{
private:
	enum class connectionTypeEnum { line, rightArrow, leftArrow, biDirectional }connectionType_;
	enum class connectionPlaceEnum { top, right, bottom, left }connectionPlace_[2];
	int figureID_[2];
	CPoint connectionCoordinates_[2];
	unsigned int id_;
	CString name_;
	int penWidth_;
	COLORREF penColor_;
	int penStyle_;
public:
	CMyPaintConnection();
	CMyPaintConnection(unsigned int, CString, int, COLORREF, int, int, int, int, CPoint);
	void setCoordinates(CPoint, int);
	int getFirstFigureID();
	void setSecondCoordinates(CPoint);
	void setSecondPlace(int);
	void setSecondFigureID(int);
	void setFirstCoordinates(CPoint);
	void setFirstPlace(int);
	CRect getInvalidRect();
	unsigned int getID();
	int getSecondFigureID();
	void moveCoordinates(CPoint*, int);
	void rotateCoordinates(CPoint, double, double, int);
	bool ifThisConnection(CPoint);
	CString getName();
	void properties(std::vector<int>, std::vector<CString>);
	int getWidth();
	int getStyle();
	COLORREF getColor();
	int getConnectionType();
	int getFirstConnectionPlace();
	int getSecondConnectionPlace();
	CPoint getFirstCoordinate();
	CPoint getSecondCoordinate();
	void draw(HDC);
};

