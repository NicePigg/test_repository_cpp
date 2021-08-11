// 1 2 3 4 5 把前k个数移到后面
// 4 5 1 2 3
void swapVec(queue<int>& q, int k, int n) // 把前k个数移动到右边
{
    int t;
    for (int i = 0; i < k; i++) {
        t = q.front();
        q.pop();
        q.push(t);
    }
    return;
}
int main()
{
    int n, Q;
    cin >> n >> Q;
    queue<int> num;
    int tmp;
    int sum = 0;
    for (int i = 0; i < n; i++) {
        cin >> tmp;
        num.push(tmp);
    }
    for (int i = 0; i < Q; i++) {
        cin >> tmp;
        sum += tmp;
        sum %= n;
    }
    
    swapVec(num, sum, n);
    for (int i = 0; i < n; i++)
    {
        if (!num.empty()) {
            cout << num.front() << ' ';
            num.pop();
        }
    }

    cout << endl;
    return 0;
}
