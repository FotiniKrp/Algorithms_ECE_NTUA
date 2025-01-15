#include <iostream>
#include <vector>
#include <deque>
using namespace std;

int main()
{   bool removed = false;
    int N, K, max_happiness, tmp;
    cin >> N >> K;

    vector<int> h(N);
    for (int i = 0; i < N; i++) {
        cin >> h[i];
    }

    vector<int> dp(N, 0), old;
    deque<int> q; // Store indices of guests, maintaining happiness in decreasing order

    for (int i = 0; i < N; i++) {
        // Remove guests from the deque who are outside the current window (of length K)
        while (!q.empty() && q.front() < i - K + 1) {
            q.pop_front();
        }
        // Keep the deque in decreasing order of happiness
        while (!q.empty() && h[q.back()] <= h[i]) {
            q.pop_back();
        }

        // Add the current guest to the deque
        q.push_back(i);

        // Iterate over all possible previous positions for the current guest (from i - K + 1 to i)
        for (int j = max(0, i - K + 1); j <= i; j++) {
            // If the front of the deque is invalid for this partition, remove it
            if (!q.empty() && q.front() < j) {
                tmp = q.front();
                old.push_back(tmp);
                q.pop_front();
                removed = true;
            }
            // Get the maximum happiness for this partition
            max_happiness = h[q.front()];
            // Calculate the previous happiness
            int previous_h = (j > 0) ? dp[j - 1] : 0;
            // Update dp[i] with the best option, considering this guest sitting at the table from j to i
            dp[i] = max(dp[i], previous_h + (i - j + 1) * max_happiness);


        }
        // If elements were removed from the deque earlier, restore them
        if (removed) {
            while(!old.empty()) {
                tmp = old.back();
                old.pop_back();
                q.push_front(tmp);
            }
            removed = false;
        }

    }

    cout << dp[N - 1] << endl; // Max total happiness for N guests

    return 0;
}
