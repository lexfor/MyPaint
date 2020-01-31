#pragma once
#include<map>
#include<vector>
#include "CMyPaintPropertiesDialog.h"
class CMyPaintFigure
{
protected:
	unsigned int id_;
	CString name_;
	int penWidth_;
	COLORREF penColor_;
	int penStyle_;
	COLORREF brushColor_;
	int brushStyle_;
	unsigned int rotateCount_;
	double rotationCos_, rotationSin_;
	std::map<int, CPoint> connectionsCoordinates_;
public:
	CMyPaintFigure();
	CMyPaintFigure(CClientDC &,unsigned int, CString);
	CMyPaintFigure(CClientDC& ,unsigned int, CString, int, COLORREF, int, COLORREF, int, unsigned int);
	virtual CRect getCoordinates() = 0;
	virtual void draw(CClientDC&,HWND&) = 0;
	virtual bool ifThisFigure(CPoint) = 0;
	virtual void setCoordinates(CPoint,bool) = 0;
	virtual void move(CPoint*) = 0;
	virtual void normalize() = 0;
	virtual bool edit(CPoint,bool) = 0;
	virtual void rotate(CPoint*,bool) = 0;
	virtual void drawTempFigure(CClientDC&, HWND&) = 0;
	virtual CRect getTempCoordinates() = 0;
	virtual void  makeTempCoordinatesNull() = 0;
	virtual CPoint getCenterCoordinates() = 0;
	virtual CPoint findConnectionCoordinates(CPoint,int) = 0;
	virtual int findConnectionPlace(int) = 0;
	virtual std::vector<int> getConnectionID() = 0;
	virtual void deleteConnection(int) = 0;
	bool checkID(unsigned int);
	unsigned int getID();
	CString getName();
	double getCos();
	double getSin();
	CPoint getConnectCoordinate(int);
	virtual void properties(std::vector<int>,std::vector<CString>) = 0;
	std::map<int, CPoint> getConnectionCoordinates();
};