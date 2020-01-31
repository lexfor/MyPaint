#include "pch.h"
#include "CMyPaintConnection.h"
#include "CMyPaintLinePropertiesDialog.h"
CMyPaintConnection::CMyPaintConnection() : id_(0), name_("Connection"), penWidth_(1), penColor_(RGB(0, 0, 0)), penStyle_(0)
{}
CMyPaintConnection::CMyPaintConnection(unsigned int id,CString name, int penWidth, COLORREF penColor,int penStyle,int connectionType,int connectionPlace,int firstFigureNum,CPoint firstCoordinates) : id_(id), name_(name), penWidth_(penWidth), penColor_(penColor), penStyle_(penStyle)
{
	switch (connectionType)
	{
	case 1:
		connectionType_ = connectionTypeEnum::line;
		break;
	case 2:
		connectionType_ = connectionTypeEnum::rightArrow;
		break;
	case 3:
		connectionType_ = connectionTypeEnum::leftArrow;
		break;
	case 4:
		connectionType_ = connectionTypeEnum::biDirectional;
		break;
	default:
		break;
	}
	switch (connectionPlace)
	{
	case 1:
		connectionPlace_[0] = connectionPlaceEnum::top;
		break;
	case 2:
		connectionPlace_[0] = connectionPlaceEnum::right;
		break;
	case 3:
		connectionPlace_[0] = connectionPlaceEnum::bottom;
		break;
	case 4:
		connectionPlace_[0] = connectionPlaceEnum::left;
		break;
	default:
		break;
	}
	figureID_[0] = firstFigureNum;
	connectionCoordinates_[0] = firstCoordinates;
}
void CMyPaintConnection::draw(CClientDC& dc) {
	CPoint arrowPoint[4],diffrence,linePoint,TempPoint;
	double arrowSin, arrowCos, radius;
	bool isChange = false;
	CPen Pen(penStyle_, penWidth_, penColor_);
	CPen* oldPen = (CPen*)dc.SelectObject(&Pen);
	switch (connectionType_) {
	case connectionTypeEnum::line:
		dc.Polyline(connectionCoordinates_, 2);
		break;
	case connectionTypeEnum::rightArrow:
		if (connectionCoordinates_[0].x < connectionCoordinates_[1].x) {
			TempPoint = connectionCoordinates_[0];
			connectionCoordinates_[0] = connectionCoordinates_[1];
			connectionCoordinates_[1] = TempPoint;
			isChange = true;
		}
		diffrence.x = connectionCoordinates_[0].x - connectionCoordinates_[1].x;
		diffrence.y = connectionCoordinates_[0].y - connectionCoordinates_[1].y;
		radius = sqrt(pow(diffrence.x, 2) + pow(diffrence.y, 2));
		arrowCos = diffrence.x / radius;
		arrowSin = diffrence.y / radius;
		linePoint.x = connectionCoordinates_[1].x + radius;
		linePoint.y = connectionCoordinates_[1].y;
		arrowPoint[0].x = linePoint.x - 20;
		arrowPoint[0].y = linePoint.y + 20;
		arrowPoint[1].x = linePoint.x - 20;
		arrowPoint[1].y = linePoint.y - 20;
		arrowPoint[2].x = (arrowPoint[0].x - connectionCoordinates_[1].x) * arrowCos - (arrowPoint[0].y - connectionCoordinates_[1].y) * arrowSin + connectionCoordinates_[1].x;
		arrowPoint[2].y = (arrowPoint[0].y - connectionCoordinates_[1].y) * arrowCos + (arrowPoint[0].x - connectionCoordinates_[1].x) * arrowSin + connectionCoordinates_[1].y;
		arrowPoint[3].x = (arrowPoint[1].x - connectionCoordinates_[1].x) * arrowCos - (arrowPoint[1].y - connectionCoordinates_[1].y) * arrowSin + connectionCoordinates_[1].x;
		arrowPoint[3].y = (arrowPoint[1].y - connectionCoordinates_[1].y) * arrowCos + (arrowPoint[1].x - connectionCoordinates_[1].x) * arrowSin + connectionCoordinates_[1].y;
		dc.MoveTo(connectionCoordinates_[0]);
		dc.LineTo(connectionCoordinates_[1]);
		dc.MoveTo(connectionCoordinates_[0]);
		dc.LineTo(arrowPoint[2]);
		dc.MoveTo(connectionCoordinates_[0]);
		dc.LineTo(arrowPoint[3]);
		if (isChange) {
			TempPoint = connectionCoordinates_[0];
			connectionCoordinates_[0] = connectionCoordinates_[1];
			connectionCoordinates_[1] = TempPoint;
			isChange = false;
		}
		break;
	case connectionTypeEnum::leftArrow:
		if (connectionCoordinates_[0].x < connectionCoordinates_[1].x) {
			TempPoint = connectionCoordinates_[0];
			connectionCoordinates_[0] = connectionCoordinates_[1];
			connectionCoordinates_[1] = TempPoint;
			isChange = true;
		}
		diffrence.x = connectionCoordinates_[1].x - connectionCoordinates_[0].x;
		diffrence.y = connectionCoordinates_[1].y - connectionCoordinates_[0].y;
		radius = sqrt(pow(diffrence.x, 2) + pow(diffrence.y, 2));
		arrowCos = diffrence.x / radius;
		arrowSin = diffrence.y / radius;
		linePoint.x = connectionCoordinates_[0].x + radius;
		linePoint.y = connectionCoordinates_[0].y;
		arrowPoint[0].x = linePoint.x - 20;
		arrowPoint[0].y = linePoint.y + 20;
		arrowPoint[1].x = linePoint.x - 20;
		arrowPoint[1].y = linePoint.y - 20;
		arrowPoint[2].x = (arrowPoint[0].x - connectionCoordinates_[0].x) * arrowCos - (arrowPoint[0].y - connectionCoordinates_[0].y) * arrowSin + connectionCoordinates_[0].x;
		arrowPoint[2].y = (arrowPoint[0].y - connectionCoordinates_[0].y) * arrowCos + (arrowPoint[0].x - connectionCoordinates_[0].x) * arrowSin + connectionCoordinates_[0].y;
		arrowPoint[3].x = (arrowPoint[1].x - connectionCoordinates_[0].x) * arrowCos - (arrowPoint[1].y - connectionCoordinates_[0].y) * arrowSin + connectionCoordinates_[0].x;
		arrowPoint[3].y = (arrowPoint[1].y - connectionCoordinates_[0].y) * arrowCos + (arrowPoint[1].x - connectionCoordinates_[0].x) * arrowSin + connectionCoordinates_[0].y;
		dc.MoveTo(connectionCoordinates_[0]);
		dc.LineTo(connectionCoordinates_[1]);
		dc.LineTo(arrowPoint[2]);
		dc.MoveTo(connectionCoordinates_[1]);
		dc.LineTo(arrowPoint[3]);
		if (isChange) {
			TempPoint = connectionCoordinates_[0];
			connectionCoordinates_[0] = connectionCoordinates_[1];
			connectionCoordinates_[1] = TempPoint;
			isChange = false;
		}
		break;
	case connectionTypeEnum::biDirectional:
		diffrence.x = connectionCoordinates_[0].x - connectionCoordinates_[1].x;
		diffrence.y = connectionCoordinates_[0].y - connectionCoordinates_[1].y;
		radius = sqrt(pow(diffrence.x, 2) + pow(diffrence.y, 2));
		arrowCos = diffrence.x / radius;
		arrowSin = diffrence.y / radius;
		linePoint.x = connectionCoordinates_[1].x + radius;
		linePoint.y = connectionCoordinates_[1].y;
		arrowPoint[0].x = linePoint.x - 20;
		arrowPoint[0].y = linePoint.y + 20;
		arrowPoint[1].x = linePoint.x - 20;
		arrowPoint[1].y = linePoint.y - 20;
		arrowPoint[2].x = (arrowPoint[0].x - connectionCoordinates_[1].x) * arrowCos - (arrowPoint[0].y - connectionCoordinates_[1].y) * arrowSin + connectionCoordinates_[1].x;
		arrowPoint[2].y = (arrowPoint[0].y - connectionCoordinates_[1].y) * arrowCos + (arrowPoint[0].x - connectionCoordinates_[1].x) * arrowSin + connectionCoordinates_[1].y;
		arrowPoint[3].x = (arrowPoint[1].x - connectionCoordinates_[1].x) * arrowCos - (arrowPoint[1].y - connectionCoordinates_[1].y) * arrowSin + connectionCoordinates_[1].x;
		arrowPoint[3].y = (arrowPoint[1].y - connectionCoordinates_[1].y) * arrowCos + (arrowPoint[1].x - connectionCoordinates_[1].x) * arrowSin + connectionCoordinates_[1].y;
		dc.MoveTo(connectionCoordinates_[0]);
		dc.LineTo(connectionCoordinates_[1]);
		dc.MoveTo(connectionCoordinates_[0]);
		dc.LineTo(arrowPoint[2]);
		dc.MoveTo(connectionCoordinates_[0]);
		dc.LineTo(arrowPoint[3]);
		diffrence.x = connectionCoordinates_[1].x - connectionCoordinates_[0].x;
		diffrence.y = connectionCoordinates_[1].y - connectionCoordinates_[0].y;
		radius = sqrt(pow(diffrence.x, 2) + pow(diffrence.y, 2));
		arrowCos = diffrence.x / radius;
		arrowSin = diffrence.y / radius;
		linePoint.x = connectionCoordinates_[0].x + radius;
		linePoint.y = connectionCoordinates_[0].y;
		arrowPoint[0].x = linePoint.x - 20;
		arrowPoint[0].y = linePoint.y + 20;
		arrowPoint[1].x = linePoint.x - 20;
		arrowPoint[1].y = linePoint.y - 20;
		arrowPoint[2].x = (arrowPoint[0].x - connectionCoordinates_[0].x) * arrowCos - (arrowPoint[0].y - connectionCoordinates_[0].y) * arrowSin + connectionCoordinates_[0].x;
		arrowPoint[2].y = (arrowPoint[0].y - connectionCoordinates_[0].y) * arrowCos + (arrowPoint[0].x - connectionCoordinates_[0].x) * arrowSin + connectionCoordinates_[0].y;
		arrowPoint[3].x = (arrowPoint[1].x - connectionCoordinates_[0].x) * arrowCos - (arrowPoint[1].y - connectionCoordinates_[0].y) * arrowSin + connectionCoordinates_[0].x;
		arrowPoint[3].y = (arrowPoint[1].y - connectionCoordinates_[0].y) * arrowCos + (arrowPoint[1].x - connectionCoordinates_[0].x) * arrowSin + connectionCoordinates_[0].y;
		dc.MoveTo(connectionCoordinates_[0]);
		dc.LineTo(connectionCoordinates_[1]);
		dc.LineTo(arrowPoint[2]);
		dc.MoveTo(connectionCoordinates_[1]);
		dc.LineTo(arrowPoint[3]); 
		break;
	}
}
void CMyPaintConnection::setCoordinates(CPoint point, int secondFigure) {

}
int CMyPaintConnection::getFirstFigureID() {
	return figureID_[0];
}
void CMyPaintConnection::setSecondCoordinates(CPoint point) {
	connectionCoordinates_[1] = point;
}
void CMyPaintConnection::setSecondPlace(int secondPlace) {
	switch (secondPlace) {
	case 1:
		connectionPlace_[1] = connectionPlaceEnum::top;
		break;
	case 2:
		connectionPlace_[1] = connectionPlaceEnum::right;
		break;
	case 3:
		connectionPlace_[1] = connectionPlaceEnum::bottom;
		break;
	case 4:
		connectionPlace_[1] = connectionPlaceEnum::left;
		break;
	default:
		break;
	}
}
void CMyPaintConnection::setSecondFigureID(int num) {
	figureID_[1] = num;
}
void CMyPaintConnection::setFirstCoordinates(CPoint firstCoordinates) {
	connectionCoordinates_[0] = firstCoordinates;
}
void CMyPaintConnection::setFirstPlace(int firstPlace) {
	switch (firstPlace) {
	case 1:
		connectionPlace_[1] = connectionPlaceEnum::top;
		break;
	case 2:
		connectionPlace_[1] = connectionPlaceEnum::right;
		break;
	case 3:
		connectionPlace_[1] = connectionPlaceEnum::bottom;
		break;
	case 4:
		connectionPlace_[1] = connectionPlaceEnum::left;
		break;
	default:
		break;
	}
}
CRect CMyPaintConnection::getInvalidRect() {
	CRect rect;
	if (connectionCoordinates_[0].x < connectionCoordinates_[1].x) {
		rect.left = connectionCoordinates_[0].x - 20;
		rect.right = connectionCoordinates_[1].x + 20;
	}
	else {
		rect.left = connectionCoordinates_[1].x - 20;
		rect.right = connectionCoordinates_[0].x + 20;
	}
	if (connectionCoordinates_[0].y < connectionCoordinates_[1].y) {
		rect.top = connectionCoordinates_[0].y - 20;
		rect.bottom = connectionCoordinates_[1].y + 20;
	}
	else {
		rect.top = connectionCoordinates_[1].y - 20;
		rect.bottom = connectionCoordinates_[0].y + 20;
	}
	return rect;
}
unsigned int CMyPaintConnection ::getID() {
	return id_;
}
int CMyPaintConnection::getSecondFigureID() {
	return figureID_[1];
}
void CMyPaintConnection:: moveCoordinates(CPoint* movePoint,int figureNum) {
	CPoint diff;
	diff.x = movePoint[1].x - movePoint[0].x;
	diff.y = movePoint[1].y - movePoint[0].y;
	connectionCoordinates_[figureNum].x += diff.x;
	connectionCoordinates_[figureNum].y += diff.y;
}
void CMyPaintConnection::rotateCoordinates(CPoint center,double Cos, double Sin,int figureNum) {
	CPoint TempPoint = connectionCoordinates_[figureNum];
	connectionCoordinates_[figureNum].x = (TempPoint.x - center.x)* Cos - (TempPoint.y - center.y)* Sin + center.x;
	connectionCoordinates_[figureNum].y = (TempPoint.y - center.y)* Cos - (TempPoint.x - center.x)* Sin + center.y;
}
bool CMyPaintConnection::ifThisConnection(CPoint point) {
	CRect rect;
	rect = getInvalidRect();
	if (point.x > rect.left && point.y > rect.top && point.x < rect.right && point.y < rect.bottom) {
		return true;
	}
	return false;
}

CString CMyPaintConnection::getName() {
	return name_;
}

void CMyPaintConnection::properties(std::vector<int> ids, std::vector<CString> names) {
	CMyPaintLinePropertiesDialog dlg;
	dlg.setID(id_);
	dlg.setName(name_);
	dlg.setWidth(penWidth_);
	dlg.setColor(penColor_);
	dlg.setStyle(penStyle_);
	dlg.setIDs(ids);
	dlg.setNames(names);
	dlg.DoModal();
	id_ = dlg.getID();
	name_ = dlg.getName();
	penWidth_ = dlg.getWidth();
	penStyle_ = dlg.getStyle();
	penColor_ = dlg.getColor();
}