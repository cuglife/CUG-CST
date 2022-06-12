#include <iostream>
#include "Replacement_Algorithm.h"

using namespace std;

int main() {
    vector<int> tasks = generate_instruction();
    vector<int> pages = trans_to_page(tasks);
    cout << fixed << setprecision(4);
    cout << "page blocks    \t FIFO\t\t  LRU\t\t  OPT\t\t" << endl;
    for (int i = 4; i < 33; i++) {
        cout << "    " << i << "\t\t\t";
        cout << FIFO_hit_rate(i, pages) << "\t\t";
        cout << LRU_hit_rate(i, pages) << "\t\t";
        cout << OPT_hit_rate(i, pages) << endl;
    }
    return 0;
}
