#pragma once


// Диалоговое окно CMyPaintWidthDialog

class CMyPaintWidthDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMyPaintWidthDialog)

public:
	CMyPaintWidthDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CMyPaintWidthDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WIDTH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	int penWidth_;
	CComboBox penWidthControl_;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void setCurrentWidth(int);
	int getCurrentWidth();
};
