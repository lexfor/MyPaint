// CMyPaintListViewDialog.cpp: файл реализации
//

#include "pch.h"
#include "MyPaint.h"
#include "CMyPaintFigure.h"
#include "CMyPaintEllipse.h"
#include "CMyPaintRect.h"
#include "CMyPaintTriangle.h"
#include "CMyPaintListViewDialog.h"
#include "CMyPaintConnection.h"
#include "afxdialogex.h"


// Диалоговое окно CMyPaintListViewDialog

IMPLEMENT_DYNAMIC(CMyPaintListViewDialog, CDialogEx)

CMyPaintListViewDialog::CMyPaintListViewDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LISTVIEW, pParent)
{

}

CMyPaintListViewDialog::~CMyPaintListViewDialog()
{
}

void CMyPaintListViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VIEW, listviewControl_);
}


BEGIN_MESSAGE_MAP(CMyPaintListViewDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMyPaintListViewDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений CMyPaintListViewDialog


BOOL CMyPaintListViewDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	int item;
	CString str,tempStr;
	listviewControl_.InsertColumn(0, L"Название", LVCFMT_LEFT, 150);
	listviewControl_.InsertColumn(1, L"ID", LVCFMT_LEFT, 150);
	listviewControl_.InsertColumn(2, L"Ширина", LVCFMT_LEFT, 150);
	listviewControl_.InsertColumn(3, L"Стиль контура", LVCFMT_LEFT, 150);
	listviewControl_.InsertColumn(4, L"Стиль заливки", LVCFMT_LEFT, 150);
	listviewControl_.InsertColumn(5, L"Первая точка(x,y) ", LVCFMT_LEFT, 150);
	listviewControl_.InsertColumn(6, L"Вторая точка(x,y)", LVCFMT_LEFT, 150);
	listviewControl_.InsertColumn(7, L"Третья точка(x,y)", LVCFMT_LEFT, 150);
	for (size_t i = 0; i < figure_.size(); i++) {
		str = figure_[i]->getName();
		item = listviewControl_.InsertItem(0, str);
		str.Format(_T("%i"), figure_[i]->getID());
		listviewControl_.SetItemText(item, 1, str);
		str.Format(_T("%i"), figure_[i]->getWidth());
		listviewControl_.SetItemText(item, 2, str);
		switch (figure_[i]->getPenStyle())
		{
		case 0:
			str = "Сплошной";
			break;
		case 1:
			str = "Прерывистый";
			break;
		case 2:
			str = "Пунктирный";
			break;
		case 3:
			str = "Штрихпунктирный";
			break;
		case 4:
			str = "Штрихпунктирный с двумя точками";
			break;
		case 5:
			str = "Невидимый";
			break;
		default:
			break;
		}
		listviewControl_.SetItemText(item, 3, str);
		switch (figure_[i]->getBrushStyle())
		{
		case 0:
			str = "Пустой";
			break;
		case 1:
			str = "Сплошной";
			break;
		case 2:
			str = "Наклонённый налево";
			break;
		case 3:
			str = "Клетка";
			break;
		case 4:
			str = "Крест на крест";
			break;
		case 5:
			str = "Наклонённый направо";
			break;
		case 6:
			str = "Горизонтальные линии";
			break;
		case 7:
			str = "Вертикальные линии";
			break;
		default:
			break;
		}
		if (figure_[i]->getFigureType() == 4) {
			str = "N/A";
		}
		listviewControl_.SetItemText(item, 4, str);
		str.Format(_T("%i"), figure_[i]->getFirstCoordinate().x);
		str += " , ";
		tempStr.Format(_T("%i"), figure_[i]->getFirstCoordinate().y);
		str += tempStr;
		listviewControl_.SetItemText(item, 5, str);
		str.Format(_T("%i"), figure_[i]->getSecondCoordinate().x);
		str += " , ";
		tempStr.Format(_T("%i"), figure_[i]->getSecondCoordinate().y);
		str += tempStr;
		listviewControl_.SetItemText(item, 6, str);
		if (figure_[i]->getSecondCoordinate() == figure_[i]->getThirdCoordinate()) {
			str = "N/A";
		}
		else {
			str.Format(_T("%i"), figure_[i]->getThirdCoordinate().x);
			str += " , ";
			tempStr.Format(_T("%i"), figure_[i]->getThirdCoordinate().y);
			str += tempStr;
		}
		listviewControl_.SetItemText(item, 7, str);
	}

	for (size_t i = 0; i < connections_.size(); i++) {
		str = connections_[i].getName();
		item = listviewControl_.InsertItem(0, str);
		str.Format(_T("%i"), connections_[i].getID());
		listviewControl_.SetItemText(item, 1, str);
		str.Format(_T("%i"), connections_[i].getWidth());
		listviewControl_.SetItemText(item, 2, str);
		switch (connections_[i].getStyle())
		{
		case 0:
			str = "Сплошной";
			break;
		case 1:
			str = "Прерывистый";
			break;
		case 2:
			str = "Пунктирный";
			break;
		case 3:
			str = "Штрихпунктирный";
			break;
		case 4:
			str = "Штрихпунктирный с двумя точками";
			break;
		case 5:
			str = "Невидимый";
			break;
		default:
			break;
		}
		listviewControl_.SetItemText(item, 3, str);
		str = "N/A";
		listviewControl_.SetItemText(item, 4, str);
		str.Format(_T("%i"), figure_[i]->getFirstCoordinate().x);
		str += " , ";
		tempStr.Format(_T("%i"), figure_[i]->getFirstCoordinate().y);
		str += tempStr;
		listviewControl_.SetItemText(item, 5, str);
		str.Format(_T("%i"), figure_[i]->getSecondCoordinate().x);
		str += " , ";
		tempStr.Format(_T("%i"), figure_[i]->getSecondCoordinate().y);
		str += tempStr;
		listviewControl_.SetItemText(item, 6, str);
		str = "N/A";
		listviewControl_.SetItemText(item, 7, str);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}


void CMyPaintListViewDialog::getFigure(std::vector<CMyPaintFigure*> figure) {
	figure_ = figure;
}


void CMyPaintListViewDialog::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnOK();
}


void CMyPaintListViewDialog::getConnections(std::vector<CMyPaintConnection> connections) {
	connections_ = connections;
}
