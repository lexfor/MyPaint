// CMyPaintBrushStyleDialog.cpp: файл реализации
//

#include "pch.h"
#include "MyPaint.h"
#include "CMyPaintBrushStyleDialog.h"
#include "afxdialogex.h"


// Диалоговое окно CMyPaintBrushStyleDialog

IMPLEMENT_DYNAMIC(CMyPaintBrushStyleDialog, CDialogEx)

CMyPaintBrushStyleDialog::CMyPaintBrushStyleDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BRUSH_STYLE, pParent)
	, brushStyle_(0)
{

}

CMyPaintBrushStyleDialog::~CMyPaintBrushStyleDialog()
{
}

void CMyPaintBrushStyleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BRUSH_STYLE, brushStyleControl_);
	DDX_CBIndex(pDX, IDC_COMBO_BRUSH_STYLE, brushStyle_);
	DDV_MinMaxInt(pDX, brushStyle_, 0, 100);
}


BEGIN_MESSAGE_MAP(CMyPaintBrushStyleDialog, CDialogEx)
	ON_BN_CLICKED(IDAPPLY, &CMyPaintBrushStyleDialog::OnBnClickedApply)
END_MESSAGE_MAP()


// Обработчики сообщений CMyPaintBrushStyleDialog


BOOL CMyPaintBrushStyleDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
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


void CMyPaintBrushStyleDialog::OnBnClickedApply()
{
	UpdateData();
	brushStyle_ = brushStyleControl_.GetCurSel();
	CDialogEx::OnOK();
}


void CMyPaintBrushStyleDialog::setBrushStyle(int brushStyle) {
	brushStyle_ = brushStyle;
}

int CMyPaintBrushStyleDialog::getBrushStyle() {
	return brushStyle_;
}