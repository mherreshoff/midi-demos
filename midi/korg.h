/*
 * KOrg is a class for reading data from the Korg nano-Kontrol 2 midi controller.
 */
#include "portmidi.h"

class KOrg {
 public:
  // The constructor finds the right the midi device and opens it.
  KOrg();

  // update processes any incoming midi events.
  void update();

  // getSlider reads the current value of the specified slider on the korg device.
  // The result will be between zero and one.
  double getSlider(long index);
  double getKnob(long index);
 private:
  static const long kMidiBufferSize;
  static const long kMaxValue;
  void processMidiEvent(const PmEvent& event);
  static bool is_initialized;
  double controller_state[128];
  PortMidiStream* midi_stream;
};

