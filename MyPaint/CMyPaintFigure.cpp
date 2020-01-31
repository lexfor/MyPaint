#include "pch.h"
#include "CMyPaintFigure.h"
CMyPaintFigure::CMyPaintFigure() : id_(0), name_("Figure"), penWidth_(1), penColor_(RGB(0, 0, 0)), penStyle_(1), brushColor_(RGB(0, 0, 0)), brushStyle_(1), rotateCount_(0)
{
	rotationCos_ = 0;
	rotationSin_ = 0;
}
CMyPaintFigure::CMyPaintFigure(CClientDC&  dc,unsigned int id, CString name) : id_(id), name_(name), penWidth_(1), penColor_(RGB(0, 0, 0)), penStyle_(1), brushColor_(RGB(0, 0, 0)), brushStyle_(1), rotateCount_(0)
{
	rotationCos_ = 0;
	rotationSin_ = 0;
}
CMyPaintFigure::CMyPaintFigure(CClientDC& dc, unsigned int id, CString name, int penWidth, COLORREF penColor, int penStyle, COLORREF brushColor, int brushStyle, unsigned int rotateCount) : id_(id), name_(name), penWidth_(penWidth), penColor_(penColor), penStyle_(penStyle), brushColor_(brushColor), brushStyle_(brushStyle), rotateCount_(rotateCount)
{
	rotationCos_ = 0;
	rotationSin_ = 0;
}
bool CMyPaintFigure::checkID(unsigned int id) {
	if (id_ == id) {
		return true;
	}
	else {
		return false;
	}
}
unsigned int CMyPaintFigure::getID() {
	return id_;
}
double CMyPaintFigure::getCos() {
	return rotationCos_;
}
double CMyPaintFigure::getSin() {
	return rotationSin_;
}
CPoint CMyPaintFigure::getConnectCoordinate(int key) {
	auto it = connectionsCoordinates_.find(key);
	return it->second;
}

CString CMyPaintFigure::getName() {
	return name_;
}


std::map<int, CPoint> CMyPaintFigure::getConnectionCoordinates() {
	return connectionsCoordinates_;
}
