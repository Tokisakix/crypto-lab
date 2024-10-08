#include <iostream>
#include <vector>
#include <random>

using namespace std;

typedef vector<vector<int>> matrix;

matrix ATA;
int m = 0, n = 0, det_inv = 0;
const int inverseTable[26] = {
    0, 1, 0, 9, 0, 21, 0, 15, 0, 3, 0, 19, 0,
    0, 0, 7, 0, 23, 0, 11, 0, 5, 0, 17, 0, 25
};

int cal_det(const matrix& mat) {
    int size = mat.size();
    if(size == 1)
        return mat[0][0] % 26;

    int det = 0;
    for (int col = 0; col < size; col++) {
        matrix submat(size - 1, vector<int>(size - 1));
        for (int i = 1; i < size; i++) {
            int sub_j = 0;
            for (int j = 0; j < size; j++) {
                if (j == col)
                    continue;
                submat[i - 1][sub_j++] = mat[i][j];
            }
        }
        det += (col % 2 == 0 ? 1 : -1) * mat[0][col] * cal_det(submat);
    }

    return (det % 26 + 26) % 26;
}

matrix cal_adj(const matrix& mat) {
    int size = mat.size();
    matrix adj_mat(size, vector<int>(size));
    
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            matrix submat(size - 1, vector<int>(size - 1));
            int sub_i = 0;
            for (int i = 0; i < size; i++) {
                if (i == row) continue;
                int sub_j = 0;
                for (int j = 0; j < size; j++) {
                    if (j == col) continue;
                    submat[sub_i][sub_j++] = mat[i][j];
                }
                sub_i++;
            }
            adj_mat[col][row] = ((row + col) % 2 == 0 ? 1 : -1) * cal_det(submat) % 26;
            adj_mat[col][row] = (adj_mat[col][row] + 26) % 26;
        }
    }
    return adj_mat;
}

matrix cal_inv(const matrix& mat, int det_inv) {
    int size = mat.size();
    matrix adj_mat = cal_adj(mat);
    matrix inv_mat(size, vector<int>(size));
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            inv_mat[i][j] = (adj_mat[i][j] * det_inv) % 26;
            inv_mat[i][j] = (inv_mat[i][j] + 26) % 26;
        }
    }
    
    return inv_mat;
}

matrix matmul(const matrix& A, const matrix& B) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int colsB = B[0].size();

    matrix res(rowsA, vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) {
                res[i][j] = (res[i][j] + A[i][k] * B[k][j]) % 26;
            }
        }
    }

    return res;
}

matrix matsub(const matrix& A, const matrix& B) {
    int rowsA = A.size();
    int colsB = B[0].size();

    matrix res(rowsA, vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            res[i][j] = (A[i][j] - B[i][j] + 26) % 26;
        }
    }

    return res;
}

int main() {
    string decode, encode;
    cin >> m; cin.get();
    getline(cin, decode);
    getline(cin, encode);

    matrix X(m, vector<int>(m));
    matrix Y(m, vector<int>(m));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            X[i][j] = decode[i * m + j] - 'A';
            Y[i][j] = encode[i * m + j] - 'A';
        }
    }

    n = encode.size() / m;
    matrix A = matrix(n, vector<int>(m));
    matrix B = matrix(n, vector<int>(m));
    matrix inv_A = matrix(m, vector<int>(n));

    random_device random;
    mt19937 gen(random());
    uniform_int_distribution<> dist(0, n - 1);

    while (true) {
        for (int i = 0; i < n; ++i) {
            int idx = dist(gen);
            for (int j = 0; j < m; ++j) {
                A[i][j] = (decode[i * m + j] - decode[idx * m + j] + 26) % 26;
                B[i][j] = (encode[i * m + j] - encode[idx * m + j] + 26) % 26;
                inv_A[j][i] = A[i][j];
            }
        }

        ATA = matmul(inv_A, A);
        det_inv = inverseTable[cal_det(ATA)];
        if (det_inv) break;
    }

    matrix C = matmul(matmul(cal_inv(ATA, det_inv), inv_A), B);
    matrix D = matsub(Y, matmul(X, C));

    int col = C[0].size();
    for (const auto& row : C) {
        for (int i = 0; i < col - 1; ++i) {
            cout << row[i] << " ";
        }
        cout << row[col - 1] << endl;
    }
    for (int i = 0; i < m; ++i) {
        cout << D[0][i] << " ";
    }
    cout << endl;

    return 0;
}
