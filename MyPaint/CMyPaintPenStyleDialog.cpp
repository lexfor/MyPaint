// CMyPaintPenStyleDialog.cpp: файл реализации
//

#include "pch.h"
#include "MyPaint.h"
#include "CMyPaintPenStyleDialog.h"
#include "afxdialogex.h"


// Диалоговое окно CMyPaintPenStyleDialog

IMPLEMENT_DYNAMIC(CMyPaintPenStyleDialog, CDialogEx)

CMyPaintPenStyleDialog::CMyPaintPenStyleDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PEN_STYLE, pParent)
	, penStyle_(PS_SOLID)
{
}

CMyPaintPenStyleDialog::~CMyPaintPenStyleDialog()
{
}

void CMyPaintPenStyleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PEN_STYLE, penStyleControl_);
	DDX_CBIndex(pDX, IDC_COMBO_PEN_STYLE, penStyle_);
	DDV_MinMaxInt(pDX, penStyle_, 0, 100);
}


BEGIN_MESSAGE_MAP(CMyPaintPenStyleDialog, CDialogEx)
	ON_BN_CLICKED(IDAPPLY, &CMyPaintPenStyleDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// Обработчики сообщений CMyPaintPenStyleDialog


BOOL CMyPaintPenStyleDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	penStyleControl_.AddString(L"Сплошной");
	penStyleControl_.AddString(L"Прерывистый");
	penStyleControl_.AddString(L"Пунктирный");
	penStyleControl_.AddString(L"Штрихпунктирный");
	penStyleControl_.AddString(L"Штрихпунктирный с двойными точками");
	penStyleControl_.AddString(L"Невидимый");
	penStyleControl_.SetCurSel(penStyle_);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CMyPaintPenStyleDialog::OnBnClickedApply()
{
	UpdateData();
	penStyle_ = penStyleControl_.GetCurSel();
	CDialogEx::OnOK();
}


void CMyPaintPenStyleDialog::setPenStyle(int penStyle) {
	penStyle_ = penStyle;
}

int CMyPaintPenStyleDialog::getPenStyle() {
	return penStyle_;
}
