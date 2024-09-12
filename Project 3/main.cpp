#include <iostream>
#include <vector>
#include "kdtree.hpp"
using namespace std;

int main() {
    vector<pair<tuple<int, int>, int>> construct = {make_pair(make_tuple(2, 3), 4), make_pair(make_tuple(4, 3), 2), make_pair(make_tuple(5, 7), 9), make_pair(make_tuple(9, 7), 7)};
    KDTree<tuple<int, int>, int> tree(construct);
    cout << "constructor with vector: ";
    for (auto &it: tree) cout << get<0>(it.first) << " " << get<1>(it.first) << " " << it.second << "     ";
    cout << endl;
    tree.insert(make_tuple(2, 3), 1);
    tree.insert(make_tuple(5, 4), 2);
    tree.insert(make_tuple(9, 6), 3);
    tree.insert(make_tuple(4, 8), 4);
    tree.insert(make_tuple(8, 1), 5);
    tree.insert(make_tuple(7, 2), 6);
    cout << "insert: ";
    for (auto &it: tree) cout << get<0>(it.first) << " " << get<1>(it.first) << " " << it.second << "     ";
    cout << endl;
    KDTree tree2 = tree;
    KDTree tree3(tree2);
    cout << "copy constructor: ";
    for (auto &it: tree2) cout << get<0>(it.first) << " " << get<1>(it.first) << " " << it.second << "     ";
    cout << endl;
    for (auto &it: tree3) cout << get<0>(it.first) << " " << get<1>(it.first) << " " << it.second << "     ";
    cout << endl;
    cout << "find max: ";
    auto max = tree.findMax<1>();
    cout << get<0>(max->first) << " " << get<1>(max->first) << " " << max->second << endl;
    cout << "find min: ";
    auto min = tree.findMin<0>();
    cout << get<0>(min->first) << " " << get<1>(min->first) << " " << min->second << endl;
    cout << "find: ";
    auto found = tree.find(make_tuple(8, 1));
    cout << found->second << endl;
    if (tree.erase(make_tuple(7, 2))) {
        for (auto &it: tree) cout << get<0>(it.first) << " " << get<1>(it.first) << " " << it.second << "     ";
        cout << endl;
    }
    tree.erase(make_tuple(5, 6));
    tree.find(make_tuple(5, 9));
    tree.find(make_tuple(8, 1));
    tree.insert(make_tuple(2, 3), 999);
    tree.findMin<1>();
    tree.findMax<0>();
    tree.insert(make_tuple(7, 8), 0);
    tree.insert(make_tuple(3, 4), 988);
    for (auto &it: tree) cout << get<0>(it.first) << " " << get<1>(it.first) << " " << it.second << "     ";
    cout << endl;
    vector<pair<tuple<int, int>, int>> te;
    KDTree<tuple<int, int>, int> tree4(te);
    for (auto &it: tree4) cout << get<0>(it.first) << " " << get<1>(it.first) << " " << it.second << "     ";
    cout << endl;
    tree4.insert(make_tuple(1, 2), 4);
    for (auto &it: tree4) cout << get<0>(it.first) << " " << get<1>(it.first) << " " << it.second << "     ";
    cout << endl;
    tree4.erase(make_tuple(1, 2));
    tree4.find(make_tuple(3, 3));
    tree4.insert(make_tuple(4, 5), 90);
    tree4.erase(make_tuple(3, 4));
    tree4.findMin<1>();
    tree4.findMax<0>();
    for (auto &it: tree4) cout << get<0>(it.first) << " " << get<1>(it.first) << " " << it.second << "     ";
    cout << endl;
    return 0;
}