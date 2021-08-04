#include <iostream>
using namespace std;

/*
    KMP算法：
    通过计算子串在匹配失败后移动的位置 nwxt[] 和 nextval[] 来优化匹配过程
    
    next[]/nextval[] 计算过程：
    初始化子串第一个字符的移动位置为 -1，代表子串整体右移重新匹配
    初始化 j = i-1 代表 [i-1] 的索引
    计算第 j 个字符的移动位置时，需要看第 j 个字符的移动位置所在处，即 next[j]
    若 next[j] 所在字符与 [i-1] 相同，令 j = next[j]，直到字符不同或 next[j] < 0
    令 next[i] = next[j] + 1
    若 [i] 与 [next[i]] 相比，若字符不同，直接令 nextval[i] = next[i]，否则取 nextval[next[i]]

    子串匹配过程：
    初始化 i = j = 0 为主串子串的索引
    每次内循环匹配一个字符成功则 i++, j++
    若 j 不小于子串长度，说明匹配成功，退出外循环
    若 nextval[j] < 0，说明子串需要重头匹配，子串主串索引都进行右移，i++, j++
    若此时子串剩余长度大于主串剩余长度，不必再进行匹配
    退出外循环后，根据 j 的值判断是否匹配成功
    若匹配成功，应有 j >= s_str.length()，返回匹配到的下标 (i - s_str.length())，否则返回 -1
*/
int KMP(string m_str, string s_str) {
    int i, j, *next, *nextval;
    // 主串长度小于子串长度，直接返回 -1
    if (m_str.length() < s_str.length()) return -1;
    // 初始化 next[] 和 nextval[]，同时置 next[0] = nextval[0] = -1，标志着主串子串索引需要进行右移
    next = new int[s_str.length()];
    nextval = new int[s_str.length()];
    next[0] = nextval[0] = -1;
    // 计算 next[] 和 nextval[]，初始化 i = j = 0 为主串子串的索引
    for (i = 1; i < s_str.length(); i++) {
        for (j = i - 1; next[j] >= 0 && s_str[i - 1] != s_str[next[j]]; j = next[j]) continue;
        next[i] = next[j] + 1;
        nextval[i] = s_str[i] == s_str[next[i]] ? nextval[next[i]] : next[i];
    }
    // 显示 next 数组
    // for (int i = 0; i < s_str.length(); i++) cout << next[i] << " ";
    cout << endl;
    // 匹配子串
    for (i = j = 0; i < m_str.length() && j < s_str.length();) {
        // 每次内循环匹配一个字符成功则 i++, j++
        while (i < m_str.length() && j < s_str.length() && m_str[i] == s_str[j]) i++, j++;
        // 若 j 不小于子串长度，说明匹配成功，退出外循环
        if (j >= s_str.length()) break;
        // 若 nextval[j] < 0，说明子串需要重头匹配，子串主串索引都进行右移，i++, j++
        if ((j = nextval[j]) < 0) i++, j++;
        // 若此时子串剩余长度大于主串剩余长度，不必再进行匹配
        if (s_str.length() - j > m_str.length() - i) break;
    }
    // 根据 j 的值判断是否匹配成功
    // 若匹配成功，应有 j >= s_str.length()，返回匹配到的下标 (i - s_str.length())，否则返回 -1
    return j >= s_str.length() ? (i - s_str.length()) : -1;
}

// 主函数
int main() {
    string m = "acabaabaabcacaabc";
    string s = "abaabcac";
    cout << KMP(m, s) << endl;
    return 0;
}