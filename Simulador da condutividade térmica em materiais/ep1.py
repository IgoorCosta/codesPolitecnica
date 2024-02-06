# EP1 - Decomposição LU para Matrizes Tridiagonais

import math


# Essa função recebe uma matriz tridiagonal definida por quatro
# vetores [a, b, c, d] e retorna as soluções x do problema.
# Para isso utiliza a decomposição de dois parâmetros L e U.

def LU_TriDiagonal(a, b, c, d, ciclica):
    n = len(a)
    u = [0] * n
    l = [0] * n
    x = [0] * n
    y = [0] * n

    u[0] = b[0]
    for i in list(range(1, n, 1)):
        # Multiplicadores
        l[i] = a[i]/u[i - 1]
        u[i] = b[i] - l[i]*c[i - 1]

    # Solução de Ly = d
    y[0] = d[0]
    for i in list(range(1, n, 1)):
        y[i] = d[i] - l[i]*y[i - 1]

    # Solução de Ux = y
    x[n - 1] = y[n - 1]/u[n - 1]
    i = n - 2
    while i >= 0:
        x[i] = (y[i] - c[i] * x[i + 1]) / u[i]
        i -= 1
    
    if ciclica == 0:
        apresentar_Parametros(u, l, y, 0, 0, 0, 0)
        
    return x


# Essa função recebe uma matriz tridiagonal cíclica definida 
# por quatro vetores [a, b, c, d] e retorna as soluções x do problema.
# Para isso divide a matriz cíclica em duas partes, uma matriz tridiagonal 
# não cíclica e dois vetores v e w.

def TriDigCiclica(a, b, c, d):
    n = len(a)
    v = [0] * (n-1)
    w = [0] * (n-1)
    
    # Determinação de v
    v[0] = a[0]
    v[n - 2] = c[n - 2]
    # Determinação de w
    w[0] = c[n - 1]
    w[n - 2] = a[n - 1]
    
    a_til = [0] * (n - 1)
    b_til = [0] * (n - 1)
    c_til = [0] * (n - 1)
    d_til = [0] * (n - 1)
    y_til = [0] * (n - 1)
    z_til = [0] * (n - 1)
    x = [0] * n
    
    # Determinação de T
    for i in list(range(0, n - 1, 1)):
        a_til[i] = a[i]
    for i in list(range(0, n - 1, 1)):
        b_til[i] = b[i]
    for i in list(range(0, n - 1, 1)):
        c_til[i] = c[i]
    for i in list(range(0, n - 1, 1)):
        d_til[i] = d[i]

    y_til = LU_TriDiagonal(a_til, b_til, c_til, d_til, 1)
    z_til = LU_TriDiagonal(a_til, b_til, c_til, v, 1)

    x[n - 1] = (d[n - 1] - c[n - 1] * y_til[0] - a[n - 1] * y_til[n-2])/(b[n - 1] - c[n - 1] * z_til[0] - a[n - 1] * z_til[n - 2])

    for i in list(range(0, n - 1, 1)):
        # Determinação das soluções
        x[i] = y_til[i] - (x[n - 1] * z_til[i])
        
    apresentar_Parametros(0, 0, y_til, v, w, z_til, 1)
        
    return x


# Gerador de matrizes tridiagonais cíclicas.

def Gerador_TriDiagonal(n):
    a = [0]*n
    b = [2]*n
    c = [0]*n
    d = [0]*n
    for i in list(range(0, n, 1)):
        j = i + 1
        if j == n:
            a[i] = (2 * j - 1) / (2 * j)
        else:
            a[i] = (2 * j - 1) / (4 * j)
        c[i] = 1 - a[i]
        d[i] = math.cos((2 * math.pi * j * j) / (n * n))
    return a, b, c, d


# Função responsável por apresentar na tela a matriz do problema.

def apresentar_Problema(a, b, c, d, ciclico):
    print("\nProblema: \n")
    i = 0
    while i < len(b): # lINHAS
        j = 0
        while j < len(b): # COLUNAS
            if j == 0:
                print("|", end=' ')
            if i == j:
                print('%.2f' %b[i], end=' ')
            elif i == (j - 1):
                print('%.2f' %c[i], end=' ')
            elif i == (j + 1):
                print('%.2f' %a[i], end=' ')
            elif (ciclico == 1):
                if (j == len(b)-1) and i == 0:
                    print('%.2f' %a[0], end=' ')
                elif (i == len(b)-1) and j == 0:
                    print('%.2f' %c[len(b)-1], end=' ')
                else:
                    print('%.2f' %0, end=' ')
            else:
                print('%.2f' %0, end=' ')
            if j == len(b) - 1:
                print('| * | X%.0f' %(i+1), "|  =  |", '%.2f' %d[i],"|", end='')
            j+=1
        print("\n", end = '')
        i+=1
    print("\n")


# Função responsável por apresentar na tela os parâmetros 
# calculados para a resolução do problema.

def apresentar_Parametros(u, l, y, v, w, z, ciclico):
    n = len(y)
    if (ciclico == 0):
        print("Parâmetros Calculados: \n")
        print("\nU = |",end=' ') 
        for i in list(range(0, n, 1)):
            print('%.2f' %u[i], end='|')
        print("\nL = |",end=' ') 
        for i in list(range(0, n, 1)):
            print('%.2f' %l[i], end='|')
        print("\nY = |",end=' ') 
        for i in list(range(0, n, 1)):
            print('%.2f' %y[i], end='|')
        print("\n")
    else:
        print("Parâmetros Calculados: \n")
        print("\nv = |",end=' ') 
        for i in list(range(0, n, 1)):
            print('%.2f' %v[i], end='|')
        print("\nw = |",end=' ') 
        for i in list(range(0, n, 1)):
            print('%.2f' %w[i], end='|')
        print("\ny = |",end=' ') 
        for i in list(range(0, n, 1)):
            print('%.2f' %y[i], end='|')
        print("\nz = |",end=' ') 
        for i in list(range(0, n, 1)):
            print('%.2f' %z[i], end='|')
        print("\n")


# Função responsável por apresentar na tela a solução do problema.

def apresentar_Solucao(x):
    print("\nSoluções do Problema: \n")
    i = 0
    while i < len(x):
        print('X%.0f = ' %(i+1), '%.12f' %x[i], end='   ')
        print("\n", end = '')
        i+=1
    print("\n")
    return


# Função responsável pela interação com o usuário.

def main():
    opcao = '1'
    while opcao != '3':
        print("\n-------- Métodos Numéricos EP1 --------\n\n" +
              " 1 - Gerador de Matrizes Tridiagonais.\n 2 - Digitar uma Matriz Tridiagonal."
              "\n 3 - Sair.")
        opcao = input(" Digite uma opção: ")
        while opcao != '1' and opcao != '2' and opcao != '3':
            opcao = input("\n Opção Inválida! Digite uma opção: ")
        if opcao == '1':
            while opcao != '1000':
                print("\n-------- Gerador de Matrizes Tridiagonais --------\n")
                print(" 1 - Gerador de Matrizes não Cíclicas.\n 2 - Gerador de Matrizes Cíclicas."
                      "\n 3 - Voltar.")
                opcao = input(" Digite uma opção: ")
                while opcao != '1' and opcao != '2' and opcao != '3':
                    opcao = input("\n Opção Inválida! Digite uma opção: ")
                if opcao == '1':
                    print("\n-------- Gerador de Matrizes não Cíclicas --------")
                    n = input("\nDigite um número para n [n > 0]: ")
                    while int(n) <= 0:
                        n = input("Opção Inválida! Digite um número maior que zero: ")

                    a, b, c, d = Gerador_TriDiagonal(int(n))
                    apresentar_Problema(a, b, c, d, 0)
                    apresentar_Solucao(LU_TriDiagonal(a, b, c, d, 0))

                if opcao == '2':
                    print("\n-------- Gerador de Matrizes Cíclicas --------")
                    n = input("\nDigite um número para n [n > 1]: ")
                    while int(n) <= 1:
                        n = input("Opção Inválida! Digite um número maior que 1: ")

                    a, b, c, d = Gerador_TriDiagonal(int(n))
                    apresentar_Problema(a, b, c, d, 1)
                    apresentar_Solucao(TriDigCiclica(a, b, c, d))

                if opcao == '3':
                    opcao = '1000'
            
        if opcao == '2':
            print("\n-------- Digitar uma Matriz Tridiagonal --------")
            n = int(input("\nDigite o número de linhas/colunas da matriz: "))
            while int(n) <= 1:
                n = int(input("Opção Inválida! Digite um número maior que 1: "))
            ciclica = int(input("\n[0] - Não Cíclica, [1] - Cíclica: "))
            while ciclica != 0 and ciclica != 1:
                ciclica = int(input("Opção Inválida! Digite outro número: "))

            a = [0] * n
            b = [0] * n
            c = [0] * n
            d = [0] * n
            
            print("\nDigite os elementos do vetor 'a': ")
            for i in list(range(0, n, 1)):
                a[i] = float(input())
            print("\n a = ", a)
            
            print("\nDigite os elementos do vetor 'b': ")
            for i in list(range(0, n, 1)):
                b[i] = float(input())
            print("\n b = ", b)
            
            print("\nDigite os elementos do vetor 'c': ")
            for i in list(range(0, n, 1)):
                c[i] = float(input())
            print("\n c = ", c)
            
            print("\nDigite os elementos do vetor 'd': ")
            for i in list(range(0, n, 1)):
                d[i] = float(input())
            print("\n a = ", d)
            
            if (ciclica == 0):
                apresentar_Problema(a, b, c, d, 0)
                apresentar_Solucao(LU_TriDiagonal(a, b, c, d, 0))
            else:
                apresentar_Problema(a, b, c, d, 1)
                apresentar_Solucao(TriDigCiclica(a, b, c, d))

        if opcao == '3':
            print("\n-------- Métodos Numéricos EP1 --------\n\n")
            return

main()




