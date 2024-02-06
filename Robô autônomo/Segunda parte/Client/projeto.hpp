#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <cekeikon.h>

#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 1   // how many pending connections queue will hold
#define MAXDATASIZE 256 // max number of bytes we can get at once 

// general purpose functions
bool testaBytes(BYTE* buf, BYTE b, int n){
    //Testa se n bytes da memoria buf possuem valor b
    bool igual=true;
    for (unsigned i=0; i<n; i++)
        if (buf[i]!=b) { igual=false; break;}
    return igual;
}

bool testaVb(const vector<BYTE> vb, BYTE b) {
    //Testa se todos os bytes de vb possuem valor b
    bool igual=true;
    for (unsigned i=0; i<vb.size(); i++)
        if (vb[i]!=b) { igual=false; break;}
    return igual;
}

// Class Headers
class DEVICE {
public:
    static void *get_in_addr(struct sockaddr *sa);

    virtual void sendBytes(int nBytesToSend, BYTE *buf)=0;
    virtual void receiveBytes(int nBytesToReceive, BYTE *buf)=0;

    void sendUint(uint32_t m);
    void receiveUint(uint32_t& m);
    
    void sendVb(const vector<BYTE>& vb);
    void receiveVb(vector<BYTE>& vb);

    void sendImg(const Mat_<COR>& img);
    void receiveImg(Mat_<COR>& img);

    void sendImgComp(const Mat_<COR>& img);
    void receiveImgComp(Mat_<COR> &img);
};

class SERVER : public DEVICE {
private:
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
public:
    SERVER();
    ~SERVER();
    void waitConnection();
    void sendBytes(int nBytesToSend, BYTE *buf);
    void receiveBytes(int nBytesToReceive, BYTE *buf);
};

class CLIENT : public DEVICE {
private:
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
public:
    CLIENT(string endereco);
    ~CLIENT();
    void sendBytes(int nBytesToSend, BYTE *buf);
    void receiveBytes(int nBytesToReceive, BYTE *buf);
};

// DEVICE functions
void *DEVICE::get_in_addr(struct sockaddr *sa)      // get sockaddr, IPv4 or IPv6:
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void DEVICE::sendUint(uint32_t m){
    m = htonl(m);               // conversão host para net (raspberry --> internet)
    sendBytes(4,(BYTE*)&m);
}

void DEVICE::receiveUint(uint32_t& m){
    receiveBytes(4,(BYTE*)&m);
    m = ntohl(m);              // conversão net para host (internet --> raspberry)
}

void DEVICE::sendVb(const vector<BYTE>& vb){;
    sendUint(vb.size());
    sendBytes(vb.size(), (BYTE *)vb.data());
}

void DEVICE::receiveVb(vector<BYTE>& vb){
    uint32_t t;
    receiveUint(t);
    vb.resize(t);
    receiveBytes(t, (BYTE *)vb.data());
}

void DEVICE::sendImg(const Mat_<COR> &img) {
    if (img.isContinuous()) {
        sendUint(img.rows);
        sendUint(img.cols);
        sendBytes(3*img.total(),img.data);
    }
}
void DEVICE::receiveImg(Mat_<COR> &img) {
    uint32_t nl, nc;
    receiveUint(nl); 
    receiveUint(nc);
    img.create(nl,nc);
    receiveBytes(3*img.total(), img.data);
}

void DEVICE::sendImgComp(const Mat_<COR> &img)
{
    if (!img.isContinuous()) erro("Erro sendImgComp: Imagem nao-continua");
    vector<BYTE> vb;
    vector<int> param{CV_IMWRITE_JPEG_QUALITY, 80};
    imencode(".jpg", img, vb, param);
    sendVb(vb);
}

void DEVICE::receiveImgComp(Mat_<COR> &img)
{
    vector<BYTE> vb;
    receiveVb(vb);
    img = imdecode(vb, 1);
}

// SERVER functions
SERVER::SERVER() {
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");
}

SERVER::~SERVER() {
    close(new_fd);  // parent doesn't need this
}

void SERVER::waitConnection() {
    sin_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (new_fd == -1) {
        perror("accept");
    }
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    printf("server: got connection from %s\n", s);
}

void SERVER::sendBytes(int nBytesToSend, BYTE *buf){
    int count = 0;
    while (count != nBytesToSend) {
        count += send(new_fd, &buf[count], nBytesToSend, 0);
    }
}

void SERVER::receiveBytes(int nBytesToReceive, BYTE *buf){
    int count = 0;
    while (count != nBytesToReceive) {
        count += recv(new_fd, &buf[count], nBytesToReceive, 0);
    }
}

// CLIENT functions
CLIENT::CLIENT(string endereco) {
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    const char *enderecoC = endereco.c_str();

    if ((rv = getaddrinfo(enderecoC, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
}

CLIENT::~CLIENT() {
    close(sockfd);
}

void CLIENT::sendBytes(int nBytesToSend, BYTE *buf){
    int count = 0;
    while (count != nBytesToSend) {
        count += send(sockfd, &buf[count], nBytesToSend, 0);
    }
}

void CLIENT::receiveBytes(int nBytesToReceive, BYTE *buf){
    int count = 0;
    while (count != nBytesToReceive) {
        count += recv(sockfd, &buf[count], nBytesToReceive, 0);
    }
}