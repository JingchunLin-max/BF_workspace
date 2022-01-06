/* Created on 2016-08-16
 * Author: Zhang Binbin
 */
#include "bf_api.h"
#include "wav.h"
#include"parse-option.h"
#include<iostream>
using namespace std;

//to calculate the covariance matrix
int covariance(const float*data, float* out_pcm,int num_channel,int numsample){
    double cov[num_channel+1]={0};
    double mean[7]={0};
    for (int i = 0; i < numsample; i++) {
        mean[0]+=out_pcm[i];
        mean[1]+=data[i*num_channel+0];
        mean[2]+=data[i*num_channel+1];
        mean[3]+=data[i*num_channel+2];
        mean[4]+=data[i*num_channel+3];
        mean[5]+=data[i*num_channel+4];
        mean[6]+=data[i*num_channel+5];
    }
mean[0]/=numsample;
mean[1]/=numsample;
mean[2]/=numsample;
mean[3]/=numsample;
mean[4]/=numsample;
mean[5]/=numsample;
mean[6]/=numsample;
    
         for(int j=0;j<numsample;j++){
             cov[1]+=(out_pcm[j]-mean[0])*(data[j*num_channel+0]-mean[1]);
             cov[2]+=(out_pcm[j]-mean[0])*(data[j*num_channel+1]-mean[2]);
             cov[3]+=(out_pcm[j]-mean[0])*(data[j*num_channel+2]-mean[3]);
             cov[4]+=(out_pcm[j]-mean[0])*(data[j*num_channel+3]-mean[4]);
             cov[5]+=(out_pcm[j]-mean[0])*(data[j*num_channel+4]-mean[5]);
             cov[6]+=(out_pcm[j]-mean[0])*(data[j*num_channel+5]-mean[6]);
         }
         double max=0;
         int chnl=0;
         for ( int i = 1; i <= num_channel; i++)
         {
              if(max<cov[i]) {
                  max=cov[i];
                  chnl=i;
              }
         }
         return max, chnl;
}


int main(int argc, char *argv[]) {

    const char *usage = "Do delay and sum beamforming\n"
                        "Usage: apply-delay-and-sum multi_channel_file output_file\n";
    ParseOptions po(usage);

    // po.Register("tdoa-window", &tdoa_window, 
    //             "window size for estimated tdoa, in sample point");
    // po.Register("beam-window", &beam_window, 
    //             "window size for delay and sum, less than tdoa-window, in sample point");
    // po.Register("margin", &margin, 
    //             "constraint for tdoa estimation");

    po.Read(argc, argv);

    if (po.NumArgs() != 2) {
        po.PrintUsage();
        exit(1);
    }
    std::string input_file = po.GetArg(1),
                output_file = po.GetArg(2);
    
    WavReader wav_reader(input_file.c_str());

    printf("input file %s info: \n"
           "sample_rate %d \n"
           "channels %d \n"
           "bits_per_sample_ %d \n",
           input_file.c_str(),
           wav_reader.SampleRate(), 
           wav_reader.NumChannel(),
           wav_reader.BitsPerSample());


    int SampleRate=wav_reader.SampleRate();
    int num_channel = wav_reader.NumChannel();
    // int num_sample = wav_reader.NumSample();
    int bit=wav_reader.BitsPerSample();
    

    int n=100;   // 输出的帧数
    int num_sample=128;//一帧单声道的数据量
    const float *pcm = wav_reader.Data(); //文本音频数据量
    float *out_pcm = (float *)calloc(sizeof(float), n*num_sample);

    struct  bf_str*entity;    //创建上下文结构体变量
    bf_init( &entity, num_channel, SampleRate,bit);    //初始化函数
    
    out_pcm=bf_feed(&entity, pcm,num_sample, n);

    // Write outfile
    WavWriter wav_writer( out_pcm, num_sample*n, 1,
                         wav_reader.SampleRate(), wav_reader.BitsPerSample());
    wav_writer.Write(output_file.c_str());

    // int chl=0;
    // float cov;
    // cov, chl=covariance(pcm, out_pcm,num_channel, num_sample);
    // cout<<chl<<' '<<cov<<endl;

    free(out_pcm);
    // output the most similar channel
    return 0;
}
