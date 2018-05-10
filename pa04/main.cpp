// main.cpp
// Implement BLS signature
// Ann Keenan
#include <gmp.h>
#include <pbc.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define MAXBUF 1024
gmp_randstate_t state;


// -------------------- Print usage statement ---------------------------------
void usage() {
  std::cout << "Usage: ./main" << std::endl;
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
  if (argc != 2) {
    std::cout << "Usage: ./main [input file] < a.param" << std::endl;
    exit(1);
  }

  // Random initialization
  gmp_randinit_default(state);
  gmp_randseed_ui(state, time(NULL));

  pairing_t pairing;
  char param[MAXBUF];
  size_t count = fread(param, sizeof(char), MAXBUF, stdin);
  pairing_init_set_buf(pairing, param, count);
  fclose(stdin);
  fflush(stdin);

  // -------------------- Input -----------------------------------------------
  element_t g1;
  element_t public_key, secret_key;
  element_t sig2;
  element_t M, M2;
  element_t h, h2;

  element_init_G1(g1, pairing);
  element_init_G1(public_key, pairing);
  element_init_Zr(secret_key, pairing);

  element_init_G1(sig2, pairing);

  element_init_Zr(M, pairing);
  element_init_Zr(M2, pairing);

  element_init_G1(h, pairing);
  element_init_G1(h2, pairing);

  // -------------------- File input ------------------------------------------
  std::ifstream infile(argv[1]);
  std::string line;

  std::vector<std::string> tmp_vec;

  // $g1
  std::getline(infile, line);
  element_set_str(g1, line.c_str(), 10);
  // element_printf("g1 = %B\n", g1);

  // $m,$sk
  std::getline(infile, line);
  tmp_vec = split(line, ",");

  char *buf1 = new char[tmp_vec[0].length() + 1];
  strcpy(buf1, tmp_vec[0].c_str());
  // printf("message = %s\n", buf1);
  element_from_hash(M, buf1, tmp_vec[0].length());
  // element_printf("H(message) = %B\n", M);
  delete [] buf1;

  element_set_str(secret_key, tmp_vec[1].c_str(), 10);
  // element_printf("secret key = %B\n", secret_key);

  // $m′,$σ′,$pk′
  std::getline(infile, line);
  tmp_vec = split(line, ",");

  char *buf2 = new char[tmp_vec[0].length() + 1];
  strcpy(buf2, tmp_vec[0].c_str());
  // element_printf("H(message') = %B\n", m2);
  element_from_hash(M2, buf2, tmp_vec[0].length());
  // element_printf("H(message') = %B\n", M2);
  delete [] buf2;

  element_set_str(sig2, (tmp_vec[1] + "," + tmp_vec[2]).c_str(), 10);
  element_set_str(public_key, (tmp_vec[3] + "," + tmp_vec[4]).c_str(), 10);
  // element_printf("σ' = %B\n", sig2);
  // element_printf("public key' = %B\n", public_key);

  // -------------------- Calculations ----------------------------------------
  element_t sig;
  element_init_G1(sig, pairing);

  element_pow_zn(h, g1, M);
  // element_printf("h = %B\n", h);
  element_pow_zn(sig, h, secret_key);
  // element_printf("σ = %B\n", sig);

  element_t tmp, tmp1, tmp2;
  element_init_G1(tmp, pairing);
  element_init_GT(tmp1, pairing);
  element_init_GT(tmp2, pairing);

  // e(g^H(m),pk) = e(σ,g2)
  element_pairing(tmp1, sig2, g1);
  // element_printf("f(σ', g1) = %B\n", tmp1);
  element_pow_zn(h2, g1, M2);
  element_pairing(tmp2, h2, public_key);
  // element_printf("f(H(m'), public key') = %B\n", tmp2);

  int b = 0;
  if (!element_cmp(tmp1, tmp2)) {
    b = 1;
  }

  // -------------------- Output ----------------------------------------------
  element_printf("%B,%B\n", sig, g1);
  printf("%d\n", b);

  // -------------------- Cleanup ---------------------------------------------
  element_clear(tmp2);
  element_clear(tmp1);
  element_clear(sig);
  element_clear(h);
  element_clear(M2);
  element_clear(M);
  element_clear(sig2);
  element_clear(secret_key);
  element_clear(public_key);
  element_clear(g1);

  return 0;
}
