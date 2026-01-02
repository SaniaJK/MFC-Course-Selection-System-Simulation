#include "pch.h"
#include "SchoolSystem.h"

// ================= Course Implementation =================
int Course::allCoursesTotalEnrollment = 0; // 静态成员初始化

Course::Course(int id, CourseType type, string name, int credit) 
    : id(id), type(type), name(name), credit(credit), currentEnrollment(0) {}

bool Course::enrollStudent() {
    if (currentEnrollment >= 15) return false; // 课程人数不能超过15
    currentEnrollment++;
    allCoursesTotalEnrollment++;
    return true;
}


void Course::cancelEnrollment() {
    if (currentEnrollment > 0) {
        currentEnrollment--;
        allCoursesTotalEnrollment--;
    }
}

bool Student::dropCourse(Course* course, string& errorMsg) {
    for (auto it = selectedCourses.begin(); it != selectedCourses.end(); ++it) {
        if ((*it)->getId() == course->getId()) {
            // 1. 从学生列表移除
            selectedCourses.erase(it);
            // 2. 课程人数减1
            course->cancelEnrollment();
            // 3. 重新计算学费
            calculateTuition();
            return true;
        }
    }
    errorMsg = "未找到该课程，无法退课。";
    return false;
}



int Course::getAllCoursesTotalEnrollment() {
    return allCoursesTotalEnrollment;
}

string Course::getTypeName() const {
    switch (type) {
    case SUBJECT_BASE: return "学科基础";
    case MAJOR_BASE:   return "专业基础";
    case MAJOR_CORE:   return "专业核心";
    default: return "未知";
    }
}

// ================= Student Implementation =================
Student::Student(string id, string name) : id(id), name(name), tuitionFee(0.0) {}

int Student::getTotalCredits() const {
    int total = 0;
    for (auto c : selectedCourses) {
        total += c->getCredit();
    }
    return total;
}

bool Student::selectCourse(Course* course, string& errorMsg) {
    if (selectedCourses.size() >= 5) {
        errorMsg = "选课失败：每个学生最多只能选5门课。";
        return false;
    }

    // 检查是否已选
    for (auto c : selectedCourses) {
        if (c->getId() == course->getId()) {
            errorMsg = "选课失败：已修读该课程。";
            return false;
        }
    }

    if (!course->enrollStudent()) {
        errorMsg = "选课失败：该课程人数已满（15人）。";
        return false;
    }

    selectedCourses.push_back(course);
    calculateTuition(); // 每次选课后重新计算学费
    return true;
}

void Student::showInfo() const {
    cout << "学号: " << id << " | 姓名: " << name << endl;
    cout << "已选课程: " << endl;
    for (auto c : selectedCourses) {
        cout << "  - [" << c->getTypeName() << "] " << c->getName() << " (" << c->getCredit() << "学分)" << endl;
    }
    cout << "总学分: " << getTotalCredits() << " | 应缴学费: " << tuitionFee << "元" << endl;
}

// ================= UGstudent Implementation =================
UGstudent::UGstudent(string id, string name, string mentor) 
    : Student(id, name), classMentor(mentor) {}

void UGstudent::calculateTuition() {
    tuitionFee = 0;
    for (auto c : selectedCourses) {
        int rate = 0;
        switch (c->getType()) {
        case SUBJECT_BASE: rate = 200; break;
        case MAJOR_BASE:   rate = 150; break;
        case MAJOR_CORE:   rate = 100; break;
        }
        tuitionFee += c->getCredit() * rate;
    }
}

void UGstudent::showInfo() const {
    cout << "=== 本科生信息 ===" << endl;
    cout << "班导师: " << classMentor << endl;
    Student::showInfo();
}

// ================= Gstudent Implementation =================
Gstudent::Gstudent(string id, string name, string supervisor) 
    : Student(id, name), supervisor(supervisor) {}

void Gstudent::calculateTuition() {
    tuitionFee = 0;
    for (auto c : selectedCourses) {
        int rate = 0;
        switch (c->getType()) {
        case SUBJECT_BASE: rate = 200; break;
        case MAJOR_BASE:   rate = 180; break; // 研究生费率不同
        case MAJOR_CORE:   rate = 150; break; // 研究生费率不同
        }
        tuitionFee += c->getCredit() * rate;
    }
}

void Gstudent::showInfo() const {
    cout << "=== 研究生信息 ===" << endl;
    cout << "导师: " << supervisor << endl;
    Student::showInfo();
}

// ================= DataManager Implementation =================
DataManager::DataManager() {
    initCourses();
    initTeachers();
    initStudents();
}

DataManager::~DataManager() {
    for (auto s : students) delete s;
    students.clear();
}

void DataManager::initCourses() {
    // 表1 数据
    courses.push_back(Course(1, SUBJECT_BASE, "英语口语", 4));
    courses.push_back(Course(2, SUBJECT_BASE, "高等数学", 2));
    courses.push_back(Course(3, SUBJECT_BASE, "大学物理", 1));
    courses.push_back(Course(4, SUBJECT_BASE, "政治经济学", 1));
    courses.push_back(Course(5, MAJOR_BASE, "程序设计", 2));
    courses.push_back(Course(6, MAJOR_BASE, "数据结构", 2));
    courses.push_back(Course(7, MAJOR_BASE, "计算机原理", 3));
    courses.push_back(Course(8, MAJOR_BASE, "汇编语言", 3));
    courses.push_back(Course(9, MAJOR_CORE, "OOP程序设计", 2));
    courses.push_back(Course(10, MAJOR_CORE, "编译原理", 2));
    courses.push_back(Course(11, MAJOR_CORE, "微机原理", 3));
    courses.push_back(Course(12, MAJOR_CORE, "计算机网络", 3));
}


void DataManager::initTeachers() {
    // 这里为了演示，硬编码一些教师工号
    // 实际项目中应从数据库读取
    teacherMap["T001"] = "张老师";
    teacherMap["T002"] = "李老师";
    teacherMap["T003"] = "王老师";

    teacherMap["T101"] = "张教授";
    teacherMap["T102"] = "李教授";
    teacherMap["T103"] = "王教授";
}


void DataManager::initStudents() {
    // ==========================================
    // 表2 2013级注册的本科生信息 (共30人)
    // 构造函数格式: new UGstudent(学号, 姓名, 班导师)
    // ==========================================
    students.push_back(new UGstudent("2014001", "李强", "张老师"));
    students.push_back(new UGstudent("2014002", "王娜", "李老师"));
    students.push_back(new UGstudent("2014003", "李飞", "王老师"));
    students.push_back(new UGstudent("2014004", "王小明", "张老师"));
    students.push_back(new UGstudent("2014005", "陈小霞", "李老师"));
    students.push_back(new UGstudent("2014006", "李玉刚", "王老师"));
    students.push_back(new UGstudent("2014007", "钟丽丽", "张老师"));
    students.push_back(new UGstudent("2014008", "谢飞翔", "李老师"));
    students.push_back(new UGstudent("2014009", "吴胜利", "王老师"));
    students.push_back(new UGstudent("2014010", "程思青", "张老师"));
    students.push_back(new UGstudent("2014011", "邹安邦", "李老师"));
    students.push_back(new UGstudent("2014012", "曹思青", "王老师"));
    students.push_back(new UGstudent("2014013", "卢永祥", "张老师"));
    students.push_back(new UGstudent("2014014", "王凯", "李老师"));
    students.push_back(new UGstudent("2014015", "王胜利", "王老师"));
    students.push_back(new UGstudent("2014016", "李珊", "张老师"));
    students.push_back(new UGstudent("2014017", "魏星", "李老师"));
    students.push_back(new UGstudent("2014018", "史东", "王老师"));
    students.push_back(new UGstudent("2014019", "谢喜莲", "张老师"));
    students.push_back(new UGstudent("2014020", "鲁迪", "李老师"));
    students.push_back(new UGstudent("2014021", "程长喜", "王老师"));
    students.push_back(new UGstudent("2014022", "齐莉莉", "张老师"));
    students.push_back(new UGstudent("2014023", "何正明", "李老师"));
    students.push_back(new UGstudent("2014024", "赵祥海", "王老师"));
    students.push_back(new UGstudent("2014025", "卢志胜", "张老师"));
    students.push_back(new UGstudent("2014026", "谢喜贤", "李老师"));
    students.push_back(new UGstudent("2014027", "邹安青", "王老师"));
    students.push_back(new UGstudent("2014028", "蔡飞翔", "张老师"));
    students.push_back(new UGstudent("2014029", "吴东升", "李老师"));
    students.push_back(new UGstudent("2014030", "李建军", "王老师"));

    // ==========================================
    // 表3 2013级注册的研究生信息 (共15人)
    // 构造函数格式: new Gstudent(学号, 姓名, 导师)
    // ==========================================
    students.push_back(new Gstudent("2014101", "李小强", "张教授"));
    students.push_back(new Gstudent("2014102", "王妮娜", "李教授"));
    students.push_back(new Gstudent("2014103", "李红飞", "王教授"));
    students.push_back(new Gstudent("2014104", "王大明", "张教授"));
    students.push_back(new Gstudent("2014105", "陈飞霞", "李教授"));
    students.push_back(new Gstudent("2014106", "李成刚", "张教授"));
    students.push_back(new Gstudent("2014107", "钟环丽", "张教授"));
    students.push_back(new Gstudent("2014108", "谢卫翔", "李教授"));
    students.push_back(new Gstudent("2014109", "吴卫国", "张教授"));
    students.push_back(new Gstudent("2014110", "程思思", "张教授"));
    students.push_back(new Gstudent("2014111", "邹爱国", "李教授"));
    students.push_back(new Gstudent("2014112", "曹宝清", "张教授"));
    students.push_back(new Gstudent("2014113", "卢新阳", "张教授"));
    students.push_back(new Gstudent("2014114", "王大凯", "李教授"));
    students.push_back(new Gstudent("2014115", "王福利", "张教授"));
}

Student* DataManager::findStudent(string id) {
    for (auto s : students) {
        if (s->getId() == id) return s;
    }
    return nullptr;
}

Course* DataManager::findCourse(int id) {
    for (auto& c : courses) {
        if (c.getId() == id) return &c;
    }
    return nullptr;
}

UserRole DataManager::checkLogin(string inputId, string& outName) {
    // 1. 检查是否是管理员
    if (inputId == "ADMIN") {
        outName = "教务处管理员";
        return ROLE_ADMIN;
    }

    // 2. 检查是否是教师
    auto it = teacherMap.find(inputId);
    if (it != teacherMap.end()) {
        outName = it->second; // 返回导师姓名，用于筛选学生
        return ROLE_TEACHER;
    }

    // 3. 检查是否是学生
    Student* pStudent = findStudent(inputId);
    if (pStudent) {
        outName = pStudent->getName();
        return ROLE_STUDENT;
    }

    return ROLE_NONE;
}