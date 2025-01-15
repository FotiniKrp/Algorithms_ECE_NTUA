#include <iostream>
#include <vector>
#include <climits>
using namespace std;

void compute_energy(vector<vector<int>> &Energy, const vector<vector<int>>& A, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = i; j < N; j++) {
            for (int x = i; x < j; x++) {
                for (int y = x + 1; y <= j; y++) {
                    Energy[i][j] += A[x][y];
                }
            }
        }
    }
}

int main() {
    int N, K;
    cin >> N >> K;

    vector<vector<int>> A(N, vector<int>(N, 0));

    for (int i = 0; i < N-1; i++) {
        for (int j = i+1; j < N; j++) {
            cin >> A[i][j];
        }
    }
    vector<vector<int>> dp(K, vector<int>(N, INT_MAX)); // dp[k-1][n-1] represents the min energy for n substances into k bottles
                                                               // Initialize it with a significantly large value (2^10)

    vector<vector<int>> Energy(N, vector<int>(N, 0)); // Energy[i][j] holds accumulated energy for bottle containing i to j substances
    compute_energy(Energy, A, N);

    for (int k = 0; k < K; k++) {
        for (int n = 0; n < N; n++) {
            if (k == 0) // There's only one bottle -> all the substances go together
                dp[k][n] = Energy[0][n];
            else {
                for (int i = 0; i < n; i++) {
                    if (dp[k-1][i] != INT_MAX)
                        dp[k][n] = min(dp[k][n], dp[k-1][i] + Energy[i+1][n]);
                    // We examine all possible partitions up to substance i:
                    // Substances 0 to i are placed into the first k-1 bottles,
                    // and the substances from i+1 to n are placed into the k-th bottle.
                    // We choose the partition with the minimum total energy.
                }
            }
        }
    }
    cout << dp[K-1][N-1] << endl; // Output the min Energy for N substances into K bottles
}
