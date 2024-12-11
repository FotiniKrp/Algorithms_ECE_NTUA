#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Struct for Union-Find (Disjoint Set Union)
struct UnionFind {
    vector<int> parent, rank;
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (x != parent[x]) parent[x] = find(parent[x]); // Path compression
        return parent[x];
    }

    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) return false;

        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        return true;
    }
};

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Struct to represent an edge
struct Edge {
    int u, v; // Nodes that the edge connects
    int profit, weight;
};

bool compareByRatio(const Edge &a, const Edge &b, double mid) {
    return (a.profit - mid*a.weight > b.profit - mid*b.weight);
}

bool compareByProfit(const Edge &a, const Edge &b) {
    return a.profit > b.profit; // Sort in descending order of profit
}

int maxProfitMST(int n, std::vector<Edge>& edges) {
    UnionFind uf(n);
    std::sort(edges.begin(), edges.end(), compareByProfit); // Sort by profit

    int totalProfit = 0;
    int edgesUsed = 0;

    for (const Edge& e : edges) {
        if (uf.unite(e.u, e.v)) {
            totalProfit += e.profit;
            ++edgesUsed;
            if (edgesUsed == n - 1) break; // We have used enough edges
        }
    }

    return totalProfit;
}

bool compareByWeight(const Edge &a, const Edge &b) {
    return a.weight < b.weight; // Sort in ascending order of weight
}

int minWeightMST(int n, std::vector<Edge>& edges) {
    UnionFind uf(n);
    std::sort(edges.begin(), edges.end(), compareByWeight); // Sort by weight

    int totalWeight = 0;
    int edgesUsed = 0;

    for (const Edge& e : edges) {
        if (uf.unite(e.u, e.v)) {
            totalWeight += e.weight;
            ++edgesUsed;
            if (edgesUsed == n - 1) break; // We have used enough edges
        }
    }

    return totalWeight;
}

bool kruskal_tree(int N, std::vector<Edge>& edges, double mid, int &profits, int &weights) {
    profits = 0; weights = 0;
    int edgesUsed = 0;
    UnionFind uf(N);
    sort(edges.begin(), edges.end(), [mid](const Edge &a, const Edge &b) { return compareByRatio(a, b, mid); });

    for (const Edge& e : edges) {
        if (uf.unite(e.u, e.v)) {
            profits += e.profit;
            weights += e.weight;
            edgesUsed++;
            if (edgesUsed == N - 1) break;
        }
    }

    return edgesUsed == N - 1; // If we successfully used N-1 edges, then we built a valid MST
}

void binary_search (int N, std::vector<Edge>& edges, int &total_profit, int &total_weight) {
    int profits, weights;
    double mid, low = 0;
    double high = static_cast<double>(maxProfitMST(N, edges)) / minWeightMST(N, edges);

    while (high - low > 1e-6) { //precision
        mid = (low + high) / 2;
        if (kruskal_tree(N, edges, mid, profits, weights) && profits - mid*weights > 0) {
            low = mid; // Increase the ratio
        } else {
            high = mid; // Decrease the ratio
        }
    }
    total_profit = profits;
    total_weight = weights;
}

int main() {
    int N, M, total_profit, total_weight;
    cin >> N >> M;

    vector<Edge> edges;
    for (int i = 0; i < M; ++i) {
        int u, v, p, w;
        cin >> u >> v >> p >> w;
        edges.push_back({u - 1, v - 1, p, w}); // Adjust indices to 0-based
    }

    binary_search(N, edges, total_profit, total_weight);

    int common_div = gcd(total_profit, total_weight);
    cout << total_profit/common_div << " " << total_weight/common_div << endl;

    return 0;
}
