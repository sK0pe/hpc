#include <iostream>
#include <vector>
#include <omp.h>
#include <random>
using namespace std;
//#define ROW 2
//#define COL 2

double getRandom(){
    static default_random_engine generator{};
    using Dist = uniform_real_distribution<double>;
    static Dist uid {};
    return uid(generator, Dist::param_type{0.0,1.0});
}

void fillMatrix(vector<vector<double>>& matrix){
    for(int r = 0; r < (int)matrix.size(); ++r){
        for(int c = 0; c < (int)matrix[0].size(); ++c){
            matrix[r][c] = getRandom();
        }
    }
}

void sequentialMult(vector<vector<double>>& m1, vector<vector<double>>& m2,
    vector<vector<double>>& result){
        // Error check
        int mat1Rows = m1.size();
        int mat2Cols = m2[0].size();
        int mat2Rows = m2.size();

        //int outerRow = m2[0].size();
        if(mat1Rows != mat2Cols){
            cerr << "Matrix 1 and 2 cannot be multiplied as inner column and row"
                "are not equivalent" << endl;
        }

        int i, j, k;
        for(i = 0; i < mat1Rows; ++i){
            for(j = 0; j < mat2Cols; ++j){
                for(k = 0; k < mat2Rows; ++k){
                    result[i][j] += m1[i][k]*m2[k][j];
                    cout << endl;
                }
            }
        }
}

void printMatrix(vector<vector<double>>& matrix){
    for(auto& i : matrix){
        for(auto & j : i){
            cout << j << ' ';
        }
        cout << endl;
    }
    cout << endl;
}


int main(){

    int row1, col1, row2, col2;
    cout << "Input Rows and Columns in numbers separated by a space for first"
        "matrix" << endl;
    cin >> row1 >> col1;
    cout << "Input Rows and Columns in numbers separated by a space for second"
        "matrix" << endl;
    cin >> row2 >> col2;

    vector<vector<double>> matrix1(row1, vector<double>(col1));
    vector<vector<double>> matrix2(row2, vector<double>(col2));
    vector<vector<double>> result(row1, vector<double>(col2));

    fillMatrix(matrix1);
    fillMatrix(matrix2);
    //matrix1 = {{3, -2, 5},{3, 0, 4}};
    //matrix2 = {{2,3}, {-9, 0}, {0, 4}};

    sequentialMult(matrix1, matrix2, result);
    cout << "mult works\n" << endl;
    printMatrix(matrix1);
    printMatrix(matrix2);
    printMatrix(result);

    return 0;
}
