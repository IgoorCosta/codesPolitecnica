#include "mbed.h"
#include "nRF24L01P.h"

// Configuração RF
Serial pc(PTA2, PTA1); // tx, rx
nRF24L01P my_nrf24l01p(PTD2, PTD3, PTC5, PTD0, PTD5, PTA13);    // mosi, miso, sck, csn, ce, irq

// Configuração dos LEDs
DigitalOut myled1(LED1); 
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);

// Configuração dos Motores
DigitalOut MotorEsqFre(PTC3); 
DigitalOut MotorEsqTra(PTC4); 
DigitalOut MotorDirFre(PTC0);
DigitalOut MotorDirTra(PTC7);

// Configuração sensor ultrassônico
DigitalOut trigger(PTB1);
DigitalIn  echo(PTC1);
Timer sonar;

// Configuração dos IRs
DigitalIn IR1(PTB8);
DigitalIn IR2(PTB9);

int distance = 0;
int correction = 0;
int configuracao = 0;
int IRe = 0;
int IRd = 0;
unsigned int contIRe = 0;
unsigned int contIRd = 0;
unsigned int cont = 0;
unsigned int histcont = 0;
int leiturax = 0;
int leituray = 0;
float x = 0.0, y = 0.0;
char stringValue[2];
int curva = 0;
int direcao = 0;

// Recebe o comando para movimentar os motores
void controle(char sentido){
    if(sentido == 'd'){ // Direita
        MotorEsqFre = 1;MotorEsqTra = 0; 
        MotorDirFre = 1;MotorDirTra = 0;
        myled1 = 0;myled2 = 1;myled3 = 1;myled4 = 1;
    }
    else if(sentido == 'a'){ // Esquerda
        MotorEsqFre = 0;MotorEsqTra = 1; 
        MotorDirFre = 0;MotorDirTra = 1;
        myled1 = 1;myled2 = 0;myled3 = 1;myled4 = 1;
    }
    else if(sentido == 'w'){ // Frente
        MotorEsqFre = 1;MotorEsqTra = 0; 
        MotorDirFre = 0; MotorDirTra = 1;
        myled1 = 0;myled2 = 0;myled3 = 0;myled4 = 1;
    }
    else if(sentido == 's'){ // Ré
        MotorEsqFre = 0;MotorEsqTra = 1; 
        MotorDirFre = 1; MotorDirTra = 0;
        myled1 = 0;myled2 = 0;myled3 = 0;myled4 = 1;
    }
    else if(sentido == 'b'){ // Parar
        MotorEsqFre = 0;MotorEsqTra = 0; 
        MotorDirFre = 0; MotorDirTra = 0;
        myled1 = 1;myled2 = 1;myled3 = 1;myled4 = 1;
    }
    else if(sentido == 'r'){ // Autônomo
        configuracao = 1;
    }
    else if(sentido == 'q'){
        configuracao = 0;
    }
}

int ultrassonico_dist(){
    trigger = 1;
    sonar.reset();
    wait_us(10.0);
    trigger = 0;
    while (echo==0) {};
    sonar.start();
    while (echo==1) {};
    sonar.stop();
    distance = (sonar.read_us()-correction)/58.0;
    return distance;
}

void virardireita(){
    controle('d');
    curva = 0;
    while(curva < 8){
        if(IR2 != IRd){
            if(IR2 == 1) IRd = 1;
            else IRd = 0;
            contIRd++;
            curva += 1; 
        }
        wait_us(1000.0);
    }
    direcao = 1;
}

void viraresquerda(){
    controle('a');
    curva = 0;
    while(curva < 8){
        if(IR2 != IRd){
            if(IR2 == 1) IRd = 1;
            else IRd = 0;
            contIRd++;
            curva += 1; 
        }
        wait_us(1000.0);
    }
    direcao = 0;
}

int conttt = 0;
void andar15(){
    controle('w');
    conttt = 0;
    while(conttt < 30) {
        if(IR2 != IRd){
            if(IR2 == 1)
                IRd = 1;
            else
                IRd = 0;
            conttt++;
            y = y - 1.5;
        }
        wait_us(1000.0);
    }
}

int main() {
    // Inicializa LEDs - desligados
    myled1 = 1;myled2 = 1;myled3 = 1;myled4 = 1;

    // Inicializa motores - desligados
    MotorEsqFre = 1;MotorEsqTra = 1;MotorDirFre = 1;MotorDirTra = 1;

    // Inicializa o sensor ultrassônico
    sonar.reset();
    sonar.start();
    while (echo==2) {};
    sonar.stop();
    correction = sonar.read_us();
    printf("Approximate software overhead timer delay is %d uS\n\r",correction);
    
    // Inicializa o RF
    #define TRANSFER_SIZE   4
    char txData[TRANSFER_SIZE], rxData[TRANSFER_SIZE];
    int txDataCnt = 0;
    int rxDataCnt = 0;
    my_nrf24l01p.powerUp();
    pc.printf( "nRF24L01+ Frequency    : %d MHz\r\n",  my_nrf24l01p.getRfFrequency() );
    pc.printf( "nRF24L01+ Output power : %d dBm\r\n",  my_nrf24l01p.getRfOutputPower() );
    pc.printf( "nRF24L01+ Data Rate    : %d kbps\r\n", my_nrf24l01p.getAirDataRate() );
    pc.printf( "nRF24L01+ TX Address   : 0x%010llX\r\n", my_nrf24l01p.getTxAddress() );
    pc.printf( "nRF24L01+ RX Address   : 0x%010llX\r\n", my_nrf24l01p.getRxAddress() );
    pc.printf( "Type keys to test transfers:\r\n  (transfers are grouped into %d characters)\r\n", TRANSFER_SIZE );
    my_nrf24l01p.setTransferSize( TRANSFER_SIZE );
    my_nrf24l01p.setReceiveMode();
    my_nrf24l01p.enable();

    while (1) {
        // Comandos pelo RF
        if (pc.readable()) {
            txData[txDataCnt++] = pc.getc();
            if (txDataCnt >= sizeof( txData ) ) {
                my_nrf24l01p.write( NRF24L01P_PIPE_P0, txData, txDataCnt );
                txDataCnt = 0;
            }
        }
        if (my_nrf24l01p.readable()) {
            rxDataCnt = my_nrf24l01p.read(NRF24L01P_PIPE_P0, rxData, sizeof(rxData));
            for (int i = 0; rxDataCnt > 0; rxDataCnt--, i++) {
                //controle(rxData[i]);
                if (rxData[i] == 'x') leiturax = 1;
                if (rxData[i] == 'y') leituray = 1;
                if (leiturax == 1 && leituray == 0) {
                    stringValue[0] = rxData[i];
                    stringValue[1] = '\0'; 
                    if (x <= 0.1) x = atof(stringValue)*10.0;
                    else x = x + atof(stringValue);
                }
                if (leituray == 1 && leiturax == 1) {
                    stringValue[0] = rxData[i];
                    stringValue[1] = '\0';
                    if (y <= 0.1) y = atof(stringValue)*10.0;
                    else {
                        y = y + atof(stringValue); 
                        curva = 0;
                        leiturax = 0;
                        leituray = 0;
                        direcao = 0;
                        controle('w');
                    }
                }
                pc.putc(rxData[i]);
            }
        }

        if(IR1 != IRe){
            if(IR1 == 1) IRe = 1;
            else IRe = 0;
            contIRe++;
        }
        if(IR2 != IRd){
            if(IR2 == 1) IRd = 1;
            else IRd = 0;
            contIRd++;
            if (direcao == 0 && x > 1.0) {
                x = x - 1.5;
                if(ultrassonico_dist() < 30) {
                    virardireita();
                    andar15();
                    viraresquerda();
                }
                controle('w');
            }
            else if (x <= 1.0 && direcao == 0){
                virardireita();
                controle('w');
            } 
            else if (direcao == 1 && y > 1.0){
                y = y - 1.5;
                controle('w');
            } 
            else if (x <= 1.0 && y <= 1.0){
                x = 0.0;
                y = 0.0;
                controle('b');
            } 
        }
        wait_us(1000.0);
    }
}