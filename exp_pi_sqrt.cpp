// exp_pi_sqrt.cpp
// Compile : g++ -O2 exp_pi_sqrt.cpp -o exp_pi_sqrt `pkg-config --cflags --libs flint`

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <flint/flint.h>
#include <flint/fmpz.h>
#include <flint/arb.h>

using namespace std;

int main(int argc, char **argv) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <x-as-decimal-string> <decimal-digits>" << endl;
    return 1;
  }

  const char *x_str = argv[1];
  long D = atol(argv[2]);

  if (D <= 0) {
    cerr << "decimal-digits must be > 0" << endl;
    return 2;
  }

  const double LOG2_10 = 3.32192809488736234787; // 3.321928094887362347870319429489
  slong prec = (slong) ceil(D * LOG2_10) + 128; // Décimales -> bits

  fmpz_t fx;
  fmpz_init(fx);

  if (fmpz_set_str(fx, x_str, 10) != 0) {
    cerr << "Erreur: " << x_str << " n'est pas un entier valide" << endl;
    fmpz_clear(fx);
    return 3;
  }

  char *s, *p;
  char first;
  int count, i, j;
  arb_t a, t, res, pi;
  arb_init(a);
  arb_init(t);
  arb_init(res);
  arb_init(pi);
  arb_const_pi(pi, prec);

  for(i=0;i<2000000;i++) {
    arb_set_fmpz(a, fx);
    arb_sqrt(a, a, prec);
    arb_mul(t, pi, a, prec);
    arb_exp(res, t, prec);
    s = arb_get_str(res, D+5, ARB_STR_NO_RADIUS); // Conversion en chaîne décimale sans intervalle
    p = strchr(s, '.'); // Trouver le point décimal
    if (p) {
      p++;
      first = *p;
      count = 1;

      for (j = 1; p[j] == first; j++)
        count++;

      if(count > 5) {
        cout << "e^(pi*sqrt(";
        fmpz_print(fx);
        cout << ")) with " << D << " decimal digits:" << endl;
        arb_printn(res, D, 0);
        cout << endl << count << " times the digit " << first << endl;
      }
    }
    flint_free(s);
    fmpz_add_ui(fx, fx, 1);
  }

  arb_clear(a);
  arb_clear(t);
  arb_clear(res);
  arb_clear(pi);
  fmpz_clear(fx);

  return 0;
}
