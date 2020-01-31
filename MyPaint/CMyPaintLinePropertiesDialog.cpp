// CMyPaintLinePropertiesDialog.cpp: файл реализации
//

#include "pch.h"
#include "MyPaint.h"
#include "CMyPaintLinePropertiesDialog.h"
#include "afxdialogex.h"


// Диалоговое окно CMyPaintLinePropertiesDialog

IMPLEMENT_DYNAMIC(CMyPaintLinePropertiesDialog, CDialogEx)

CMyPaintLinePropertiesDialog::CMyPaintLinePropertiesDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LINE_PROPERTIES, pParent)
	, name_(_T(""))
	, id_(0)
	, width_(0)
	, style_(0)
	, color_(RGB(0,0,0))
{
	for (size_t i = 0; i < 16; i++) {
		customColors_[i] = RGB(16 * i, 16 * i, 16 * i);
	}
}

CMyPaintLinePropertiesDialog::~CMyPaintLinePropertiesDialog()
{
}

void CMyPaintLinePropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, name_);
	DDV_MaxChars(pDX, name_, 100);
	DDX_Text(pDX, IDC_EDIT_ID, id_);
	DDX_Control(pDX, IDC_COMBO_WIDTH, widthControl_);
	DDX_CBIndex(pDX, IDC_COMBO_WIDTH, width_);
	DDX_Control(pDX, IDC_COMBO_STYLE, styleControl_);
	DDX_CBIndex(pDX, IDC_COMBO_STYLE, style_);
}


BEGIN_MESSAGE_MAP(CMyPaintLinePropertiesDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyPaintLinePropertiesDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &CMyPaintLinePropertiesDialog::OnBnClickedButtonColor)
END_MESSAGE_MAP()


// Обработчики сообщений CMyPaintLinePropertiesDialog


void CMyPaintLinePropertiesDialog::OnBnClickedOk()
{
	UpdateData();
	width_ = widthControl_.GetCurSel() + 1;
	style_ = styleControl_.GetCurSel();
	if (width_ > 1 && style_ > 0) {
		AfxMessageBox(L"Данная ширина и стиль контура несовместимы");
		width_ = 1;
		widthControl_.SetCurSel(width_ - 1);
	}
	for (size_t i = 0; i < ids_.size(); i++) {
		if (id_ == ids_[i]) {
			id_ = tempID_;
			AfxMessageBox(L"Данный ID уже существует");
			return;
		}
	}
	for (size_t i = 0; i < names_.size(); i++) {
		if (name_ == names_[i]) {
			name_ = tempName_;
			AfxMessageBox(L"Данное имя уже существует");
			return;
		}
	}
	CDialogEx::OnOK();
}


void CMyPaintLinePropertiesDialog::setName(CString name) {
	name_ = name;
}

void CMyPaintLinePropertiesDialog::setID(int id) {
	id_ = id;
}

void CMyPaintLinePropertiesDialog::setWidth(int width) {
	width_ = width;
}

void CMyPaintLinePropertiesDialog::setStyle(int style) {
	style_ = style;
}

void CMyPaintLinePropertiesDialog::setColor(COLORREF color) {
	color_ = color;
}

CString CMyPaintLinePropertiesDialog::getName() {
	return name_;
}

int CMyPaintLinePropertiesDialog::getID() {
	return id_;
}

int CMyPaintLinePropertiesDialog::getWidth() {
	return width_;
}

int CMyPaintLinePropertiesDialog::getStyle() {
	return style_;
}

COLORREF CMyPaintLinePropertiesDialog::getColor() {
	return color_;
}

BOOL CMyPaintLinePropertiesDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	for (auto i = 1; i < 31; i++) {
		str.Format(_T("%d"), i);
		widthControl_.AddString(str);
	}
	widthControl_.SetCurSel(width_ - 1);

	styleControl_.AddString(L"Сплошной");
	styleControl_.AddString(L"Прерывистый");
	styleControl_.AddString(L"Пунктирный");
	styleControl_.AddString(L"Штрихпунктирный");
	styleControl_.AddString(L"Штрихпунктирный с двойными точками");
	styleControl_.AddString(L"Невидимый");
	styleControl_.SetCurSel(style_);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CMyPaintLinePropertiesDialog::OnBnClickedButtonColor()
{
	CColorDialog dlg;
	dlg.m_cc.rgbResult = color_;
	dlg.m_cc.lpCustColors = customColors_;
	dlg.DoModal();
	color_ = dlg.m_cc.rgbResult;
}


void CMyPaintLinePropertiesDialog::setNames(std::vector<CString> names) {
	names_ = names;
}


void CMyPaintLinePropertiesDialog::setIDs(std::vector<int> ids) {
	ids_ = ids;
}