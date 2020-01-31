#pragma once


// Диалоговое окно CMyPaintPenStyleDialog

class CMyPaintPenStyleDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMyPaintPenStyleDialog)

public:
	CMyPaintPenStyleDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CMyPaintPenStyleDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PEN_STYLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	CComboBox penStyleControl_;
	int penStyle_;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedApply();
	void setPenStyle(int);
	int getPenStyle();
};
