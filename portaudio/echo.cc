/*
 * This is a test of portaudio.  It echoes its microphone input to speaker output for ten seconds.
 * This test is suceptible to feedback when used with regular speakers.
 *
 * Complile with g++ echo.cc -l portaudio -o echo
 *
 * Author: Marcello Mathias Herreshoff <marcello.herreshoff@gmail.com>
 */
#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>

#include "portaudio.h"

#define PA_SAMPLE_TYPE      paFloat32
const int SAMPLE_RATE = 44100;
const int FRAMES_PER_BUFFER = 256;
const int CHANNELS = 2;
using namespace std;


class Statistics {
 private:
  long count;
  double total;
  double total_squares;

 public:
   Statistics() {
     count = 0;
     total = total_squares = 0.0;
   }

  void addDataPoint(double data) {
    count++;
    total += data;
    total_squares += data*data;
  }

  double getMean() {
    return total / (double) count;
  }

  double getVariance() {
    double mu = getMean();
    return (total_squares/count)-mu*mu;
  }
};

Statistics audioStats;

void quit (PaError err, string s) {
  cerr << "Audio Stats: mean = " << audioStats.getMean() << " var = " << audioStats.getVariance() << endl;
  cerr << "Quit: " << s << endl;
  cerr << "Error Message: " << Pa_GetErrorText(err) << endl;
  Pa_Terminate();
  exit(0);
}

static int paCallback( const void *inputBuffer, void *outputBuffer,

                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData ) {

  float* in = (float*) inputBuffer;
  float* out = (float*) outputBuffer;
  for (int i=0; i < framesPerBuffer*CHANNELS ; i++) {
    float f = in[i];
    out[i] = f;
    audioStats.addDataPoint(f);
  }
  return paContinue;
}

int main (void) {
  int clock;
  PaError err = Pa_Initialize();
  if (err != paNoError) quit(err, "Couldn't initialize portaudio.");

  PaStreamParameters inputParameters;
  inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
  inputParameters.channelCount = CHANNELS;
  inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  PaStreamParameters outputParameters;
  outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
  outputParameters.channelCount = CHANNELS;
  outputParameters.sampleFormat = PA_SAMPLE_TYPE;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  PaStream *stream;
  err = Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
              paCallback,
              NULL );
  if (err != paNoError) quit(err, "Couldn't open stream.");

  err = Pa_StartStream( stream );
  if( err != paNoError ) quit(err, "Couldn't start stream.");

  /* Sleep for several seconds. */
  Pa_Sleep(10*1000);

  err = Pa_StopStream( stream );
  if( err != paNoError ) quit(err, "Couldn't stop stream.");
  err = Pa_CloseStream( stream );
  if( err != paNoError ) quit(err, "Couldn't close stream.");

  quit(err, "Success.");
}
