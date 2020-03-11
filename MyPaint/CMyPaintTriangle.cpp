#include "pch.h"
#include "CMyPaintTriangle.h"
CMyPaintTriangle::CMyPaintTriangle() : triangleCoordinates_(), CMyPaintFigure()
{
	clickCount_ = clickCountEnum_::firstClick;
}
CMyPaintTriangle::CMyPaintTriangle(unsigned int id, CString name, int penWidth, COLORREF penColor, int penStyle, COLORREF brushColor, int brushStyle, CPoint FirstPoint, CPoint SecondPoint, CPoint ThirdPoint) : CMyPaintFigure(id, name, penWidth, penColor, penStyle, brushColor, brushStyle)
{
	triangleCoordinates_[0] = FirstPoint;
	triangleCoordinates_[1] = SecondPoint;
	triangleCoordinates_[2] = ThirdPoint;
	findCenterCoordinates();
	clickCount_ = clickCountEnum_::firstClick;
}
void CMyPaintTriangle::findCenterCoordinates() {
	triangleCenter_.x = (triangleCoordinates_[0].x + triangleCoordinates_[1].x + triangleCoordinates_[2].x) / 3;
	triangleCenter_.y = (triangleCoordinates_[0].y + triangleCoordinates_[1].y + triangleCoordinates_[2].y) / 3;
}
bool CMyPaintTriangle::ifThisFigure(CPoint point) {
	int SummOne, SummTwo, SummThree;
	SummOne = (triangleCoordinates_[0].x - point.x) * (triangleCoordinates_[1].y - triangleCoordinates_[0].y) - (triangleCoordinates_[1].x - triangleCoordinates_[0].x) * (triangleCoordinates_[0].y - point.y);
	SummTwo = (triangleCoordinates_[1].x - point.x) * (triangleCoordinates_[2].y - triangleCoordinates_[1].y) - (triangleCoordinates_[2].x - triangleCoordinates_[1].x) * (triangleCoordinates_[1].y - point.y);
	SummThree = (triangleCoordinates_[2].x - point.x) * (triangleCoordinates_[0].y - triangleCoordinates_[2].y) - (triangleCoordinates_[0].x - triangleCoordinates_[2].x) * (triangleCoordinates_[2].y - point.y);
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
		triangleCoordinates_[0] = point;
		findCenterCoordinates();
		if (isClickEnd) {
			clickCount_ = clickCountEnum_::secondClick;
		}
		break;
	case clickCountEnum_::secondClick:
		triangleCoordinates_[1] = point;
		findCenterCoordinates();
		if (isClickEnd) {
			clickCount_ = clickCountEnum_::thirdClick;
		}
		break;
	case clickCountEnum_::thirdClick:
		triangleCoordinates_[2] = point;
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
		point.x = (triangleCoordinates_[0].x + triangleCoordinates_[1].x) / 2;
		point.y = (triangleCoordinates_[0].y + triangleCoordinates_[1].y) / 2;
		if (it->second == point) {
			place.push_back(1);
			continue;
		}
		point.x = (triangleCoordinates_[1].x + triangleCoordinates_[2].x) / 2;
		point.y = (triangleCoordinates_[1].y + triangleCoordinates_[2].y) / 2;
		if (it->second == point) {
			place.push_back(2);
			continue;
		}
		point.x = (triangleCoordinates_[2].x + triangleCoordinates_[3].x) / 2;
		point.y = (triangleCoordinates_[2].y + triangleCoordinates_[3].y) / 2;
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
	trianglePoint[0] = triangleCoordinates_[1];
	trianglePoint[1] = triangleCoordinates_[0];
	length = sqrt(pow(triangleCoordinates_[1].x - triangleCoordinates_[0].x, 2) + pow(triangleCoordinates_[1].y - triangleCoordinates_[0].y, 2));
	if (length > sqrt(pow(triangleCoordinates_[2].x - triangleCoordinates_[1].x, 2) + pow(triangleCoordinates_[2].y - triangleCoordinates_[1].y, 2))) {
		length = sqrt(pow(triangleCoordinates_[2].x - triangleCoordinates_[1].x, 2) + pow(triangleCoordinates_[2].y - triangleCoordinates_[1].y, 2));
		trianglePoint[0] = triangleCoordinates_[2];
		trianglePoint[1] = triangleCoordinates_[1];
	}
	if (length > sqrt(pow(triangleCoordinates_[0].x - triangleCoordinates_[2].x, 2) + pow(triangleCoordinates_[0].y - triangleCoordinates_[2].y, 2))) {
		length = sqrt(pow(triangleCoordinates_[0].x - triangleCoordinates_[2].x, 2) + pow(triangleCoordinates_[0].y - triangleCoordinates_[2].y, 2));
		trianglePoint[0] = triangleCoordinates_[0];
		trianglePoint[1] = triangleCoordinates_[2];
	}
	triangleCoordinates_[0] = trianglePoint[0];
	triangleCoordinates_[1] = trianglePoint[1];
	triangleCoordinates_[2].x = (triangleCoordinates_[1].x - triangleCoordinates_[0].x) * cos60 - (triangleCoordinates_[1].y - triangleCoordinates_[0].y) * sin60 + triangleCoordinates_[0].x;
	triangleCoordinates_[2].y = (triangleCoordinates_[1].y - triangleCoordinates_[0].y) * cos60 + (triangleCoordinates_[1].x - triangleCoordinates_[0].x) * sin60 + triangleCoordinates_[0].y;
	findCenterCoordinates();
	auto it = connectionsCoordinates_.begin();
	for (auto i = 0; i < place.size(); i++, it++) {
		switch (place[i])
		{
		case 1:
			it->second.x = (triangleCoordinates_[0].x + triangleCoordinates_[1].x) / 2;
			it->second.y = (triangleCoordinates_[0].y + triangleCoordinates_[1].y) / 2;
			break;
		case 2:
			it->second.x = (triangleCoordinates_[1].x + triangleCoordinates_[2].x) / 2;
			it->second.y = (triangleCoordinates_[1].y + triangleCoordinates_[2].y) / 2;
			break;
		case 3:
			it->second.x = (triangleCoordinates_[2].x + triangleCoordinates_[3].x) / 2;
			it->second.y = (triangleCoordinates_[2].y + triangleCoordinates_[3].y) / 2;
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
	arrowPoint[0].x = (triangleCoordinates_[0].x + triangleCoordinates_[1].x) / 2;
	arrowPoint[0].y = (triangleCoordinates_[0].y + triangleCoordinates_[1].y) / 2;
	arrowPoint[1].x = (triangleCoordinates_[1].x + triangleCoordinates_[2].x) / 2;
	arrowPoint[1].y = (triangleCoordinates_[1].y + triangleCoordinates_[2].y) / 2;
	arrowPoint[2].x = (triangleCoordinates_[2].x + triangleCoordinates_[0].x) / 2;
	arrowPoint[2].y = (triangleCoordinates_[2].y + triangleCoordinates_[0].y) / 2;
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
	arrowPoint[0].x = (triangleCoordinates_[0].x + triangleCoordinates_[1].x) / 2;
	arrowPoint[0].y = (triangleCoordinates_[0].y + triangleCoordinates_[1].y) / 2;
	arrowPoint[1].x = (triangleCoordinates_[1].x + triangleCoordinates_[2].x) / 2;
	arrowPoint[1].y = (triangleCoordinates_[1].y + triangleCoordinates_[2].y) / 2;
	arrowPoint[2].x = (triangleCoordinates_[2].x + triangleCoordinates_[0].x) / 2;
	arrowPoint[2].y = (triangleCoordinates_[2].y + triangleCoordinates_[0].y) / 2;
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
		point.x = (triangleCoordinates_[0].x + triangleCoordinates_[1].x) / 2;
		point.y = (triangleCoordinates_[0].y + triangleCoordinates_[1].y) / 2;
		if (it->second == point) {
			place.push_back(1);
			continue;
		}
		point.x = (triangleCoordinates_[1].x + triangleCoordinates_[2].x) / 2;
		point.y = (triangleCoordinates_[1].y + triangleCoordinates_[2].y) / 2;
		if (it->second == point) {
			place.push_back(2);
			continue;
		}
		point.x = (triangleCoordinates_[2].x + triangleCoordinates_[3].x) / 2;
		point.y = (triangleCoordinates_[2].y + triangleCoordinates_[3].y) / 2;
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
	dlg.setFirstPoint(triangleCoordinates_[0]);
	dlg.setSecondPoint(triangleCoordinates_[1]);
	dlg.setThirdPoint(triangleCoordinates_[2]);
	dlg.setWidth(penWidth_);
	dlg.setPenStyle(penStyle_);
	dlg.setBrushStyle(brushStyle_);
	dlg.setPenColor(penColor_);
	dlg.setBrushColor(brushColor_);
	dlg.DoModal();
	name_ = dlg.getName();
	id_ = dlg.getID();
	triangleCoordinates_[0] = dlg.getFirstPoint();
	triangleCoordinates_[1] = dlg.getSecondPoint();
	triangleCoordinates_[2] = dlg.getThirdPoint();
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
			it->second.x = (triangleCoordinates_[0].x + triangleCoordinates_[1].x) / 2;
			it->second.y = (triangleCoordinates_[0].y + triangleCoordinates_[1].y) / 2;
			break;
		case 2:
			it->second.x = (triangleCoordinates_[1].x + triangleCoordinates_[2].x) / 2;
			it->second.y = (triangleCoordinates_[1].y + triangleCoordinates_[2].y) / 2;
			break;
		case 3:
			it->second.x = (triangleCoordinates_[2].x + triangleCoordinates_[3].x) / 2;
			it->second.y = (triangleCoordinates_[2].y + triangleCoordinates_[3].y) / 2;
			break;
		default:
			break;
		}
	}
}

CPoint CMyPaintTriangle::getFirstCoordinate() {
	return triangleCoordinates_[0];
}

CPoint CMyPaintTriangle::getSecondCoordinate() {
	return triangleCoordinates_[1];
}

CPoint CMyPaintTriangle::getThirdCoordinate() {
	return triangleCoordinates_[2];
}

int CMyPaintTriangle::getFigureType() {
	return 3;
}

void CMyPaintTriangle::setFirstCoordinate(CPoint point) {
	triangleCoordinates_[0] = point;
}

void CMyPaintTriangle::setSecondCoordinate(CPoint point) {
	triangleCoordinates_[1] = point;
}

void CMyPaintTriangle::setThirdCoordinate(CPoint point) {
	triangleCoordinates_[2] = point;
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