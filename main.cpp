#include <iostream>
#include <algorithm>
#include "fixed_vector.h"

int main() {
    fixed_vector<int, 5> a;
    a.push_back(0);
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    std::sort(a.begin(), a.end());

    a.print();

    // std::sort(a.rbegin(), a.rend());
    a.insert(a.begin() + 1, 4);

    a.print();

    fixed_vector<int, 6> b;
    b = a;

    b.push_back(1);

    a.print();
    b.print();

    swap(a, b);

    a.print();
    b.print();

    return 0;
}