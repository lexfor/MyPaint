#pragma once
#include <vector>

// Диалоговое окно CMyPaintPropertiesDialog

class CMyPaintPropertiesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMyPaintPropertiesDialog)

public:
	CMyPaintPropertiesDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CMyPaintPropertiesDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	CString Name_;
	CString tempName_;
	int id_;
	int tempid_;
	COLORREF customPenColor_[16];
	COLORREF customBrushColor_[16];
public:
	CMyPaintPropertiesDialog(CString, int, CPoint, CPoint, CPoint, int, int, int, COLORREF, COLORREF);
	virtual BOOL OnInitDialog();
	void setName(CString);
	void setID(int);
	CString getName();
	int getID();
	void setFirstPoint(CPoint);
	void setSecondPoint(CPoint);
	void setThirdPoint(CPoint);
	CPoint getFirstPoint();
	CPoint getSecondPoint();
	CPoint getThirdPoint();
	void setWidth(int);
	int getWidth();
	void setPenStyle(int);
	int getPenStyle();
	void setBrushStyle(int);
	int getBrushStyle();
	void setPenColor(COLORREF);
	COLORREF getPenColor();
	void setBrushColor(COLORREF);
	COLORREF getBrushColor();
	void getIDs(std::vector<int>);
	void getNames(std::vector<CString>);
private:
	int firstPointX_;
	int firstPointY_;
	int secondPointX_;
	int secondPointY_;
	int thirdPointX_;
	int thirdPointY_;
	CComboBox widthControl_;
	int width_;
	CComboBox penStyleControl_;
	int penStyle_;
	CComboBox brushStyleControl_;
	int brushStyle_;
	COLORREF penColor_;
	COLORREF brushColor_;
	std::vector<int> ids_;
	std::vector<CString> names_;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonPenColor();
	afx_msg void OnBnClickedButtonBrushColor();
};
