//Disjoint-Set 并查集 C++ 通用代码
class Djset {
public:
    // 用于记录该节点的父节点，所有父节点相同的节点位于同一连通图
    vector<int> parent; 
    // 节点的秩，主要记录该节点目前位于的树的深度，从子节点出发
    // 主要用于优化，在合并两个父节点时，通过rank的大小判断谁父谁子
    vector<int> rank;
    // 用于记录并查集的数量,某些情况下该成员非必要
    int count;
    Djset(int n): count(n) {
        for (int i = 0; i < n; i++) {
            parent.push_back(i);
            rank.push_back(0);
        }
    }
    // 路径压缩， 遍历过程中的所有父节点直接指向根节点，
    // 减少后续查找次数
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    // 合并两个节点
    // 如果处于同一个并查集， 不需要合并
    // 如果不处于同一个并查集，判断两个rootx和rooty谁的秩大
    bool merge(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx != ry) {
            if (rank[rx] < rank[ry]) {
                swap(rx, ry);
            }
            parent[ry] = rx;
            rank[rx] += rank[ry]
            count--;
            return true;
        }
        return false;
    }
    // 判断两个点是否连通
    bool connected(int x, int y){
        x = find(x);
        y = find(y);
        return x == y;
    }
};

