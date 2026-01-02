#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <map>


using namespace std;

enum UserRole {
    ROLE_NONE,
    ROLE_STUDENT,
    ROLE_TEACHER,
    ROLE_ADMIN
};


// 课程类型枚举
enum CourseType {
    SUBJECT_BASE, // 学科基础
    MAJOR_BASE,   // 专业基础
    MAJOR_CORE    // 专业核心
};

// ==========================================
// 1. 课程类 (Course)
// ==========================================
class Course {
private:
    int id;                 // 课程序号
    CourseType type;        // 课程类型
    string name;            // 课程名称
    int credit;             // 学分
    int currentEnrollment;  // 当前选课人数
    static int allCoursesTotalEnrollment; // 所有课程选课总人数 (静态成员)

public:
    Course(int id, CourseType type, string name, int credit);
    //void cancelEnrollment();
    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    int getCredit() const { return credit; }
    CourseType getType() const { return type; }
    int getCurrentEnrollment() const { return currentEnrollment; }
    
    // 增加选课人数
    bool enrollStudent();
    
    // 静态成员函数
    static int getAllCoursesTotalEnrollment();
    
    // 获取类型名称（用于显示）
    string getTypeName() const;

    void cancelEnrollment();
};

// ==========================================
// 2. 学生基类 (Student)
// ==========================================
class Student {
protected:
    string id;              // 学号
    string name;            // 姓名
    vector<Course*> selectedCourses; // 所选课程
    double tuitionFee;      // 学费

public:
    Student(string id, string name);
    virtual ~Student() {}

    string getId() const { return id; }
    string getName() const { return name; }
    int getSelectedCount() const { return selectedCourses.size(); }
    
    // 计算总学分
    int getTotalCredits() const;

    // 选课操作
    bool selectCourse(Course* course, string& errorMsg);
    bool dropCourse(Course* course, string& errorMsg);

    // 纯虚函数：计算学费（由子类实现）
    virtual void calculateTuition() = 0;
    
    // 纯虚函数：显示/获取信息
    virtual void showInfo() const;
    
    // 获取已选课程列表（用于UI显示）
    const vector<Course*>& getSelectedCourses() const { return selectedCourses; }
    double getTuition() const { return tuitionFee; }
};

// ==========================================
// 3. 本科生类 (UGstudent)
// ==========================================
class UGstudent : public Student {
private:
    string classMentor; // 班导师

public:
    UGstudent(string id, string name, string mentor);
    virtual void calculateTuition() override;
    virtual void showInfo() const override;
    string getMentor() const { return classMentor; }
};

// ==========================================
// 4. 研究生类 (Gstudent)
// ==========================================
class Gstudent : public Student {
private:
    string supervisor; // 导师

public:
    Gstudent(string id, string name, string supervisor);
    virtual void calculateTuition() override;
    virtual void showInfo() const override;
    string getSupervisor() const { return supervisor; }
};

// ==========================================
// 5. 数据管理辅助类 (DataManager)
//    用于初始化数据和查找对象
// ==========================================
class DataManager {
public:
    vector<Course> courses;
    vector<Student*> students;
    map<string, string> teacherMap; // [新增] 工号 -> 姓名 映射

    DataManager();
    ~DataManager();

    void initCourses();
    void initStudents();

    // [新增] 初始化教师数据
    void initTeachers();

    Student* findStudent(string id);
    Course* findCourse(int id);

    // [新增] 统一登录验证接口
    // 输入ID，返回角色类型。如果是导师，通过 reference 返回导师姓名
    UserRole checkLogin(string inputId, string& outName);
};