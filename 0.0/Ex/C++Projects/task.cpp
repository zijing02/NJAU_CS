#include <bits/stdc++.h>
using namespace std;
int n, a[100005], maxVal = 0;
int main()
{

    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
        for (int j = i; j >= 0; j--)
            maxVal = max(maxVal, (a[i] ^ a[j]));
    }
    cout << maxVal;
    return 0;
}