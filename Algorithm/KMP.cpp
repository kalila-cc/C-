#include <iostream>
using namespace std;

/*
    KMP�㷨��
    ͨ�������Ӵ���ƥ��ʧ�ܺ��ƶ���λ�� nwxt[] �� nextval[] ���Ż�ƥ�����
    
    next[]/nextval[] ������̣�
    ��ʼ���Ӵ���һ���ַ����ƶ�λ��Ϊ -1�������Ӵ�������������ƥ��
    ��ʼ�� j = i-1 ���� [i-1] ������
    ����� j ���ַ����ƶ�λ��ʱ����Ҫ���� j ���ַ����ƶ�λ�����ڴ����� next[j]
    �� next[j] �����ַ��� [i-1] ��ͬ���� j = next[j]��ֱ���ַ���ͬ�� next[j] < 0
    �� next[i] = next[j] + 1
    �� [i] �� [next[i]] ��ȣ����ַ���ͬ��ֱ���� nextval[i] = next[i]������ȡ nextval[next[i]]

    �Ӵ�ƥ����̣�
    ��ʼ�� i = j = 0 Ϊ�����Ӵ�������
    ÿ����ѭ��ƥ��һ���ַ��ɹ��� i++, j++
    �� j ��С���Ӵ����ȣ�˵��ƥ��ɹ����˳���ѭ��
    �� nextval[j] < 0��˵���Ӵ���Ҫ��ͷƥ�䣬�Ӵ������������������ƣ�i++, j++
    ����ʱ�Ӵ�ʣ�೤�ȴ�������ʣ�೤�ȣ������ٽ���ƥ��
    �˳���ѭ���󣬸��� j ��ֵ�ж��Ƿ�ƥ��ɹ�
    ��ƥ��ɹ���Ӧ�� j >= s_str.length()������ƥ�䵽���±� (i - s_str.length())�����򷵻� -1
*/
int KMP(string m_str, string s_str) {
    int i, j, *next, *nextval;
    // ��������С���Ӵ����ȣ�ֱ�ӷ��� -1
    if (m_str.length() < s_str.length()) return -1;
    // ��ʼ�� next[] �� nextval[]��ͬʱ�� next[0] = nextval[0] = -1����־�������Ӵ�������Ҫ��������
    next = new int[s_str.length()];
    nextval = new int[s_str.length()];
    next[0] = nextval[0] = -1;
    // ���� next[] �� nextval[]����ʼ�� i = j = 0 Ϊ�����Ӵ�������
    for (i = 1; i < s_str.length(); i++) {
        for (j = i - 1; next[j] >= 0 && s_str[i - 1] != s_str[next[j]]; j = next[j]) continue;
        next[i] = next[j] + 1;
        nextval[i] = s_str[i] == s_str[next[i]] ? nextval[next[i]] : next[i];
    }
    // ��ʾ next ����
    // for (int i = 0; i < s_str.length(); i++) cout << next[i] << " ";
    cout << endl;
    // ƥ���Ӵ�
    for (i = j = 0; i < m_str.length() && j < s_str.length();) {
        // ÿ����ѭ��ƥ��һ���ַ��ɹ��� i++, j++
        while (i < m_str.length() && j < s_str.length() && m_str[i] == s_str[j]) i++, j++;
        // �� j ��С���Ӵ����ȣ�˵��ƥ��ɹ����˳���ѭ��
        if (j >= s_str.length()) break;
        // �� nextval[j] < 0��˵���Ӵ���Ҫ��ͷƥ�䣬�Ӵ������������������ƣ�i++, j++
        if ((j = nextval[j]) < 0) i++, j++;
        // ����ʱ�Ӵ�ʣ�೤�ȴ�������ʣ�೤�ȣ������ٽ���ƥ��
        if (s_str.length() - j > m_str.length() - i) break;
    }
    // ���� j ��ֵ�ж��Ƿ�ƥ��ɹ�
    // ��ƥ��ɹ���Ӧ�� j >= s_str.length()������ƥ�䵽���±� (i - s_str.length())�����򷵻� -1
    return j >= s_str.length() ? (i - s_str.length()) : -1;
}

// ������
int main() {
    string m = "acabaabaabcacaabc";
    string s = "abaabcac";
    cout << KMP(m, s) << endl;
    return 0;
}