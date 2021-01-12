#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdio.h>
using namespace std;
/* 斐波那契数列 */
class Solution {
public:
// 递归
    int fib_1(int n){
        if(n < 2) return n;
        return fib_1(n-1)+fib_1(n-2);
    }
// 记忆递归
    int fib_2(int n, unordered_map<int,int> &m){
        if(n < 2) return n;
        if(n == m.count(n)) return m[n];
        m[n] = fib_2(n-1,m) + fib_2(n-2,m);
        return m[n];
    }
// 动态规划
    int fib_3(int n) {
        if(n < 2) return n;
        vector<int> dp(n+1);
        dp[0]=0;
        dp[1]=1;
        for(int i = 2; i < n+1; i++){
            dp[i] = (dp[i-1]+dp[i-2])%1000000007;
        }
        return dp[n];
    }
};


int main(){
  Solution sol; 
  unordered_map<int,int> m;
  int n = 10;
  cout << "fib_1["<<n<<"] is : "<<sol.fib_1(10)<<endl;
  cout << "fib_2["<<n<<"] is : "<<sol.fib_2(10,m)<<endl;
  cout << "fib_3["<<n<<"] is : "<<sol.fib_3(10)<<endl;
  return 0;
}
