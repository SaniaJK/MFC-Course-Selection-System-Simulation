// CourseSystemMFCDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CourseSystemMFC.h"
#include "CourseSystemMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// --- CAboutDlg ---
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}
void CAboutDlg::DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); }
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
// ----------------

CCourseSystemMFCDlg::CCourseSystemMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COURSESYSTEMMFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCourseSystemMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCourseSystemMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CCourseSystemMFCDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_SELECT, &CCourseSystemMFCDlg::OnBnClickedBtnSelect)
	ON_BN_CLICKED(IDOK, &CCourseSystemMFCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_DROP, &CCourseSystemMFCDlg::OnBnClickedBtnDrop)
	ON_BN_CLICKED(IDC_BTN_LOGOUT, &CCourseSystemMFCDlg::OnBnClickedBtnLogout)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CCourseSystemMFCDlg::OnBnClickedBtnSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER, &CCourseSystemMFCDlg::OnCbnSelchangeComboFilter)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COURSES, &CCourseSystemMFCDlg::OnNMClickListCourses)
END_MESSAGE_MAP()

BOOL CCourseSystemMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FILTER);
	pCombo->AddString(_T("全部类型"));
	pCombo->AddString(_T("学科基础"));
	pCombo->AddString(_T("专业基础"));
	pCombo->AddString(_T("专业核心"));
	pCombo->SetCurSel(0);

	currentStudent = nullptr;
	targetStudent = nullptr;
	currentRole = ROLE_NONE;

	((CEdit*)GetDlgItem(IDC_EDIT_ID))->SetCueBanner(L"输入学号/工号/ADMIN", TRUE);

	CListCtrl* pLeft = (CListCtrl*)GetDlgItem(IDC_LIST_COURSES);
	pLeft->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CListCtrl* pRight = (CListCtrl*)GetDlgItem(IDC_LIST_MYCOURSES);
	pRight->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;
}

void CCourseSystemMFCDlg::OnBnClickedBtnLogin()
{
	CString strId;
	GetDlgItemText(IDC_EDIT_ID, strId);
	string inputId = CT2A(strId);
	string loginName;
	currentRole = dataMgr.checkLogin(inputId, loginName);

	currentStudent = nullptr;
	targetStudent = nullptr;

	CListCtrl* pLeft = (CListCtrl*)GetDlgItem(IDC_LIST_COURSES);
	CListCtrl* pRight = (CListCtrl*)GetDlgItem(IDC_LIST_MYCOURSES);
	pLeft->DeleteAllItems(); pRight->DeleteAllItems();
	while (pLeft->DeleteColumn(0)); while (pRight->DeleteColumn(0));

	if (currentRole == ROLE_NONE) {
		MessageBox(_T("用户不存在"), _T("错误"));
		return;
	}
	else if (currentRole == ROLE_STUDENT) {
		currentStudent = dataMgr.findStudent(inputId);
		// 注意：这里的欢迎语只是暂时的，RefreshRightList 会立刻覆盖成详细统计信息，符合要求
		CString strWelcome;
		strWelcome.Format(_T("欢迎同学: %s"), (LPCTSTR)CA2W(loginName.c_str(), 936));
		SetDlgItemText(IDC_STATIC_INFO, strWelcome);

		// [修改] 恢复“人数”列
		pLeft->InsertColumn(0, _T("ID"), LVCFMT_LEFT, 40);
		pLeft->InsertColumn(1, _T("课程名称"), LVCFMT_LEFT, 100);
		pLeft->InsertColumn(2, _T("类型"), LVCFMT_LEFT, 80);
		pLeft->InsertColumn(3, _T("学分"), LVCFMT_LEFT, 50);
		pLeft->InsertColumn(4, _T("人数"), LVCFMT_LEFT, 60); // 恢复人数列
		pLeft->InsertColumn(5, _T("状态"), LVCFMT_LEFT, 60);

		pRight->InsertColumn(0, _T("已选课程"), LVCFMT_LEFT, 120);
		pRight->InsertColumn(1, _T("费用"), LVCFMT_LEFT, 80);

		GetDlgItem(IDC_BTN_SELECT)->ShowWindow(SW_SHOW);
		RefreshLeftList();
		RefreshRightList(); // 这里会更新 showinfo
	}
	else {
		currentTeacherName = loginName;
		CString strTitle;
		if (currentRole == ROLE_ADMIN) strTitle = _T("【管理员模式】");
		else strTitle.Format(_T("【导师模式】%s"), (LPCTSTR)CA2W(loginName.c_str(), 936));
		SetDlgItemText(IDC_STATIC_INFO, strTitle);

		pLeft->InsertColumn(0, _T("学号"), LVCFMT_LEFT, 80);
		pLeft->InsertColumn(1, _T("姓名"), LVCFMT_LEFT, 80);
		pLeft->InsertColumn(2, _T("身份"), LVCFMT_LEFT, 60);
		pLeft->InsertColumn(3, _T("导师"), LVCFMT_LEFT, 80);
		pLeft->InsertColumn(4, _T("已选"), LVCFMT_LEFT, 50);

		pRight->InsertColumn(0, _T("所选课程"), LVCFMT_LEFT, 120);
		pRight->InsertColumn(1, _T("学分"), LVCFMT_LEFT, 50);
		pRight->InsertColumn(2, _T("费用"), LVCFMT_LEFT, 80);

		GetDlgItem(IDC_BTN_SELECT)->ShowWindow(SW_HIDE);
		RefreshLeftList();
	}
}

void CCourseSystemMFCDlg::RefreshLeftList()
{
	if (currentRole == ROLE_STUDENT) {
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_COURSES);
		pList->DeleteAllItems();
		CString strSearch; GetDlgItemText(IDC_EDIT_SEARCH, strSearch);
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FILTER);
		int nSel = pCombo->GetCurSel();
		CourseType targetType = SUBJECT_BASE;
		bool needFilter = (nSel > 0);
		if (nSel == 1) targetType = SUBJECT_BASE;
		if (nSel == 2) targetType = MAJOR_BASE;
		if (nSel == 3) targetType = MAJOR_CORE;

		int i = 0;
		for (auto& c : dataMgr.courses) {
			if (needFilter && c.getType() != targetType) continue;
			if (!strSearch.IsEmpty() && CString((LPCTSTR)CA2W(c.getName().c_str(), 936)).Find(strSearch) == -1) continue;

			CString strStatus = _T("未选");
			if (currentStudent) {
				for (auto s : currentStudent->getSelectedCourses())
					if (s->getId() == c.getId()) { strStatus = _T("★已选"); break; }
			}
			CString strId; strId.Format(_T("%d"), c.getId());
			CString strCredit; strCredit.Format(_T("%d"), c.getCredit());
			// [修改] 恢复人数显示
			CString strCount; strCount.Format(_T("%d/15"), c.getCurrentEnrollment());

			int idx = pList->InsertItem(i, strId);
			pList->SetItemText(idx, 1, (LPCTSTR)CA2W(c.getName().c_str(), 936));
			pList->SetItemText(idx, 2, (LPCTSTR)CA2W(c.getTypeName().c_str(), 936));
			pList->SetItemText(idx, 3, strCredit);
			pList->SetItemText(idx, 4, strCount); // 显示人数
			pList->SetItemText(idx, 5, strStatus); // 状态移到第5列
			pList->SetItemData(idx, (DWORD_PTR)&c);
			i++;
		}
	}
	else {
		RefreshStudentListToLeft();
	}
}

// 辅助函数：显示学生名单到左侧 (已添加搜索功能)
void CCourseSystemMFCDlg::RefreshStudentListToLeft()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_COURSES);
	pList->DeleteAllItems();

	// 1. 获取搜索框内容
	CString strSearch;
	GetDlgItemText(IDC_EDIT_SEARCH, strSearch);
	strSearch.Trim(); // 去除首尾空格

	int i = 0;
	for (auto s : dataMgr.students) {
		bool show = false;
		UGstudent* pUG = dynamic_cast<UGstudent*>(s);
		Gstudent* pG = dynamic_cast<Gstudent*>(s);

		// ---------------------------------------------------------
		// A. 第一层过滤：角色权限 (Admin看所有，Teacher看自己)
		// ---------------------------------------------------------
		if (currentRole == ROLE_ADMIN) {
			show = true;
		}
		else if (currentRole == ROLE_TEACHER) {
			if (pUG && pUG->getMentor() == currentTeacherName) show = true;
			if (pG && pG->getSupervisor() == currentTeacherName) show = true;
		}

		// ---------------------------------------------------------
		// B. 第二层过滤：搜索关键词 (姓名 或 学号)
		// ---------------------------------------------------------
		if (show && !strSearch.IsEmpty()) {
			// 将姓名和学号转为 CString 以便查找
			CString strName = (LPCTSTR)CA2W(s->getName().c_str(), 936);
			CString strId = (LPCTSTR)CA2W(s->getId().c_str());

			// Find 返回 -1 表示没找到
			// 如果 姓名 和 学号 都不包含关键词，则不显示
			if (strName.Find(strSearch) == -1 && strId.Find(strSearch) == -1) {
				show = false;
			}
		}

		// ---------------------------------------------------------
		// C. 插入列表
		// ---------------------------------------------------------
		if (show) {
			CString strRole, strMentor;
			if (pUG) { strRole = _T("本科生"); strMentor = (LPCTSTR)CA2W(pUG->getMentor().c_str(), 936); }
			if (pG) { strRole = _T("研究生"); strMentor = (LPCTSTR)CA2W(pG->getSupervisor().c_str(), 936); }
			CString strCount; strCount.Format(_T("%d门"), s->getSelectedCourses().size());

			int idx = pList->InsertItem(i, (LPCTSTR)CA2W(s->getId().c_str()));
			pList->SetItemText(idx, 1, (LPCTSTR)CA2W(s->getName().c_str(), 936));
			pList->SetItemText(idx, 2, strRole);
			pList->SetItemText(idx, 3, strMentor);
			pList->SetItemText(idx, 4, strCount);

			// 绑定 Student* 指针
			pList->SetItemData(idx, (DWORD_PTR)s);
			i++;
		}
	}
}

void CCourseSystemMFCDlg::OnEnChangeEditSearch()
{
	// 输入框内容改变时，自动刷新
	RefreshLeftList();
}

void CCourseSystemMFCDlg::RefreshRightList()
{
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_MYCOURSES);
	pList->DeleteAllItems();

	// [修改] 确定查看对象：如果是学生看自己，如果是Admin/Teacher看选中的学生
	Student* pView = (currentRole == ROLE_STUDENT) ? currentStudent : targetStudent;

	// 如果没有查看对象，直接返回
	if (!pView) return;

	int i = 0;
	for (auto c : pView->getSelectedCourses()) {
		int rate = 0;
		if (dynamic_cast<UGstudent*>(pView)) {
			if (c->getType() == SUBJECT_BASE) rate = 200;
			else if (c->getType() == MAJOR_BASE) rate = 150;
			else rate = 100;
		}
		else {
			if (c->getType() == SUBJECT_BASE) rate = 200;
			else if (c->getType() == MAJOR_BASE) rate = 180;
			else rate = 150;
		}
		CString strFee; strFee.Format(_T("%d"), rate * c->getCredit());
		CString strCredit; strCredit.Format(_T("%d"), c->getCredit());

		pList->InsertItem(i, (LPCTSTR)CA2W(c->getName().c_str(), 936));
		pList->SetItemText(i, 1, strCredit);
		pList->SetItemText(i, 2, strFee);
		i++;
	}

	// [修改] 恢复 showinfo 功能
	// 无论什么角色，只要正在查看某个学生，就显示该学生的详细统计信息
	CString strInfo, strIdentity, strMentorInfo;
	UGstudent* pUG = dynamic_cast<UGstudent*>(pView);
	Gstudent* pG = dynamic_cast<Gstudent*>(pView);

	if (pUG) {
		strIdentity = _T("本科生");
		strMentorInfo.Format(_T("班导师: %s"), (LPCTSTR)CA2W(pUG->getMentor().c_str(), 936));
	}
	else if (pG) {
		strIdentity = _T("研究生");
		strMentorInfo.Format(_T("导师: %s"), (LPCTSTR)CA2W(pG->getSupervisor().c_str(), 936));
	}

	strInfo.Format(_T("姓名: %s | 身份: %s\n%s | 已选学分: %d | 应缴学费: %.2f 元"),
		(LPCTSTR)CA2W(pView->getName().c_str(), 936),
		strIdentity,
		strMentorInfo,
		pView->getTotalCredits(),
		pView->getTuition());

	SetDlgItemText(IDC_STATIC_INFO, strInfo);
}

void CCourseSystemMFCDlg::OnNMClickListCourses(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nIndex = pNMItemActivate->iItem;
	if (nIndex != -1) {
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_COURSES);
		DWORD_PTR data = pList->GetItemData(nIndex);
		if (currentRole != ROLE_STUDENT) {
			targetStudent = (Student*)data;
			RefreshRightList();
		}
	}
	*pResult = 0;
}

void CCourseSystemMFCDlg::OnBnClickedBtnSelect()
{
	if (currentRole != ROLE_STUDENT) { MessageBox(_T("教师/管理员不能选课")); return; }
	if (!currentStudent) return;
	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_COURSES);
	POSITION pos = pList->GetFirstSelectedItemPosition();
	if (!pos) return;
	int nItem = pList->GetNextSelectedItem(pos);
	Course* pCourse = (Course*)pList->GetItemData(nItem);
	string msg;
	if (currentStudent->selectCourse(pCourse, msg)) {
		MessageBox(_T("选课成功"));
		RefreshLeftList(); RefreshRightList();
	}
	else {
		MessageBox((LPCTSTR)CA2W(msg.c_str(), 936));
	}
}

void CCourseSystemMFCDlg::OnBnClickedBtnDrop()
{
	// [修改] 允许管理员退课
	if (currentRole == ROLE_TEACHER) { MessageBox(_T("教师无权退课，仅供查看。")); return; }

	// [修改] 确定操作对象：学生自己操作自己，管理员操作选中的学生
	Student* pOperateStudent = (currentRole == ROLE_STUDENT) ? currentStudent : targetStudent;

	if (!pOperateStudent) {
		if (currentRole == ROLE_ADMIN) MessageBox(_T("请先在左侧选择一名学生"));
		else MessageBox(_T("请先登录"));
		return;
	}

	CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_MYCOURSES);
	POSITION pos = pList->GetFirstSelectedItemPosition();
	if (!pos) {
		MessageBox(_T("请在右侧列表中选择要退选的课程"));
		return;
	}
	int nItem = pList->GetNextSelectedItem(pos);

	CString strName = pList->GetItemText(nItem, 0);
	Course* pTarget = nullptr;
	for (auto c : pOperateStudent->getSelectedCourses()) {
		if (CString((LPCTSTR)CA2W(c->getName().c_str(), 936)) == strName) { pTarget = c; break; }
	}

	if (pTarget) {
		string msg;
		if (pOperateStudent->dropCourse(pTarget, msg)) {
			MessageBox(_T("退课成功"));
			// 刷新左右列表
			// 如果是学生模式，左侧刷新课程名额
			// 如果是管理员模式，左侧刷新该学生的已选门数
			RefreshLeftList();
			RefreshRightList();
		}
		else {
			MessageBox((LPCTSTR)CA2W(msg.c_str(), 936));
		}
	}
}

void CCourseSystemMFCDlg::OnBnClickedBtnLogout()
{
	currentStudent = nullptr; targetStudent = nullptr; currentRole = ROLE_NONE;
	CListCtrl* pL = (CListCtrl*)GetDlgItem(IDC_LIST_COURSES); pL->DeleteAllItems();
	CListCtrl* pR = (CListCtrl*)GetDlgItem(IDC_LIST_MYCOURSES); pR->DeleteAllItems();
	while (pL->DeleteColumn(0)); while (pR->DeleteColumn(0));
	SetDlgItemText(IDC_STATIC_INFO, _T("请先登录"));
	SetDlgItemText(IDC_EDIT_ID, _T(""));
	MessageBox(_T("已退出"));
}

void CCourseSystemMFCDlg::OnBnClickedBtnSearch() { RefreshLeftList(); }
void CCourseSystemMFCDlg::OnCbnSelchangeComboFilter() { RefreshLeftList(); }
void CCourseSystemMFCDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) { CAboutDlg dlgAbout; dlgAbout.DoModal(); }
	else CDialogEx::OnSysCommand(nID, lParam);
}
void CCourseSystemMFCDlg::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON); int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect; GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2; int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else CDialogEx::OnPaint();
}
HCURSOR CCourseSystemMFCDlg::OnQueryDragIcon() { return static_cast<HCURSOR>(m_hIcon); }
void CCourseSystemMFCDlg::OnLvnItemchangedListCourses(NMHDR* pNMHDR, LRESULT* pResult) { *pResult = 0; }
void CCourseSystemMFCDlg::OnEnChangeEditId() {}
//void CCourseSystemMFCDlg::OnEnChangeEditSearch() {}
void CCourseSystemMFCDlg::OnBnClickedOk() { CDialogEx::OnOK(); }

BOOL CCourseSystemMFCDlg::PreTranslateMessage(MSG* pMsg) {
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
		CWnd* pFocus = GetFocus();
		if (pFocus) {
			int nID = pFocus->GetDlgCtrlID();
			if (nID == IDC_EDIT_ID) { OnBnClickedBtnLogin(); return TRUE; }
			else if (nID == IDC_EDIT_SEARCH) { OnBnClickedBtnSearch(); return TRUE; }
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}