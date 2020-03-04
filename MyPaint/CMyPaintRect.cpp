#include "pch.h"
#include "CMyPaintRect.h"
CMyPaintRect::CMyPaintRect() : CMyPaintFigure()
{}
CMyPaintRect::CMyPaintRect(unsigned int id, CString name, int penWidth, COLORREF penColor, int penStyle, COLORREF brushColor, int brushStyle, CRect rectCoordinates) : CMyPaintFigure(id, name, penWidth, penColor, penStyle, brushColor, brushStyle)
{
	rectCoordinates_[0].x = rectCoordinates.left;
	rectCoordinates_[0].y = rectCoordinates.top;
	rectCoordinates_[1].x = rectCoordinates.right;
	rectCoordinates_[1].y = rectCoordinates.top;
	rectCoordinates_[2].x = rectCoordinates.right;
	rectCoordinates_[2].y = rectCoordinates.bottom;
	rectCoordinates_[3].x = rectCoordinates.left;
	rectCoordinates_[3].y = rectCoordinates.bottom;
	findCenterCoordinates();
}
void CMyPaintRect::setCoordinates(CPoint point, bool isClickEnd) {
	rectCoordinates_[1].x = point.x;
	rectCoordinates_[1].y = rectCoordinates_[0].y;
	rectCoordinates_[2].x = point.x;
	rectCoordinates_[2].y = point.y;
	rectCoordinates_[3].x = rectCoordinates_[0].x;
	rectCoordinates_[3].y = point.y;
	findCenterCoordinates();

}
void CMyPaintRect::findCenterCoordinates() {
	rectCenter_.x = (rectCoordinates_[0].x + rectCoordinates_[2].x) / 2;
	rectCenter_.y = (rectCoordinates_[0].y + rectCoordinates_[2].y) / 2;
}

bool CMyPaintRect::ifThisFigure(CPoint point) {
	double MaxX, MaxY, MinX, MinY;
	MinX = rectCoordinates_[0].x;
	MinY = rectCoordinates_[0].y;
	MaxX = rectCoordinates_[0].x;
	MaxY = rectCoordinates_[0].y;
	for (size_t i = 1; i < 4; i++) {
		if (MinX > rectCoordinates_[i].x) {
			MinX = rectCoordinates_[i].x;
		}
		if (MinY > rectCoordinates_[i].y) {
			MinY = rectCoordinates_[i].y;
		}
		if (MaxX < rectCoordinates_[i].x) {
			MaxX = rectCoordinates_[i].x;
		}
		if (MaxY < rectCoordinates_[i].y) {
			MaxY = rectCoordinates_[i].y;
		}
	}
	if (point.x > MinX&& point.y > MinY&& point.x < MaxX && point.y < MaxY) {
		return true;
	}
	return false;
}
void CMyPaintRect::move(CPoint* movePoint) {
	CPoint diff;
	diff.x = movePoint[1].x - movePoint[0].x;
	diff.y = movePoint[1].y - movePoint[0].y;
	for (size_t i = 0; i < 4; i++) {
		rectCoordinates_[i].x += diff.x;
		rectCoordinates_[i].y += diff.y;
	}
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		it->second.x += diff.x;
		it->second.y += diff.y;
	}
	findCenterCoordinates();
}
void CMyPaintRect::rotate(CPoint* rotatePoint, bool realCoordinates) {
	double resultX, resultY;
	double radius;
	double Rcos, Rsin;
	resultX = rotatePoint[1].x - rotatePoint[0].x;
	resultY = rotatePoint[1].y - rotatePoint[0].y;
	radius = sqrt(pow(resultX, 2) + pow(resultY, 2));
	Rcos = resultX / radius;
	Rsin = resultY / radius;
	for (size_t i = 0; i < 4; i++) {
		tempRect_[i] = rectCoordinates_[i];
	}
	if (realCoordinates) {
		for (size_t i = 0; i < 4; i++) {
			rectCoordinates_[i].x = (tempRect_[i].x - rectCenter_.x) * Rcos - (tempRect_[i].y - rectCenter_.y) * Rsin + rectCenter_.x;
			rectCoordinates_[i].y = (tempRect_[i].y - rectCenter_.y) * Rcos + (tempRect_[i].x - rectCenter_.x) * Rsin + rectCenter_.y;
			//makeTempCoordinatesNull();
		}
	}
	else {
		for (size_t i = 0; i < 4; i++) {
			tempRect_[i].x = (rectCoordinates_[i].x - rectCenter_.x) * Rcos - (rectCoordinates_[i].y - rectCenter_.y) * Rsin + rectCenter_.x;
			tempRect_[i].y = (rectCoordinates_[i].y - rectCenter_.y) * Rcos + (rectCoordinates_[i].x - rectCenter_.x) * Rsin + rectCenter_.y;
		}
	}
	if (realCoordinates) {
		rotationCos_ = Rcos;
		rotationSin_ = Rsin;
		for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
			CPoint tempPoint;
			tempPoint = it->second;
			it->second.x = (tempPoint.x - rectCenter_.x) * rotationCos_ - (tempPoint.y - rectCenter_.y) * rotationSin_ + rectCenter_.x;
			it->second.y = (tempPoint.y - rectCenter_.y) * rotationCos_ + (tempPoint.x - rectCenter_.x) * rotationSin_ + rectCenter_.y;
		}
	}
}
void CMyPaintRect::normalize() {
	std::vector<int>place;
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		CPoint point;
		point.x = (rectCoordinates_[0].x + rectCoordinates_[1].x) / 2;
		point.y = (rectCoordinates_[0].y + rectCoordinates_[1].y) / 2;
		if (it->second == point) {
			place.push_back(1);
			continue;
		}
		point.x = (rectCoordinates_[1].x + rectCoordinates_[2].x) / 2;
		point.y = (rectCoordinates_[1].y + rectCoordinates_[2].y) / 2;
		if (it->second == point) {
			place.push_back(2);
			continue;
		}
		point.x = (rectCoordinates_[2].x + rectCoordinates_[3].x) / 2;
		point.y = (rectCoordinates_[2].y + rectCoordinates_[3].y) / 2;
		if (it->second == point) {
			place.push_back(3);
			continue;
		}
		point.x = (rectCoordinates_[3].x + rectCoordinates_[0].x) / 2;
		point.y = (rectCoordinates_[3].y + rectCoordinates_[0].y) / 2;
		if (it->second == point) {
			place.push_back(4);
			continue;
		}
	}
	CPoint diffrence;
	diffrence.y = rectCoordinates_[2].y - rectCoordinates_[0].y;
	diffrence.x = rectCoordinates_[2].x - rectCoordinates_[0].x;
	if ((diffrence.x > 0 && diffrence.y < 0) || (diffrence.y > 0 && diffrence.x < 0)) {
		if (abs(diffrence.x) > abs(diffrence.y)) {
			rectCoordinates_[2].x = rectCoordinates_[0].x - diffrence.y;
			rectCoordinates_[2].y = rectCoordinates_[0].y + diffrence.y;
			changeOtherCoordinates();
		}
		else {
			rectCoordinates_[2].x = rectCoordinates_[0].x + diffrence.x;
			rectCoordinates_[2].y = rectCoordinates_[0].y - diffrence.x;
			changeOtherCoordinates();
		}
	}
	if ((diffrence.x >= 0 && diffrence.y >= 0) || (diffrence.x < 0 && diffrence.y < 0)) {
		if (abs(diffrence.x) > abs(diffrence.y)) {
			rectCoordinates_[2].x = rectCoordinates_[0].x + diffrence.y;
			rectCoordinates_[2].y = rectCoordinates_[0].y + diffrence.y;
			changeOtherCoordinates();
		}
		else {
			rectCoordinates_[2].x = rectCoordinates_[0].x + diffrence.x;
			rectCoordinates_[2].y = rectCoordinates_[0].y + diffrence.x;
			changeOtherCoordinates();
		}
	}
	findCenterCoordinates();

	if (rotationCos_ != 0 || rotationSin_ != 0) {
		for (size_t i = 0; i < 4; i++) {
			tempRect_[i] = rectCoordinates_[i];
		}
		for (size_t i = 0; i < 4; i++) {
			rectCoordinates_[i].x = (tempRect_[i].x - rectCenter_.x) * rotationCos_ - (tempRect_[i].y - rectCenter_.y) * rotationSin_ + rectCenter_.x;
			rectCoordinates_[i].y = (tempRect_[i].y - rectCenter_.y) * rotationCos_ + (tempRect_[i].x - rectCenter_.x) * rotationSin_ + rectCenter_.y;
		}
	}
	auto it = connectionsCoordinates_.begin();
	for (auto i = 0; i < place.size(); i++, it++) {
		switch (place[i])
		{
		case 1:
			it->second.x = (rectCoordinates_[0].x + rectCoordinates_[1].x) / 2;
			it->second.y = (rectCoordinates_[0].y + rectCoordinates_[1].y) / 2;
			break;
		case 2:
			it->second.x = (rectCoordinates_[1].x + rectCoordinates_[2].x) / 2;
			it->second.y = (rectCoordinates_[1].y + rectCoordinates_[2].y) / 2;
			break;
		case 3:
			it->second.x = (rectCoordinates_[2].x + rectCoordinates_[3].x) / 2;
			it->second.y = (rectCoordinates_[2].y + rectCoordinates_[3].y) / 2;
			break;
		case 4:
			it->second.x = (rectCoordinates_[3].x + rectCoordinates_[0].x) / 2;
			it->second.y = (rectCoordinates_[3].y + rectCoordinates_[0].y) / 2;
			break;
		default:
			break;
		}
	}
}
void CMyPaintRect::changeOtherCoordinates()
{
	rectCoordinates_[1].x = rectCoordinates_[2].x;
	rectCoordinates_[1].y = rectCoordinates_[0].y;
	rectCoordinates_[3].x = rectCoordinates_[0].x;
	rectCoordinates_[3].y = rectCoordinates_[2].y;
}
bool CMyPaintRect::edit(CPoint editPoint, bool isEnd) {
	setCoordinates(editPoint, true);
	return true;
}
void CMyPaintRect::makeTempCoordinatesNull() {
	for (size_t i = 0; i < 4; i++) {
		tempRect_[i].x = 0;
		tempRect_[i].y = 0;
	}
}
CPoint CMyPaintRect::getCenterCoordinates() {
	return rectCenter_;
}
CPoint CMyPaintRect::findConnectionCoordinates(CPoint centerPoint, int connectionNum) {
	CPoint arrowPoint[4];
	double length;
	int pointNum;
	arrowPoint[0].x = (rectCoordinates_[0].x + rectCoordinates_[1].x) / 2;
	arrowPoint[0].y = (rectCoordinates_[0].y + rectCoordinates_[1].y) / 2;
	arrowPoint[1].x = (rectCoordinates_[1].x + rectCoordinates_[2].x) / 2;
	arrowPoint[1].y = (rectCoordinates_[1].y + rectCoordinates_[2].y) / 2;
	arrowPoint[2].x = (rectCoordinates_[2].x + rectCoordinates_[3].x) / 2;
	arrowPoint[2].y = (rectCoordinates_[2].y + rectCoordinates_[3].y) / 2;
	arrowPoint[3].x = (rectCoordinates_[3].x + rectCoordinates_[0].x) / 2;
	arrowPoint[3].y = (rectCoordinates_[3].y + rectCoordinates_[0].y) / 2;
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
int CMyPaintRect::findConnectionPlace(int key) {
	CPoint arrowPoint[4];
	arrowPoint[0].x = (rectCoordinates_[0].x + rectCoordinates_[1].x) / 2;
	arrowPoint[0].y = (rectCoordinates_[0].y + rectCoordinates_[1].y) / 2;
	arrowPoint[1].x = (rectCoordinates_[1].x + rectCoordinates_[2].x) / 2;
	arrowPoint[1].y = (rectCoordinates_[1].y + rectCoordinates_[2].y) / 2;
	arrowPoint[2].x = (rectCoordinates_[2].x + rectCoordinates_[3].x) / 2;
	arrowPoint[2].y = (rectCoordinates_[2].y + rectCoordinates_[3].y) / 2;
	arrowPoint[3].x = (rectCoordinates_[3].x + rectCoordinates_[0].x) / 2;
	arrowPoint[3].y = (rectCoordinates_[3].y + rectCoordinates_[0].y) / 2;
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
std::vector<int> CMyPaintRect::getConnectionID() {
	std::vector<int> nums;
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		nums.push_back(it->first);
	}
	return nums;
}
void CMyPaintRect::deleteConnection(int key) {
	auto it = connectionsCoordinates_.find(key);
	connectionsCoordinates_.erase(it);
}

void CMyPaintRect::properties(std::vector<int>ids, std::vector<CString> names) {
	std::vector<int>place;
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		CPoint point;
		point.x = (rectCoordinates_[0].x + rectCoordinates_[1].x) / 2;
		point.y = (rectCoordinates_[0].y + rectCoordinates_[1].y) / 2;
		if (it->second == point) {
			place.push_back(1);
			continue;
		}
		point.x = (rectCoordinates_[1].x + rectCoordinates_[2].x) / 2;
		point.y = (rectCoordinates_[1].y + rectCoordinates_[2].y) / 2;
		if (it->second == point) {
			place.push_back(2);
			continue;
		}
		point.x = (rectCoordinates_[2].x + rectCoordinates_[3].x) / 2;
		point.y = (rectCoordinates_[2].y + rectCoordinates_[3].y) / 2;
		if (it->second == point) {
			place.push_back(3);
			continue;
		}
		point.x = (rectCoordinates_[3].x + rectCoordinates_[0].x) / 2;
		point.y = (rectCoordinates_[3].y + rectCoordinates_[0].y) / 2;
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
	dlg.setFirstPoint(rectCoordinates_[0]);
	dlg.setSecondPoint(rectCoordinates_[2]);
	dlg.setThirdPoint(rectCoordinates_[2]);
	dlg.setWidth(penWidth_);
	dlg.setPenStyle(penStyle_);
	dlg.setBrushStyle(brushStyle_);
	dlg.setPenColor(penColor_);
	dlg.setBrushColor(brushColor_);
	dlg.DoModal();
	name_ = dlg.getName();
	id_ = dlg.getID();
	rectCoordinates_[0] = dlg.getFirstPoint();
	rectCoordinates_[2] = dlg.getSecondPoint();
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
			it->second.x = (rectCoordinates_[0].x + rectCoordinates_[1].x) / 2;
			it->second.y = (rectCoordinates_[0].y + rectCoordinates_[1].y) / 2;
			break;
		case 2:
			it->second.x = (rectCoordinates_[1].x + rectCoordinates_[2].x) / 2;
			it->second.y = (rectCoordinates_[1].y + rectCoordinates_[2].y) / 2;
			break;
		case 3:
			it->second.x = (rectCoordinates_[2].x + rectCoordinates_[3].x) / 2;
			it->second.y = (rectCoordinates_[2].y + rectCoordinates_[3].y) / 2;
			break;
		case 4:
			it->second.x = (rectCoordinates_[3].x + rectCoordinates_[0].x) / 2;
			it->second.y = (rectCoordinates_[3].y + rectCoordinates_[0].y) / 2;
			break;
		default:
			break;
		}
	}
}

CPoint CMyPaintRect::getFirstCoordinate() {
	return rectCoordinates_[0];
}

CPoint CMyPaintRect::getSecondCoordinate() {
	return rectCoordinates_[2];
}

CPoint CMyPaintRect::getThirdCoordinate() {
	return rectCoordinates_[2];
}

int CMyPaintRect::getFigureType() {
	return 2;
}

void CMyPaintRect::setFirstCoordinate(CPoint point) {
	rectCoordinates_[0] = point;
}

void CMyPaintRect::setSecondCoordinate(CPoint point) {
	rectCoordinates_[2] = point;
}

void CMyPaintRect::setThirdCoordinate(CPoint point) {
}
void CMyPaintRect::draw(HDC hdc) {
	CPen Pen(penStyle_, penWidth_, penColor_);
	SelectObject(hdc, Pen);
	if (brushStyle_ == 0) {
		HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		SelectObject(hdc, hBrush);
		Polygon(hdc, rectCoordinates_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 1) {
		HBRUSH hBrush = CreateSolidBrush(brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, rectCoordinates_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 2) {
		CBrush hBrush(HS_BDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, rectCoordinates_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 3) {
		CBrush hBrush(HS_CROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, rectCoordinates_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 4) {
		CBrush hBrush(HS_DIAGCROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, rectCoordinates_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 5) {
		CBrush hBrush(HS_FDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, rectCoordinates_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 6) {
		CBrush hBrush(HS_HORIZONTAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, rectCoordinates_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 7) {
		CBrush hBrush(HS_VERTICAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, rectCoordinates_, 4);
		DeleteObject(hBrush);
	}
	DeleteObject(Pen);
}

void CMyPaintRect::tempDraw(HDC hdc) {
	CPen Pen(penStyle_, penWidth_, penColor_);
	SelectObject(hdc, Pen);
	if (brushStyle_ == 0) {
		HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempRect_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 1) {
		HBRUSH hBrush = CreateSolidBrush(brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempRect_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 2) {
		CBrush hBrush(HS_BDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempRect_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 3) {
		CBrush hBrush(HS_CROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempRect_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 4) {
		CBrush hBrush(HS_DIAGCROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempRect_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 5) {
		CBrush hBrush(HS_FDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempRect_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 6) {
		CBrush hBrush(HS_HORIZONTAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempRect_, 4);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 7) {
		CBrush hBrush(HS_VERTICAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempRect_, 4);
		DeleteObject(hBrush);
	}
	DeleteObject(Pen);
}
void CMyPaintRect::select(HDC hdc) {
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
	Polygon(hdc, rectCoordinates_, 4);
	DeleteObject(hBrush);
	DeleteObject(Pen);
}