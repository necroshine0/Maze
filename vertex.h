#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "help_functions.h"
#include "bijection.h"

using graph_int = std::vector<std::vector<int>>;
const int INF = 10000000;

class Vertex {
private:
    std::pair<int, int> point;
public:
    int  number = -1;
    bool bio_value = false;
    bool is_entry = false;
    bool is_exit = false;
    bool is_feed = false;

    Vertex() {}
    Vertex(int num) { number = num; }
    Vertex(int num, bool bv, bool en, bool ex, bool fd) {
        number = num;
        bio_value = bv;
        is_entry = en;
        is_exit = ex;
        is_feed = fd;
    }
    Vertex(int num, bool bv, bool en, bool ex, bool fd, int x, int y) {
        number = num;
        bio_value = bv;
        is_entry = en;
        is_exit = ex;
        is_feed = fd;
        point.first = x;
        point.second = y;
    }

    const std::pair<int, int> GetPoint() const { return point; }
    const int GetX() const { return point.first; }
    const int GetY() const { return point.second; }

    void SetPoint(int x, int y) {
        point.first = x;
        point.second = y;
    }
    void SetPoint(std::pair<int, int> pt) { point = pt; }
    void SetX(int x) { point.first = x; }
    void SetY(int y) {  point.second = y; }
};

// ����� ������� ���� Vertex
void alert_vertex(const Bjn& bjn,
    const std::vector<std::vector<int>>& tbl, const Vertex& v) {
    std::cout << "�����: " << bjn[v.number] << '\n';
    std::cout << "�����: " << v.number << '\n';
    std::cout << "����������: (" << v.GetX() << ", " << v.GetY() << ")\n";
    std::cout << "������������� ��������: " << v.bio_value << '\n';
    if (v.bio_value) {
        if (v.is_entry)
            std::cout << "����� �������� ������ � ��������\n";
        if (v.is_exit)
            std::cout << "����� �������� ������� �� ���������\n";
        if (v.is_feed)
            std::cout << "����� �������� ���������\n";
    }
    std::cout << "������� �����: ";
    alert(int_to_char(bjn, tbl[v.number]));
    std::cout << '\n';
}

// ����� ������� ������ ���� Vertex
void alert_vertexes(const Bjn& bjn,
    const std::vector<std::vector<int>> tbl, std::vector<Vertex> vs) {
    for (auto it = vs.begin(); it != vs.end(); ++it) {
        alert_vertex(bjn, tbl, *it);
    }
}

class Directions {
private:
    std::vector<std::vector<std::string>> dirs;

public:
    Directions() {}
    Directions(std::vector<Vertex> vs, graph_int mat) {
        size_t n = vs.size();
        dirs.resize(n);
        for (size_t i = 0; i < n; ++i) {
            dirs[i].resize(n);
            for (size_t j = 0; j < n; ++j) {
                Vertex u = vs[i];
                Vertex v = vs[j];
                if (i == j) { dirs[i][j] = ' '; }
                else if (mat[i][j] == INF) { dirs[i][j] = "E"; }
                else if (u.GetX() - v.GetX() == 0) {
                    if (u.GetY() - v.GetY() > 0)
                        dirs[i][j] = "D";
                    else dirs[i][j] = "U";
                }
                else if (u.GetY() - v.GetY() == 0) {
                    if (u.GetX() - v.GetX() > 0)
                        dirs[i][j] = "L";
                    else dirs[i][j] = "R";
                }  // ���������, ���� ����� ����������
                /*else if (u.GetX() != v.GetX() && u.GetY() != v.GetY()){
                    if (u.GetX() > v.GetX() && u.GetY() > v.GetY()) {
                        dirs[i][j] = "UR";
                    }
                    else if (u.GetX() > v.GetX() && u.GetY() < v.GetY()) {
                        dirs[i][j] = "DR";
                    }
                    else if (u.GetX() < v.GetX() && u.GetY() > v.GetY()) {
                        dirs[i][j] = "UL";
                    }
                    else if (u.GetX() < v.GetX() && u.GetY() < v.GetY()) {
                        dirs[i][j] = "DL";
                    }
                }*/
            }
        }
    }

    void PrintDirections(const Bjn& bjn) const {
        std::cout << '\t';
        for (size_t i = 0; i < dirs.size(); ++i) {
            std::cout << bjn[i] << '\t';
        }

        std::cout << '\n';

        for (size_t i = 0; i < dirs.size(); ++i) {
            std::cout << bjn[i] << ":\t";
            for (size_t j = 0; j < dirs[i].size(); ++j) {
                std::cout << dirs[i][j] << "\t";
            }

            std::cout << '\n';
        }
    }

    template <typename T>
    const std::string DirectionBetween(T i, T j) const { return dirs[i][j]; }

    std::vector<std::string> convert_to_dirs(std::vector<int> v) {
        // try catch if v.size is invalid <= 1
        std::vector<std::string> converted;
        converted.reserve(v.size() - 1);
        for (size_t i = 0; i < v.size() - 1; ++i) {
            converted.push_back(this->DirectionBetween(v[i], v[i + 1]));
        }

        return converted;
    };

    template <typename T>
    std::vector<int> convert_to_v(std::vector<std::string> str_v, T start_v) {
        std::vector<int> converted;
        converted.reserve(str_v.size() + 1);
        converted.push_back(start_v);

        int current_v = start_v;
        for (size_t i = 0; i < str_v.size(); ++i) {
            int u = 0;

            while (u < dirs.size() && dirs[current_v][u] != str_v[i]) { ++u; }
            try {
                if (dirs[current_v][u] != str_v[i]) {
                    throw std::invalid_argument("������ ��������������");
                }
            } catch (std::invalid_argument & e) {
                std::cerr << e.what() << ": ";
                std::cerr << "���������� ������������ ������������������\n";
                exit(1);
            }

            converted.push_back(u);
            current_v = u;
        }

        return converted;
    }
};

void switch_directions(std::string& dr, int MODE) {
    if (MODE == 0) {
        if (dr == "U") { dr = "D"; }
        else if (dr == "D") { dr = "U"; }
    }
    else if (MODE == 1) {
        if (dr == "L") { dr = "R"; }
        else if (dr == "R") { dr = "L"; }
    }
    else {
        if (dr == "U") { dr = "D"; }
        else if (dr == "D") { dr = "U"; }
        else if (dr == "L") { dr = "R"; }
        else if (dr == "R") { dr = "L"; }
    }
}

void inverse_dirs(std::vector<std::string>& drs, std::string TYPE) {
    int MODE;
    if (TYPE == "horizontal") {
        MODE = 0;
    }
    else if (TYPE == "vertical") {
        MODE = 1;
    }
    else {
        MODE = 12;
    }

    for (size_t i = 0; i < drs.size(); ++i) {
        switch_directions(drs[i], MODE);
    }
}
