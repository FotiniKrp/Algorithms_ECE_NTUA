#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int maxInterval(vector<pair<int, int>> &intervals) {
    int maxInt = 0;
    for (const auto& interval : intervals) {
        maxInt = max(maxInt, interval.second - interval.first);
    }
    return maxInt;
}

bool PlaceShops(int N, vector<pair<int, int>>& intervals, int distance) {
    int count = 0;
    int last_placed = -1;

    for (const auto& interval : intervals) {
        int si = interval.first;
        int fi = interval.second;
        int pos = max(si, last_placed + distance); // start from the left-most position possible

        while (pos <= fi) {
            count++; // place shop here
            last_placed = pos; // remember last placed shop's position
            pos += distance; // next shop's position will be "distance" steps away

            if (count == N) break;
        }
        if (count == N) break;
    }

    return count == N; // returns true if all shops are placed
}

int maxMinDistance(int N, vector<pair<int,int>> &intervals) {
    int low = 0, maxMinD = 0, mid;
    int high = maxInterval(intervals);

    sort(intervals.begin(), intervals.end());

    // Binary Search
    while (low <= high) {
        mid = (low + high) / 2;
        if (PlaceShops(N, intervals, mid)) { // if shops can be placed with distance = mid
            maxMinD = mid;
            low = mid + 1; // search for a larger distance
        } else {
            high = mid - 1; // search for a smaller distance
        }
    }

    return maxMinD;
}

int main() {
    int N, M, result;

    cin >> N >> M;
    vector<pair<int, int>> intervals(M);
    for (int i = 0; i < M; ++i) {
        cin >> intervals[i].first >> intervals[i].second;
    }

    result = maxMinDistance(N, intervals);
    cout << result << endl;

    return 0;
}
