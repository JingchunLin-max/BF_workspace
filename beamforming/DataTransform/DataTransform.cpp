#include "DataTransform.h" 
#include<iostream>
using namespace std;
int main(void){
	/*unsigned char * charBuf = new unsigned char[4];
	float in = 5.194;
	float out = -1;

	floatToBytesLittle(in, charBuf, 0);
	byteToFloat(charBuf, &out);
	cout << out << endl;*/


	// float floatArr[] = {12.1, 12.24, 13.3, 13.5, 4.5, 0.01};

	// const unsigned int len = 6;

	float * floatArr;
	floatArr = (float*) calloc(2, sizeof(float));
	const unsigned int len = 2;
	char* byteBuf =(char*) calloc(4, sizeof(char));
	char* byteBuf0 =(char*) calloc(4, sizeof(char));
	float* floatArrOut =(float*) calloc(2, sizeof(float));
	for(int i=0;i<4;i++){
	byteBuf[i]=i;
	cout<<(short)byteBuf[i]<<endl;
	}
	bytes2FloatArr(byteBuf, 2 * len, floatArrOut);
  	floatArr2Bytes(floatArrOut, len, byteBuf0 );
	for (int i = 0; i < len; i++) {
		cout << floatArrOut[i] << ", ";
	}
	cout << endl;

   for(int j=0; j<2*len;j++){
	   cout << (short)byteBuf0[j]<<",";
   }
   cout<<endl;

	return 0;
}
