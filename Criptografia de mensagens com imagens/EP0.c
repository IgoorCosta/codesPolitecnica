/*******************************************************************/
/**                                                               **/
/**  Igor Costa D Oliveira                  Número USP: 11391446  **/
/**  Exercício - Programa EP0                                     **/
/**  Professor: Yoshiharu Kohayakawa                              **/
/**  Turma 03                                                     **/
/**                                                               **/
/*******************************************************************/

// O programa recebe um número n >= 0 e devolve o seu correspondente
// na sequência de números de Tetranacci [T0 = 0, T1 = 0, T2 = 0,
// T3 = 1, para n > 3 temos Tn = Tn-1 + Tn-2 + Tn-3 +Tn-4].

#include <stdio.h>

int main (int argc, char*argv[])
{
  int n, tetra = 1, i, c = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 1;
  printf ("Digite um numero [n >= 0]: ");
  scanf ("%d", &n);
  if (n < 3) {
    printf ("Como n eh: %d, Tn eh: %d", n, tetra - 1);
  }
  //Recebe n < 3 devolve 0.
  else {
    for (i = 4; i <= n; i++) {
      //Neste ponto, tetra é a soma de quatro números
      //de Tetranacci consecutivos e devolve o valor em Tn.
      tetra = x1 + x2 + x3 + x4;
      if (c == 0) {
        x1 = tetra;
        c = 1;
        //A variável "c" controla a sequência.
      }
      else if (c == 1) {
        x2 = tetra;
        c = 2;
      }
      else if (c == 2) {
        x3 = tetra;
        c = 3;
      }
      else {
        x4 = tetra;
        c = 0;
      }
    }
    printf ("Como n eh: %d, Tn eh: %d", n, tetra);
  }
  return 0;
}
