#include "bf_api.h"
#include<stdlib.h>
#include "tdoa.h"
#include"ds.h"

void bf_init(struct bf_str **entity, int channel, int rate, int bit)
{
     *entity = (struct bf_str*)malloc(sizeof(struct bf_str));
    (*entity)->channel=channel;
    (*entity)->rate=rate;
    (*entity)->bit=bit;
}

void bf_feed(struct bf_str **entity, const float *pcm, float* out_pcm, int len, int frame){
  
  int num=len*frame; //单声道总的数据量
  int num_channel=(*entity)->channel;
  out_pcm = (float *)calloc(sizeof(float), len*frame); //初始化输出音频内存空间
  int *tdoa = (int *)calloc(sizeof(int), num_channel);//延迟量变量计算
  int tdoa_window = 128;
  int beam_window = 128;
  int margin = 16;

     for (int i = 0; i < num; i += beam_window) {
        int tdoa_window_size = (i + tdoa_window > len*frame) ? 
                            num - i : tdoa_window;

        int beam_window_size = (i + beam_window > len*frame) ? 
                            num - i : beam_window;
   
        assert(beam_window_size <= tdoa_window_size);
        // rearrange channel data
        float *data = (float *)calloc(sizeof(float), 
                                      tdoa_window_size * num_channel);
        float *beam_data = (float *)calloc(sizeof(float), 
                                      beam_window_size * num_channel);
    
        //6*128个数据
        for (int j = 0; j < num_channel; j++) {
            for (int k = 0; k < tdoa_window_size; k++) {
                data[j * tdoa_window_size + k] = pcm[(k+i) * num_channel + j];
            }
            for (int k = 0; k < beam_window_size; k++) {
                beam_data[j * beam_window_size + k] = pcm[ (k+i )* num_channel + j];
            }
        }
        // calc delay
        int tao = margin < tdoa_window_size / 2 ? margin : tdoa_window_size / 2;

        //改变增强声道
        GccPhatTdoa(data, num_channel, tdoa_window_size, 0, tao, tdoa);
        // for (int j = 0; j < num_channel; j++) {
        //     printf("%d ", tdoa[j]);
        // }
        // printf("\n");
        DelayAndSum(beam_data, num_channel, beam_window_size, tdoa, out_pcm+i );
        free(data);
        free(beam_data);
    }
    free(tdoa);
}

int bf_deinit(struct bf_str **entity);
