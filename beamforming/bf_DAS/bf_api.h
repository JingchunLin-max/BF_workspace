
#ifndef BF_H
#define BF_H

struct bf_str {
	int channel;
	int rate;
	int bit;
   int tdoa_window;
   int beam_window;
};
void bf_init(struct bf_str **entity, int channel, int rate, int bit);
void  bf_feed(struct bf_str**entity, const float *pcm, float* out_pcm, int len, int frame);
int bf_deinit(struct bf_str **entity);

#endif
 