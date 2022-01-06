
#include"bf_api_mvdr.h"
#include <stdio.h>
#include "tdoa.h"
#include "mvdr.h"
#include "vad.h"

void bf_init(struct bf_str**entity, float energy_thresh, int sil_to_speech_trigger, int speech_to_sil_trigger,
int sample_rate, int fft_point, int num_channel, int num_sample, int num_point_shift, int num_point_per_frame){
      
     (*entity) = (struct bf_str*)malloc(sizeof(struct bf_str));
    
     (*entity) -> energy_thresh=energy_thresh;
      //printf("%f\n",energy_thresh);
     (*entity) -> sil_to_speech_trigger = sil_to_speech_trigger;
     //printf("%d\n",sil_to_speech_trigger);
     (*entity) -> speech_to_sil_trigger = speech_to_sil_trigger;
     //printf("%d\n",speech_to_sil_trigger);
     (*entity) -> sample_rate = sample_rate;
    //printf("%d\n",sample_rate);
     (*entity) -> fft_point = fft_point;
     //printf("%d\n",fft_point);
     (*entity) -> num_channel = num_channel;
     //printf("%d\n",num_channel);
     (*entity) -> num_sample = num_sample;
     //printf("%d\n",num_sample);
     (*entity) -> num_point_shift = num_point_shift;
     //printf("%d\n",num_point_shift);
     (*entity) -> num_point_per_frame = num_point_per_frame;
     //printf("%d\n",num_point_per_frame);   
}
 void bf_feed_mvdr(struct bf_str**entity, const float*pcm,float*out_pcm){

    float energy_thresh = (*entity) -> energy_thresh;
    int sil_to_speech_trigger = (*entity) -> sil_to_speech_trigger;
    int speech_to_sil_trigger = (*entity) -> speech_to_sil_trigger;
    int sample_rate = (*entity) -> sample_rate;
    int fft_point = (*entity) -> fft_point;
    int num_channel = (*entity) -> num_channel;
    int num_sample = (*entity) -> num_sample;
    int num_point_shift = (*entity) -> num_point_shift;
    int num_point_per_frame = (*entity) -> num_point_per_frame;
    
    
    //float *out_pcm = (float *) calloc( sizeof(float), num_sample );
    float *tdoa = (float *) calloc( sizeof(float), num_channel ); 
    Vad vad;
    VadInit( &vad, energy_thresh, sil_to_speech_trigger, speech_to_sil_trigger );
    //  printf("fft_point_input: %d \n",fft_point);

    Mvdr mvdr(sample_rate, fft_point, num_channel);
    for (int i = 0; i < num_sample; i += num_point_shift) {
        // last frame
        if (i + num_point_per_frame > num_sample) break; 
        // rearrange channel data
        float *data = (float *) calloc( sizeof(float), 
                                      num_point_per_frame * num_channel );
        for (int j = 0; j < num_channel;  j++) {
            for ( int k = 0; k < num_point_per_frame; k++ ) {
                data[j * num_point_per_frame + k] = pcm[(i + k) * num_channel + j];
            }
        }
        // Because gccphat based time-delay is not very precise
        // so here we time-delay is not supported here
        // we suppose the signal arrive sensors at the same time(90)
        // do vad (is noise or not)
        
        bool is_speech = IsSpeech(&vad, data, num_point_per_frame); 
        
        // printf("is_speech:%d\n", is_speech);
        // do MVDR
        //  printf("num_point_per_frame: %d \n",num_point_per_frame);
        mvdr.DoBeamformimg(data, num_point_per_frame, !is_speech, 
                           tdoa, out_pcm + i);
        free(tdoa);
    }
}
