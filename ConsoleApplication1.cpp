#include <process.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool isPrime(int number) {
    if (number == 1)
        return false;
    for (int i = 2; i * i <= number; i++) {
        if (number % i == 0)
            return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    vector<int> primes;

    if (argc == 1) {

        char psBuffer[128];
        FILE* pPipe[10] = {};

        int offset = 1000;

        for (int i = 0; i < 10; i++) {
            string prog = argv[0];
            string arg1;

            if (i == 0)
                arg1 = to_string(i + 1);
            else
                arg1 = to_string(i * offset + 1);

            string arg2 = to_string((i + 1) * offset);

            string command = "\"" + prog + "\"" + " " + arg1 + " " + arg2;

            pPipe[i] = _popen(command.c_str(), "rt");
        }

        for (int i = 0; i < 10; i++) {
            while (fgets(psBuffer, 128, pPipe[i]) != nullptr) {
                int num = atoi(psBuffer);
                if (num != 0)
                    primes.push_back(num);
            }
            _pclose(pPipe[i]);
        }
    }
    else {
        int a = atoi(argv[1]), b = atoi(argv[2]);
        for (int i = a; i <= b; i++) {
            if (isPrime(i)) {
                cout << i << endl;
            }
        }
    }

    sort(primes.begin(), primes.end());
    for (int prime_num : primes) {
        cout << prime_num << " ";
    }

    return 0;
}