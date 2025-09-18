#include <iostream>
#include <vector>
using namespace std;
int main() {
    int n, a, b;
    cin >> n;
    vector<int> vec(n);
    vector<int> p(n);
    int presum = 0;
    for (int i = 0; i < n; i++) {
        cin >> vec[i];
        presum += vec[i];
        p[i] = presum;
    }

    while (cin >> a >> b) {
        int sum;
        if (a == 0) sum = p[b];
        else sum = p[b] - p[a - 1];
        cout << sum << endl;
    }
}