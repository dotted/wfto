#if !defined(AFX_TARGETDLG_H__683E529D_7282_484C_A403_367A6A90CC57__INCLUDED_)
#define AFX_TARGETDLG_H__683E529D_7282_484C_A403_367A6A90CC57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TargetDlg.h : �w�b�_�[ �t�@�C��
//


/////////////////////////////////////////////////////////////////////////////
// CTargetDlg �_�C�A���O

class CTargetDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CTargetDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTargetDlg)
	enum { IDD = IDD_TARGET };
	CEdit	m_File;
	int		m_DXVersion;
	BOOL	m_EmulatePal;
	BOOL	m_HookDI;
	BOOL	m_ModifyMouse;
	BOOL	m_OutTrace;
	BOOL	m_UnNotify;
	CString	m_FilePath;
	BOOL	m_SaveLoad;
	int		m_InitX;
	int		m_InitY;
	int		m_MaxX;
	int		m_MaxY;
	int		m_MinX;
	int		m_MinY;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTargetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTargetDlg)
	afx_msg void OnOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TARGETDLG_H__683E529D_7282_484C_A403_367A6A90CC57__INCLUDED_)
