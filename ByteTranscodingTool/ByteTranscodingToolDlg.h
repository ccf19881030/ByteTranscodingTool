
// ByteTranscodingToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CByteTranscodingToolDlg 对话框
class CByteTranscodingToolDlg : public CDialogEx
{
// 构造
public:
	CByteTranscodingToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BYTETRANSCODINGTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 数据类型的复选变量
	CComboBox m_datatypeComboBox;
public:
	afx_msg void OnCbnDropdownComboDatatype();
	afx_msg void OnBnClickedButtonConvert();
	afx_msg void OnBnClickedButtonFloatConvert();
	afx_msg void OnBnClickedButtonDoubleConvert();
	afx_msg void OnBnClickedButtonInt16Convert();
	afx_msg void OnBnClickedButtonUint16Convert();
	afx_msg void OnBnClickedButtonInt32Convert();
	afx_msg void OnBnClickedButtonUint32Convert();
	afx_msg void OnBnClickedButtonInt64Convert();
	afx_msg void OnBnClickedButtonUint64Convert();

public:
	// 起始位置偏移量
	int m_nStartOffset;
	// 输入字节数组
	CString m_strInput;
	// 输出
	CString m_strResult;
	// 是否为小端
	BOOL m_bLittleEndian;
	// 是否需要Swap
	BOOL m_bSwap;
	float m_floatVal;
	double m_doubleVal;
	short m_shortVal;
//	unsigned short m_ushortVal;
	int m_int32Val;
//	unsigned int m_uint32Val;
	long long m_int64Val;
//	unsigned long long m_uint64Val;
	CString m_strFloatResult;
	CString m_strDoubleResult;
	CString m_strInt16Result;
	CString m_strUint16Result;
//	CString m_strInt32Result;
	CString m_strUint32Result;
	CString m_strInt64Result;
	CString m_strUint64Result;
	// 大小端的Byte Swap的选项
	int m_checkIndex;
	CString m_strInt32Result;
	// unsigned short
	CString m_strUint16Val;
	CString m_strUint32Val;
	CString m_strUint64Val;
};
