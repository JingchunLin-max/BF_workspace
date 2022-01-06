#include "DataTransform/DataTransform.h"
#include "bf_DAS/bf_api.h"
#include "record/capture.h"
#include <stdlib.h>
#include <alsa/asoundlib.h>
int main()
{
    long loops;  
	int rc;
	int size;  
	FILE *fp ;
	char *buffer;
	float* bf_buf_float_BF;

start:
	system("rm sound.pcm");
	record_init();
	if ((fp = fopen("sound.pcm","w")) < 0)
		printf("open sound.pcm fial\n");

	size = rec_obj.frames * 2; /* 2 bytes/sample, 1 channels */  

	printf(">>>>>>>>>>>>>>>record start<<<<<<<<<<<<<<<\n");
	//printf("size = %d\n",size);
	buffer = (char *) malloc(size);  
	bf_buf_float_BF = (float*) calloc(size/2, sizeof(float));

	loops = 1120;
	struct  bf_str*entity;
    bf_init(&entity, 6, 16000, 16);
	while (loops > 0) {  
		loops--;  
		rc = snd_pcm_readi(rec_obj.handle, buffer, rec_obj.frames); 
		if (rc == -EPIPE) {  
			/* EPIPE means overrun */  
			fprintf(stderr, "overrun occurred/n");  
			snd_pcm_prepare(rec_obj.handle);  
		} else if (rc < 0) {  
			fprintf(stderr,  
					"error from read: %s/n",  
					snd_strerror(rc));  
		} else if (rc != (int)rec_obj.frames) {  
			fprintf(stderr, "short read, read %d frames/n", rc);  
		}
		//>>>>>>>>数据转换  &&BF>>>>>>>>>>
		 float* bf_buf_float ;
         bf_buf_float = (float*) calloc(size/2, sizeof(float)); 
        bytes2FloatArr(buffer,size,bf_buf_float);         
        bf_feed(&entity,(const float*) bf_buf_float, bf_buf_float_BF,size/2, 1);
        char*bf_buf;
	    bf_buf=(char*) calloc(size,sizeof(char));
		floatArr2Bytes(bf_buf_float, size/2, bf_buf);
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>  
		rc = fwrite( bf_buf,1, size,fp);  
		if (rc != size)  
			fprintf(stderr,  "short write: wrote %d bytes/n", rc);  
	}
	record_deinit();
	printf(">>>>>>>>>>>>>>>record end<<<<<<<<<<<<<<<\n");
	fclose(fp); 
	free(buffer);
	// free(bf_buf_float);
	// free(bf_buf_float_BF);
	printf(">>>>>>>>>>>>>>>player start<<<<<<<<<<<<<<<\n");
	system("ffplay -f s16le -ar 16000 -ac 1 -autoexit sound.pcm");
	printf(">>>>>>>>>>>>>>>player end<<<<<<<<<<<<<<<\n");
	goto start;
	return 0;
}
