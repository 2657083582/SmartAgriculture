
// smsdemoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CsmsdemoDlg �Ի���
class CsmsdemoDlg : public CDialogEx
{
// ����
public:
	CsmsdemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SMSDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit editappid;
	afx_msg void OnBnClickedOk();
	CEdit editsecretKey;
	CEdit editmobile;
	CEdit editsmssign;
	CEdit editsmstemplateid;
	CEdit editsmscode;
};
