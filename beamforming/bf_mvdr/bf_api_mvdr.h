
#ifndef MVDR_H
#define MVDR_H


struct bf_str
{
    int sample_rate;
    int  num_channel;
    int fft_point;
    int num_point_shift;
    int num_point_per_frame;
    int num_sample;
    //vad
    int sil_to_speech_trigger;
    int speech_to_sil_trigger;
    float  energy_thresh;
};

void bf_init(struct bf_str**entity, float energy_thresh, int sil_to_speech_trigger, int speech_to_sil_trigger,
int sample_rate,int fft_point,int num_channel,int num_sample,int num_point_shift,int num_point_per_frame);
void bf_feed_mvdr(struct bf_str**entity, const float*pcm, float*out_pcm);
void bf_deinit();

#endif