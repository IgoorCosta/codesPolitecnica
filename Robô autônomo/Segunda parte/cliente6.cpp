// Igor Costa D'Oliveira 11391446
// Lucas Kairuz Martins  11805377

#include "projeto.hpp"
#include <cekeikon.h>

#define RAZAO 0.8663
#define CONTRASTE 3.7
#define BRILHO -2

MNIST mnist(14, true, true);

uint32_t state = 0;
void on_mouse(int event, int c, int l, int flags, void* userdata) {
  if (event == EVENT_LBUTTONDOWN) {
    if      (0 <= l   && l < 80  && 0 <= c   && c < 80)  state = 7;
    else if (0 <= l   && l < 80  && 80 <= c  && c < 160) state = 8;
    else if (0 <= l   && l < 80  && 160 <= c && c < 240) state = 9;
    else if (80 <= l  && l < 160 && 0 <= c   && c < 80)  state = 4;
    else if (80 <= l  && l < 160 && 160 <= c && c < 240) state = 6;
    else if (160 <= l && l < 240 && 0 <= c   && c < 80)  state = 1;
    else if (160 <= l && l < 240 && 80 <= c  && c < 160) state = 2;
    else if (160 <= l && l < 240 && 160 <= c && c < 240) state = 3;
    else state = 0;
  } 
  else if (event == EVENT_LBUTTONUP) state = 0;
}

int main(int argc, char *argv[]) {
    mnist.le("/home/psi3422/shared/mnist");
    vector<int> indices(1); vector<float> dists(1);
    flann::Index ind(mnist.ax,flann::KDTreeIndexParams(4));
    if (argc<2) erro("Colocar argumentos\n");

    CLIENT client(argv[1]);   
	uint32_t ready = 1;
	uint32_t close = 0;
	COR gray(128, 128, 128);
	COR green(0, 255, 0);
	Mat_<COR> teclado (240, 240, gray);
	Mat_<COR> tela;
    Mat_<COR> tela1;
	
	reta(teclado, 80, 0, 80, 240, COR(0,0,0));
	reta(teclado, 160, 0, 160, 240, COR(0,0,0));
	reta(teclado, 0, 80, 240, 80, COR(0,0,0));
	reta(teclado, 0, 160, 240, 160, COR(0,0,0));

	putTxt(teclado, 110, 100,"Brabo");
	flecha(teclado, 60, 120, 20, 120, green); 	// CIMA
	flecha(teclado, 180, 120, 220, 120, green); // BAIXO
	flecha(teclado, 120, 60, 120, 20, green); 	// ESQUERDA
	flecha(teclado, 120, 180, 120, 220, green); // DIREITA

	flecha(teclado, 60, 60, 20, 20, green); 	// Diagonal CIMA/ESQUERDA
	flecha(teclado, 60, 180, 20, 220, green); 	// diagonal CIMA/DIREITA
	flecha(teclado, 180, 60, 220, 20, green); 	// Diagonal BAIXO/ESQUERDA
	flecha(teclado, 180, 180, 220, 220, green); // Diagonal BAIXO/DIREITA

    namedWindow("janela", 1);
    Mat_<COR> img;
    char ch = -1;
    string arquivo = "";

    COR cinza(0,64,128);
    COR vermelho(255,0,0);

    namedWindow("janela", WINDOW_NORMAL);
    setMouseCallback("janela", on_mouse);
    if (argc == 3) arquivo = argv[2];
    VideoWriter vo(arquivo, CV_FOURCC('X','V','I','D'), 16, Size(240+320+120,240));
    
    //TimePoint t1 = timePoint();
    //int i = 0;

    uint32_t estado = 0;
    Mat_<FLT> T;
    Mat_<FLT> TRezise[10];
    le(T,"quadrado.png");
    Mat_<FLT> b;
    Mat_<FLT> RCC[10];
    float escala[10];
    escala[0] = 0.1721;
    int iCard, jCard;
    Mat_<COR> digito;
    Mat_<COR> digitoResize(120,120,cinza);
    Mat_<COR> digitoBoxResize(120,120,cinza);
    Mat_<FLT> digitoFloat, digitoBox;
    int numero = 0;
    int funcao = 0;

    while (ch != 27)
    {
        int x = 0;
        int escalaCard = 0;
        int card = false;
        int digitoReconhecido = 0;
        client.sendUint(100);
        ch = (signed char) waitKey(30);

        if(ch == 109) funcao = 0;
        else if(ch == 97) funcao = 1;
        client.sendUint(funcao);

        // Modo Manual
        if (funcao == 0) {
            client.sendUint(state);
            client.receiveImgComp(img);
            converte(img, b);

            tela1 = grudaH(img, grudaV(digitoBoxResize, digitoResize));
            tela = grudaH(teclado, tela1);
            imshow("janela", tela);

            vo << tela;            
        }

        // Modo Autônomo
        if (funcao == 1) {
            // finalizacao do programa
            if (estado == 4) return 0;

            client.receiveImgComp(img);
            converte(img, b);

            // calculo das escalas
            for (int i = 0; i < 10; i++)
                if (i > 0)
                    escala[i] = escala[i-1] * RAZAO;
            
            // busca CC
            #pragma omp parallel for
            for (int i = 0; i < 10; i++){
                resize(T, TRezise[i], Size(), escala[i], escala[i], INTER_NEAREST);
                TRezise[i] = somaAbsDois(dcReject(TRezise[i],1.0));
                RCC[i] = matchTemplateSame(b, TRezise[i],CV_TM_CCORR,0.0);
            }

            float menor = 0;
            float pixel[20][5];
            for (int i = 0; i < 20; i++)
                for (int j = 0; j < 5; j++)
                    pixel[i][j] = 0;

            // busca dos maiores valores
            int count = 0;
            #pragma omp parallel for
            for (int k = 0; k < 10; k++)
            {
                for (int i = 0; i < 240; i++)
                {
                    for (int j = 0; j < 320; j++)
                    {
                        if (RCC[k][i][j] > menor){
                            for (int l = 0; l < 20; l++)
                            {
                                int nEscala = pixel[l][3];

                                if ((pixel[l][1]+40 > i && pixel[l][1]-40 < i)||(pixel[l][2]+40> j && pixel[l][2]-40< j)){
                                    if (pixel[l][0] <  RCC[k][i][j]){
                                        pixel[l][0] = RCC[k][i][j];
                                        pixel[l][1] = i;
                                        pixel[l][2] = j;
                                        pixel[l][3] = k;
                                        break;
                                    }
                                }
                                else if (pixel[l][0] == menor){
                                    count++;
                                    if (count > 20){
                                        menor = RCC[k][i][j];
                                    }
                                    pixel[l][0] = RCC[k][i][j];
                                    pixel[l][1] = i;
                                    pixel[l][2] = j;
                                    pixel[l][3] = k;
                                    break;
                                }
                            }
                            for (int l = 0; l < 20; l++)
                            {
                                if (pixel[l][0] < menor){
                                    menor = pixel[l][0];
                                }
                            }
                            
                        }
                    }
                }
            }
            
            Mat_<FLT> RNCC[10];
            bool escalasCC[10];
            for (int i = 0; i < 10; i++)
                escalasCC[i] = false;

            // busca NCC
            int thickness = 2;
            #pragma omp parallel for
            for (int l = 0; l < 20; l++)
            {
                int i = pixel[l][1];
                int j = pixel[l][2];
                int k = pixel[l][3];
                if(escalasCC[k] == false){
                    RNCC[k]=matchTemplateSame(b,TRezise[k],CV_TM_CCOEFF_NORMED,0.0);
                    escalasCC[k] == true;
                }
            }
            bool achou = false;
            
            // busca do cartao de acordo com os resultados CC e NCC
            for (int l = 0; l < 20; l++)
            {
                int i = pixel[l][1];
                int j = pixel[l][2];
                int k = pixel[l][3];
                if (RNCC[k][i][j] > 0.55 && achou == false){
                    card = true;
                    iCard = i;
                    jCard = j;
                    escalaCard = pixel[l][3];
                    line(img, Point(j - (int)(escala[k]*401)/2,i - (int)(escala[k]*401)/2), Point(j + (int)(escala[k]*401)/2,i - (int)(escala[k]*401)/2), Scalar(0,128,128),6);
                    line(img, Point(j + (int)(escala[k]*401)/2,i - (int)(escala[k]*401)/2), Point(j + (int)(escala[k]*401)/2,i + (int)(escala[k]*401)/2), Scalar(0,128,128),6);
                    line(img, Point(j + (int)(escala[k]*401)/2,i + (int)(escala[k]*401)/2), Point(j - (int)(escala[k]*401)/2,i + (int)(escala[k]*401)/2), Scalar(0,128,128),6);
                    line(img, Point(j - (int)(escala[k]*401)/2,i + (int)(escala[k]*401)/2), Point(j - (int)(escala[k]*401)/2,i - (int)(escala[k]*401)/2), Scalar(0,128,128),6);
                    
                    //copia o trecho do frame contendo o digito a ser lido
                    img(Rect(j - (int)(escala[k]*401)*1/5, i - (int)(escala[k]*401)*1/5, (int)(escala[k]*401)*2/5, (int)(escala[k]*401)*2/5)).copyTo(digito);
                    converte(digito,digitoFloat);
                    digitoBox = mnist.bbox(digitoFloat);
                    digitoBox.convertTo(digitoBox, -1, CONTRASTE, BRILHO); //ajuste de contraste e brilho
                    // transforma matriz em array 1D
                    std::vector<float> array;
                    if (digitoBox.isContinuous()) {
                        array.assign((float*)digitoBox.data, (float*)digitoBox.data + digitoBox.total()*digitoBox.channels());
                    } else {
                        for (int i = 0; i < digitoBox.rows; ++i) {
                            array.insert(array.end(), digitoBox.ptr<float>(i), digitoBox.ptr<float>(i)+digitoBox.cols*digitoBox.channels());
                        }
                    }
                    ind.knnSearch(array,indices,dists,1,flann::SearchParams(32));
                    numero = mnist.ay(indices[0]);

                    // salva o resultado se a escala for menor que 2
                    if (k < 2){
                        digitoReconhecido = numero;
                        putText(img, std::to_string(numero), Point(j - (int)(escala[k]*401)/2-20, i+15), 0, 1, Scalar(0,0,255), 2);
                    }

                    converte(digitoBox,digitoBoxResize);
                    resize(digito, digitoResize, Size(120,120), 0, 0, INTER_NEAREST);
                    resize(digitoBoxResize, digitoBoxResize, Size(120,120), 0, 0, INTER_NEAREST);
                    putText(img, std::to_string((int)pixel[l][3]), Point(j + (int)(escala[k]*401)/2+10, i-15), 0, 0.5, Scalar(128,128,0), 0.5);
                    break;
                }
            }

            x = (320/2)-jCard;
            tela1 = grudaH(img, grudaV(digitoBoxResize, digitoResize));
            tela = grudaH(teclado, tela1);
            putText(tela, "estado = " + std::to_string(estado), Point(260, 20), 0, 0.4, Scalar(0,0,255), 2);
            imshow("janela", tela);

            if (argc == 3) vo << tela;

            // envio dos dados do cartao e recebimento do estado atual do carrinho
            client.sendUint(card);
            client.sendUint(x);
            client.sendUint(escalaCard);
            client.sendUint(digitoReconhecido);
            client.receiveUint(estado);
        }

        // Caso ESC
        if (ch == 27){
            client.sendUint(0);
            break;
        }
    }
    return 0;
}

