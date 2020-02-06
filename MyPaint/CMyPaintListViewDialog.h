#pragma once
#include "CMyPaintConnection.h"


// Диалоговое окно CMyPaintListViewDialog

class CMyPaintListViewDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMyPaintListViewDialog)

public:
	CMyPaintListViewDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CMyPaintListViewDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LISTVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void getFigure(std::vector<CMyPaintFigure*>);
	void getConnections(std::vector<CMyPaintConnection>);
private:
	std::vector<CMyPaintFigure*> figure_;
	std::vector<CMyPaintConnection>connections_;
	CListCtrl listviewControl_;
public:
	afx_msg void OnBnClickedOk();
};
