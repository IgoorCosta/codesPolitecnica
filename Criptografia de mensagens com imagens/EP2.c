/**********************************************************************/
/**                                                                  **/
/** Igor Costa D Oliveira                      Número USP: 11391446  **/
/** Exercício - Programa EP2                                         **/
/** Professor: Yoshiharu Kohayakawa                                  **/
/** Turma 03                                                         **/
/**                                                                  **/
/**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BISSEC_TOL (1e-6)
#define RANDOM_SEED 1234

void ativa_sorteador ()
{
#ifdef RANDOM_SEED
srand(RANDOM_SEED);
#else
srand(time(NULL));
#endif
}

double sorteia_real ()
{
  return (double)rand()/RAND_MAX;
}

int sorteia_voto_com_falha (double f)
{
  if (sorteia_real() <= f) return 1;
  else return 0;
}

double prob_erro (int N, double a, double f, int T)
{
  int simulacoes, eleitores, falha_A, falha_B, erro = 0;
  for (simulacoes = 1; simulacoes <= T; simulacoes++) {
    falha_A = 0; falha_B = 0;
    for (eleitores = 1; eleitores <= (int)(N*a); eleitores++)  /* Calcula o numero de falhas do candidato A para o B */
      falha_A += sorteia_voto_com_falha(f);
    for (eleitores = 1; eleitores <= (int)(N - N*a); eleitores++)  /* Calcula o numero de falhas do candidato B para o A */
      falha_B += sorteia_voto_com_falha(f);
    if (((int)(N*a) - falha_A + falha_B - N/2) <= 0) erro++;  /* Soma 1 quando o candidato A perde a eleicao */
  }
  return ((double)erro/T);
}

double bissecta (int N, double a, int T, double tol)
{
  double fa = 0.0, fb = 1.0, f;
  f = sorteia_real();
  while ((fb - fa) >= BISSEC_TOL)
  {
    if (prob_erro(N, a, f, T) >= tol) {
      fb = f;
      f = ((f + fa) / 2.0);  /* Se f for maior ou igual à tolerancia de erro ele é diminuido */
    }
    else {
      fa = f;
      f = ((f + fb) / 2.0);  /* Se f for menor à tolerancia de erro ele é aumentado */
    }
  }
  return (f);
}

int main (int argc, char*argv[])
{
  int N, T;
  double a, tol;
  ativa_sorteador();
  printf(" Digite o numero de votantes (0 < N <= 2x10^9): ");
  scanf("%d", &N);
  printf(" Digite a porcentagem de votos no candidato A (0.5 < a<= 1): ");
  scanf("%lf", &a);
  printf(" Digite o numero de simulacoes (0 < T <= 2x10^9): ");
  scanf("%d", &T);
  printf(" Digite a probabilidade toleravel de erros (0 <= tol <= 1): ");
  scanf("%lf", &tol);
  printf("\n Maior falha das urnas toleravel: %g", bissecta(N, a, T, tol));
  return 0;
}
