#pragma once
#include <iostream>
#include <vector>
#include "vertex.h"
#include "help_functions.h"

#define all(v) (v).begin(), (v).end()
#define forn(i, N) for (size_t i = 0; i != (N); ++i)
#define foran(i, a, N) for (size_t i = a; i != (N); ++i)
#define forin(elem, v) for (auto elem : v)

using graph_int = std::vector<std::vector<int>>;
using tensor_3D = std::vector<std::vector<std::vector<int>>>;
extern const int INF;

class Symmetric_Tensor3D {
private:
    std::vector<std::vector<int>> ST;
    // graph_int dists;
    size_t quantity_V = 0;
    size_t size = 0;

public:
    Symmetric_Tensor3D() {}
    Symmetric_Tensor3D(size_t n) {
        ST.reserve(n * (n + 1) / 2);
    }

    void SetVertsQuantity(size_t V) { quantity_V = V; }

    const std::vector<int> operator[] (size_t i) const { return ST[i]; }
    // const graph_int GetDists() const { return dists; }
    const void PrintPaths(const Bjn& bjn) const {
        for (size_t i = 0; i < size; ++i) {
            int u = ST[i][0];
            int v = ST[i].back();
            std::cout << bjn[u] << "-" << bjn[v] << ":";
            for (auto it = ST[i].begin(); it != ST[i].end(); ++it) {
                std::cout << ' ' << bjn[*it];
            }

            std::cout << '\n';
        }
    }
    // �������� ������� ����������
    std::vector<int> operator() (size_t k, size_t j) const {
        // ���� �����, ������� ������ std::vector<int> ({ k })
        if (k == j) {
            // std::cout << "��������� ������ ������\n";
            return std::vector<int>();
        }

        size_t i;
        std::vector<int> tmp;
        // i < j - ������� ������, ����� (>) ������ � ������� � ���������� ������������ ������
        if (k < j) {
            i = j - (k + 1) + k * quantity_V - k * (1 + k) / 2;
            tmp = ST[i];
        } else {
            i = k - (j + 1) + j * quantity_V - j * (1 + j) / 2;  // [j][k]
            tmp = ST[i];
            std::reverse(all(tmp));
        }

        std::vector<int> path(ST[i].size() - 1);
        foran(t, 1, tmp.size()) { 
            path[t - 1] = tmp[t];
        }

        return path;
    }

    void pb(std::vector<int> v) {
        ST.push_back(v);
        ++size;
    }
};

// �������� ������� ��������� �� ������ � ������ 1
graph_int construct_table(const graph_int& gph) {
    size_t n = gph.size();
    graph_int tbl(n, std::vector<int>(n));

    forn(i, n) {
        tbl[i].assign(n, INF);
        tbl[i][i] = 0;
        forn(j, gph[i].size()) {
            if (i != gph[i][j]) {
                tbl[i][gph[i][j]] = 1;
            }
        }
    }

    return tbl;
}

// �������������: ��������� ��������, ������������� ���������� ��� ���
Symmetric_Tensor3D floyd_warshall(const graph_int& gph) {
    size_t n = gph.size();
    graph_int result = gph;
    graph_int H(n, std::vector<int>(n));

    forn(i, n) {
        forn(j, n) {
            if (gph[i][j] != INF && i != j) {
                H[i][j] = j;
            }
        }
    }

    forn(middle, n) {
        forn(from, n) {
            forn(to, n) {
                if (result[from][middle] < INF && result[middle][to] < INF &&
                    result[from][middle] + result[middle][to] < result[from][to]) {

                    result[from][to] = result[from][middle] + result[middle][to];
                    if (middle != to) {
                        H[from][to] = H[from][middle];
                    }
                }
            }
        }
    }

    Symmetric_Tensor3D recovered(n);
    recovered.SetVertsQuantity(n);

    std::vector<int> path;
    forn(i, n) {
        foran(j, i + 1, n) {
            int k = i;
            while (k != j) {
                path.push_back(k);
                k = H[k][j];
            }
            path.push_back(j);
            recovered.pb(path);
            path.clear();
        }
    }

    return recovered;
}
