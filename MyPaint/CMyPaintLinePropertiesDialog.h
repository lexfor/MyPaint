#pragma once
#include<vector>

// Диалоговое окно CMyPaintLinePropertiesDialog

class CMyPaintLinePropertiesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMyPaintLinePropertiesDialog)

public:
	CMyPaintLinePropertiesDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CMyPaintLinePropertiesDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LINE_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	std::vector<int> ids_;
	std::vector<CString> names_;
	CString name_;
	CString tempName_;
	int id_;
	int tempID_;
	CComboBox widthControl_;
	int width_;
	CComboBox styleControl_;
	int style_;
	COLORREF customColors_[16];
    COLORREF color_;
public:
	afx_msg void OnBnClickedOk();
	void setName(CString);
	CString getName();
	void setID(int);
	int getID();
	void setWidth(int);
	int getWidth();
	void setStyle(int);
	int getStyle();
	void setColor(COLORREF);
	COLORREF getColor();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonColor();
	void setNames(std::vector<CString>);
	void setIDs(std::vector<int>);
};
