#include "pch.h"
#include "CMyPaintLine.h"
CMyPaintLine::CMyPaintLine() : CMyPaintFigure()
{}

CMyPaintLine::CMyPaintLine(unsigned int id, CString name, int penWidth, COLORREF penColor, int penStyle,CPoint coordinates[2]) : CMyPaintFigure(id,name,penWidth,penColor,penStyle)
{
	lineCoordinates_[0] = coordinates[0];
	lineCoordinates_[1] = coordinates[1];
	findCenterCoordinates();
}

void CMyPaintLine::findCenterCoordinates() {
	lineCenter_.x = (lineCoordinates_[0].x + lineCoordinates_[1].x) / 2;
	lineCenter_.y = (lineCoordinates_[0].y + lineCoordinates_[1].y) / 2;
}

void CMyPaintLine::draw(CClientDC& dc, HWND& m_HWND) {
	CPen Pen(penStyle_, penWidth_, penColor_);
	CPen* oldPen = (CPen*)dc.SelectObject(&Pen);
	dc.MoveTo(lineCoordinates_[0]);
	dc.LineTo(lineCoordinates_[1]);
}

void CMyPaintLine::setCoordinates(CPoint point, bool isClickEnd) {
	lineCoordinates_[1] = point;
	findCenterCoordinates();
}

CRect CMyPaintLine::getCoordinates() {
	CRect rect;
	if (lineCoordinates_[0].x > lineCoordinates_[1].x) {
		rect.right = lineCoordinates_[0].x + 10;
		rect.left = lineCoordinates_[1].x + 10;
	}
	else {
		rect.right = lineCoordinates_[1].x + 10;
		rect.left = lineCoordinates_[0].x + 10;
	}
	if (lineCoordinates_[0].y > lineCoordinates_[1].y) {
		rect.bottom = lineCoordinates_[0].y + 10;
		rect.top = lineCoordinates_[1].y + 10;
	}
	else {
		rect.bottom = lineCoordinates_[1].y + 10;
		rect.top = lineCoordinates_[0].y + 10;
	}
	return rect;
}

bool CMyPaintLine::ifThisFigure(CPoint point) {
	CRect rect = getCoordinates();
	if (point.x > rect.left&& point.x < rect.right && point.y > rect.bottom&& point.y < rect.top) {
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
	}
}

void CMyPaintLine::normalize() {
	CPoint diff;
	diff.x = abs(lineCoordinates_[0].x - lineCenter_.x);
	diff.y = abs(lineCoordinates_[0].x - lineCenter_.y);
	if (diff.x > abs(lineCoordinates_[1].x - lineCenter_.x)) {
		diff.x = abs(lineCoordinates_[1].x - lineCenter_.x);
	}
	if (diff.y > abs(lineCoordinates_[1].y - lineCenter_.y)) {
		diff.y = abs(lineCoordinates_[1].y - lineCenter_.y);
	}
	lineCoordinates_[0].x = lineCenter_.x - diff.x;
	lineCoordinates_[1].x = lineCenter_.x + diff.x;
	lineCoordinates_[0].y = lineCenter_.y - diff.y;
	lineCoordinates_[1].y = lineCenter_.y + diff.y;
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
}

void CMyPaintLine::drawTempFigure(CClientDC& dc, HWND& m_HWND) {
	CPen Pen(penStyle_, penWidth_, penColor_);
	CPen* oldPen = (CPen*)dc.SelectObject(&Pen);
	dc.MoveTo(tempLineCoordinates_[0]);
	dc.LineTo(tempLineCoordinates_[1]);
}

CRect CMyPaintLine::getTempCoordinates() {
	CRect rect;
	if (tempLineCoordinates_[0].x > tempLineCoordinates_[1].x) {
		rect.right = tempLineCoordinates_[0].x + 10;
		rect.left = tempLineCoordinates_[1].x + 10;
	}
	else {
		rect.right = tempLineCoordinates_[1].x + 10;
		rect.left = tempLineCoordinates_[0].x + 10;
	}
	if (tempLineCoordinates_[0].y > tempLineCoordinates_[1].y) {
		rect.bottom = tempLineCoordinates_[0].y + 10;
		rect.top = tempLineCoordinates_[1].y + 10;
	}
	else {
		rect.bottom = tempLineCoordinates_[1].y + 10;
		rect.top = tempLineCoordinates_[0].y + 10;
	}
	return rect;
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
	dlg.setFirstPoint(lineCoordinates_[0]);
	dlg.setSecondPoint(lineCoordinates_[1]);
	dlg.setThirdPoint(lineCoordinates_[1]);
	dlg.setWidth(penWidth_);
	dlg.setPenStyle(penStyle_);
	dlg.setBrushStyle(brushStyle_);
	dlg.setPenColor(penColor_);
	dlg.setBrushColor(brushColor_);
	dlg.DoModal();
	name_ = dlg.getName();
	id_ = dlg.getID();
	lineCoordinates_[0] = dlg.getFirstPoint();
	lineCoordinates_[1] = dlg.getSecondPoint();
	penWidth_ = dlg.getWidth();
	penStyle_ = dlg.getPenStyle();
	brushStyle_ = dlg.getBrushStyle();
	penColor_ = dlg.getPenColor();
	brushColor_ = dlg.getBrushColor();
}

CPoint CMyPaintLine::getFirstCoordinate() {
	return lineCoordinates_[0];
}

CPoint CMyPaintLine::getSecondCoordinate() {
	return lineCoordinates_[1];
}

CPoint CMyPaintLine::getThirdCoordinate() {
	return lineCoordinates_[1];
}

int CMyPaintLine::getFigureType() {
	return 1;
}

void CMyPaintLine::setFirstCoordinate(CPoint point) {
	lineCoordinates_[0] = point;
}

void CMyPaintLine::setSecondCoordinate(CPoint point) {
	lineCoordinates_[1] = point;
}

void CMyPaintLine::setThirdCoordinate(CPoint point) {
}