#include "pch.h"
#include "CMyPaintConnection.h"
#include "CMyPaintLinePropertiesDialog.h"

#define FIRSTCONNECTIONPLACE 0
#define SECONDCONNECTIONPLACE 1
#define FIRSTCONNECTIONCOORDINATE 0
#define SECONDCONNECTIONCOORDINATE 1

CMyPaintConnection::CMyPaintConnection() : id_(0), name_("Connection"), penWidth_(1), penColor_(RGB(0, 0, 0)), penStyle_(0)
{}
CMyPaintConnection::CMyPaintConnection(unsigned int id, CString name, int penWidth, COLORREF penColor, int penStyle, int connectionType, int connectionPlace, int firstFigureNum, CPoint firstCoordinates) : id_(id), name_(name), penWidth_(penWidth), penColor_(penColor), penStyle_(penStyle)
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
		connectionPlace_[FIRSTCONNECTIONPLACE] = connectionPlaceEnum::top;
		break;
	case 2:
		connectionPlace_[FIRSTCONNECTIONPLACE] = connectionPlaceEnum::right;
		break;
	case 3:
		connectionPlace_[FIRSTCONNECTIONPLACE] = connectionPlaceEnum::bottom;
		break;
	case 4:
		connectionPlace_[FIRSTCONNECTIONPLACE] = connectionPlaceEnum::left;
		break;
	default:
		break;
	}
	figureID_[0] = firstFigureNum;
	connectionCoordinates_[FIRSTCONNECTIONCOORDINATE] = firstCoordinates;
}
void CMyPaintConnection::setCoordinates(CPoint point, int secondFigure) {

}
int CMyPaintConnection::getFirstFigureID() {
	return figureID_[0];
}
void CMyPaintConnection::setSecondCoordinates(CPoint point) {
	connectionCoordinates_[SECONDCONNECTIONCOORDINATE] = point;
}
void CMyPaintConnection::setSecondPlace(int secondPlace) {
	switch (secondPlace) {
	case 1:
		connectionPlace_[SECONDCONNECTIONPLACE] = connectionPlaceEnum::top;
		break;
	case 2:
		connectionPlace_[SECONDCONNECTIONPLACE] = connectionPlaceEnum::right;
		break;
	case 3:
		connectionPlace_[SECONDCONNECTIONPLACE] = connectionPlaceEnum::bottom;
		break;
	case 4:
		connectionPlace_[SECONDCONNECTIONPLACE] = connectionPlaceEnum::left;
		break;
	default:
		break;
	}
}
void CMyPaintConnection::setSecondFigureID(int num) {
	figureID_[1] = num;
}
void CMyPaintConnection::setFirstCoordinates(CPoint firstCoordinates) {
	connectionCoordinates_[FIRSTCONNECTIONCOORDINATE] = firstCoordinates;
}
void CMyPaintConnection::setFirstPlace(int firstPlace) {
	switch (firstPlace) {
	case 1:
		connectionPlace_[FIRSTCONNECTIONPLACE] = connectionPlaceEnum::top;
		break;
	case 2:
		connectionPlace_[FIRSTCONNECTIONPLACE] = connectionPlaceEnum::right;
		break;
	case 3:
		connectionPlace_[FIRSTCONNECTIONPLACE] = connectionPlaceEnum::bottom;
		break;
	case 4:
		connectionPlace_[FIRSTCONNECTIONPLACE] = connectionPlaceEnum::left;
		break;
	default:
		break;
	}
}
CRect CMyPaintConnection::getInvalidRect() {
	CRect rect;
	if (connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x < connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x) {
		rect.left = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x - 20;
		rect.right = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x + 20;
	}
	else {
		rect.left = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x - 20;
		rect.right = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x + 20;
	}
	if (connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y < connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y) {
		rect.top = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y - 20;
		rect.bottom = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y + 20;
	}
	else {
		rect.top = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y - 20;
		rect.bottom = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y + 20;
	}
	return rect;
}
unsigned int CMyPaintConnection::getID() {
	return id_;
}
int CMyPaintConnection::getSecondFigureID() {
	return figureID_[1];
}
void CMyPaintConnection::moveCoordinates(CPoint* movePoint, int figureNum) {
	CPoint diff;
	diff.x = movePoint[1].x - movePoint[0].x;
	diff.y = movePoint[1].y - movePoint[0].y;
	connectionCoordinates_[figureNum].x += diff.x;
	connectionCoordinates_[figureNum].y += diff.y;
}
void CMyPaintConnection::rotateCoordinates(CPoint center, double Cos, double Sin, int figureNum) {
	CPoint TempPoint = connectionCoordinates_[figureNum];
	connectionCoordinates_[figureNum].x = (TempPoint.x - center.x) * Cos - (TempPoint.y - center.y) * Sin + center.x;
	connectionCoordinates_[figureNum].y = (TempPoint.y - center.y) * Cos - (TempPoint.x - center.x) * Sin + center.y;
}
bool CMyPaintConnection::ifThisConnection(CPoint point) {
	CRect rect;
	rect = getInvalidRect();
	if (point.x > rect.left&& point.y > rect.top&& point.x < rect.right && point.y < rect.bottom) {
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


int CMyPaintConnection::getWidth() {
	return penWidth_;
}

int CMyPaintConnection::getStyle() {
	return penStyle_;
}

COLORREF CMyPaintConnection::getColor() {
	return penColor_;
}

int CMyPaintConnection::getConnectionType() {
	switch (connectionType_)
	{
	case CMyPaintConnection::connectionTypeEnum::line:
		return 1;
		break;
	case CMyPaintConnection::connectionTypeEnum::rightArrow:
		return 2;
		break;
	case CMyPaintConnection::connectionTypeEnum::leftArrow:
		return 3;
		break;
	case CMyPaintConnection::connectionTypeEnum::biDirectional:
		return 4;
		break;
	default:
		break;
	}
}


int CMyPaintConnection::getFirstConnectionPlace() {
	switch (connectionPlace_[0])
	{
	case CMyPaintConnection::connectionPlaceEnum::top:
		return 1;
		break;
	case CMyPaintConnection::connectionPlaceEnum::right:
		return 2;
		break;
	case CMyPaintConnection::connectionPlaceEnum::bottom:
		return 3;
		break;
	case CMyPaintConnection::connectionPlaceEnum::left:
		return 4;
		break;
	default:
		break;
	}
}


int CMyPaintConnection::getSecondConnectionPlace() {
	switch (connectionPlace_[1])
	{
	case CMyPaintConnection::connectionPlaceEnum::top:
		return 1;
		break;
	case CMyPaintConnection::connectionPlaceEnum::right:
		return 2;
		break;
	case CMyPaintConnection::connectionPlaceEnum::bottom:
		return 3;
		break;
	case CMyPaintConnection::connectionPlaceEnum::left:
		return 4;
		break;
	default:
		break;
	}
}


CPoint CMyPaintConnection::getFirstCoordinate() {
	return connectionCoordinates_[FIRSTCONNECTIONCOORDINATE];
}


CPoint CMyPaintConnection::getSecondCoordinate() {
	return connectionCoordinates_[SECONDCONNECTIONCOORDINATE];
}

void CMyPaintConnection::draw(HDC hdc) {
	CPoint arrowPoint[4], diffrence, linePoint, TempPoint;
	double arrowSin, arrowCos, radius;
	bool isChange = false;
	CPen Pen(penStyle_, penWidth_, penColor_);
	CPen* oldPen = (CPen*)SelectObject(hdc, &Pen);
	switch (connectionType_) {
	case connectionTypeEnum::line:
		Polyline(hdc, connectionCoordinates_, 2);
		break;
	case connectionTypeEnum::rightArrow:
		if (connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x < connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x) {
			TempPoint = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE];
			connectionCoordinates_[FIRSTCONNECTIONCOORDINATE] = connectionCoordinates_[SECONDCONNECTIONCOORDINATE];
			connectionCoordinates_[SECONDCONNECTIONCOORDINATE] = TempPoint;
			isChange = true;
		}
		diffrence.x = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x;
		diffrence.y = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y;
		radius = sqrt(pow(diffrence.x, 2) + pow(diffrence.y, 2));
		arrowCos = diffrence.x / radius;
		arrowSin = diffrence.y / radius;
		linePoint.x = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x + radius;
		linePoint.y = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y;
		arrowPoint[0].x = linePoint.x - 20;
		arrowPoint[0].y = linePoint.y + 20;
		arrowPoint[1].x = linePoint.x - 20;
		arrowPoint[1].y = linePoint.y - 20;
		arrowPoint[2].x = (arrowPoint[0].x - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x) * arrowCos - (arrowPoint[0].y - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y) * arrowSin + connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x;
		arrowPoint[2].y = (arrowPoint[0].y - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y) * arrowCos + (arrowPoint[0].x - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x) * arrowSin + connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y;
		arrowPoint[3].x = (arrowPoint[1].x - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x) * arrowCos - (arrowPoint[1].y - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y) * arrowSin + connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x;
		arrowPoint[3].y = (arrowPoint[1].y - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y) * arrowCos + (arrowPoint[1].x - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x) * arrowSin + connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y;
		Polyline(hdc, connectionCoordinates_, 2);
		arrowPoint[0] = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE];
		arrowPoint[1] = arrowPoint[2];
		Polyline(hdc, arrowPoint, 2);
		arrowPoint[1] = arrowPoint[3];
		Polyline(hdc, arrowPoint, 2);
		if (isChange) {
			TempPoint = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE];
			connectionCoordinates_[FIRSTCONNECTIONCOORDINATE] = connectionCoordinates_[SECONDCONNECTIONCOORDINATE];
			connectionCoordinates_[SECONDCONNECTIONCOORDINATE] = TempPoint;
			isChange = false;
		}
		break;
	case connectionTypeEnum::leftArrow:
		if (connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x < connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x) {
			TempPoint = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE];
			connectionCoordinates_[FIRSTCONNECTIONCOORDINATE] = connectionCoordinates_[SECONDCONNECTIONCOORDINATE];
			connectionCoordinates_[SECONDCONNECTIONCOORDINATE] = TempPoint;
			isChange = true;
		}
		diffrence.x = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x;
		diffrence.y = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y;
		radius = sqrt(pow(diffrence.x, 2) + pow(diffrence.y, 2));
		arrowCos = diffrence.x / radius;
		arrowSin = diffrence.y / radius;
		linePoint.x = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x + radius;
		linePoint.y = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y;
		arrowPoint[0].x = linePoint.x - 20;
		arrowPoint[0].y = linePoint.y + 20;
		arrowPoint[1].x = linePoint.x - 20;
		arrowPoint[1].y = linePoint.y - 20;
		arrowPoint[2].x = (arrowPoint[0].x - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x) * arrowCos - (arrowPoint[0].y - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y) * arrowSin + connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x;
		arrowPoint[2].y = (arrowPoint[0].y - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y) * arrowCos + (arrowPoint[0].x - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x) * arrowSin + connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y;
		arrowPoint[3].x = (arrowPoint[1].x - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x) * arrowCos - (arrowPoint[1].y - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y) * arrowSin + connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x;
		arrowPoint[3].y = (arrowPoint[1].y - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y) * arrowCos + (arrowPoint[1].x - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x) * arrowSin + connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y;
		Polyline(hdc, connectionCoordinates_, 2);
		arrowPoint[0] = connectionCoordinates_[SECONDCONNECTIONCOORDINATE];
		arrowPoint[1] = arrowPoint[2];
		Polyline(hdc, arrowPoint, 2);
		arrowPoint[1] = arrowPoint[3];
		Polyline(hdc, arrowPoint, 2);
		if (isChange) {
			TempPoint = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE];
			connectionCoordinates_[FIRSTCONNECTIONCOORDINATE] = connectionCoordinates_[SECONDCONNECTIONCOORDINATE];
			connectionCoordinates_[SECONDCONNECTIONCOORDINATE] = TempPoint;
			isChange = false;
		}
		break;
	case connectionTypeEnum::biDirectional:
		diffrence.x = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x;
		diffrence.y = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y;
		radius = sqrt(pow(diffrence.x, 2) + pow(diffrence.y, 2));
		arrowCos = diffrence.x / radius;
		arrowSin = diffrence.y / radius;
		linePoint.x = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x + radius;
		linePoint.y = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y;
		arrowPoint[0].x = linePoint.x - 20;
		arrowPoint[0].y = linePoint.y + 20;
		arrowPoint[1].x = linePoint.x - 20;
		arrowPoint[1].y = linePoint.y - 20;
		arrowPoint[2].x = (arrowPoint[0].x - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x) * arrowCos - (arrowPoint[0].y - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y) * arrowSin + connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x;
		arrowPoint[2].y = (arrowPoint[0].y - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y) * arrowCos + (arrowPoint[0].x - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x) * arrowSin + connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y;
		arrowPoint[3].x = (arrowPoint[1].x - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x) * arrowCos - (arrowPoint[1].y - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y) * arrowSin + connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x;
		arrowPoint[3].y = (arrowPoint[1].y - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y) * arrowCos + (arrowPoint[1].x - connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x) * arrowSin + connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y;
		Polyline(hdc, connectionCoordinates_, 2);
		arrowPoint[0] = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE];
		arrowPoint[1] = arrowPoint[2];
		Polyline(hdc, arrowPoint, 2);
		arrowPoint[1] = arrowPoint[3];
		Polyline(hdc, arrowPoint, 2);
		diffrence.x = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].x - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x;
		diffrence.y = connectionCoordinates_[SECONDCONNECTIONCOORDINATE].y - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y;
		radius = sqrt(pow(diffrence.x, 2) + pow(diffrence.y, 2));
		arrowCos = diffrence.x / radius;
		arrowSin = diffrence.y / radius;
		linePoint.x = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x + radius;
		linePoint.y = connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y;
		arrowPoint[0].x = linePoint.x - 20;
		arrowPoint[0].y = linePoint.y + 20;
		arrowPoint[1].x = linePoint.x - 20;
		arrowPoint[1].y = linePoint.y - 20;
		arrowPoint[2].x = (arrowPoint[0].x - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x) * arrowCos - (arrowPoint[0].y - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y) * arrowSin + connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x;
		arrowPoint[2].y = (arrowPoint[0].y - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y) * arrowCos + (arrowPoint[0].x - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x) * arrowSin + connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y;
		arrowPoint[3].x = (arrowPoint[1].x - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x) * arrowCos - (arrowPoint[1].y - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y) * arrowSin + connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x;
		arrowPoint[3].y = (arrowPoint[1].y - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y) * arrowCos + (arrowPoint[1].x - connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].x) * arrowSin + connectionCoordinates_[FIRSTCONNECTIONCOORDINATE].y;
		Polyline(hdc, connectionCoordinates_, 2);
		arrowPoint[0] = connectionCoordinates_[SECONDCONNECTIONCOORDINATE];
		arrowPoint[1] = arrowPoint[2];
		Polyline(hdc, arrowPoint, 2);
		arrowPoint[1] = arrowPoint[3];
		Polyline(hdc, arrowPoint, 2);
		break;
	}
}
void CMyPaintConnection::scrollConnection(CPoint point) {
	for (auto i = 0; i < 2; i++) {
		connectionCoordinates_[i].x += point.x;
		connectionCoordinates_[i].y += point.y;
	}	
}