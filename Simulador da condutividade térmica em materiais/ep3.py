# função que determina os nós em um intervalo de 0 até L
def nos(n, L):
    x = [0] * (n + 2)  # declara o vetor dos nós iniciado com zeros
    i = 0
    while i != (n + 2):
        x[i] = (L * i) / (n + 1)  # divide o intervalo em n + 1 partições e cada nó recebe um múltiplo dessa partição
        i = i + 1
    return x

# função que resolve matrizes tridiagonais pelo método lu
def LUTriDig(a, b, c, d):
    n = len(a)  # tem o tamanho dos vetores
    u = [0] * n  # declara e inicializa os vetores de coeficientes de resultados
    l = [0] * n
    x = [0] * n
    y = [0] * n

    u[0] = b[0]
    for i in list(range(1, n, 1)):
        l[i] = a[i] / u[i - 1]  # determinação dos termos na matriz diagonal superior e na inferior
        u[i] = b[i] - l[i] * c[i - 1]
    # solução de Ly = d
    y[0] = d[0]
    for i in list(range(1, n, 1)):
        y[i] = d[i] - l[i] * y[i - 1]

    # solução de Ux = y
    x[n - 1] = y[n - 1] / u[n - 1]

    i = n - 2
    while i >= 0:
        x[i] = (y[i] - c[i] * x[i + 1]) / u[i]
        i -= 1
    return x

# função que cria as funções chapéu
def chapeu(n, L, x):
    no = nos(n, L)
    phi = [0] * (n + 2)  # vetor que retorna a função chpéu pra cada i
    dphi = [0] * (n + 2)  # vetor que retorna a derivada da função chpéu pra cada i
    i = 1
    while i != (n + 1):  # implementação da lógica da função
        if no[i - 1] <= x <= no[i]:
            phi[i - 1] = (x - no[i - 1]) * (n + 1) / L  # descreve uma reta crescente
            dphi[i - 1] = (n + 1) / L
        elif no[i] <= x <= no[i + 1]:
            phi[i - 1] = (no[i + 1] - x) * (n + 1) / L  # descreve uma reta decrescente
            dphi[i - 1] = -1 * (n + 1) / L
        else:
            phi[i] = 0 # fora do intervalo a função assume valor 0
            dphi[i] = 0
        i = i + 1

    return phi, dphi

# função que define o parâmetro de condutividade térmica dependendo do exercício
def k(x, ex): 
    if ex == 1:
        f = 1
    elif ex == 2:
        f = 2.718281828459045235 ** x
    elif ex == 3:
        f = 3600
    return f

# função que define o parâmetro q dependendo do exercício
def q(x):
    f = 0
    return f

# função que define o parâmetro f dependendo do exercício
def f(x, ex): 
    if ex == 1:
        res = 12 * x * (1 - x) - 2
    elif ex == 2:
        res = 1 + (2.718281828459045235 ** x)
    elif ex == 3:
        res = 37500000 * (2.718281828459045235 ** (-((x-0.01)**2)))
        res = res - (7500000 * ((2.718281828459045235 ** (-(x**2)/25))+(2.718281828459045235 ** (-(x-0.02)**2)/25)))
    return res

# função que define o parâmetro da derivada de f dependendo do exercício
def df(x, ex):
    if ex == 1:
        res = 12 - 24 * x
    elif ex == 2:
        res = 2.718281828459045235 ** x
    elif ex == 3:
        res = 37500000 * (2.718281828459045235 ** (-((x-0.01)**2)))*(-2*x+0.02)
        res = res - (7500000 * (((-2*x/25)*2.718281828459045235 ** (-(x**2)/25))+(((-2*x+0.04)/25)*2.718281828459045235 ** (-(x-0.02)**2)/25)))
    return res

# função pra determinar o vetor das diagonais laterais da matriz de coeficintes
def vetorA(n, L, ex):
    no = nos(n, L)
    vA = [0] * n
    aux1 = [0] * n
    aux2 = [0] * n

    j = 0
    while j != (n - 1):
        c = no[j + 2]
        b = no[j + 1]
        a = no[j]

        A1 = (b - a) / 2  # determina o peso da aproximação de gauss
        A2 = (c - b) / 2

        x1 = (b + a) / 2 - ((b - a) / (2 * 1.732050808))  # determina o x1 e o x2 nós na aproximação de gauss
        x2 = (b + a) / 2 + ((b - a) / (2 * 1.732050808))

        x3 = (b + c) / 2 - ((c - b) / (2 * 1.732050808))  # determina o x3 e o x4 nós na aproximação de gauss
        x4 = (b + c) / 2 + ((c - b) / (2 * 1.732050808))

        phi1, dphi1 = chapeu(n, L, x1)  # calcula a função chapéu pra x1
        phi2, dphi2 = chapeu(n, L, x2)  # calcula a função chapéu pra x2
        phi3, dphi3 = chapeu(n, L, x3)  # calcula a função chapéu pra x3
        phi4, dphi4 = chapeu(n, L, x4)  # calcula a função chapéu pra x4

        aux1[j] = A1 * ((k(x1, ex) * dphi1[j] * dphi1[j + 1] + q(x1) * phi1[j] * phi1[j + 1]) + (
                k(x2, ex) * dphi2[j + 1] * dphi2[j] + q(x2) * phi2[j + 1] * phi2[j]))
        aux2[j] = A2 * ((k(x3, ex) * dphi3[j] * dphi3[j + 1] + q(x3) * phi3[j] * phi3[j + 1]) + (
                k(x4, ex) * dphi4[j + 1] * dphi4[j] + q(x4) * phi4[j + 1] * phi4[j]))
        vA[j + 1] = aux1[j] + aux2[j]
        j = j + 1

    return vA

# função pra determinar o vetor da diagonal principal da matriz de coeficintes
def vetorB(n, L, ex): 
    no = nos(n, L)
    vB = [0] * n
    aux1 = [0] * n
    aux2 = [0] * n

    j = 0
    while j != n:
        c = no[j + 2]
        b = no[j + 1]
        a = no[j]

        A1 = (b - a) / 2  # determina o peso da aproximação de gauss
        A2 = (c - b) / 2

        x1 = (b + a) / 2 - ((b - a) / (2 * 1.732050808))  # determina o x1 e o x2 nós na aproximação de gauss
        x2 = (b + a) / 2 + ((b - a) / (2 * 1.732050808))

        x3 = (b + c) / 2 - ((c - b) / (2 * 1.732050808))
        x4 = (b + c) / 2 + ((c - b) / (2 * 1.732050808))

        phi1, dphi1 = chapeu(n, L, x1)  # calcula a função chapéu pra x1
        phi2, dphi2 = chapeu(n, L, x2)  # calcula a função chapéu pra x2
        phi3, dphi3 = chapeu(n, L, x3)  # calcula a função chapéu pra x3
        phi4, dphi4 = chapeu(n, L, x4)  # calcula a função chapéu pra x4

        aux1[j] = A1 * ((k(x1, ex) * dphi1[j] * dphi1[j] + q(x1) * phi1[j] * phi1[j]) + (
                k(x2, ex) * dphi2[j] * dphi2[j] + q(x2) * phi2[j] * phi2[j]))
        aux2[j] = A2 * ((k(x3, ex) * dphi3[j] * dphi3[j] + q(x3) * phi3[j] * phi3[j]) + (
                k(x4, ex) * dphi4[j] * dphi4[j] + q(x4) * phi4[j] * phi4[j]))
        vB[j] = (aux1[j] + aux2[j])
        j = j + 1

    return vB

# função pra determinar o vetor das diagonais laterais da matriz de coeficintes
def vetorC(n, L, ex):
    no = nos(n, L)
    vC = [0] * n
    aux1 = [0] * n
    aux2 = [0] * n

    j = 0
    while j != n:
        c = no[j + 2]
        b = no[j + 1]
        a = no[j]

        A1 = (b - a) / 2  # determina o peso da aproximação de gauss
        A2 = (c - b) / 2

        x1 = (b + a) / 2 - ((b - a) / (2 * 1.732050808))  # determina o x1 e o x2 nós na aproximação de gauss
        x2 = (b + a) / 2 + ((b - a) / (2 * 1.732050808))

        x3 = (b + c) / 2 - ((c - b) / (2 * 1.732050808))  # determina o x3 e o x4 nós na aproximação de gauss
        x4 = (b + c) / 2 + ((c - b) / (2 * 1.732050808))

        phi1, dphi1 = chapeu(n, L, x1)  # calcula a função chapéu pra x1
        phi2, dphi2 = chapeu(n, L, x2)  # calcula a função chapéu pra x2
        phi3, dphi3 = chapeu(n, L, x3)  # calcula a função chapéu pra x3
        phi4, dphi4 = chapeu(n, L, x4)  # calcula a função chapéu pra x4

        aux1[j] = A1 * ((k(x1, ex) * dphi1[j] * dphi1[j + 1] + q(x1) * phi1[j] * phi1[j + 1]) + (
                k(x2, ex) * dphi2[j + 1] * dphi2[j] + q(x2) * phi2[j + 1] * phi2[j]))
        aux2[j] = A2 * ((k(x3, ex) * dphi3[j] * dphi3[j + 1] + q(x3) * phi3[j] * phi3[j + 1]) + (
                k(x4, ex) * dphi4[j + 1] * dphi4[j] + q(x4) * phi4[j + 1] * phi4[j]))
        vC[j] = aux1[j] + aux2[j]
        j = j + 1

    return vC

# função pra determinar o vetor das respostas
def vetorD(n, L, ex): 
    no = nos(n, L)
    vD = [0] * n
    j = 0
    while j != n:
        c = no[j + 2]
        b = no[j + 1]
        a = no[j]

        A1 = (b - a) / 2  # determina o peso da aproximação de gauss
        A2 = (c - b) / 2  # determina o peso da aproximação de gauss

        x1 = (b + a) / 2 - ((b - a) / (2 * 1.732050808))  # determina o x1 e o x2 nós na aproximação de gauss
        x2 = (b + a) / 2 + ((b - a) / (2 * 1.732050808))
        x3 = (b + c) / 2 - ((c - b) / (2 * 1.732050808))  # determina o x1 e o x2 nós na aproximação de gauss
        x4 = (b + c) / 2 + ((c - b) / (2 * 1.732050808))

        phi1, dphi1 = chapeu(n, L, x1)  # calcula a função chapéu pra x1
        phi2, dphi2 = chapeu(n, L, x2)  # calcula a função chapéu pra x2
        phi3, dphi3 = chapeu(n, L, x3)  # calcula a função chapéu pra x1
        phi4, dphi4 = chapeu(n, L, x4)  # calcula a função chapéu pra x2
        vD[j] = A1 * ((phi1[j] * f(x1, ex)) + (phi2[j] * f(x2, ex))) + A2 * (
                (phi3[j] * f(x3, ex)) + (phi4[j] * f(x4, ex)))
        j = j + 1
    return vD

# função pra determinar Un(x) das respostas dependendo do exercício
def Un(n, L, alfa, x):
    i = 0
    soma = 0
    i_chapeu, di_chapeu = chapeu(n, L, x)
    while i != n:
        soma = soma + alfa[i] * i_chapeu[i]
        i = i + 1
    return soma

# função pra determinar U(x) das respostas dependendo do exercício
def U(x, ex): 
    if ex == 1:
        res = x * x * (1 - x) * (1 - x)
    elif ex == 2:
        res = (x - 1)*(-1+(2.718281828459045235**(-1*x)))
    elif ex == 3:
        res = 293.15 + (250*x/3) - (12500*x*x/3)
    return res

# função pra determinar o erro e Un(x) e U(x) das respostas
def resolucao(n, x, ex):
    a_t = vetorA(n, 1, ex)
    b_t = vetorB(n, 1, ex)
    c_t = vetorC(n, 1, ex)
    d_t = vetorD(n, 1, ex)
    alfas = LUTriDig(a_t, b_t, c_t, d_t)
    print("Un(", x,") = ", Un(n, 1, alfas, x))
    print("U(", x,") =", U(x, ex))
    no = nos(n, 1)
    i = 0
    e_max = 0
    while i != n: # Determina o erro máximo
        e = U(no[i], ex) - Un(n, 1, alfas, no[i])
        if e < 0:
            e = e * -1
        if e > e_max:
            e_max = e
        i = i + 1
    print("Erro máximo = ", e_max)
    
# função pra determinar o erro e Un(x) e U(x) das respostas do ex 3
def resolucao3(n, x, L, ex):
    a_t = vetorA(n, L, ex)
    b_t = vetorB(n, L, ex)
    c_t = vetorC(n, L, ex)
    d_t = vetorD(n, L, ex)
    alfas = LUTriDig(a_t, b_t, c_t, d_t)
    print("Un(", x,") = ", Un(n, L, alfas, x)+293.15)
    print("U(", x,") =", U(x, ex))
    no = nos(n, L)
    i = 0
    e_max = 0
    while i != n: # Determina o erro máximo
        e = U(no[i], ex) - (Un(n, L, alfas, no[i])+293.15)
        if e < 0:
            e = e * -1
        if e > e_max:
            e_max = e
        i = i + 1
    print("Erro máximo = ", e_max)

# função main que é responsável pela interface com o usuário
def main():
    ex = '0'
    while ex != '4':
        print("\n-------- Métodos Numéricos EP3 --------\n\n"
              "1 - Validação: f(x) = 12x(x-1) - 2.\n2 - Validação: f(x) = "
              "e^x - 1.\n3 - Equilíbrio com forças de calor.\n4 - Sair.")
        ex = input("Digite uma opção: ")
        while ex != '1' and ex != '2' and ex != '3'and ex != '4':
            ex = input("Opção Inválida! Digite uma opção válida: ")
        if ex == '1':
            while ex != '1000':
                print("\n-------- Validação: f(x) = 12x(x-1) - 2 --------")
                print("\n1 - Escolher um 'N' e um 'x'.\n2 - Usar parâmetros do enunciado."
                      "\n3 - Voltar.")
                ex = input("Digite uma opção: ")
                while ex != '1' and ex != '2' and ex != '3':
                    ex = input("\n Opção Inválida! Digite uma opção válida: ")
                if ex == '1':
                    n = int(input("\nEscolha uma quantidade N de nós: "))
                    x = float(input("Escolha um valor para x entre 0 e 1 como argumento das funções: "))
                    print("\nParamêtros utilizados:\n\na = 0 \nb = L = 1\nq(x) = 0"
                          "\nk(x) = 1\nf(x) = 12 * x * (x - 1) - 2\nu(x) = x^2 * (1 - x)^2\n\nResultados:\n")
                    resolucao(n, x, 1)                        
                if ex == '2':
                    print("\nParamêtros utilizados:\n\na = 0 \nb = L = 1\nq(x) = 0"
                          "\nk(x) = 1\nf(x) = 12 * x * (x - 1) - 2\nu(x) = x^2 * (1 - x)^2\n")
                    print("Resultados para n = 7:")
                    resolucao(7, 0.5, 1)
                    print("\nResultados para n = 15:")
                    resolucao(15, 0.5, 1)
                    print("\nResultados para n = 31:")
                    resolucao(31, 0.5, 1)
                    print("\nResultados para n = 63:")
                    resolucao(63, 0.5, 1)
                    
                if ex == '3':
                    ex = '1000'
        if ex == '2':
            while ex != '1000':
                print("\n-------- Validação: f(x) = e^x - 1 --------")
                print("\n1 - Escolher um 'N' e um 'x'.\n2 - Usar parâmetros do enunciado."
                      "\n3 - Voltar.")
                ex = input("Digite uma opção: ")
                while ex != '1' and ex != '2' and ex != '3':
                    ex = input("\n Opção Inválida! Digite uma opção válida: ")
                if ex == '1':
                    n = int(input("\nEscolha uma quantidade N de nós: "))
                    x = float(input("Escolha um valor para x entre 0 e 1 como argumento das funções: "))
                    print("\nParamêtros utilizados:\n\na = 0 \nb = L = 1\nq(x) = 0"
                          "\nk(x) = e^x\nf(x) = e^x + 1\nu(x) = (x - 1) * (e^(-x)- 1 )\n\nResultados:\n")
                    resolucao(n, x, 2)                        
                if ex == '2':
                    print("\nParamêtros utilizados:\n\na = 0 \nb = L = 1\nq(x) = 0"
                          "\nk(x) = e^x\nf(x) = e^x + 1\nu(x) = (x - 1) * (e^(-x)- 1 )\n")
                    print("Resultados para n = 7:")
                    resolucao(7, 0.5, 2)
                    print("\nResultados para n = 15:")
                    resolucao(15, 0.5, 2)
                    print("\nResultados para n = 31:")
                    resolucao(31, 0.5, 2)
                    print("\nResultados para n = 63:")
                    resolucao(63, 0.5, 2)
                    
                if ex == '3':
                    ex = '1000'
        if ex == '3':
            while ex != '1000':
                print("\n-------- Equilíbrio com forças de calor --------")
                print("\n1 - Usar parâmetros do enunciado."
                      "\n2 - Voltar.")
                ex = input("Digite uma opção: ")
                while ex != '1' and ex != '2':
                    ex = input("\n Opção Inválida! Digite uma opção válida: ")                       
                if ex == '1':
                    print("\nParamêtros utilizados:\n\nL = 0.02\nx = 0.01\n")
                    print("Resultados para n = 7:")
                    resolucao3(7, 0.01, 0.02, 3)
                    print("\nResultados para n = 15:")
                    resolucao3(15, 0.01, 0.02, 3)
                    print("\nResultados para n = 31:")
                    resolucao3(31, 0.01, 0.02, 3)
                    print("\nResultados para n = 63:")
                    resolucao3(63, 0.01, 0.02, 3)
                    
                if ex == '2':
                    ex = '1000'
                    
main()
