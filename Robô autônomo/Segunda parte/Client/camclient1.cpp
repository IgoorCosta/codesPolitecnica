#include "projeto.hpp"
#include <cekeikon.h>

int main(int argc, char *argv[]) {
	if (argc!=2) erro("client6 servidorIpAddr\n");
    CLIENT client(argv[1]);   
	uint32_t recebi = 1;
	Mat_<COR> b;
	namedWindow("Window");
	
	while (true)  {
		client.receiveImg(b);
		imshow("Window",b);
		int ch = (signed char)(waitKey(30)); // E necessario (signed char)
		client.sendUint(recebi);
		cout << ch << endl;
		if (ch == 27)  {
			client.sendUint(0);
			break;
		}
  	}
}


