#include "pch.h"
#include "CMyPaintEllipse.h"

#define FIRSTELLIPSECOORDINATE 0
#define SECONDELLIPSECOORDINATE 1
#define THIRDELLIPSECOORDINATE 2
#define FOURTHELLIPSECOORDINATE 3

CMyPaintEllipse::CMyPaintEllipse() :CMyPaintFigure()
{}
CMyPaintEllipse::CMyPaintEllipse(unsigned int id, CString name, int penWidth, COLORREF penColor, int penStyle, COLORREF brushColor, int brushStyle, CRect ellipseCoordinates) : CMyPaintFigure(id, name, penWidth, penColor, penStyle, brushColor, brushStyle)
{
	ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x = ellipseCoordinates.left;
	ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y = ellipseCoordinates.top;
	ellipseCoordinates_[SECONDELLIPSECOORDINATE].x = ellipseCoordinates.right;
	ellipseCoordinates_[SECONDELLIPSECOORDINATE].y = ellipseCoordinates.top;
	ellipseCoordinates_[THIRDELLIPSECOORDINATE].x = ellipseCoordinates.right;
	ellipseCoordinates_[THIRDELLIPSECOORDINATE].y = ellipseCoordinates.bottom;
	ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x = ellipseCoordinates.left;
	ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y = ellipseCoordinates.bottom;
	findCenterCoordinates();
}
void CMyPaintEllipse::setCoordinates(CPoint point, bool isClickEnd) {
	ellipseCoordinates_[SECONDELLIPSECOORDINATE].x = point.x;
	ellipseCoordinates_[SECONDELLIPSECOORDINATE].y = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y;
	ellipseCoordinates_[THIRDELLIPSECOORDINATE].x = point.x;
	ellipseCoordinates_[THIRDELLIPSECOORDINATE].y = point.y;
	ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x;
	ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y = point.y;
	findCenterCoordinates();

}
void CMyPaintEllipse::findCenterCoordinates() {
	ellipseCenter_.x = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x + ellipseCoordinates_[THIRDELLIPSECOORDINATE].x) / 2;
	ellipseCenter_.y = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y + ellipseCoordinates_[THIRDELLIPSECOORDINATE].y) / 2;

}
bool CMyPaintEllipse::ifThisFigure(CPoint point) {
	double MaxX, MaxY, MinX, MinY;
	MinX = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x;
	MinY = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y;
	MaxX = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x;
	MaxY = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y;
	for (size_t i = 1; i < 4; i++) {
		if (MinX > ellipseCoordinates_[i].x) {
			MinX = ellipseCoordinates_[i].x;
		}
		if (MinY > ellipseCoordinates_[i].y) {
			MinY = ellipseCoordinates_[i].y;
		}
		if (MaxX < ellipseCoordinates_[i].x) {
			MaxX = ellipseCoordinates_[i].x;
		}
		if (MaxY < ellipseCoordinates_[i].y) {
			MaxY = ellipseCoordinates_[i].y;
		}
	}
	if (point.x > MinX&& point.y > MinY&& point.x < MaxX && point.y < MaxY) {
		return true;
	}
	return false;
}
void  CMyPaintEllipse::move(CPoint* movePoint) {
	CPoint diff;
	diff.x = movePoint[1].x - movePoint[0].x;
	diff.y = movePoint[1].y - movePoint[0].y;
	for (auto& i : ellipseCoordinates_) {
		i.x += diff.x;
		i.y += diff.y;
	}
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		it->second.x += diff.x;
		it->second.y += diff.y;
	}
	findCenterCoordinates();
}
void CMyPaintEllipse::rotate(CPoint* rotatePoint, bool realCoordinates) {
	double resultX, resultY;
	double radius;
	double Rcos, Rsin;
	resultX = rotatePoint[1].x - rotatePoint[0].x;
	resultY = rotatePoint[1].y - rotatePoint[0].y;
	radius = sqrt(pow(resultX, 2) + pow(resultY, 2));
	Rcos = resultX / radius;
	Rsin = resultY / radius;
	for (size_t i = 0; i < 4; i++) {
		tempEllipse_[i] = ellipseCoordinates_[i];
	}
	if (realCoordinates) {
		for (size_t i = 0; i < 4; i++) {
			ellipseCoordinates_[i].x = (tempEllipse_[i].x - ellipseCenter_.x) * Rcos - (tempEllipse_[i].y - ellipseCenter_.y) * Rsin + ellipseCenter_.x;
			ellipseCoordinates_[i].y = (tempEllipse_[i].y - ellipseCenter_.y) * Rcos + (tempEllipse_[i].x - ellipseCenter_.x) * Rsin + ellipseCenter_.y;
		}
	}
	else {
		for (size_t i = 0; i < 4; i++) {
			tempEllipse_[i].x = (ellipseCoordinates_[i].x - ellipseCenter_.x) * Rcos - (ellipseCoordinates_[i].y - ellipseCenter_.y) * Rsin + ellipseCenter_.x;
			tempEllipse_[i].y = (ellipseCoordinates_[i].y - ellipseCenter_.y) * Rcos + (ellipseCoordinates_[i].x - ellipseCenter_.x) * Rsin + ellipseCenter_.y;
		}
	}
	if (realCoordinates) {
		rotationCos_ = Rcos;
		rotationSin_ = Rsin;
		for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
			CPoint tempPoint;
			tempPoint = it->second;
			it->second.x = (tempPoint.x - ellipseCenter_.x) * rotationCos_ - (tempPoint.y - ellipseCenter_.y) * rotationSin_ + ellipseCenter_.x;
			it->second.y = (tempPoint.y - ellipseCenter_.y) * rotationCos_ + (tempPoint.x - ellipseCenter_.x) * rotationSin_ + ellipseCenter_.y;
		}
	}
}
void CMyPaintEllipse::normalize() {
	std::vector<int>place;
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		CPoint point;
		point.x = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x + ellipseCoordinates_[SECONDELLIPSECOORDINATE].x) / 2;
		point.y = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y + ellipseCoordinates_[SECONDELLIPSECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(1);
			continue;
		}
		point.x = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].x + ellipseCoordinates_[THIRDELLIPSECOORDINATE].x) / 2;
		point.y = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].y + ellipseCoordinates_[THIRDELLIPSECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(2);
			continue;
		}
		point.x = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].x + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x) / 2;
		point.y = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].y + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(3);
			continue;
		}
		point.x = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x) / 2;
		point.y = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(4);
			continue;
		}
	}
	CPoint diffrence;
	diffrence.y = ellipseCoordinates_[THIRDELLIPSECOORDINATE].y - ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y;
	diffrence.x = ellipseCoordinates_[THIRDELLIPSECOORDINATE].x - ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x;
	if ((diffrence.x > 0 && diffrence.y < 0) || (diffrence.y > 0 && diffrence.x < 0)) {
		if (abs(diffrence.x) > abs(diffrence.y)) {
			ellipseCoordinates_[THIRDELLIPSECOORDINATE].x = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x - diffrence.y;
			ellipseCoordinates_[THIRDELLIPSECOORDINATE].y = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y + diffrence.y;
			changeOtherCoordinates();
		}
		else {
			ellipseCoordinates_[THIRDELLIPSECOORDINATE].x = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x + diffrence.x;
			ellipseCoordinates_[THIRDELLIPSECOORDINATE].y = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y - diffrence.x;
			changeOtherCoordinates();
		}
	}
	if ((diffrence.x >= 0 && diffrence.y >= 0) || (diffrence.x < 0 && diffrence.y < 0)) {
		if (abs(diffrence.x) > abs(diffrence.y)) {
			ellipseCoordinates_[THIRDELLIPSECOORDINATE].x = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x + diffrence.y;
			ellipseCoordinates_[THIRDELLIPSECOORDINATE].y = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y + diffrence.y;
			changeOtherCoordinates();
		}
		else {
			ellipseCoordinates_[THIRDELLIPSECOORDINATE].x = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x + diffrence.x;
			ellipseCoordinates_[THIRDELLIPSECOORDINATE].y = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y + diffrence.x;
			changeOtherCoordinates();
		}
	}
	findCenterCoordinates();
	if (rotationCos_ != 0 || rotationSin_ != 0) {
		for (size_t i = 0; i < 4; i++) {
			tempEllipse_[i] = ellipseCoordinates_[i];
		}
		for (size_t i = 0; i < 4; i++) {
			ellipseCoordinates_[i].x = (tempEllipse_[i].x - ellipseCenter_.x) * rotationCos_ - (tempEllipse_[i].y - ellipseCenter_.y) * rotationSin_ + ellipseCenter_.x;
			ellipseCoordinates_[i].y = (tempEllipse_[i].y - ellipseCenter_.y) * rotationCos_ + (tempEllipse_[i].x - ellipseCenter_.x) * rotationSin_ + ellipseCenter_.y;
		}
	}
	auto it = connectionsCoordinates_.begin();
	for (auto i = 0; i < place.size(); i++, it++) {
		switch (place[i])
		{
		case 1:
			it->second.x = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x + ellipseCoordinates_[SECONDELLIPSECOORDINATE].x) / 2;
			it->second.y = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y + ellipseCoordinates_[SECONDELLIPSECOORDINATE].y) / 2;
			break;
		case 2:
			it->second.x = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].x + ellipseCoordinates_[THIRDELLIPSECOORDINATE].x) / 2;
			it->second.y = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].y + ellipseCoordinates_[THIRDELLIPSECOORDINATE].y) / 2;
			break;
		case 3:
			it->second.x = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].x + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x) / 2;
			it->second.y = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].y + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y) / 2;
			break;
		case 4:
			it->second.x = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x) / 2;
			it->second.y = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y) / 2;
			break;
		default:
			break;
		}
	}
}
void CMyPaintEllipse::changeOtherCoordinates()
{
	ellipseCoordinates_[SECONDELLIPSECOORDINATE].x = ellipseCoordinates_[THIRDELLIPSECOORDINATE].x;
	ellipseCoordinates_[SECONDELLIPSECOORDINATE].y = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y;
	ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x;
	ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y = ellipseCoordinates_[THIRDELLIPSECOORDINATE].y;
}
bool CMyPaintEllipse::edit(CPoint editPoint, bool isEnd) {
	setCoordinates(editPoint, true);
	return true;
}
void CMyPaintEllipse::makeTempCoordinatesNull() {
	for (size_t i = 0; i < 4; i++) {
		tempEllipse_[i].x = 0;
		tempEllipse_[i].y = 0;
	}
}
CPoint CMyPaintEllipse::getCenterCoordinates() {
	return ellipseCenter_;
}
CPoint CMyPaintEllipse::findConnectionCoordinates(CPoint centerPoint, int connectionNum) {
	CPoint arrowPoint[4];
	double length;
	int pointNum;
	arrowPoint[0].x = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x + ellipseCoordinates_[SECONDELLIPSECOORDINATE].x) / 2;
	arrowPoint[0].y = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y + ellipseCoordinates_[SECONDELLIPSECOORDINATE].y) / 2;
	arrowPoint[1].x = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].x + ellipseCoordinates_[THIRDELLIPSECOORDINATE].x) / 2;
	arrowPoint[1].y = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].y + ellipseCoordinates_[THIRDELLIPSECOORDINATE].y) / 2;
	arrowPoint[2].x = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].x + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x) / 2;
	arrowPoint[2].y = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].y + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y) / 2;
	arrowPoint[3].x = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x) / 2;
	arrowPoint[3].y = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y) / 2;
	length = sqrt(pow(arrowPoint[0].x - centerPoint.x, 2) + pow(arrowPoint[0].y - centerPoint.y, 2));
	pointNum = 0;
	if (length > sqrt(pow(arrowPoint[1].x - centerPoint.x, 2) + pow(arrowPoint[1].y - centerPoint.y, 2))) {
		length = sqrt(pow(arrowPoint[1].x - centerPoint.x, 2) + pow(arrowPoint[1].y - centerPoint.y, 2));
		pointNum = 1;

	}
	if (length > sqrt(pow(arrowPoint[2].x - centerPoint.x, 2) + pow(arrowPoint[2].y - centerPoint.y, 2))) {
		length = sqrt(pow(arrowPoint[2].x - centerPoint.x, 2) + pow(arrowPoint[2].y - centerPoint.y, 2));
		pointNum = 2;

	}
	if (length > sqrt(pow(arrowPoint[3].x - centerPoint.x, 2) + pow(arrowPoint[3].y - centerPoint.y, 2))) {
		length = sqrt(pow(arrowPoint[3].x - centerPoint.x, 2) + pow(arrowPoint[3].y - centerPoint.y, 2));
		pointNum = 3;
	}
	std::pair<int, CPoint> Pair(connectionNum, arrowPoint[pointNum]);
	connectionsCoordinates_.insert(Pair);
	return arrowPoint[pointNum];
}
int CMyPaintEllipse::findConnectionPlace(int key) {
	CPoint arrowPoint[4];
	arrowPoint[0].x = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x + ellipseCoordinates_[SECONDELLIPSECOORDINATE].x) / 2;
	arrowPoint[0].y = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y + ellipseCoordinates_[SECONDELLIPSECOORDINATE].y) / 2;
	arrowPoint[1].x = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].x + ellipseCoordinates_[THIRDELLIPSECOORDINATE].x) / 2;
	arrowPoint[1].y = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].y + ellipseCoordinates_[THIRDELLIPSECOORDINATE].y) / 2;
	arrowPoint[2].x = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].x + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x) / 2;
	arrowPoint[2].y = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].y + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y) / 2;
	arrowPoint[3].x = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x) / 2;
	arrowPoint[3].y = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y) / 2;
	auto it = connectionsCoordinates_.find(key);
	if (it->second == arrowPoint[0]) {
		return 1;
	}
	if (it->second == arrowPoint[1]) {
		return 2;
	}
	if (it->second == arrowPoint[2]) {
		return 3;
	}
	if (it->second == arrowPoint[3]) {
		return 4;
	}
}
std::vector<int> CMyPaintEllipse::getConnectionID() {
	std::vector<int> nums;
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		nums.push_back(it->first);
	}
	return nums;
}
void CMyPaintEllipse::deleteConnection(int key) {
	auto it = connectionsCoordinates_.find(key);
	connectionsCoordinates_.erase(it);
}

void CMyPaintEllipse::properties(std::vector<int>ids, std::vector<CString> names) {
	std::vector<int>place;
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		CPoint point;
		point.x = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x + ellipseCoordinates_[SECONDELLIPSECOORDINATE].x) / 2;
		point.y = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y + ellipseCoordinates_[SECONDELLIPSECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(1);
			continue;
		}
		point.x = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].x + ellipseCoordinates_[THIRDELLIPSECOORDINATE].x) / 2;
		point.y = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].y + ellipseCoordinates_[THIRDELLIPSECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(2);
			continue;
		}
		point.x = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].x + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x) / 2;
		point.y = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].y + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(3);
			continue;
		}
		point.x = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x) / 2;
		point.y = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(4);
			continue;
		}
	}
	CMyPaintPropertiesDialog dlg;
	dlg.getIDs(ids);
	dlg.getNames(names);
	dlg.setName(name_);
	dlg.setID(id_);
	dlg.setFirstPoint(ellipseCoordinates_[0]);
	dlg.setSecondPoint(ellipseCoordinates_[2]);
	dlg.setThirdPoint(ellipseCoordinates_[2]);
	dlg.setWidth(penWidth_);
	dlg.setPenStyle(penStyle_);
	dlg.setBrushStyle(brushStyle_);
	dlg.setPenColor(penColor_);
	dlg.setBrushColor(brushColor_);
	dlg.DoModal();
	name_ = dlg.getName();
	id_ = dlg.getID();
	ellipseCoordinates_[0] = dlg.getFirstPoint();
	ellipseCoordinates_[2] = dlg.getSecondPoint();
	penWidth_ = dlg.getWidth();
	penStyle_ = dlg.getPenStyle();
	brushStyle_ = dlg.getBrushStyle();
	penColor_ = dlg.getPenColor();
	brushColor_ = dlg.getBrushColor();
	changeOtherCoordinates();
	findCenterCoordinates();
	auto it = connectionsCoordinates_.begin();
	for (auto i = 0; i < place.size(); i++, it++) {
		switch (place[i])
		{
		case 1:
			it->second.x = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x + ellipseCoordinates_[SECONDELLIPSECOORDINATE].x) / 2;
			it->second.y = (ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y + ellipseCoordinates_[SECONDELLIPSECOORDINATE].y) / 2;
			break;
		case 2:
			it->second.x = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].x + ellipseCoordinates_[THIRDELLIPSECOORDINATE].x) / 2;
			it->second.y = (ellipseCoordinates_[SECONDELLIPSECOORDINATE].y + ellipseCoordinates_[THIRDELLIPSECOORDINATE].y) / 2;
			break;
		case 3:
			it->second.x = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].x + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x) / 2;
			it->second.y = (ellipseCoordinates_[THIRDELLIPSECOORDINATE].y + ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y) / 2;
			break;
		case 4:
			it->second.x = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].x + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x) / 2;
			it->second.y = (ellipseCoordinates_[FOURTHELLIPSECOORDINATE].y + ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y) / 2;
			break;
		default:
			break;
		}
	}
}


CPoint CMyPaintEllipse::getFirstCoordinate() {
	return ellipseCoordinates_[FIRSTELLIPSECOORDINATE];
}

CPoint CMyPaintEllipse::getSecondCoordinate() {
	return ellipseCoordinates_[THIRDELLIPSECOORDINATE];
}

CPoint CMyPaintEllipse::getThirdCoordinate() {
	return ellipseCoordinates_[THIRDELLIPSECOORDINATE];
}

int CMyPaintEllipse::getFigureType() {
	return 1;
}

void CMyPaintEllipse::setFirstCoordinate(CPoint point) {
	ellipseCoordinates_[FIRSTELLIPSECOORDINATE] = point;
}

void CMyPaintEllipse::setSecondCoordinate(CPoint point) {
	ellipseCoordinates_[THIRDELLIPSECOORDINATE] = point;
}

void CMyPaintEllipse::setThirdCoordinate(CPoint point) {
}

void CMyPaintEllipse::draw(HDC hdc) {
	CPen Pen(penStyle_, penWidth_, penColor_);
	SelectObject(hdc, Pen);
	if (brushStyle_ == 0) {
		HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y, ellipseCoordinates_[THIRDELLIPSECOORDINATE].x, ellipseCoordinates_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 1) {
		HBRUSH hBrush = CreateSolidBrush(brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y, ellipseCoordinates_[THIRDELLIPSECOORDINATE].x, ellipseCoordinates_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 2) {
		CBrush hBrush(HS_BDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y, ellipseCoordinates_[THIRDELLIPSECOORDINATE].x, ellipseCoordinates_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 3) {
		CBrush hBrush(HS_CROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y, ellipseCoordinates_[THIRDELLIPSECOORDINATE].x, ellipseCoordinates_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 4) {
		CBrush hBrush(HS_DIAGCROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y, ellipseCoordinates_[THIRDELLIPSECOORDINATE].x, ellipseCoordinates_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 5) {
		CBrush hBrush(HS_FDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y, ellipseCoordinates_[THIRDELLIPSECOORDINATE].x, ellipseCoordinates_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 6) {
		CBrush hBrush(HS_HORIZONTAL, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y, ellipseCoordinates_[THIRDELLIPSECOORDINATE].x, ellipseCoordinates_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 7) {
		CBrush hBrush(HS_VERTICAL, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y, ellipseCoordinates_[THIRDELLIPSECOORDINATE].x, ellipseCoordinates_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	DeleteObject(Pen);
}

void CMyPaintEllipse::tempDraw(HDC hdc) {
	CPen Pen(penStyle_, penWidth_, penColor_);
	SelectObject(hdc, Pen);
	if (brushStyle_ == 0) {
		HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, tempEllipse_[FIRSTELLIPSECOORDINATE].x, tempEllipse_[FIRSTELLIPSECOORDINATE].y, tempEllipse_[THIRDELLIPSECOORDINATE].x, tempEllipse_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 1) {
		HBRUSH hBrush = CreateSolidBrush(brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, tempEllipse_[FIRSTELLIPSECOORDINATE].x, tempEllipse_[FIRSTELLIPSECOORDINATE].y, tempEllipse_[THIRDELLIPSECOORDINATE].x, tempEllipse_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 2) {
		CBrush hBrush(HS_BDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, tempEllipse_[FIRSTELLIPSECOORDINATE].x, tempEllipse_[FIRSTELLIPSECOORDINATE].y, tempEllipse_[THIRDELLIPSECOORDINATE].x, tempEllipse_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 3) {
		CBrush hBrush(HS_CROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, tempEllipse_[FIRSTELLIPSECOORDINATE].x, tempEllipse_[FIRSTELLIPSECOORDINATE].y, tempEllipse_[THIRDELLIPSECOORDINATE].x, tempEllipse_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 4) {
		CBrush hBrush(HS_DIAGCROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, tempEllipse_[FIRSTELLIPSECOORDINATE].x, tempEllipse_[FIRSTELLIPSECOORDINATE].y, tempEllipse_[THIRDELLIPSECOORDINATE].x, tempEllipse_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 5) {
		CBrush hBrush(HS_FDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, tempEllipse_[FIRSTELLIPSECOORDINATE].x, tempEllipse_[FIRSTELLIPSECOORDINATE].y, tempEllipse_[THIRDELLIPSECOORDINATE].x, tempEllipse_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 6) {
		CBrush hBrush(HS_HORIZONTAL, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, tempEllipse_[FIRSTELLIPSECOORDINATE].x, tempEllipse_[FIRSTELLIPSECOORDINATE].y, tempEllipse_[THIRDELLIPSECOORDINATE].x, tempEllipse_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 7) {
		CBrush hBrush(HS_VERTICAL, brushColor_);
		SelectObject(hdc, hBrush);
		Ellipse(hdc, tempEllipse_[FIRSTELLIPSECOORDINATE].x, tempEllipse_[FIRSTELLIPSECOORDINATE].y, tempEllipse_[THIRDELLIPSECOORDINATE].x, tempEllipse_[THIRDELLIPSECOORDINATE].y);
		DeleteObject(hBrush);
	}
	DeleteObject(Pen);
}
void CMyPaintEllipse::select(HDC hdc) {
	int selectStyle;
	COLORREF selectColor;
	if (penStyle_ != PS_DOT) {
		selectStyle = PS_DOT;
		selectColor = RGB(0, 0, 0);
	}
	else {
		selectStyle = PS_SOLID;
		selectColor = RGB(0, 0, 0);
	}
	CPen Pen(selectStyle, 1, selectColor);
	SelectObject(hdc, Pen);
	HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	SelectObject(hdc, hBrush);
	Ellipse(hdc, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x, ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y, ellipseCoordinates_[THIRDELLIPSECOORDINATE].x, ellipseCoordinates_[THIRDELLIPSECOORDINATE].y);
	DeleteObject(hBrush);
	DeleteObject(Pen);
}
void CMyPaintEllipse::scrollFigure(CPoint point) {
	for (auto i = 0; i < 4; i++) {
		ellipseCoordinates_[i].x +=point.x;
		ellipseCoordinates_[i].y += point.y;
	}
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		it->second.x += point.x;
		it->second.y += point.y;
	}
	findCenterCoordinates();
}

std::vector<CPoint> CMyPaintEllipse::getLeftCoordinate() {
	CPoint temp;
	CPoint tempEllipseCoordinate[4];
	tempEllipseCoordinate[FIRSTELLIPSECOORDINATE].x = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x;
	tempEllipseCoordinate[FIRSTELLIPSECOORDINATE].y = ellipseCenter_.y;
	tempEllipseCoordinate[SECONDELLIPSECOORDINATE].x = ellipseCenter_.x;
	tempEllipseCoordinate[SECONDELLIPSECOORDINATE].y = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y;
	tempEllipseCoordinate[THIRDELLIPSECOORDINATE].x = ellipseCoordinates_[THIRDELLIPSECOORDINATE].x;
	tempEllipseCoordinate[THIRDELLIPSECOORDINATE].y = ellipseCenter_.y;
	tempEllipseCoordinate[FOURTHELLIPSECOORDINATE].x = ellipseCenter_.x;
	tempEllipseCoordinate[FOURTHELLIPSECOORDINATE].y = ellipseCoordinates_[THIRDELLIPSECOORDINATE].y;

	temp = tempEllipseCoordinate[0];
	for (auto i = 1; i < 4; i++) {
		if (tempEllipseCoordinate[i].x < temp.x) {
			temp = tempEllipseCoordinate[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<CPoint> CMyPaintEllipse::getBottomCoordinate() {
	CPoint temp;
	CPoint tempEllipseCoordinate[4];
	tempEllipseCoordinate[FIRSTELLIPSECOORDINATE].x = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x;
	tempEllipseCoordinate[FIRSTELLIPSECOORDINATE].y = ellipseCenter_.y;
	tempEllipseCoordinate[SECONDELLIPSECOORDINATE].x = ellipseCenter_.x;
	tempEllipseCoordinate[SECONDELLIPSECOORDINATE].y = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y;
	tempEllipseCoordinate[THIRDELLIPSECOORDINATE].x = ellipseCoordinates_[THIRDELLIPSECOORDINATE].x;
	tempEllipseCoordinate[THIRDELLIPSECOORDINATE].y = ellipseCenter_.y;
	tempEllipseCoordinate[FOURTHELLIPSECOORDINATE].x = ellipseCenter_.x;
	tempEllipseCoordinate[FOURTHELLIPSECOORDINATE].y = ellipseCoordinates_[THIRDELLIPSECOORDINATE].y;

	temp = tempEllipseCoordinate[0];
	for (auto i = 1; i < 4; i++) {
		if (tempEllipseCoordinate[i].y < temp.y) {
			temp = tempEllipseCoordinate[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<CPoint> CMyPaintEllipse::getRightCoordinate() {
	CPoint temp;
	CPoint tempEllipseCoordinate[4];
	tempEllipseCoordinate[FIRSTELLIPSECOORDINATE].x = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x;
	tempEllipseCoordinate[FIRSTELLIPSECOORDINATE].y = ellipseCenter_.y;
	tempEllipseCoordinate[SECONDELLIPSECOORDINATE].x = ellipseCenter_.x;
	tempEllipseCoordinate[SECONDELLIPSECOORDINATE].y = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y;
	tempEllipseCoordinate[THIRDELLIPSECOORDINATE].x = ellipseCoordinates_[THIRDELLIPSECOORDINATE].x;
	tempEllipseCoordinate[THIRDELLIPSECOORDINATE].y = ellipseCenter_.y;
	tempEllipseCoordinate[FOURTHELLIPSECOORDINATE].x = ellipseCenter_.x;
	tempEllipseCoordinate[FOURTHELLIPSECOORDINATE].y = ellipseCoordinates_[THIRDELLIPSECOORDINATE].y;

	temp = tempEllipseCoordinate[0];
	for (auto i = 1; i < 4; i++) {
		if (tempEllipseCoordinate[i].x > temp.x) {
			temp = tempEllipseCoordinate[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<CPoint> CMyPaintEllipse::getTopCoordinate() {
	CPoint temp;
	CPoint tempEllipseCoordinate[4];
	tempEllipseCoordinate[FIRSTELLIPSECOORDINATE].x = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x;
	tempEllipseCoordinate[FIRSTELLIPSECOORDINATE].y = ellipseCenter_.y;
	tempEllipseCoordinate[SECONDELLIPSECOORDINATE].x = ellipseCenter_.x;
	tempEllipseCoordinate[SECONDELLIPSECOORDINATE].y = ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y;
	tempEllipseCoordinate[THIRDELLIPSECOORDINATE].x = ellipseCoordinates_[THIRDELLIPSECOORDINATE].x;
	tempEllipseCoordinate[THIRDELLIPSECOORDINATE].y = ellipseCenter_.y;
	tempEllipseCoordinate[FOURTHELLIPSECOORDINATE].x = ellipseCenter_.x;
	tempEllipseCoordinate[FOURTHELLIPSECOORDINATE].y = ellipseCoordinates_[THIRDELLIPSECOORDINATE].y;

	temp = tempEllipseCoordinate[0];
	for (auto i = 1; i < 4; i++) {
		if (tempEllipseCoordinate[i].y > temp.y) {
			temp = tempEllipseCoordinate[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<LONG> CMyPaintEllipse::getMaxMinX() {
	std::vector<LONG>MaxMin;
	MaxMin.push_back(ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x);
	MaxMin.push_back(ellipseCoordinates_[FIRSTELLIPSECOORDINATE].x);
	for (auto i = 1; i < 4; i++) {
		if (ellipseCoordinates_[i].x > MaxMin[0]) {
			MaxMin[0] = ellipseCoordinates_[i].x;
		}
		if (ellipseCoordinates_[i].x < MaxMin[1]) {
			MaxMin[1] = ellipseCoordinates_[i].x;
		}
	}
	return MaxMin;
}

std::vector<LONG> CMyPaintEllipse::getMaxMinY() {
	std::vector<LONG>MaxMin;
	MaxMin.push_back(ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y);
	MaxMin.push_back(ellipseCoordinates_[FIRSTELLIPSECOORDINATE].y);
	for (auto i = 1; i < 4; i++) {
		if (ellipseCoordinates_[i].y > MaxMin[0]) {
			MaxMin[0] = ellipseCoordinates_[i].y;
		}
		if (ellipseCoordinates_[i].y < MaxMin[1]) {
			MaxMin[1] = ellipseCoordinates_[i].y;
		}
	}
	return MaxMin;
}