#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

struct road {
    int u, v;   //connection between cities u and v
    int l;      //time needed to go from city u to city v
};

//run dijkstra algorithm permitting at most <limit> boosts
bool dijkstra_of_k_boosts (int N, int s, int t, int B, int limit, vector<road> &roads) {
    const int inf = 1e10;
    vector<vector<int>> dist(N+1, vector<int>(limit+1, inf)); //dist[i][k] -> time to reach
    dist[s][0] = 0;                                                  //city i using k boosts

    vector<vector<pair<int, int>>> adj(N + 1); //create adjacency matrix
    for (auto& r : roads) {
        adj[r.u].emplace_back(r.v, r.l);
    }

    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;
    pq.emplace(0,s,0); //initial state

    while (!pq.empty()) {
        tuple<int, int, int> top = pq.top();
        pq.pop();
        int time = get<0>(top);
        int node = get<1>(top);
        int k = get<2>(top);

        if (node == t && time <= B) return true;
        if (time > B) continue;

        for (auto& edge : adj[node]) {
            int next = edge.first;
            int duration = edge.second;
            // Normal move (without boost)
            if (time + duration < dist[next][k]) {
                dist[next][k] = time + duration;
                pq.emplace(dist[next][k], next, k);
            }
            // Boosted move (if we have remaining boosts)
            if (k < limit && time < dist[next][k + 1]) {
                dist[next][k + 1] = time;
                pq.emplace(dist[next][k + 1], next, k + 1);
            }
        }
    }
    return false;
}

//calculate shortest path from city s to city t with minimum number of boosts using binary search
int min_boosts_road(int N, int M, int s, int t, int B, vector<road> &roads) {
    int left = 0, right = M, min_boosts = -1;
    while (left <= right) {
        int mid = (left + right)/ 2;
        if (dijkstra_of_k_boosts(N, s, t, B, mid, roads)) {
            min_boosts = mid;
            right = mid-1;
        } else {
            left = mid+1;
        }
    }
    return min_boosts;
}

int main() {
    int N, M, s, t, B;
    cin >> N >> M >> s >> t >> B;
    vector<road> roads(M);
    for (int i = 0; i < M; i++) {
        int u, v, l;
        cin >> u >> v >> l;
        roads[i] = {u, v, l};
    }
    cout << min_boosts_road(N, M, s, t, B, roads) << endl;
    return 0;
}
