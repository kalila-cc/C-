#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
using namespace std;

/* 声明 */
struct Person;
string birth_fmt(string birth); // 用于将yyyymmdd的生日格式化为yyyy-mm-dd
string sex_fmt(bool sex); // 用于将0/1映射为女/男
bool compare(const Person &r1, const Person &r2); // 用于排序的时候进行比较
int Exists(vector<Person> &r, string name); // 用于判断是否存在某个联系人
void Add(vector<Person> &r); // 用于添加联系人信息
void Display(vector<Person> &r); // 用于浏览联系人列表
void Find(vector<Person> &r); // 用于查询某个具体的联系人的信息
void Sort(vector<Person> &r); // 用于对联系人信息进行排序
void Modify(vector<Person> &r); // 用于修改某个联系人的信息
void Delete(vector<Person> &r); // 用于删除某个联系人的信息

string birth_fmt(string birth) {
    string s;
    s = birth.substr(0, 4) + "-" + birth.substr(4, 2) + "-" + birth.substr(6, 2);
    return s;
}

string sex_fmt(bool sex) {
    string s = sex ? "男" : "女";
    return s;
}

struct Person {
    bool sex;   // 男1女0
    string name, birth, tel;
};

bool compare(const Person &r1, const Person &r2) {
    return r1.birth < r2.birth;
}

void OperationList() {
    cout << endl;
    cout << "*      1、联系人录入       *" << endl;
    cout << "*      2、联系人浏览       *" << endl;
    cout << "*      3、联系人查询       *" << endl;
    cout << "*      4、联系人排序       *" << endl;
    cout << "*      5、联系人修改       *" << endl;
    cout << "*      6、联系人删除       *" << endl;
    cout << "*      7、退出通讯录       *" << endl;
    cout << "请选择一个功能：" << endl;
}

int Exists(vector<Person> &r, string name) {
    for (int i = 0; i < r.size(); ++i)
        if (r[i].name == name)
            return i;
    return -1;
}

void Add(vector<Person> &r) {
    string name;
    cout << "请分别输入姓名、性别(男1女0)、生日(yyyymmdd)，电话号码：" << endl;
    Person person;
    cin >> person.name >> person.sex >> person.birth >> person.tel;
    r.push_back(person);
    cout << "添加联系人成功" << endl;
}

void Display(vector<Person> &r) {
    if (r.size() == 0) {
        cout << "当前通讯录没有联系人" << endl;
    } else {
        printf("%24s%12s%24s%24s\n", "姓名", "性别", "生日", "电话");
        for (int i = 0; i < r.size(); ++i) {
            printf("%24s", r[i].name.c_str());
            printf("%12s", sex_fmt(r[i].sex).c_str());
            printf("%24s", birth_fmt(r[i].birth).c_str());
            printf("%24s\n", r[i].tel.c_str());
        }
    }
}

void Find(vector<Person> &r) {
    int index = -1;
    string name;
    cout << "请输入要查找联系人的姓名：" << endl;
    cin >> name;
    index = Exists(r, name);
    if (index < 0) {
        cout << "没有此联系人" << endl;
    } else {
        printf("%24s%12s%24s%24s\n", "姓名", "性别", "生日", "电话");
        printf("%24s", r[index].name.c_str());
        printf("%12s", sex_fmt(r[index].sex).c_str());
        printf("%24s", birth_fmt(r[index].birth).c_str());
        printf("%24s\n", r[index].tel.c_str());
    }
}

void Sort(vector<Person> &r) {
    if (r.size() > 0)
        sort(r.begin(), r.end(), compare);
    cout << "联系人排序成功，可以通过联系人浏览查看" << endl;
}

void Modify(vector<Person> &r) {
    string name;
    cout << "请输入需要修改的联系人的姓名：" << endl;
    cin >> name;
    int index = Exists(r, name);
    if (index < 0) {
        cout << "查找不到此联系人" << endl;
    } else {
        cout << "请重新分别输入该联系人的生日(yyyymmdd)，电话号码：" << endl;
        cin >> r[index].birth >> r[index].tel;
        cout << "联系人信息修改成功" << endl;
    }
}

void Delete(vector<Person> &r) {
    int i;
    string name;
    cout << "请输入需要删除的联系人的姓名：" << endl;
    cin >> name;
    i = Exists(r, name);
    if (i < 0) {
        cout << "该联系人不存在" << endl;
    } else {
        r.erase(r.begin() + i, r.begin() + i + 1);
        cout << "删除联系人成功！" << endl;
    }
}

int main() {
    vector<Person> addrbook;
    int choice = -1;
    cout << "****** 欢迎来到通讯录 ******" << endl;
    while (true) {
        OperationList();
        cin >> choice;
        switch (choice) {
        case 1:
            Add(addrbook);
            break;
        case 2:
            Display(addrbook);
            break;
        case 3:
            Find(addrbook);
            break;
        case 4:
            Sort(addrbook);
            break;
        case 5:
            Modify(addrbook);
            break;
        case 6:
            Delete(addrbook);
            break;
        case 7:
            cout << "欢迎下次使用，再见" << endl;
            fflush(stdin);
            getchar();
            return 0;
        default:
            cout << "输入指令错误，请重新输入" << endl;
            break;
        }
    }
}
