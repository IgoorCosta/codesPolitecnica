/**********************************************************************/
/**                                                                  **/
/**  Igor Costa D'Oliveira                     N�mero USP: 11391446  **/
/**  Exerc�cio - Programa EP1                                        **/
/**  Professor: Yoshiharu Kohayakawa                                 **/
/**  Turma 03                                                        **/
/**                                                                  **/
/**********************************************************************/

/* O programa recebe como entrada uma sequ�ncia de inteiros n com     */
/* 0 < n < 10^8, terminada por um 0 e, para cada inteiro dado, decide */
/* se ele � ou n�o � a soma dos quadrados de 4 primos consecutivos.   */

#include <stdio.h>

int main (int argc, char*argv[])
{
  int n = 1, a, b, p = 0, a1, a2, a3, a4;
  int soma = 0, c = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 0;
  while (n) {
    printf ("Digite um numero (0 < n < 10^8): ");
    scanf ("%d", &n);
    soma = 0, c = 0, x1 = 0, x2 = 0, x3 = 0, x4 = 0;
    for (a = 1; soma < n; a++) {
      for (b = 1; b <= a && p < 3; b++) {
        if (a % b == 0) p++;
        /* Neste ponto, h� a sele��o dos n�meros      */
        /* primos (p = 2), inseridos na vari�vel "a". */
      }
      if (p == 2 && c == 0) {x1 = a; c = 1;}
      else if (p == 2 && c == 1) {x2 = a; c = 2;}
      else if (p == 2 && c == 2) {x3 = a; c = 3;}
      else if (p == 2 && c == 3) {x4 = a; c = 0;}
      /* A vari�vel "c" controla a sequ�ncia dos n�meros primos.*/
      soma = x1*x1 + x2*x2 + x3*x3 + x4*x4;
      p = 0;
    }
    if (c == 0) {a1 = x1; a2 = x2; a3 = x3; a4 = x4;}
    else if (c == 1) {a1 = x2; a2 = x3; a3 = x4; a4 = x1;}
    else if (c == 2) {a1 = x3; a2 = x4; a3 = x1; a4 = x2;}
    else {a1 = x4; a2 = x1; a3 = x2; a4 = x3;}
    /* Insere os n�meros primos [x1, x2, x3, x4] em ordem crescente.*/
    if (n == soma && x4)
      printf("%d = %d^2 + %d^2 + %d^2 + %d^2\n\n", n, a1, a2, a3, a4);
    else if (n)
      printf("Nao ha 4 numeros primos consecutivos cujos quadrados somam %d.\n\n", n);
  }
  return 0;
}
