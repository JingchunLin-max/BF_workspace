#ifndef DTF
#define DTF

#include<stdlib.h>
void floatToBytesLittle(float value, unsigned char*cSendBuff, int pos)
{
	unsigned short i = 0;
	float floatVariable = value;
	unsigned char *pdata = (unsigned char *)&floatVariable;
	for (i = 0; i < 4; i++)
	{
		cSendBuff[i + pos] = *pdata++;//float转BYTE
	}
}

void floatArr2Bytes(float* floatArr, unsigned int len, char* byteBufOut) {
	unsigned int pos = 0;
	char * temp = NULL;
	short*  sam =  (short*) calloc(len, sizeof(len));
    //转成short
	for(int i=0;i<len;i++){
		sam[i] =(short) floatArr[i];
	}

	int k = 0;
	for (int i = 0; i < len; i++) {
		temp = (char *)(&sam[i]);
		for (k = 0; k < 2; k++) {
			byteBufOut[pos++] = *temp++;
		}
	}
	free(sam);
	free(floatArr);
}

void byteToFloat(unsigned char * charBuf, float* out) {
	
	unsigned char  i;
	void *pf = out;
	unsigned char * px = charBuf;

	for (i = 0; i < 4; i++){
		*((char*)pf + i) = *(px + i);
	}
}

void bytes2FloatArr(char* bytes, unsigned int len, float* floatArrOut) {
	unsigned int position = 0;
	unsigned int floatCount = 0;
	short* sam = (short*)calloc(1, sizeof(short));
	float * temp = NULL;
	for (int i = 0; i < len; ) {
		for (int k = 0; k < 2; k++) {
			// temp = &floatArrOut[floatCount];
			*((char*)sam + k) = *(bytes + i);
			i++;
		}
		floatArrOut[floatCount]=(float) *sam;
		floatCount++;
	}
}

#endif