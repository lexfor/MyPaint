// CMyPaintWidthDialog.cpp: файл реализации
//

#include "pch.h"
#include "MyPaint.h"
#include "CMyPaintWidthDialog.h"
#include "afxdialogex.h"

#define DEFAULTWIDTH 1

// Диалоговое окно CMyPaintWidthDialog

IMPLEMENT_DYNAMIC(CMyPaintWidthDialog, CDialogEx)

CMyPaintWidthDialog::CMyPaintWidthDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_WIDTH, pParent)
	, penWidth_(DEFAULTWIDTH)
{

}

CMyPaintWidthDialog::~CMyPaintWidthDialog()
{
}

void CMyPaintWidthDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_WIDTH, penWidth_);
	//DDV_MinMaxInt(pDX, penWidth_, 1, 30);
	DDX_Control(pDX, IDC_COMBO_WIDTH, penWidthControl_);
}


BEGIN_MESSAGE_MAP(CMyPaintWidthDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyPaintWidthDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений CMyPaintWidthDialog


BOOL CMyPaintWidthDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	for (auto i = 1; i < 31; i++) {
		str.Format(_T("%d"), i);
		penWidthControl_.AddString(str);
	}
	penWidthControl_.SetCurSel(penWidth_ - 1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CMyPaintWidthDialog::OnBnClickedOk()
{
	UpdateData();
	penWidth_ = penWidthControl_.GetCurSel();
	CDialogEx::OnOK();
}

void CMyPaintWidthDialog::setCurrentWidth(int width) {
	penWidth_ = width;
}

int CMyPaintWidthDialog::getCurrentWidth() {
	return (penWidth_ + 1);
}
