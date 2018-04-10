// main.cpp
// Implement all algorithms in the Paillier Cryptosystem
// Ann Keenan

#include <iostream>
#include <gmp.h>
#include <vector>
#include <string>

#define KEY 1024
gmp_randstate_t state;

// 727757385905277549635344566785720456227488993539217332209406514238269168145357161923958494470932641752762595482266246664554402902190139072495584221111755684636577499112220753818839112000931462027220857354699321957775173031045329976388542589805501217226657421150770807011772100966987884468483567276534098907001322698532062869030803516349551251616426055134021611318478821111985245148183929661581153845832405670527636302756782594768695975380157918772184828982372426768122927326762772839344004527392628365745851808341689124883882320109959479466716310133213869461456895515236890312734449215131092776105969,1275376998371541682303366726923860319339736622720761546565933182693014480972312707210564818003127869589338633606641131192032526401474432091170599748787,1275376998371541682303366726923860319339736622720761546565933182693014480972312707210564818003127869589338633606641131192032526401474432091170599748849
// 1523036736

// -------------------- Print usage statement ---------------------------------
void usage() {
  std::cout << "Usage: ./main $c,$p,$q" << std::endl
            << "  $c    public key value" << std::endl
            << "  $p    first prime number" << std::endl
            << "  $q    second prime number" << std::endl;
}

// -------------------- Parse a string by delimiter ---------------------------
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
  // Random initialization
  gmp_randinit_default(state);
  gmp_randseed_ui(state, time(NULL));

  // -------------------- Input -----------------------------------------
  // Check for command line input
  if (argc != 2) {
    std::cout << "Error: No input parameter specified" << std::endl;
    usage();
    return 1;
  }

  // Split the input by comma
  std::vector<std::string> values = split(argv[1], ",");

  // Check for valid format of input string
  if (values.size() != 3) {
    std::cout << "Error: Incorrect format of string" << std::endl;
    usage();
    return 1;
  }

  // Input message
  std::string msg;
  std::cin >> msg;

  // -------------------- Initialize input parameters--------------------------
  mpz_t c, p, q, m2;
  mpz_inits(c, p, q, m2, NULL);
  mpz_set_str(c, values[0].c_str(), 10);
  mpz_set_str(p, values[1].c_str(), 10);
  mpz_set_str(q, values[2].c_str(), 10);
  mpz_set_str(m2, msg.c_str(), 10);

  // Initialize tmp parameters
  mpz_t tmp1, tmp2;
  mpz_inits(tmp1, tmp2, NULL);

  // Common parameters
  mpz_t n, n2;
  mpz_inits(n, n2, NULL);
  mpz_mul(n, p, q);  // n = pq
  mpz_pow_ui(n2, n, 2);  // n2 = n^2

  // -------------------- Key generation --------------------------------------
  mpz_t g, lambda, mu;
  mpz_inits(g, lambda, mu, NULL);

  // g = 1 + n
  mpz_add_ui(g, n, 1);
  // lambda = phi = (p-1)(q-1)
  mpz_sub_ui(tmp1, p, 1);  // tmp1 = p - 1
  mpz_sub_ui(tmp2, q, 1);  // tmp2 = q - 1
  mpz_mul(lambda, tmp1, tmp2);
  // mu = phi^-1 mod n = lambda^-1 mod n
  mpz_invert(mu, lambda, n);

  // -------------------- Encryption ------------------------------------------
  mpz_t c2, r;
  mpz_inits(c2, r, NULL);

  while (1) {
    mpz_urandomm(r, state, n);
    if (mpz_cmp_ui(r, 0) == 0) continue;
    break;
  }

  // c = g^m*r^n mod(n^2)
  mpz_powm(tmp1, g, m2, n2);  // tmp2 = g^m mod n^2
  mpz_powm(tmp2, r, n, n2);  // tmp3 = r^n mod n^2
  mpz_mul(c2, tmp1, tmp2);
  mpz_mod(c2, c2, n2); // c2 = c2 mod n^2

  // -------------------- Decryption ------------------------------------------
  mpz_t m;
  mpz_init(m);
  // m = ((c^lambda - 1) mod(n^2))/n * mu mod(n)
  mpz_powm(m, c, lambda, n2);
  mpz_sub_ui(m, m, 1);
  mpz_cdiv_q(m, m, n);
  mpz_mul(m, m, mu);
  mpz_mod(m, m, n);

  // -------------------- Output ----------------------------------------------
  std::cout << std::endl;
  mpz_out_str(stdout, 10, m);
  std::cout << std::endl << std::endl;
  mpz_out_str(stdout, 10, c2);
  std::cout << ",";
  mpz_out_str(stdout, 10, p);
  std::cout << ",";
  mpz_out_str(stdout, 10, q);
  std::cout << std::endl;

  return 0;
}
