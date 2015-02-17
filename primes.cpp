#include <stdio.h>
#include <vector>
#include <ctime>
#include <iostream>

bool isPrime(int number, std::vector<int> primes){
	bool is_prime = true;

	if (number % 2 == 0) {
		return false;
	}

	for (std::vector<int>::iterator it = primes.begin(); it != primes.end(); ++it){
    	is_prime = is_prime && (number % *it != 0);

			if (!is_prime || *it > (number / 2)) {
				break;
			}
    }

	return is_prime;
}

int main() {

	for (int i = 0; i < 5; ++i) {
		printf("%d\n", i);

		int n;
		// printf("\nEnter the max bound: ");
		// std::cin >> n;
		n = 1000000;

		std::vector<int> primes2;
		primes2.push_back(2);

		long start = clock();

		for (int i = 2; i < n; ++i){
			if (isPrime(i, primes2)){
				primes2.push_back(i);
			}
		}

		long finish = clock();
		double elapsed = double(finish - start) / CLOCKS_PER_SEC;

		/*for (std::vector<int>::iterator it = primes.begin(); it != primes.end(); ++it){
			printf("%d\n", *it);
		}*/

		printf("  Found %d primes under %d in %.3f seconds\n", primes2.size(), n, elapsed);
	}
		return 0;
}
