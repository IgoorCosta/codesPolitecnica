/*************************************************************************************************/
/**                                                                                             **/
/**  Igor Costa D Oliveira                                                Número USP: 11391446  **/
/**  Exercício - Programa EP3                                                                   **/
/**  Professor: Yoshiharu Kohayakawa                                                            **/
/**  Turma 03                                                                                   **/
/**                                                                                             **/
/*************************************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#define MAX 400
#define MAX2 160000
#define FNMAX 200

int LeDesenho (char nomearq[FNMAX], int M[MAX][MAX], int *pm, int *pn, int *pmax);
int LeTexto (char nomearq[FNMAX], char T[MAX2], int *pk);
int BeDe (int k, int m, int n, int *pb, int *pd);
int ProximosBBits (char T[MAX2], int b, int *pik, int *pib);
void Codifica (int D[MAX][MAX], int m, int n, char T[MAX2], int k, int Dl[MAX][MAX], int b, int d, int modo);
int Maximo (int D[MAX][MAX], int m, int n);
int EscreveDesenho (char nomearq[FNMAX], int M[MAX][MAX], int m, int n, int max);
void DeBeDe (int D[MAX][MAX], int Dl[MAX][MAX], int m, int n, int *pb, int *pd);
int DeCodifica (int D[MAX][MAX], int Dl[MAX][MAX], int m, int n, int b, int d, char T[MAX2], int modo);
int EscreveTexto (char nomearq[FNMAX], char T[MAX2], int k);

int min_max (int a, int b, int c);  /* Recebe dois inteiros e devolve o menor ou o maior */
int Multiplo_de_8 (int pb);  /* Recebe um inteiro e devolve um inteiro multiplo de 8 igual ou maior que o recebido*/
int F_Expon (int b);  /* Recebe um inteiro e devolve o numero 2 elevado a ele */
int F_Conv_Binario_Decimal (int num, int x);  /* Recebe um numero binario e o devolve em decimal */

int main (int argc, char*argv[])
{
  int opcao, pm, pn, pk, pd, pb, pmax, Pmax, Dm, Dn, i, j, verbor = 0, x = 0;
  static int M[MAX][MAX], D[MAX][MAX], Dl[MAX][MAX];
  char nomearq[FNMAX], T[MAX2];
  do {
    for (i = 0; i < MAX; i++)
      for (j = 0; j < MAX; j++) {     /* Todos os valores das matrizes sao zerados */
        M[i][j] = 0;
        D[i][j] = 0;
        Dl[i][j] = 0;
      }
    printf("Digite a operacao desejada:\n   1. Codificar\n   2. Decodificar\n   3. Mostrar mensagem\n");
    printf("   4. Ativar modo verborragico\n   0. Finalizar\n\nOPCAO: ");
    scanf("%d", &opcao);
    while (opcao < 0 || opcao > 4) {
      printf(" ERRO: Opcao invalida. Digite novamente: ");
      scanf("%d", &opcao);
    }
    if (opcao == 1) {
      printf("Arquivo que contem o desenho original: ");
      if (LeDesenho (nomearq, M, &pm, &pn, &pmax) == 0) {
        printf("m = %d n = %d\nArquivo que contem a mensagem a ser ocultada: ", pm, pn);
        if (LeTexto (nomearq, T, &pk) == 0) {
          printf(" k = %d\n", pk);
          if (BeDe (pk, pm, pn, &pb, &pd) == 0) {
            printf(" b = %d d = %d\n", pb, pd);
            for (i = 0; i < pm; i++)               /* Atualiza os valores de D e Dl para    */
              for (j = 0; j < pn; j++) {           /* serem utilizados nas proximas funcoes */
                D[i][j] = M[i][j];
                Dl[i][j] = M[i][j];
              }
            Codifica (D, pm, pn, T, pk, Dl, pb, pd, verbor);
            for (i = 0; i < pm; i++)
              for (j = 0; j < pn; j++) {           /* Atualiza os valores de D e M para     */
                M[i][j] = Dl[i][j];                /* serem utilizados nas proximas funcoes */
                D[i][j] = Dl[i][j];
              }
            Pmax = Maximo (D, pm, pn);
            pmax = min_max (pmax, Pmax, 1);
            if (EscreveDesenho (nomearq, M, pm, pn, pmax) == 0) printf("\n");
            else printf("\n\n ERRO: Operacao abortada pois mensagem nao pode ser ocultada\n\n");
          }
          else printf("\n\n ERRO: Operacao abortada pois mensagem nao pode ser ocultada\n\n");
        }
        else printf(" ERRO: Operacao abortada devido a erro na leitura\n\n\n");
      }
      else printf(" ERRO: Operacao abortada devido a erro na leitura\n\n\n");
    }
    else if (opcao == 2) {
      printf("Arquivo que contem o desenho original: ");
      if (LeDesenho (nomearq, M, &pm, &pn, &pmax) == 0) {
        for (i = 0; i < pm; i++)
          for (j = 0; j < pn; j++) {     /* A matriz D recebe a matriz do primeiro arquivo que */
            D[i][j] = M[i][j];           /* foi colocada na matriz M pela funcao LeDesenho.    */
          }
        Dm = pm;   /* Sao salvos os valores de pm e pn do primeiro arquivo*/
        Dn = pn;
        printf(" m = %d n = %d\nArquivo que contem o desenho esteganografado: ", pm, pn);
        if (LeDesenho (nomearq, M, &pm, &pn, &pmax) == 0) {
          for (i = 0; i < pm; i++)
            for (j = 0; j < pn; j++) {      /* A matriz Dl recebe a matriz do segundo arquivo que */
              Dl[i][j] = M[i][j];           /* foi colocada na matriz M pela funcao LeDesenho.    */
            }
          printf(" m = %d n = %d\n", pm, pn);
          if ((Dm == pm) && (Dn == pn)) {         /* As matrizes devem possuir o mesmo tamanho */
            DeBeDe (D, Dl, pm, pn, &pb, &pd );
            printf(" b = %d d = %d\n", pb, pd);
            pk = DeCodifica (D, Dl, pm, pn, pb, pd, T, verbor);
            printf(" k = %d\nGravar mensagem decodificada em: ", pk);
            scanf("%s", nomearq);
            if (EscreveTexto (nomearq, T, pk ) == 0) printf("\n");
            else printf(" ERRO: Operacao abortada devido a erro na escrita\n\n\n");
          }
          else printf("\n ERRO: Operacao abortada. Desenhos devem ser do mesmo tamanho\n\n\n");
        }
        else printf(" ERRO: Operacao abortada devido a erro na leitura\n\n\n");
      }
      else printf(" ERRO: Operacao abortada devido a erro na leitura\n\n\n");
    }
    else if (opcao == 3) {
      printf(" Arquivo que contem a mensagem: ");
      if (LeTexto (nomearq, T, &pk) == 0)
        printf("k = %d\n\n MENSAGEM contida no arquivo %s (tudo entre ---> e <---)\n--->%s<---\n\n\n", pk, nomearq, T);
      else
        printf(" ERRO: Operacao abortada devido a erro na leitura\n\n");
    }
    else if (opcao == 4) {
      if (verbor == 0) {
        printf(" Modo verborragico ativado\n\n");
        verbor = 1;
      }
      else {
        printf(" Modo verborragico desativado\n\n");
        verbor = 0;
      }
    }
    else return 0;
  } while (x == 0);
  return 0;
}

int LeDesenho (char nomearq[FNMAX], int M[MAX][MAX], int *pm, int *pn, int *pmax) {
  FILE *fp;
  int i = 0, j = 0;
  char Vetor[2];

  scanf("%s", nomearq);
  if (!(fp = fopen (nomearq, "r"))) {
    printf ("Erro na abertura do arquivo\n\n");
    return 1;
  }
  fscanf(fp, "%s", Vetor);
  fscanf(fp, "%d", &(*pn));
  fscanf(fp, "%d", &(*pm));
  fscanf(fp, "%d", &(*pmax));
  if (*pm > 400 || *pn > 400) {  /* Imprime erro para arquivos que excedem o tamanho maximo */
    printf ("O arquivo supera os valores de MAX\n\n");
    fclose (fp);
    return 1;
  }
  if (Vetor[0] != 80 /*P*/|| Vetor[1] != 50 /*2*/) {  /* Imprime erro para arquivos diferentes do pgm */
    printf ("Formato desconhecido\n\n");
    fclose (fp);
    return 1;
  }
  while (i < *pm) {
    fscanf(fp, "%d", &M[i][j]);
    if ((M[i][j] < 0) || (M[i][j] > 255)) {
      printf("Formato ou dado incorreto, valor desconhecido = %d\n\n", M[i][j]);
      fclose (fp);
      return 1;
    }
    j++;
    if (j == *pn) { i++; j = 0;}
  }
  fclose (fp);
  return 0;
}

int LeTexto (char nomearq[FNMAX], char T[MAX2], int *pk ) {
  FILE *fp;
  *pk = 0;

  scanf("%s", nomearq);
  if (!(fp = fopen(nomearq, "r"))){
    printf ("Erro na abertura do arquivo\n\n");
    return 1;
  }
  while ((*pk <= MAX2) && (fscanf(fp, "%c", &T[*pk]) > 0)) {
    if (T[*pk] < 0) {  /* Imprime erro para valores desconhecidos na tabela ASCII */
      printf("Caractere %c desconhecido. Codigo do caractere: %d\n\n", T[*pk], T[*pk]);
      fclose (fp);
      return 1;
    }
    *pk = *pk + 1;
  }
  fclose (fp);
  T[*pk] = '\0';     /* Indica que a mensagem acabou */
  if (*pk < MAX2) return 0;
  else {
    printf ("Mensagem muito longa\n\n");
    return 1;
  }
}

int BeDe (int k, int m, int n, int *pb, int *pd ) {
  int cont = 0, d;
  double resto, res;

  resto = (8*k*1.0)/(m*n);
  *pb = (8*k)/(m*n);                  /* Formulas para encontrar o valor de b */
  if (resto > 0.0001) *pb = *pb + 1;
  if (*pb > 8) {
    printf("\n ERRO: Nao e possivel ocultar os %d caracteres da mensagem", k);
    return 1;
  }
  *pb = Multiplo_de_8 (*pb);

  res = ((1.0*8*k + *pb*1.0)/ *pb*1.0);
  for (d = 1; cont == 0; d++) {
    if (((m/d)*(n/d))*1.0 - res >= -0.0001) {       /* Formulas para encontrar o valor de d */
      d++;
      if (((m/d)*(n/d))*1.0 - res < -0.0001)
        cont++;
      d--;
    }
    else {
      printf("\n ERRO: Nao e possivel ocultar os %d caracteres da mensagem", k);
      return 1;
    }
  }
  d--;
  if (d > min_max (m,n,0)) *pd = min_max (m,n,0);
  else *pd = d;
  return 0;
}

int ProximosBBits (char T[MAX2], int b, int *pik, int *pib ) {
  int inteiro, i, res = 0, Byte[8];
  inteiro = T[*pik];
  for (i = 0; i <= 7; i++) {           /* Converte o caracterer  */
    if (inteiro%2 == 0) Byte[i] = 0;   /* T em um numero binario */
    else Byte[i] = 1;                  /* no vetor Byte          */
    inteiro = inteiro/2;
  }
  if (b == 1) {
    res = Byte[*pib];
    *pib = *pib + 1;
  }
  else if (b == 2) {
    res = Byte[*pib + 1]*2 + Byte[*pib];
    *pib = *pib + 2;
  }
  else if (b == 4) {
    res = Byte[*pib+3]*8 + Byte[*pib+2]*4 + Byte[*pib+1]*2 + Byte[*pib];
    *pib = *pib + 4;
  }
  if (b == 8) {
    res = T[*pik];
    *pik = *pik + 1;
  }
  else if (*pib == 8) {    /* Avanca para o proximo caracterer */
    *pib = 0;              /* apos a leitura total do anterior */
    *pik = *pik + 1;
  }
  return (res);
}

void Codifica (int D[MAX][MAX], int m, int n, char T[MAX2], int k, int Dl[MAX][MAX], int b, int d, int modo ) {
  int i, j, pik = 0, pib = 0, bits = 0, original = 0, codificado = 0, fab = 0;

  bits = b;
  original = D[d - 1][d - 1];                                                          /* Codifica os parametros */
  codificado = (((original/F_Expon(b))*F_Expon(b)) + ((original + bits)%F_Expon(b)));  /* b e d na matriz Dl     */
  Dl[d - 1][d - 1] = codificado;

  for (i = 1; i <= m/d; i++){
    for (j = 1; j <= n/d; j++){
      if (i != 1 || j != 1) {     /* i = 1 e j = 1 foi utilizado para codificar o b e o d */
        bits = ProximosBBits (T, b, &pik, &pib);
        original = D[i*d - 1][j*d - 1];
        codificado = (((original/F_Expon (b))*F_Expon (b)) + ((original + bits)%F_Expon (b)));  /* Codifica os caracteres */
        if (codificado < 0) codificado = -codificado;                                           /* na funcao Dl           */
        Dl[i*d - 1][j*d - 1] = codificado;
      }
      if (modo == 1)
        printf(" (%d,%d) bits %d original %.2x codificado %.2x\n", i*d - 1, j*d - 1, bits, original, codificado);
      fab++;
      if (fab >= k*(8/b)+ 1) {    /* Interrompe o processo quando acabar todos os caracteres de k */
        i = m/d + 1;
        j = n/d + 1;
      }
    }
  }
  if (modo == 1) printf("\n");
}

int Maximo (int D[MAX][MAX], int m, int n ) {
  int i, j, a, b = 0;
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++) {
      a = D[i][j];
      if (a > b) b = a;
    }
  return (b);
}

int EscreveDesenho (char nomearq[FNMAX], int M[MAX][MAX], int m, int n, int max ) {
  FILE *fc;
  int i, j;

  printf("Guardar desenho esteganografado em: ");
  scanf("%s", nomearq);
  if (!(fc = fopen (nomearq, "w"))) {
    printf ("Erro na impressao do arquivo\n\n");
    return 1;
  }
  fprintf(fc, "P2\n%d %d\n%d\n", n, m, max);
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      if (M[i][j] >= 100) fprintf (fc, "%d ", M[i][j]);        /* Imprime os valores da */
      else if (M[i][j] >= 10) fprintf (fc, "0%d ", M[i][j]);   /* matriz M com ate três */
      else fprintf (fc, "00%d ", M[i][j]);                     /* casas apos a virgula  */
    }                                                          /* no arquivo            */
    fprintf(fc, "\n");
  }
  fclose(fc);
  return 0;
}

void DeBeDe (int D[MAX][MAX], int Dl[MAX][MAX], int m, int n, int *pb, int *pd ) {
  int i, j, k = 0;
  *pb = 0; *pd = 0;
  for (i = 0; i < min_max (m,n,0); i++)
    if ((Dl[i][i] - D[i][i] != 0) && (k == 0)) {
      for (j = 0; j < 4; j++) {  /* Formula para encontrar o parametro b */
        if (j == 0) *pb = 1;
        else *pb = 2*j;
        if (*pb == ((Dl[i][i] - D[i][i] + 256)%F_Expon (*pb))) j = 4;
      }
      *pd = i + 1;
      k++;
    }
}

int DeCodifica (int D[MAX][MAX], int Dl[MAX][MAX], int m, int n, int b, int d, char T[MAX2], int modo ) {
  int i, a = 0, j, k = 0, g = 0, num = 0, res = 0;

  if (d == 0) return 0;  /* Se as imagens forem iguais retorna 0 */
  for (i = d - 1; i < m; i = i + d) {
    for (j = d - 1; j < n; j = j + d) {
      if (((i != d - 1) || (j != d - 1)) && (a == 0)) {  /* i=d-1 e j=d-1 eh o pixel que oculta b e d */
        num = (((Dl[i][j] - D[i][j]) + 256) % (F_Expon(b)));
        res = res + F_Conv_Binario_Decimal (num, g);
        if (b == 1) g++;
        else if (b == 2) g = g + 2;               /* Formulas para imprimir em T os  */
        else if (b == 4) g = g + 4;               /* caracteres ocultados nas        */
        else if (b == 8) {                        /* matrizes D e Dl dependendo do b */
          T[k] = num;
          if (num == 0) {i = m; j = n;}
          else k++;
        }
        if (g == 8) {
          g = 0;
          T[k] = res;
          if (res == 0) a = 1;
          else {
            k++;
            res = 0;
          }
        }
      }
      if (modo == 1) printf(" (%d,%d) bits %d original %.2x codificado %.2x\n", i, j, num, D[i][j], Dl[i][j]);
    }
  }
  return (k);
}

int EscreveTexto (char nomearq[FNMAX], char T[MAX2], int k ) {
  FILE *fc;
  int i = 0;

  if (!(fc = fopen (nomearq, "w"))) {
    printf ("Erro na impressao do arquivo\n\n");
    return 1;
  }
  for (i = 0; i < k; i++) {
    if (T[i] < 0) {
      printf("Caractere %c desconhecido. Codigo do caractere: %d\n\n", T[i], T[i]); fclose(fc);
      return 1;
    }
    fprintf(fc, "%c", T[i]);
  }
  fclose(fc);
  return 0;
}

int Multiplo_de_8 (int pb) {
   if (pb == 1) return (pb);
   else if ((pb%2 == 0)&&(pb!=6)) return (pb);
   else {
     if (pb == 3) return 4;
     return 8;
  }
}

int min_max (int a, int b, int c) {
  if (c == 0) {
    if (a > b) return (b);
    else return (a);
  }
  else {
    if (a > b) return (a);
    else return (b);
  }
}

int F_Expon (int b) {
  int i, a = 1;
  for (i = 1; i <= b; i++) a = a*2;
  return (a);
}

int F_Conv_Binario_Decimal (int num, int x) {
  if (x == 0) return (num);
  else if (x == 1) return (num*2);
  else if (x == 2) return (num*4);
  else if (x == 3) return (num*8);
  else if (x == 4) return (num*16);
  else if (x == 5) return (num*32);
  else if (x == 6) return (num*64);
  else if (x == 7) return (num*128);
  return 0;
}
