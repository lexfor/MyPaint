#include "pch.h"
#include "CMyPaintTriangle.h"

#define FIRSTTRIANGLECOORDINATE 0
#define SECONDTRIANGLECOORDINATE 1
#define THIRDTRIANGLECOORDINATE 2


CMyPaintTriangle::CMyPaintTriangle() : triangleCoordinates_(), CMyPaintFigure()
{
	clickCount_ = clickCountEnum_::firstClick;
}
CMyPaintTriangle::CMyPaintTriangle(unsigned int id, CString name, int penWidth, COLORREF penColor, int penStyle, COLORREF brushColor, int brushStyle, CPoint FirstPoint, CPoint SecondPoint, CPoint ThirdPoint) : CMyPaintFigure(id, name, penWidth, penColor, penStyle, brushColor, brushStyle)
{
	triangleCoordinates_[FIRSTTRIANGLECOORDINATE] = FirstPoint;
	triangleCoordinates_[SECONDTRIANGLECOORDINATE] = SecondPoint;
	triangleCoordinates_[THIRDTRIANGLECOORDINATE] = ThirdPoint;
	findCenterCoordinates();
	clickCount_ = clickCountEnum_::firstClick;
}
void CMyPaintTriangle::findCenterCoordinates() {
	triangleCenter_.x = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x + triangleCoordinates_[SECONDTRIANGLECOORDINATE].x + triangleCoordinates_[THIRDTRIANGLECOORDINATE].x) / 3;
	triangleCenter_.y = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y + triangleCoordinates_[SECONDTRIANGLECOORDINATE].y + triangleCoordinates_[THIRDTRIANGLECOORDINATE].y) / 3;
}
bool CMyPaintTriangle::ifThisFigure(CPoint point) {
	int SummOne, SummTwo, SummThree;
	SummOne = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x - point.x) * (triangleCoordinates_[SECONDTRIANGLECOORDINATE].y - triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y) - (triangleCoordinates_[SECONDTRIANGLECOORDINATE].x - triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x) * (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y - point.y);
	SummTwo = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].x - point.x) * (triangleCoordinates_[THIRDTRIANGLECOORDINATE].y - triangleCoordinates_[SECONDTRIANGLECOORDINATE].y) - (triangleCoordinates_[THIRDTRIANGLECOORDINATE].x - triangleCoordinates_[SECONDTRIANGLECOORDINATE].x) * (triangleCoordinates_[SECONDTRIANGLECOORDINATE].y - point.y);
	SummThree = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].x - point.x) * (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y - triangleCoordinates_[THIRDTRIANGLECOORDINATE].y) - (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x - triangleCoordinates_[THIRDTRIANGLECOORDINATE].x) * (triangleCoordinates_[THIRDTRIANGLECOORDINATE].y - point.y);
	if (SummOne > 0 && SummTwo > 0 && SummThree > 0) {
		return true;
	}
	if (SummOne < 0 && SummTwo < 0 && SummThree < 0) {
		return true;
	}
	return false;
}

void CMyPaintTriangle::setCoordinates(CPoint point, bool isClickEnd) {
	switch (clickCount_) {
	case clickCountEnum_::firstClick:
		triangleCoordinates_[FIRSTTRIANGLECOORDINATE] = point;
		findCenterCoordinates();
		if (isClickEnd) {
			clickCount_ = clickCountEnum_::secondClick;
		}
		break;
	case clickCountEnum_::secondClick:
		triangleCoordinates_[SECONDTRIANGLECOORDINATE] = point;
		findCenterCoordinates();
		if (isClickEnd) {
			clickCount_ = clickCountEnum_::thirdClick;
		}
		break;
	case clickCountEnum_::thirdClick:
		triangleCoordinates_[THIRDTRIANGLECOORDINATE] = point;
		findCenterCoordinates();
		if (isClickEnd) {
			clickCount_ = clickCountEnum_::secondClick;
		}
		break;
	default:
		break;
	}
}

void CMyPaintTriangle::move(CPoint* movePoint) {
	CPoint diff;
	diff.x = movePoint[1].x - movePoint[0].x;
	diff.y = movePoint[1].y - movePoint[0].y;
	for (size_t i = 0; i < 3; i++) {
		triangleCoordinates_[i].x += diff.x;
		triangleCoordinates_[i].y += diff.y;
	}
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		it->second.x += diff.x;
		it->second.y += diff.y;
	}
	findCenterCoordinates();
}
void CMyPaintTriangle::rotate(CPoint* rotatePoint, bool realCoordinates) {
	double resultX, resultY;
	double radius;
	double Rcos, Rsin;
	resultX = rotatePoint[1].x - rotatePoint[0].x;
	resultY = rotatePoint[1].y - rotatePoint[0].y;
	radius = sqrt(pow(resultX, 2) + pow(resultY, 2));
	Rcos = resultX / radius;
	Rsin = resultY / radius;
	for (size_t i = 0; i < 3; i++) {
		tempTriangle_[i] = triangleCoordinates_[i];
	}
	if (realCoordinates) {
		for (size_t i = 0; i < 3; i++) {
			triangleCoordinates_[i].x = (tempTriangle_[i].x - triangleCenter_.x) * Rcos - (tempTriangle_[i].y - triangleCenter_.y) * Rsin + triangleCenter_.x;
			triangleCoordinates_[i].y = (tempTriangle_[i].y - triangleCenter_.y) * Rcos + (tempTriangle_[i].x - triangleCenter_.x) * Rsin + triangleCenter_.y;
		}
	}
	else {
		for (size_t i = 0; i < 3; i++) {
			tempTriangle_[i].x = (triangleCoordinates_[i].x - triangleCenter_.x) * Rcos - (triangleCoordinates_[i].y - triangleCenter_.y) * Rsin + triangleCenter_.x;
			tempTriangle_[i].y = (triangleCoordinates_[i].y - triangleCenter_.y) * Rcos + (triangleCoordinates_[i].x - triangleCenter_.x) * Rsin + triangleCenter_.y;
		}
	}
	if (realCoordinates) {
		rotationCos_ = Rcos;
		rotationSin_ = Rsin;
		for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
			CPoint tempPoint;
			tempPoint = it->second;
			it->second.x = (tempPoint.x - triangleCenter_.x) * rotationCos_ - (tempPoint.y - triangleCenter_.y) * rotationSin_ + triangleCenter_.x;
			it->second.y = (tempPoint.y - triangleCenter_.y) * rotationCos_ + (tempPoint.x - triangleCenter_.x) * rotationSin_ + triangleCenter_.y;
		}
	}
}
void CMyPaintTriangle::normalize() {
	std::vector<int>place;
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		CPoint point;
		point.x = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x + triangleCoordinates_[SECONDTRIANGLECOORDINATE].x) / 2;
		point.y = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y + triangleCoordinates_[SECONDTRIANGLECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(1);
			continue;
		}
		point.x = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].x + triangleCoordinates_[THIRDTRIANGLECOORDINATE].x) / 2;
		point.y = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].y + triangleCoordinates_[THIRDTRIANGLECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(2);
			continue;
		}
		point.x = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].x + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x) / 2;
		point.y = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].y + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(3);
			continue;
		}
	}
	double length;
	double cos60, sin60;
	cos60 = 1 / 2;
	sin60 = sqrt(3) / 2;
	CPoint trianglePoint[2];
	trianglePoint[0] = triangleCoordinates_[SECONDTRIANGLECOORDINATE];
	trianglePoint[1] = triangleCoordinates_[FIRSTTRIANGLECOORDINATE];
	length = sqrt(pow(triangleCoordinates_[SECONDTRIANGLECOORDINATE].x - triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x, 2) + pow(triangleCoordinates_[SECONDTRIANGLECOORDINATE].y - triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y, 2));
	if (length > sqrt(pow(triangleCoordinates_[THIRDTRIANGLECOORDINATE].x - triangleCoordinates_[SECONDTRIANGLECOORDINATE].x, 2) + pow(triangleCoordinates_[THIRDTRIANGLECOORDINATE].y - triangleCoordinates_[SECONDTRIANGLECOORDINATE].y, 2))) {
		length = sqrt(pow(triangleCoordinates_[THIRDTRIANGLECOORDINATE].x - triangleCoordinates_[SECONDTRIANGLECOORDINATE].x, 2) + pow(triangleCoordinates_[THIRDTRIANGLECOORDINATE].y - triangleCoordinates_[SECONDTRIANGLECOORDINATE].y, 2));
		trianglePoint[FIRSTTRIANGLECOORDINATE] = triangleCoordinates_[THIRDTRIANGLECOORDINATE];
		trianglePoint[SECONDTRIANGLECOORDINATE] = triangleCoordinates_[SECONDTRIANGLECOORDINATE];
	}
	if (length > sqrt(pow(triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x - triangleCoordinates_[THIRDTRIANGLECOORDINATE].x, 2) + pow(triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y - triangleCoordinates_[THIRDTRIANGLECOORDINATE].y, 2))) {
		length = sqrt(pow(triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x - triangleCoordinates_[THIRDTRIANGLECOORDINATE].x, 2) + pow(triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y - triangleCoordinates_[THIRDTRIANGLECOORDINATE].y, 2));
		trianglePoint[FIRSTTRIANGLECOORDINATE] = triangleCoordinates_[FIRSTTRIANGLECOORDINATE];
		trianglePoint[SECONDTRIANGLECOORDINATE] = triangleCoordinates_[THIRDTRIANGLECOORDINATE];
	}
	triangleCoordinates_[FIRSTTRIANGLECOORDINATE] = trianglePoint[FIRSTTRIANGLECOORDINATE];
	triangleCoordinates_[SECONDTRIANGLECOORDINATE] = trianglePoint[SECONDTRIANGLECOORDINATE];
	triangleCoordinates_[THIRDTRIANGLECOORDINATE].x = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].x - triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x) * cos60 - (triangleCoordinates_[SECONDTRIANGLECOORDINATE].y - triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y) * sin60 + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x;
	triangleCoordinates_[THIRDTRIANGLECOORDINATE].y = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].y - triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y) * cos60 + (triangleCoordinates_[SECONDTRIANGLECOORDINATE].x - triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x) * sin60 + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y;
	findCenterCoordinates();
	auto it = connectionsCoordinates_.begin();
	for (auto i = 0; i < place.size(); i++, it++) {
		switch (place[i])
		{
		case 1:
			it->second.x = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x + triangleCoordinates_[SECONDTRIANGLECOORDINATE].x) / 2;
			it->second.y = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y + triangleCoordinates_[SECONDTRIANGLECOORDINATE].y) / 2;
			break;
		case 2:
			it->second.x = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].x + triangleCoordinates_[THIRDTRIANGLECOORDINATE].x) / 2;
			it->second.y = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].y + triangleCoordinates_[THIRDTRIANGLECOORDINATE].y) / 2;
			break;
		case 3:
			it->second.x = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].x + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x) / 2;
			it->second.y = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].y + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y) / 2;
			break;
		default:
			break;
		}
	}
}
bool CMyPaintTriangle::edit(CPoint editPoint, bool isEnd) {
	if (isEnd) {
		switch (clickCount_)
		{
		case clickCountEnum_::secondClick:
			setCoordinates(editPoint, true);
			return false;
			break;
		case clickCountEnum_::thirdClick:
			setCoordinates(editPoint, true);
			return true;
			break;
		default:
			break;
		}
	}
	else {
		switch (clickCount_)
		{
		case clickCountEnum_::secondClick:
			setCoordinates(editPoint, false);
			return false;
			break;
		case clickCountEnum_::thirdClick:
			setCoordinates(editPoint, false);
			return true;
			break;
		default:
			break;
		}
	}
}
void CMyPaintTriangle::makeTempCoordinatesNull() {
	for (size_t i = 0; i < 4; i++) {
		tempTriangle_[i].x = 0;
		tempTriangle_[i].y = 0;
	}
	clickCount_ = clickCountEnum_::secondClick;
}
CPoint CMyPaintTriangle::getCenterCoordinates() {
	return triangleCenter_;
}
CPoint CMyPaintTriangle::findConnectionCoordinates(CPoint centerPoint, int connectionNum) {
	CPoint arrowPoint[4];
	double length;
	int pointNum;
	arrowPoint[0].x = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x + triangleCoordinates_[SECONDTRIANGLECOORDINATE].x) / 2;
	arrowPoint[0].y = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y + triangleCoordinates_[SECONDTRIANGLECOORDINATE].y) / 2;
	arrowPoint[1].x = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].x + triangleCoordinates_[THIRDTRIANGLECOORDINATE].x) / 2;
	arrowPoint[1].y = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].y + triangleCoordinates_[THIRDTRIANGLECOORDINATE].y) / 2;
	arrowPoint[2].x = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].x + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x) / 2;
	arrowPoint[2].y = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].y + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y) / 2;
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
	std::pair<int, CPoint> Pair(connectionNum, arrowPoint[pointNum]);
	connectionsCoordinates_.insert(Pair);
	return arrowPoint[pointNum];
}
int CMyPaintTriangle::findConnectionPlace(int key) {
	CPoint arrowPoint[4];
	arrowPoint[0].x = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x + triangleCoordinates_[SECONDTRIANGLECOORDINATE].x) / 2;
	arrowPoint[0].y = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y + triangleCoordinates_[SECONDTRIANGLECOORDINATE].y) / 2;
	arrowPoint[1].x = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].x + triangleCoordinates_[THIRDTRIANGLECOORDINATE].x) / 2;
	arrowPoint[1].y = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].y + triangleCoordinates_[THIRDTRIANGLECOORDINATE].y) / 2;
	arrowPoint[2].x = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].x + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x) / 2;
	arrowPoint[2].y = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].y + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y) / 2;
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
}
std::vector<int> CMyPaintTriangle::getConnectionID() {
	std::vector<int> nums;
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		nums.push_back(it->first);
	}
	return nums;
}
void CMyPaintTriangle::deleteConnection(int key) {
	auto it = connectionsCoordinates_.find(key);
	connectionsCoordinates_.erase(it);
}

void CMyPaintTriangle::properties(std::vector<int>ids, std::vector<CString> names) {
	std::vector<int>place;
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		CPoint point;
		point.x = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x + triangleCoordinates_[SECONDTRIANGLECOORDINATE].x) / 2;
		point.y = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y + triangleCoordinates_[SECONDTRIANGLECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(1);
			continue;
		}
		point.x = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].x + triangleCoordinates_[THIRDTRIANGLECOORDINATE].x) / 2;
		point.y = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].y + triangleCoordinates_[THIRDTRIANGLECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(2);
			continue;
		}
		point.x = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].x + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x) / 2;
		point.y = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].y + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y) / 2;
		if (it->second == point) {
			place.push_back(3);
			continue;
		}
	}
	CMyPaintPropertiesDialog dlg;
	dlg.getIDs(ids);
	dlg.getNames(names);
	dlg.setName(name_);
	dlg.setID(id_);
	dlg.setFirstPoint(triangleCoordinates_[FIRSTTRIANGLECOORDINATE]);
	dlg.setSecondPoint(triangleCoordinates_[SECONDTRIANGLECOORDINATE]);
	dlg.setThirdPoint(triangleCoordinates_[THIRDTRIANGLECOORDINATE]);
	dlg.setWidth(penWidth_);
	dlg.setPenStyle(penStyle_);
	dlg.setBrushStyle(brushStyle_);
	dlg.setPenColor(penColor_);
	dlg.setBrushColor(brushColor_);
	dlg.DoModal();
	name_ = dlg.getName();
	id_ = dlg.getID();
	triangleCoordinates_[FIRSTTRIANGLECOORDINATE] = dlg.getFirstPoint();
	triangleCoordinates_[SECONDTRIANGLECOORDINATE] = dlg.getSecondPoint();
	triangleCoordinates_[THIRDTRIANGLECOORDINATE] = dlg.getThirdPoint();
	penWidth_ = dlg.getWidth();
	penStyle_ = dlg.getPenStyle();
	brushStyle_ = dlg.getBrushStyle();
	penColor_ = dlg.getPenColor();
	brushColor_ = dlg.getBrushColor();
	findCenterCoordinates();
	auto it = connectionsCoordinates_.begin();
	for (auto i = 0; i < place.size(); i++, it++) {
		switch (place[i])
		{
		case 1:
			it->second.x = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x + triangleCoordinates_[SECONDTRIANGLECOORDINATE].x) / 2;
			it->second.y = (triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y + triangleCoordinates_[SECONDTRIANGLECOORDINATE].y) / 2;
			break;
		case 2:
			it->second.x = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].x + triangleCoordinates_[THIRDTRIANGLECOORDINATE].x) / 2;
			it->second.y = (triangleCoordinates_[SECONDTRIANGLECOORDINATE].y + triangleCoordinates_[THIRDTRIANGLECOORDINATE].y) / 2;
			break;
		case 3:
			it->second.x = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].x + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x) / 2;
			it->second.y = (triangleCoordinates_[THIRDTRIANGLECOORDINATE].y + triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y) / 2;
			break;
		default:
			break;
		}
	}
}

CPoint CMyPaintTriangle::getFirstCoordinate() {
	return triangleCoordinates_[FIRSTTRIANGLECOORDINATE];
}

CPoint CMyPaintTriangle::getSecondCoordinate() {
	return triangleCoordinates_[SECONDTRIANGLECOORDINATE];
}

CPoint CMyPaintTriangle::getThirdCoordinate() {
	return triangleCoordinates_[THIRDTRIANGLECOORDINATE];
}

int CMyPaintTriangle::getFigureType() {
	return 3;
}

void CMyPaintTriangle::setFirstCoordinate(CPoint point) {
	triangleCoordinates_[FIRSTTRIANGLECOORDINATE] = point;
}

void CMyPaintTriangle::setSecondCoordinate(CPoint point) {
	triangleCoordinates_[SECONDTRIANGLECOORDINATE] = point;
}

void CMyPaintTriangle::setThirdCoordinate(CPoint point) {
	triangleCoordinates_[THIRDTRIANGLECOORDINATE] = point;
}
void CMyPaintTriangle::changeOtherCoordinates() {

}

void CMyPaintTriangle::draw(HDC hdc) {
	CPen Pen(penStyle_, penWidth_, penColor_);
	SelectObject(hdc, Pen);
	if (brushStyle_ == 0) {
		HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		SelectObject(hdc, hBrush);
		Polygon(hdc, triangleCoordinates_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 1) {
		HBRUSH hBrush = CreateSolidBrush(brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, triangleCoordinates_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 2) {
		CBrush hBrush(HS_BDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, triangleCoordinates_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 3) {
		CBrush hBrush(HS_CROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, triangleCoordinates_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 4) {
		CBrush hBrush(HS_DIAGCROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, triangleCoordinates_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 5) {
		CBrush hBrush(HS_FDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, triangleCoordinates_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 6) {
		CBrush hBrush(HS_HORIZONTAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, triangleCoordinates_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 7) {
		CBrush hBrush(HS_VERTICAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, triangleCoordinates_, 3);
		DeleteObject(hBrush);
	}
	DeleteObject(Pen);
}

void CMyPaintTriangle::tempDraw(HDC hdc) {
	CPen Pen(penStyle_, penWidth_, penColor_);
	SelectObject(hdc, Pen);
	if (brushStyle_ == 0) {
		HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempTriangle_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 1) {
		HBRUSH hBrush = CreateSolidBrush(brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempTriangle_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 2) {
		CBrush hBrush(HS_BDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempTriangle_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 3) {
		CBrush hBrush(HS_CROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempTriangle_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 4) {
		CBrush hBrush(HS_DIAGCROSS, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempTriangle_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 5) {
		CBrush hBrush(HS_FDIAGONAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempTriangle_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 6) {
		CBrush hBrush(HS_HORIZONTAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempTriangle_, 3);
		DeleteObject(hBrush);
	}
	if (brushStyle_ == 7) {
		CBrush hBrush(HS_VERTICAL, brushColor_);
		SelectObject(hdc, hBrush);
		Polygon(hdc, tempTriangle_, 3);
		DeleteObject(hBrush);
	}
	DeleteObject(Pen);
}

void CMyPaintTriangle::select(HDC hdc) {
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
	Polygon(hdc, triangleCoordinates_, 3);
	DeleteObject(hBrush);
	DeleteObject(Pen);
}
void CMyPaintTriangle::scrollFigure(CPoint point) {
	for (auto i = 0; i < 3; i++) {
		triangleCoordinates_[i].x += point.x;
		triangleCoordinates_[i].y += point.y;
	}
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		it->second.x += point.x;
		it->second.y += point.y;
	}
	findCenterCoordinates();
}

std::vector<CPoint> CMyPaintTriangle::getLeftCoordinate() {
	CPoint temp;
	temp = triangleCoordinates_[FIRSTTRIANGLECOORDINATE];
	for (auto i = 1; i < 3; i++) {
		if (triangleCoordinates_[i].x < temp.x) {
			temp = triangleCoordinates_[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<CPoint> CMyPaintTriangle::getBottomCoordinate() {
	CPoint temp;
	temp = triangleCoordinates_[FIRSTTRIANGLECOORDINATE];
	for (auto i = 1; i < 3; i++) {
		if (triangleCoordinates_[i].y < temp.y) {
			temp = triangleCoordinates_[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<CPoint> CMyPaintTriangle::getRightCoordinate() {
	CPoint temp;
	temp = triangleCoordinates_[FIRSTTRIANGLECOORDINATE];
	for (auto i = 1; i < 3; i++) {
		if (triangleCoordinates_[i].x > temp.x) {
			temp = triangleCoordinates_[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<CPoint> CMyPaintTriangle::getTopCoordinate() {
	CPoint temp;
	temp = triangleCoordinates_[FIRSTTRIANGLECOORDINATE];
	for (auto i = 1; i < 3; i++) {
		if (triangleCoordinates_[i].y > temp.y) {
			temp = triangleCoordinates_[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<LONG> CMyPaintTriangle::getMaxMinX() {
	std::vector<LONG>MaxMin;
	MaxMin.push_back(triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x);
	MaxMin.push_back(triangleCoordinates_[FIRSTTRIANGLECOORDINATE].x);
	for (auto i = 1; i < 3; i++) {
		if (triangleCoordinates_[i].x > MaxMin[0]) {
			MaxMin[0] = triangleCoordinates_[i].x;
		}
		if (triangleCoordinates_[i].x < MaxMin[1]) {
			MaxMin[1] = triangleCoordinates_[i].x;
		}
	}
	return MaxMin;
}

std::vector<LONG> CMyPaintTriangle::getMaxMinY() {
	std::vector<LONG>MaxMin;
	MaxMin.push_back(triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y);
	MaxMin.push_back(triangleCoordinates_[FIRSTTRIANGLECOORDINATE].y);
	for (auto i = 1; i < 3; i++) {
		if (triangleCoordinates_[i].y > MaxMin[0]) {
			MaxMin[0] = triangleCoordinates_[i].y;
		}
		if (triangleCoordinates_[i].y < MaxMin[1]) {
			MaxMin[1] = triangleCoordinates_[i].y;
		}
	}
	return MaxMin;
}