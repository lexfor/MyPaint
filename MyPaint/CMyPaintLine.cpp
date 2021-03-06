#include "pch.h"
#include "CMyPaintLine.h"

#define FIRSTLINECOORDINATE 0
#define SECONDLINECOORDINATE 1
CMyPaintLine::CMyPaintLine() : CMyPaintFigure()
{}

CMyPaintLine::CMyPaintLine(unsigned int id, CString name, int penWidth, COLORREF penColor, int penStyle, CPoint firstPoint, CPoint secondPoint) : CMyPaintFigure(id, name, penWidth, penColor, penStyle)
{
	lineCoordinates_[FIRSTLINECOORDINATE] = firstPoint;
	lineCoordinates_[SECONDLINECOORDINATE] = secondPoint;
	findCenterCoordinates();
}

void CMyPaintLine::findCenterCoordinates() {
	lineCenter_.x = (lineCoordinates_[FIRSTLINECOORDINATE].x + lineCoordinates_[SECONDLINECOORDINATE].x) / 2;
	lineCenter_.y = (lineCoordinates_[FIRSTLINECOORDINATE].y + lineCoordinates_[SECONDLINECOORDINATE].y) / 2;
}

void CMyPaintLine::setCoordinates(CPoint point, bool isClickEnd) {
	lineCoordinates_[SECONDLINECOORDINATE] = point;
	findCenterCoordinates();
}

bool CMyPaintLine::ifThisFigure(CPoint point) {
	CRect rect;
	if (lineCoordinates_[FIRSTLINECOORDINATE].x > lineCoordinates_[SECONDLINECOORDINATE].x) {
		rect.right = lineCoordinates_[FIRSTLINECOORDINATE].x + 10;
		rect.left = lineCoordinates_[SECONDLINECOORDINATE].x - 10;
	}
	else {
		rect.right = lineCoordinates_[SECONDLINECOORDINATE].x + 10;
		rect.left = lineCoordinates_[FIRSTLINECOORDINATE].x - 10;
	}
	if (lineCoordinates_[FIRSTLINECOORDINATE].y > lineCoordinates_[SECONDLINECOORDINATE].y) {
		rect.bottom = lineCoordinates_[FIRSTLINECOORDINATE].y + 10;
		rect.top = lineCoordinates_[SECONDLINECOORDINATE].y - 10;
	}
	else {
		rect.bottom = lineCoordinates_[SECONDLINECOORDINATE].y + 10;
		rect.top = lineCoordinates_[FIRSTLINECOORDINATE].y - 10;
	}
	if (point.x > rect.left&& point.x < rect.right && point.y < rect.bottom && point.y > rect.top) {
		return true;
	}
	return false;
}

void CMyPaintLine::move(CPoint* movePoint) {
	CPoint diff;
	diff.x = movePoint[1].x - movePoint[0].x;
	diff.y = movePoint[1].y - movePoint[0].y;
	for (auto& i : lineCoordinates_) {
		i.x += diff.x;
		i.y += diff.y;
	}
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		it->second.x += diff.x;
		it->second.y += diff.y;
	}
	findCenterCoordinates();
}

void CMyPaintLine::rotate(CPoint* rotatePoint, bool realCoordinates) {
	double resultX, resultY;
	double radius;
	double Rcos, Rsin;
	resultX = rotatePoint[1].x - rotatePoint[0].x;
	resultY = rotatePoint[1].y - rotatePoint[0].y;
	radius = sqrt(pow(resultX, 2) + pow(resultY, 2));
	Rcos = resultX / radius;
	Rsin = resultY / radius;
	for (size_t i = 0; i < 2; i++) {
		tempLineCoordinates_[i] = lineCoordinates_[i];
	}
	if (realCoordinates) {
		for (size_t i = 0; i < 2; i++) {
			lineCoordinates_[i].x = (tempLineCoordinates_[i].x - lineCenter_.x) * Rcos - (tempLineCoordinates_[i].y - lineCenter_.y) * Rsin + lineCenter_.x;
			lineCoordinates_[i].y = (tempLineCoordinates_[i].y - lineCenter_.y) * Rcos + (tempLineCoordinates_[i].x - lineCenter_.x) * Rsin + lineCenter_.y;
		}
	}
	else {
		for (size_t i = 0; i < 2; i++) {
			tempLineCoordinates_[i].x = (lineCoordinates_[i].x - lineCenter_.x) * Rcos - (lineCoordinates_[i].y - lineCenter_.y) * Rsin + lineCenter_.x;
			tempLineCoordinates_[i].y = (lineCoordinates_[i].y - lineCenter_.y) * Rcos + (lineCoordinates_[i].x - lineCenter_.x) * Rsin + lineCenter_.y;
		}
	}
	if (realCoordinates) {
		rotationCos_ = Rcos;
		rotationSin_ = Rsin;
		for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
			CPoint tempPoint;
			tempPoint = it->second;
			it->second.x = (tempPoint.x - lineCenter_.x) * rotationCos_ - (tempPoint.y - lineCenter_.y) * rotationSin_ + lineCenter_.x;
			it->second.y = (tempPoint.y - lineCenter_.y) * rotationCos_ + (tempPoint.x - lineCenter_.x) * rotationSin_ + lineCenter_.y;
		}
	}
}

void CMyPaintLine::normalize() {
	CPoint diff;
	diff.x = abs(lineCoordinates_[FIRSTLINECOORDINATE].x - lineCenter_.x);
	diff.y = abs(lineCoordinates_[FIRSTLINECOORDINATE].x - lineCenter_.y);
	if (diff.x > abs(lineCoordinates_[SECONDLINECOORDINATE].x - lineCenter_.x)) {
		diff.x = abs(lineCoordinates_[SECONDLINECOORDINATE].x - lineCenter_.x);
	}
	if (diff.y > abs(lineCoordinates_[SECONDLINECOORDINATE].y - lineCenter_.y)) {
		diff.y = abs(lineCoordinates_[SECONDLINECOORDINATE].y - lineCenter_.y);
	}
	lineCoordinates_[FIRSTLINECOORDINATE].x = lineCenter_.x - diff.x;
	lineCoordinates_[SECONDLINECOORDINATE].x = lineCenter_.x + diff.x;
	lineCoordinates_[FIRSTLINECOORDINATE].y = lineCenter_.y - diff.y;
	lineCoordinates_[SECONDLINECOORDINATE].y = lineCenter_.y + diff.y;
	findCenterCoordinates();
	if (rotationCos_ != 0 || rotationSin_ != 0) {
		for (size_t i = 0; i < 2; i++) {
			tempLineCoordinates_[i] = lineCoordinates_[i];
		}
		for (size_t i = 0; i < 2; i++) {
			lineCoordinates_[i].x = (tempLineCoordinates_[i].x - lineCenter_.x) * rotationCos_ - (tempLineCoordinates_[i].y - lineCenter_.y) * rotationSin_ + lineCenter_.x;
			lineCoordinates_[i].y = (tempLineCoordinates_[i].y - lineCenter_.y) * rotationCos_ + (tempLineCoordinates_[i].x - lineCenter_.x) * rotationSin_ + lineCenter_.y;
		}
	}
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		it->second = lineCenter_;
	}
}
void CMyPaintLine::changeOtherCoordinates() {

}

bool CMyPaintLine::edit(CPoint editPoint, bool isEnd) {
	setCoordinates(editPoint, true);
	return true;
}

void CMyPaintLine::makeTempCoordinatesNull() {
	for (auto i = 0; i < 2; i++) {
		tempLineCoordinates_[i].x = 0;
		tempLineCoordinates_[i].y = 0;
	}
}

CPoint CMyPaintLine::getCenterCoordinates() {
	return lineCenter_;
}

void CMyPaintLine::properties(std::vector<int>ids, std::vector<CString> names) {
	CMyPaintPropertiesDialog dlg;
	dlg.getIDs(ids);
	dlg.getNames(names);
	dlg.setName(name_);
	dlg.setID(id_);
	dlg.setFirstPoint(lineCoordinates_[FIRSTLINECOORDINATE]);
	dlg.setSecondPoint(lineCoordinates_[SECONDLINECOORDINATE]);
	dlg.setThirdPoint(lineCoordinates_[SECONDLINECOORDINATE]);
	dlg.setWidth(penWidth_);
	dlg.setPenStyle(penStyle_);
	dlg.setBrushStyle(brushStyle_);
	dlg.setPenColor(penColor_);
	dlg.setBrushColor(brushColor_);
	dlg.DoModal();
	name_ = dlg.getName();
	id_ = dlg.getID();
	lineCoordinates_[FIRSTLINECOORDINATE] = dlg.getFirstPoint();
	lineCoordinates_[SECONDLINECOORDINATE] = dlg.getSecondPoint();
	penWidth_ = dlg.getWidth();
	penStyle_ = dlg.getPenStyle();
	brushStyle_ = dlg.getBrushStyle();
	penColor_ = dlg.getPenColor();
	brushColor_ = dlg.getBrushColor();
	findCenterCoordinates();
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		it->second = lineCenter_;
	}
}

CPoint CMyPaintLine::getFirstCoordinate() {
	return lineCoordinates_[FIRSTLINECOORDINATE];
}

CPoint CMyPaintLine::getSecondCoordinate() {
	return lineCoordinates_[SECONDLINECOORDINATE];
}

CPoint CMyPaintLine::getThirdCoordinate() {
	return lineCoordinates_[SECONDLINECOORDINATE];
}

int CMyPaintLine::getFigureType() {
	return 4;
}

void CMyPaintLine::setFirstCoordinate(CPoint point) {
	lineCoordinates_[FIRSTLINECOORDINATE] = point;
}

void CMyPaintLine::setSecondCoordinate(CPoint point) {
	lineCoordinates_[SECONDLINECOORDINATE] = point;
}

void CMyPaintLine::setThirdCoordinate(CPoint point) {
}

CPoint CMyPaintLine::findConnectionCoordinates(CPoint centerPoint, int connectionNum) {
	std::pair<int, CPoint> Pair(connectionNum, lineCenter_);
	connectionsCoordinates_.insert(Pair);
	return lineCenter_;
}
int CMyPaintLine::findConnectionPlace(int key) {
	return 1;
}
std::vector<int> CMyPaintLine::getConnectionID() {
	std::vector<int> nums;
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		nums.push_back(it->first);
	}
	return nums;
}
void CMyPaintLine::deleteConnection(int key) {
	auto it = connectionsCoordinates_.find(key);
	connectionsCoordinates_.erase(it);
}

void CMyPaintLine::tempDraw(HDC hdc) {
	CPen Pen(penStyle_, penWidth_, penColor_);
	SelectObject(hdc, Pen);
	Polygon(hdc, tempLineCoordinates_, 2);
	DeleteObject(Pen);
}

void CMyPaintLine::draw(HDC hdc) {
	CPen Pen(penStyle_, penWidth_, penColor_);
	SelectObject(hdc, Pen);
	Polygon(hdc, lineCoordinates_, 2);
	DeleteObject(Pen);
}

void CMyPaintLine::select(HDC hdc) {
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
	Polygon(hdc, lineCoordinates_, 2);
	DeleteObject(hBrush);
	DeleteObject(Pen);
}
void CMyPaintLine::scrollFigure(CPoint point) {
	for (auto i = 0; i < 2; i++) {
		lineCoordinates_[i].x += point.x;
		lineCoordinates_[i].y += point.y;
	}
	for (auto it = connectionsCoordinates_.begin(); it != connectionsCoordinates_.end(); it++) {
		it->second.x += point.x;
		it->second.y += point.y;
	}
	findCenterCoordinates();
}

std::vector<CPoint> CMyPaintLine::getLeftCoordinate() {
	CPoint temp;
	temp = lineCoordinates_[FIRSTLINECOORDINATE];
	for (auto i = 1; i < 2; i++) {
		if (lineCoordinates_[i].x < temp.x) {
			temp = lineCoordinates_[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<CPoint> CMyPaintLine::getBottomCoordinate() {
	CPoint temp;
	temp = lineCoordinates_[FIRSTLINECOORDINATE];
	for (auto i = 1; i < 2; i++) {
		if (lineCoordinates_[i].y < temp.y) {
			temp = lineCoordinates_[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<CPoint> CMyPaintLine::getRightCoordinate() {
	CPoint temp;
	temp = lineCoordinates_[FIRSTLINECOORDINATE];
	for (auto i = 1; i < 2; i++) {
		if (lineCoordinates_[i].x > temp.x) {
			temp = lineCoordinates_[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<CPoint> CMyPaintLine::getTopCoordinate() {
	CPoint temp;
	temp = lineCoordinates_[FIRSTLINECOORDINATE];
	for (auto i = 1; i < 2; i++) {
		if (lineCoordinates_[i].y > temp.y) {
			temp = lineCoordinates_[i];
		}
	}
	std::vector<CPoint> rez;
	rez.push_back(temp);
	return rez;
}

std::vector<LONG> CMyPaintLine::getMaxMinX() {
	std::vector<LONG>MaxMin;
	MaxMin.push_back(lineCoordinates_[FIRSTLINECOORDINATE].x);
	MaxMin.push_back(lineCoordinates_[FIRSTLINECOORDINATE].x);
	for (auto i = 1; i < 2; i++) {
		if (lineCoordinates_[i].x > MaxMin[0]) {
			MaxMin[0] = lineCoordinates_[i].x;
		}
		if (lineCoordinates_[i].x < MaxMin[1]) {
			MaxMin[1] = lineCoordinates_[i].x;
		}
	}
	return MaxMin;
}

std::vector<LONG> CMyPaintLine::getMaxMinY() {
	std::vector<LONG>MaxMin;
	MaxMin.push_back(lineCoordinates_[FIRSTLINECOORDINATE].y);
	MaxMin.push_back(lineCoordinates_[FIRSTLINECOORDINATE].y);
	for (auto i = 1; i < 2; i++) {
		if (lineCoordinates_[i].y > MaxMin[0]) {
			MaxMin[0] = lineCoordinates_[i].y;
		}
		if (lineCoordinates_[i].y < MaxMin[1]) {
			MaxMin[1] = lineCoordinates_[i].y;
		}
	}
	return MaxMin;
}