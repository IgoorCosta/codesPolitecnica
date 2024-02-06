import math  # importação das bibliotecas

# armazenamento dos nós e pesos para n = 6 , n = 8 e  n = 10
nos_n6 = ([-0.9324695142031520278123016, -0.6612093864662645136613996, -0.2386191860831969086305017,
           0.2386191860831969086305017, 0.6612093864662645136613996, 0.9324695142031520278123016])

nos_n8 = ([-0.9602898564975362316835609, -0.7966664774136267395915539, -0.5255324099163289858177390,
           -0.1834346424956498049394761, 0.1834346424956498049394761, 0.5255324099163289858177390,
           0.7966664774136267395915539, 0.9602898564975362316835609])

nos_n10 = ([-0.9739065285171717200779640, -0.8650633666889845107320967, -0.6794095682990244062343274,
            -0.4333953941292471907992659, -0.1488743389816312108848260, 0.1488743389816312108848260,
            0.4333953941292471907992659, 0.6794095682990244062343274, 0.8650633666889845107320967,
            0.9739065285171717200779640])

peso_n6 = ([0.1713244923791703450402961, 0.3607615730481386075698335, 0.4679139345726910473898703,
            0.4679139345726910473898703, 0.3607615730481386075698335, 0.1713244923791703450402961])

peso_n8 = ([0.1012285362903762591525314, 0.2223810344533744705443560, 0.3137066458778872873379622,
            0.3626837833783619829651504, 0.3626837833783619829651504, 0.3137066458778872873379622,
            0.2223810344533744705443560, 0.1012285362903762591525314])

peso_n10 = ([0.0666713443086881375935688, 0.1494513491505805931457763, 0.2190863625159820439955349,
             0.2692667193099963550912269, 0.2955242247147528701738930, 0.2955242247147528701738930,
             0.2692667193099963550912269, 0.2190863625159820439955349, 0.1494513491505805931457763,
             0.0666713443086881375935688])


def funcao(x, y, ex):  # função para facilitar a atribuição da função a ser integrada
    # a variável EX recebida pelo usuário define qual o exercício ,logo :
    # a função no integrando e os seus extremos de integração também

    if ex == 1.0:
        f = 1  # ex = 1.0 define a F no cálculo do volume do cubo
    elif ex == 1.1:
        f = -x - y + 1  # ex = 1.1 define a F no cálculo do volume do tetraedro

    elif ex == 2.0:  # ex = 2.0 define a F no cálculo da Aréa integrando em Y depois em X
        f = 1
    elif ex == 2.1:  # ex = 2.1 define a F no cálculo da Aréa integrando em X depois em Y
        f = 1

    elif ex == 3.0:  # ex = 3.0 define a F para o cálculo da superfície da função no ex 3.1
        f = (((2.718282**(2 * y / x)) * (x * x + y * y) + x ** 4) ** (1 / 2)) / (x ** 2)
    elif ex == 3.1:  # ex = 3.1 define a F para o cálculo do volume na região R
        f = 2.718282 ** (y / x)
    elif ex == 4.0:  # ex = 4.0 define a F que é a distância até o eixo de revolução Y
        f = math.sqrt(1 - y ** 2)
    elif ex == 4.1:  # ex = 4.1 define a F que é a distância até o eixo de revolução Y
        f = f = 2.718282**(-1 * (y * y))
    return f


# define o limite inferior de integração da primeira integral
# note que os limites variam com os exercícios
def limite_a(ex):
    if ex == 1.0:
        f = 0
    elif ex == 1.1:
        f = 0

    elif ex == 2.0:
        f = 0
    elif ex == 2.1:
        f = 0

    elif ex == 3.0:
        f = 0.1
    elif ex == 3.1:
        f = 0.1

    elif ex == 4.0:
        f = 0.75
    elif ex == 4.1:
        f = -1
    return f


# define o limite superior de integração da primeira integral
# note que os limites variam com os exercícios
def limite_b(ex):
    if ex == 1.0:
        f = 1
    elif ex == 1.1:
        f = 1

    elif ex == 2.0:
        f = 1
    elif ex == 2.1:
        f = 1

    elif ex == 3.0:
        f = 0.5
    elif ex == 3.1:
        f = 0.5

    elif ex == 4.0:
        f = 1
    elif ex == 4.1:
        f = 1

    return f


# define o limite inferior de integração da segunda integral
# note que os limites variam com os exercícios
def limite_c(x, ex):
    if ex == 1.0:  # note que os extremos dependem de uma variável X agora
        f = 0
    elif ex == 1.1:
        f = 0

    elif ex == 2.0:
        f = 0
    elif ex == 2.1:
        f = 0

    elif ex == 3.0:
        f = x * x * x

    elif ex == 3.1:
        f = x * x * x

    elif ex == 4.0:
        f = 0
    elif ex == 4.1:
        f = 0
    return f


# define o limite superior de integração da segunda integral
# note que os limites variam com os exercícios
def limite_d(x, ex):  # note que os extremos dependem de uma variável X agora
    if ex == 1.0:
        f = 1
    elif ex == 1.1:
        f = -x + 1

    elif ex == 2.0:
        f = 1 - x * x
    elif ex == 2.1:
        f = (1 - x) ** (1 / 2)

    elif ex == 3.0:
        f = x * x
    elif ex == 3.1:
        f = x * x

    elif ex == 4.0:
        f = (1 - x ** 2) ** 1 / 2 #  expressão para x retirada da equação da circunferência
    elif ex == 4.1:
        f = 2.718282**(-1 * (x * x))
    return f


def equacao_gauss_2_integrais(nos, peso, limit, ex):
    i = 0  # declara uma variável oomo contadora
    y = 0  # declara uma variável para armazenar o valor da funçaõ após a mudança de variável
    xi = [0] * 10  # vetor com os novos nós
    ui = [0] * 10  # vetor com os novos pesos

    while i != limit:
        # desloca os nós linearmnete para outro intervalo e os armazena em um vetor
        xi[i] = ((limite_a(ex) + limite_b(ex)) / 2) + ((limite_b(ex) - limite_a(ex)) * nos[i] / 2)
        # multiplica os pesos por um fator de escalaa e os armazena em um vetor
        ui[i] = peso[i] * (limite_b(ex) - limite_a(ex)) / 2
        # calcula o valor da função para a nova variável
        y = y + funcao(xi[i], 0, ex) * ui[i]
        i += 1  # incrementa o contador

    I = 0  # declaração da variável que armazenará o valor final da integração
    i = 0  # declaração das variáveis contadoras
    j = 0
    yij = [[0] * 10] * 10  # declaração de uma matriz para receber os n² nós
    vij = [[0] * 10] * 10  # declaração de uma matriz para receber os n² pesos

    while i != limit:
        j = 0  # zera o contador
        F = 0  # armazena o valor calulado para cada iteração em j
        while j != limit:
            # desloca os nós  linearmente para outro intervalo
            yij[i][j] = ((limite_c(xi[i], ex) + limite_d(xi[i], ex)) / 2) + (
                    (limite_d(xi[i], ex) - limite_c(xi[i], ex)) * nos[j] / 2)
            # multiplica os pesos por um fator de escala
            vij[i][j] = peso[j] * (limite_d(xi[i], ex) - limite_c(xi[i], ex)) / 2
            # soma o valor calculado com os valores anteriormente calculados
            F = F + funcao(xi[i], yij[i][j], ex) * vij[i][j]
            j += 1  # incrementa o contador
        I = I + F * ui[i]  # adiciona o falor da função multiplicado pelo peso
                           # aos valores anteriormente calculados
        i += 1  # incrementa o contador
    return I


def main():  # a main basicamente recebe do usuário qual é o teste o qual ele quer verificar
    opcao = '1'
    while opcao != '5':  # printa para o usuário que tarefa que cada opção executa
        print("\n-------- Métodos Numéricos EP2 --------\n\n"
              " 1 - Exemplo 1.\n 2 - Exemplo 2."
              "\n 3 - Exemplo 3. \n 4 - Exemplo 4. \n 5 - Sair.")
        opcao = input(" Digite uma opção: ")
        while opcao != '1' and opcao != '2' and opcao != '3' and opcao != '4' and opcao != '5':
            # o código só roda para uma das opções válidas para a variável
            opcao = input("\n Opção Inválida! Digite uma opção: ")  # recebe a variável opção com entrada

        if opcao == '1':  # o método é usado para calular o volume do cubo e do tetraedro
            v6 = equacao_gauss_2_integrais(nos_n6, peso_n6, 6, 1.0)
            v8 = equacao_gauss_2_integrais(nos_n8, peso_n8, 8, 1.0)
            v10 = equacao_gauss_2_integrais(nos_n10, peso_n10, 10, 1.0)
            t6 = equacao_gauss_2_integrais(nos_n6, peso_n6, 6, 1.1)
            t8 = equacao_gauss_2_integrais(nos_n8, peso_n8, 8, 1.1)
            t10 = equacao_gauss_2_integrais(nos_n10, peso_n10, 10, 1.1)

            # exibição dos resultados
            print("\n-------- Exemplo 1 --------")
            print("\n 1. Volume do Cubo: \nPara n = 6 o volume calculado V = ", v6,
                  "m, V exato = 1m.\nPara n = 8 o volume calculado V = ", v8, "m, V exato = 1m."
                                                                              "\nPara n = 10 o volume calculado V = ",
                  v10, "m, V exato = 1m."
                       "\n 2. Volume do tetraedro: \nPara n = 6 o volume calculado V = ", t6, "m, V exato = 1/6m."
                                                                                              "\nPara n = 8 o volume calculado V = ",
                  t8, "m, V exato = 1/6m.\nPara n = 10 o volume calculado V = ", t10, "m, V exato = 1/6m.\n")

        if opcao == '2':  # o método é usado para calular a aŕea de uma região com duas ordens de integração diferentes
            i6 = equacao_gauss_2_integrais(nos_n6, peso_n6, 6, 2.0)
            i8 = equacao_gauss_2_integrais(nos_n8, peso_n8, 8, 2.0)
            i10 = equacao_gauss_2_integrais(nos_n10, peso_n10, 10, 2.0)
            j6 = equacao_gauss_2_integrais(nos_n6, peso_n6, 6, 2.1)
            j8 = equacao_gauss_2_integrais(nos_n8, peso_n8, 8, 2.1)
            j10 = equacao_gauss_2_integrais(nos_n10, peso_n10, 10, 2.1)

            # exibição dos resultados
            print("\n-------- Exemplo 2 --------")
            print("\n 1. Integral dydx: \nPara n = 6 a integral calculada I = ", i6,
                  "\nPara n = 8 a integral calculada I = ", i8,
                  "\nPara n = 10 a integral calculada I = ", i10,
                  "\n 2. Integral do dxdy: \nPara n = 6 a integral calculada I = ", j6,
                  "\nPara n = 8 a integral calculada I = ", j8, "\nPara n = 10 a integral calculada I = ", j10, "\n")

        if opcao == '3':  # o método é usado para calular a área de uma superfície e o volume entre ela e os eixos
            s6 = equacao_gauss_2_integrais(nos_n6, peso_n6, 6, 3.0)
            s8 = equacao_gauss_2_integrais(nos_n8, peso_n8, 8, 3.0)
            s10 = equacao_gauss_2_integrais(nos_n10, peso_n10, 10, 3.0)
            k6 = equacao_gauss_2_integrais(nos_n6, peso_n6, 6, 3.1)
            k8 = equacao_gauss_2_integrais(nos_n8, peso_n8, 8, 3.1)
            k10 = equacao_gauss_2_integrais(nos_n10, peso_n10, 10, 3.1)

            # exibição dos resultados
            print("\n-------- Exemplo 3 --------")
            print("\nPara n = 6  a superficie calculada foi  = ", s6,
                  "\nPara n = 8  a superficie calculada foi  = ", s8,
                  "\nPara n = 10 a superficie calculada foi  = ", s10)
            print("\nPara n = 6  o volume calculado foi  = ", k6,
                  "\nPara n = 8  o volume calculado foi  = ", k8,
                  "\nPara n = 10 o volume calculado foi  = ", k10)

        if opcao == '4':  # o método é usado para calular o volume de uma calota esférica e de um sólido de revolução
            c6 = 2 * 3.14159 * equacao_gauss_2_integrais(nos_n6, peso_n6, 6, 4.0)
            c8 = 2 *3.14159 * equacao_gauss_2_integrais(nos_n8, peso_n8, 8, 4.0)
            c10 = 2 * 3.14159 * equacao_gauss_2_integrais(nos_n10, peso_n10, 10, 4.0)
            r6 = 3.14159 * equacao_gauss_2_integrais(nos_n6, peso_n6, 6, 4.1)
            r8 = 3.14159 * equacao_gauss_2_integrais(nos_n8, peso_n8, 8, 4.1)
            r10 = 3.14159 * equacao_gauss_2_integrais(nos_n10, peso_n10, 10, 4.1)

            # exibição dos resultados
            print("\n-------- Exemplo 4 --------")
            print("\nPara n = 6  o volume calculado da calota foi  = ", c6,
                  "\nPara n = 8  o volume calculado da calota foi   = ", c8,
                  "\nPara n = 10 o volume calculado da calota foi  = ", c10)
            print("\nPara n = 6  o volume calculado do sólido foi  = ", r6,
                  "\nPara n = 8  o volume calculado do sólido foi   = ", r8,
                  "\nPara n = 10 o volume calculado do sólido foi  = ", r10)
        if opcao == '5': # encerra o programa
            print("\n-------- Métodos Numéricos EP2 --------\n\n")
            return


main() # chama-se a função main
