// main.cpp
// Implement all algorithms in ElGamal
// Ann Keenan

#include <iostream>
#include <gmp.h>
#include <vector>
#include <string>
#include <set>

#define KEY 1024
gmp_randstate_t state;

// Print a usage statement
void usage() {
  std::cout << "Usage: ./main $m" << std::endl
            << "  $m    message to be encrypted" << std::endl;
}

int main(int argc, char *argv[]) {
  // Random initialization
  gmp_randinit_default(state);
  gmp_randseed_ui(state, time(NULL));

  // Check if input was provided
  if (argc != 2) {
    std::cout << "Error: No input parameter specified" << std::endl;
    usage();
    return 1;
  }

  // Initialize the input parameter
  mpz_t m;
  mpz_init(m);
  mpz_set_str(m, argv[1], 10);

  // Private key
  mpz_t x;
  mpz_init(x);
  mpz_set_ui(x, 40622201812345);

  // Get a prime number, p = 2q + 1
  mpz_t p, q;
  mpz_inits(p, q, NULL);

  unsigned long int n = KEY;
  mpz_rrandomb(q, state, n);

  int isPrime = 0;
  while (isPrime == 0) {
    mpz_nextprime(q, q);
    // p = 2q + 1
    mpz_mul_ui(p, q, 2);  // p = 2q
    mpz_add_ui(p, p, 1);  // p += 1
    isPrime = mpz_probab_prime_p(p, 50);
  }

  // Encrypt 5 times
  for (int i=0; i<5; i++) {
    // Calculate a random generator
    mpz_t g, tmp1, tmp2;
    mpz_inits(g, tmp1, tmp2, NULL);

    while (1) {
      mpz_urandomm(g, state, p);
      mpz_pow_ui(tmp1, g, 2);  // tmp1 = g^2
      mpz_powm(tmp2, g, q, p);  // tmp2 = g^q

      // g = e, g^2 = e, or g^q = e --> not a generator
      if (mpz_cmp_ui(g, 1) != 0 && mpz_cmp_ui(tmp1, 1) != 0 && mpz_cmp_ui(tmp2, 1) != 0) {
        break;
      }
    }

    // g = g^2 --> avoid attacks based on QR/QNR
    mpz_pow_ui(g, g, 2);

    // Compute h
    mpz_t h;
    mpz_init(h);
    mpz_powm(h, g, x, p);

    // Select random r
    mpz_t r;
    mpz_init(r);
    mpz_urandomm(r, state, p);
    while (mpz_cmp_ui(r, 0) == 0) {  // ensure that r != 0
      mpz_urandomm(r, state, p);
    }

    // Initialize output parameters
    mpz_t c1, c2;
    mpz_inits(c1, c2, NULL);

    // Compute c1
    mpz_powm(c1, g, r, p);

    // Compute c2
    mpz_powm(c2, h, r, p);
    mpz_mul(c2, m, c2);

    // Output the answers to stdout
    mpz_out_str(stdout, 10, c1);
    std::cout << ", ";
    mpz_out_str(stdout, 10, c2);
    std::cout << ", ";
    mpz_out_str(stdout, 10, p);
    std::cout << std::endl << std::endl;
  }

  return 0;
}
