#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <map>
#include <string>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <numeric>
using namespace std;
//递归:
void quicksort_dg(vector<int>& data, int i, int j)
{
    int l = i, r = j;
    int x = data[l];
    if (l >= r) return;
    while (l < r) {
        while (l < r && data[r] > x) r--;
        if (l < r) {
            data[l] = data[r];
            l++;
        }

        while (l < r && data[l] < x) l++;
        if (l < r) {
            data[r] = data[l];
            r--;
        }
    }
    data[l] = x;
    quicksort_dg(data, i, l - 1);
    quicksort_dg(data, l + 1, j);
    return;
}
//非递归： 所有迭代都可以用栈来模拟
int partition(vector<int>& data, int l, int r)
{
    int x = data[l];
    if (l >= r) return l;
    while (l < r && data[r] > x) r--;
    if (l < r) {
        data[l] = data[r];
        l++;
    }

    while (l < r && data[l] < x) l++;
    if (l < r) {
        data[r] = data[l];
        r--;
    }
    data[l] = x;
    return l;
}

void quicksort_stk(vector<int>& data, int l, int r)
{
    stack<int> st;
    if (l < r) {
        int tmp = partition(data, l, r);
        if (tmp - 1 > l)  // 左边不止一个元素
        {
            st.push(tmp - 1);//push右边界
            st.push(l);//push左边界
        }
        if (tmp + 1 < r)  // 右边不止一个元素
        {
            st.push(r);
            st.push(tmp + 1);
        }
    }
    while (!st.empty())
    {
        int start = st.top();
        st.pop();
        int end = st.top();
        st.pop();

        int tmp = partition(data, start, end);
        if (tmp - 1 > start) {
            st.push(tmp - 1);
            st.push(start);
        }
        if (tmp + 1 < end)  // 右边不止一个元素
        {
            st.push(end);
            st.push(tmp + 1);
        }
    }
    return;
}
void printNum(vector<int>& data)
{
    for (int i = 0; i < data.size(); i++)
    {
        cout << data[i] << ' ';
    }
    cout << endl;
    return ;
}

int main()
{
    vector<int> num{ 4,2,1,5,6,7,3,0,9 };
    vector<int> num1 = num;
    quicksort_dg(num, 0, num.size() - 1);
    quicksort_stk(num1, 0, num1.size() - 1);
    printNum(num);
    printNum(num1);
    return 0;
}
