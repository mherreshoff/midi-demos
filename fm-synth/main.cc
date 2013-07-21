#include <iostream>
#include <math.h>
#include "portaudio.h"

#include "korg.h"


using namespace std;

#define PA_SAMPLE_TYPE      paFloat32
const int SAMPLE_RATE = 44100;
const int FRAMES_PER_BUFFER = 256;
const int CHANNELS = 2;

void quit (PaError err, string s) {
  cerr << "Quit: " << s << endl;
  cerr << "Error Message: " << Pa_GetErrorText(err) << endl;
  Pa_Terminate();
  exit(0);
}

/*
 * Phase is a straight-forward class for representing and updating the phase of a waveform.
 */
class Phase {
 public:
  Phase() : phase(0) {}
  double get() {
    return phase;
  }
  void advance(double frequency) {
    phase += frequency*2*M_PI/SAMPLE_RATE;
    if (phase > 2*M_PI) phase -= 2*M_PI;
  }
 private:
  double phase;
};

// TODO: Make a state struct containing these and the KOrg*.
Phase main_phase;
Phase modulator_phase;

// The multipliers show which modulator is controlled by each slider.
int num_modulators = 8;
double modulator_multiplier[] = {.25, .5, 1, 1.5, 2, 3, 3.5, 4};

/*
 * This is the function port audio calls to generate the sound. 
 * Right now, it outputs a sine wave modulated by some other sine waves whose
 * frequencies are multiples of the original one.
 */
static int paCallback( const void *inputBuffer, void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData ) {
  KOrg* korg = (KOrg*) userData;
  float* out = (float*) outputBuffer;
  for (int i=0; i < framesPerBuffer*CHANNELS ; i++) {
    if (i % 100 == 0) {
      korg->update();
    }

    double volume = korg->getKnob(0);
    double frequency = 220 + 220*korg->getKnob(1);

    double modulator = 0;
    for (int j = 0; j < num_modulators; j++) {
      modulator += sin(modulator_phase.get()*modulator_multiplier[j]) * korg->getSlider(j)*2;
    }

    float main_wave = volume*sin(main_phase.get() + modulator);
    out[i] = main_wave;
    modulator_phase.advance(frequency);
    main_phase.advance(frequency);
  }
  return paContinue;
}




int main (void) {
  // Set up the midi device:
  KOrg* korg = new KOrg();

  // Set up the audio event loop:
  PaError err = Pa_Initialize();
  if (err != paNoError) quit(err, "Couldn't initialize portaudio.");

  PaStreamParameters outputParameters;
  outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
  outputParameters.channelCount = CHANNELS;
  outputParameters.sampleFormat = PA_SAMPLE_TYPE;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  PaStream *stream;
  err = Pa_OpenStream(
              &stream,
              NULL,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
              paCallback,
              (void*)korg /* pass in the midi device so port audio can see it. */);
  if (err != paNoError) quit(err, "Couldn't open stream.");

  err = Pa_StartStream( stream );
  if( err != paNoError ) quit(err, "Couldn't start stream.");

  /* Sleep. */
  while(true) {
    Pa_Sleep(10*1000);
  }

  err = Pa_StopStream( stream );
  if( err != paNoError ) quit(err, "Couldn't stop stream.");
  err = Pa_CloseStream( stream );
  if( err != paNoError ) quit(err, "Couldn't close stream.");

  quit(err, "Success.");
}
