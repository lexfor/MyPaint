#pragma once


// Диалоговое окно CMyPaintBrushStyleDialog

class CMyPaintBrushStyleDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMyPaintBrushStyleDialog)

public:
	CMyPaintBrushStyleDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CMyPaintBrushStyleDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BRUSH_STYLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	CComboBox brushStyleControl_;
	int brushStyle_;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedApply();
	void setBrushStyle(int);
	int getBrushStyle();
};
