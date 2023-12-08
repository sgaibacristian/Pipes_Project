#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
bool isPrime(int n) {
    if (n <= 1)
        return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}
int main() {
    const int numProcesses = 10;
    const int range = 10000;
    const int numbersPerProcess = range / numProcesses;
    int pipes[numProcesses][2];
    for (int i = 0; i < numProcesses; ++i) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }
    for (int i = 0; i < numProcesses; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            close(pipes[i][0]);
            int start = i * numbersPerProcess + 1;
            int end = (i + 1) * numbersPerProcess;
            for (int num = start; num <= end; ++num) {
                if (isPrime(num)) {
                    write(pipes[i][1], &num, sizeof(num));
                }
            }
            close(pipes[i][1]);
            exit(0);
        }
    }
    for (int i = 0; i < numProcesses; ++i) {
        close(pipes[i][1]);
    }
    std::vector<int> primeNumbers;
    int num;
    for (int i = 0; i < numProcesses; ++i) {
        while (read(pipes[i][0], &num, sizeof(num)) > 0) {
            primeNumbers.push_back(num);
        }
        close(pipes[i][0]);
    }
    std::cout << "Prime numbers found: ";
    for (int prime : primeNumbers) {
        std::cout << prime << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < numProcesses; ++i) {
        wait(nullptr);
    }
    return 0;
}
