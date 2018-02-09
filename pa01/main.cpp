// main.cpp
// Implement all algorithms in RSA
// Ann Keenan
#include <iostream>
#include <gmp.h>
#include <vector>
#include <string>

// Print a usage statement
void usage() {
  std::cout << "Usage: ./main $p,$q,$e,$m" << std::endl
            << "  $p    first prime number" << std::endl
            << "  $q    second prime number" << std::endl
            << "  $e    public key value" << std::endl
            << "  $m    value of message in RSA encryption" << std::endl;
}

// Parsing the string by delimiter
// Base code from stack overflow @ https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
std::vector<std::string> split(std::string str, std::string delim) {
  size_t pos = 0;
  std::string token;
  std::vector<std::string> strings;
  while ((pos = str.find(delim)) != std::string::npos) {
      token = str.substr(0, pos);
      strings.push_back(token);
      str.erase(0, pos + delim.length());
  }
  strings.push_back(str);
  return strings;
}

int main(int argc, char *argv[]) {
  // Check if input was provided
  if (argc != 2) {
    std::cout << "Error: No input parameter specified" << std::endl;
    usage();
    return 1;
  }

  // Split the input by comma
  std::vector<std::string> values = split(argv[1], ",");

  // Check if four parameters were provided
  if (values.size() != 4) {
    std::cout << "Error: Incorrect format of string" << std::endl;
    usage();
    return 1;
  }

  // Initialize the input parameters as mpz integers
  mpz_t p, q, e, m;
  mpz_inits(p, q, e, m, NULL);
  mpz_set_str(p, values[0].c_str(), 10);
  mpz_set_str(q, values[1].c_str(), 10);
  mpz_set_str(e, values[2].c_str(), 10);
  mpz_set_str(m, values[3].c_str(), 10);

  // Initialize output parameters
  mpz_t n, d, c;
  mpz_inits(n, d, c, NULL);
  // Calculate n
  mpz_mul(n, p, q);
  // Calculate d
  mpz_t tmp1, tmp2, phi; // = (p-1)(q-1)
  mpz_inits(tmp1, tmp2, phi, NULL);
  mpz_sub_ui(tmp1, p, 1);
  mpz_sub_ui(tmp2, q, 1);
  mpz_mul(phi, tmp1, tmp2);
  mpz_invert(d, e, phi);
  // Calculate c
  mpz_powm(c, m, e, n);

  // Output the answers to stdout
  std::cout << "n = ";
  mpz_out_str(stdout, 10, n);
  std::cout << ", d = ";
  mpz_out_str(stdout, 10, d);
  std::cout << ", c = ";
  mpz_out_str(stdout, 10, c);
  std::cout << std::endl;

  return 0;
}
