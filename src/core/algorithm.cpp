#include <iostream>
#include <vector>
#include <format>
using namespace std;

int main()
{
    int n;
    cout << "Masukkan n (grid n x n): ";
    cin >> n;

    vector<vector<int>> matrix(n, vector<int>(n, 0));

    for (vector<int> row : matrix)
    {
        cout << "[ ";
        for (int el : row)
        {
            cout << el << " ";
        }
        cout << "]";
        cout << "\n";
    }

    return 0;
}