// CMyPaintPropertiesDialog.cpp: файл реализации
//

#include "pch.h"
#include "MyPaint.h"
#include <afxdlgs.h>
#include "CMyPaintPropertiesDialog.h"
#include "afxdialogex.h"


// Диалоговое окно CMyPaintPropertiesDialog

IMPLEMENT_DYNAMIC(CMyPaintPropertiesDialog, CDialogEx)

CMyPaintPropertiesDialog::CMyPaintPropertiesDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROPERTIES, pParent)
	, Name_(_T(""))
	, id_(0)
	, firstPointX_(0)
	, firstPointY_(0)
	, secondPointX_(0)
	, secondPointY_(0)
	, thirdPointX_(0)
	, thirdPointY_(0)
	, width_(0)
	, penStyle_(0)
	, brushStyle_(0)
	, penColor_(RGB(0,0,0))
	, brushColor_(RGB(0,0,0))
{
	for (size_t i = 0; i < 16; i++) {
		customPenColor_[i] = RGB(16 * i, 16 * i, 16 * i);
	}
	for (size_t i = 0; i < 16; i++) {
		customBrushColor_[i] = RGB(16 * i, 16 * i, 16 * i);
	}
}


CMyPaintPropertiesDialog::CMyPaintPropertiesDialog(CString Name, int id, CPoint firstPoint, CPoint secondPoint, CPoint thirdPoint, int width, int penStyle, int brushStyle,COLORREF penColor,COLORREF brushColor) : Name_(Name), id_(id), width_(width), penStyle_(penStyle), brushStyle_(brushStyle),penColor_(penColor),brushColor_(brushColor)
{
	firstPointX_ = firstPoint.x;
	firstPointY_ = firstPoint.y;
	secondPointX_ = secondPoint.x;
	secondPointY_ = secondPoint.y;
	thirdPointX_ = thirdPoint.x;
	thirdPointY_ = thirdPoint.y;
}

CMyPaintPropertiesDialog::~CMyPaintPropertiesDialog()
{
}

void CMyPaintPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, Name_);
	DDV_MaxChars(pDX, Name_, 100);
	DDX_Text(pDX, IDC_EDIT_ID, id_);
	DDV_MinMaxInt(pDX, id_, 0, INT_MAX);
	DDX_Text(pDX, IDC_EDIT_FIRST_POINT_X, firstPointX_);
	DDV_MinMaxInt(pDX, firstPointX_, 0, INT_MAX);
	DDX_Text(pDX, IDC_EDIT_FIRST_POINT_Y, firstPointY_);
	DDV_MinMaxInt(pDX, firstPointY_, 0, INT_MAX);
	DDX_Text(pDX, IDC_EDIT_SECOND_POINT_X, secondPointX_);
	DDV_MinMaxInt(pDX, secondPointX_, 0, INT_MAX);
	DDX_Text(pDX, IDC_EDIT_SECOND_POINT_Y, secondPointY_);
	DDV_MinMaxInt(pDX, secondPointY_, 0, INT_MAX);
	DDX_Text(pDX, IDC_EDIT_THIRD_POINT_X, thirdPointX_);
	DDV_MinMaxInt(pDX, thirdPointX_, 0, INT_MAX);
	DDX_Text(pDX, IDC_EDIT_THIRD_POINT_Y, thirdPointY_);
	DDV_MinMaxInt(pDX, thirdPointY_, 0, INT_MAX);
	DDX_Control(pDX, IDC_COMBO_WIDTH, widthControl_);
	DDX_CBIndex(pDX, IDC_COMBO_WIDTH, width_);
	DDX_Control(pDX, IDC_COMBO_PEN_STYLE, penStyleControl_);
	DDX_CBIndex(pDX, IDC_COMBO_PEN_STYLE, penStyle_);
	DDX_Control(pDX, IDC_COMBO_BRUSH_STYLE, brushStyleControl_);
	DDX_CBIndex(pDX, IDC_COMBO_BRUSH_STYLE, brushStyle_);
}


BEGIN_MESSAGE_MAP(CMyPaintPropertiesDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyPaintPropertiesDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PEN_COLOR, &CMyPaintPropertiesDialog::OnBnClickedButtonPenColor)
	ON_BN_CLICKED(IDC_BUTTON_BRUSH_COLOR, &CMyPaintPropertiesDialog::OnBnClickedButtonBrushColor)
END_MESSAGE_MAP()


// Обработчики сообщений CMyPaintPropertiesDialog


BOOL CMyPaintPropertiesDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	for (auto i = 1; i < 31; i++) {
		str.Format(_T("%d"), i);
		widthControl_.AddString(str);
	}
	widthControl_.SetCurSel(width_ - 1);

	penStyleControl_.AddString(L"Сплошной");
	penStyleControl_.AddString(L"Прерывистый");
	penStyleControl_.AddString(L"Пунктирный");
	penStyleControl_.AddString(L"Штрихпунктирный");
	penStyleControl_.AddString(L"Штрихпунктирный с двойными точками");
	penStyleControl_.AddString(L"Невидимый");
	penStyleControl_.SetCurSel(penStyle_);

	brushStyleControl_.AddString(L"Пустой");
	brushStyleControl_.AddString(L"Сплошной");
	brushStyleControl_.AddString(L"Наклонённый налево");
	brushStyleControl_.AddString(L"Клетка");
	brushStyleControl_.AddString(L"Крест на крест");
	brushStyleControl_.AddString(L"Наклонённый направо");
	brushStyleControl_.AddString(L"Горизонтальные линии");
	brushStyleControl_.AddString(L"Вертикальные линии");
	brushStyleControl_.SetCurSel(brushStyle_);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}



void CMyPaintPropertiesDialog::setName(CString name) {
	Name_ = name;
	tempName_ = name;
}


void CMyPaintPropertiesDialog::setID(int id) {
	id_ = id;
	tempid_ = id;
}


CString CMyPaintPropertiesDialog::getName() {
	return Name_;
}


int CMyPaintPropertiesDialog::getID() {
	return id_;
}


void CMyPaintPropertiesDialog::setFirstPoint(CPoint firstPoint) {
	firstPointX_ = firstPoint.x;
	firstPointY_ = firstPoint.y;
}


void CMyPaintPropertiesDialog::setSecondPoint(CPoint secondPoint) {
	secondPointX_ = secondPoint.x;
	secondPointY_ = secondPoint.y;
}


void CMyPaintPropertiesDialog::setThirdPoint(CPoint thirdPoint) {
	if (thirdPoint.x == secondPointX_ && thirdPoint.y == secondPointY_) {
		thirdPointX_ = 0;
		thirdPointY_ = 0;
	}
	else {
		thirdPointX_ = thirdPoint.x;
		thirdPointY_ = thirdPoint.y;
	}
}


CPoint CMyPaintPropertiesDialog::getFirstPoint() {
	CPoint point;
	point.x = firstPointX_;
	point.y = firstPointY_;
	return point;
}


CPoint CMyPaintPropertiesDialog::getSecondPoint() {
	CPoint point;
	point.x = secondPointX_;
	point.y = secondPointY_;
	return point;
}


CPoint CMyPaintPropertiesDialog::getThirdPoint() {
	CPoint point;
	point.x = thirdPointX_;
	point.y = thirdPointY_;
	return point;
}


void CMyPaintPropertiesDialog::setWidth(int width) {
	width_ = width;
}


int CMyPaintPropertiesDialog::getWidth() {
	return width_ + 1;
}


void CMyPaintPropertiesDialog::setPenStyle(int penStyle) {
	penStyle_ = penStyle;
}


int CMyPaintPropertiesDialog::getPenStyle() {
	return penStyle_;
}


void CMyPaintPropertiesDialog::setBrushStyle(int brushStyle) {

}


int CMyPaintPropertiesDialog::getBrushStyle() {
	return brushStyle_;
}


void CMyPaintPropertiesDialog::setPenColor(COLORREF penColor) {
	penColor_ = penColor;
}


COLORREF CMyPaintPropertiesDialog::getPenColor() {
	return penColor_;
}


void CMyPaintPropertiesDialog::setBrushColor(COLORREF brushColor) {
	brushColor_ = brushColor;
}


COLORREF CMyPaintPropertiesDialog::getBrushColor() {
	return brushColor_;
}

void CMyPaintPropertiesDialog::OnBnClickedOk()
{
	UpdateData();
	width_ = widthControl_.GetCurSel();
	penStyle_ = penStyleControl_.GetCurSel();
	brushStyle_ = brushStyleControl_.GetCurSel();
	if (width_ > 0 && penStyle_ > 0) {
		AfxMessageBox(L"Данная ширина и стиль контура несовместимы");
		width_ = 0;
		widthControl_.SetCurSel(width_);
	}
	for (size_t i = 0; i < ids_.size(); i++) {
		if (id_ == ids_[i]) {
			id_ = tempid_;
			AfxMessageBox(L"Данный ID уже существует");
			return;
		}
	}
	for (size_t i = 0; i < names_.size(); i++) {
		if (Name_ == names_[i]) {
			Name_ = tempName_;
			AfxMessageBox(L"Данное имя уже существует");
			return;
		}
	}
	CDialogEx::OnOK();
}


void CMyPaintPropertiesDialog::getIDs(std::vector<int>ids) {
	ids_ = ids;
}


void CMyPaintPropertiesDialog::getNames(std::vector<CString>names) {
	names_ = names;
}

void CMyPaintPropertiesDialog::OnBnClickedButtonPenColor()
{
	CColorDialog dlg;
	dlg.m_cc.rgbResult = penColor_;
	dlg.m_cc.lpCustColors = customPenColor_;
	dlg.DoModal();
	penColor_ = dlg.m_cc.rgbResult;
}


void CMyPaintPropertiesDialog::OnBnClickedButtonBrushColor()
{
	CColorDialog dlg;
	dlg.m_cc.rgbResult = brushColor_;
	dlg.m_cc.lpCustColors = customBrushColor_;
	dlg.DoModal();
	brushColor_ = dlg.m_cc.rgbResult;
}
