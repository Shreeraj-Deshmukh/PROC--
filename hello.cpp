#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    vector<int> numbers;
    numbers.push_back(1);
    numbers.push_back(2);
    numbers.push_back(3);

    for (int i = 0; i < numbers.size(); i++) {
        cout << numbers[i] << "\n";
    }

    if (numbers.size() > 0) {
        cout << "Total items: " << numbers.size() << "\n";
        cout << "WELCOME TO PROCLIFE." << "\n";
        cout << "YOU DONT CHOOSE PROCLIFE, PROCLIFE CHOOSES YOU" << "\n";
    } else {
        cout << "Empty vector." << "\n";
        cout << "BYE BYE BOWLEEE.." << "\n";
    }

    return 0;

}
