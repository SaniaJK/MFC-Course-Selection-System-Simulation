#pragma once
#include "SchoolSystem.h" // 【关键】必须放在第一行非注释代码

// CCourseSystemMFCDlg 对话框
class CCourseSystemMFCDlg : public CDialogEx
{
    // 构造
public:
    CCourseSystemMFCDlg(CWnd* pParent = nullptr);	// 标准构造函数

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_COURSESYSTEMMFC_DIALOG };
#endif

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

    // 键盘消息拦截
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    DECLARE_MESSAGE_MAP()

public:
    // ==========================================
    // 核心变量声明 (编译器找不到就是因为这里没声明)
    // ==========================================
    DataManager dataMgr;
    Student* currentStudent;

    // 角色与权限相关
    UserRole currentRole;
    string currentTeacherName;
    Student* targetStudent;

    // ==========================================
    // 刷新函数声明 (编译器找不到就是因为这里没声明)
    // ==========================================
    void RefreshLeftList();
    void RefreshRightList();
    void RefreshStudentListToLeft();

public:
    // ==========================================
    // 按钮与事件响应函数
    // ==========================================
    afx_msg void OnBnClickedBtnLogin();
    afx_msg void OnBnClickedBtnSelect();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedBtnDrop();
    afx_msg void OnBnClickedBtnLogout();
    afx_msg void OnBnClickedBtnSearch();
    afx_msg void OnCbnSelchangeComboFilter();
    afx_msg void OnEnChangeEditSearch();
    afx_msg void OnEnChangeEditId();
    afx_msg void OnLvnItemchangedListCourses(NMHDR* pNMHDR, LRESULT* pResult);

    // 列表点击事件
    afx_msg void OnNMClickListCourses(NMHDR* pNMHDR, LRESULT* pResult);
};