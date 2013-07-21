#include "korg.h"

#include <iostream>
#include <string>

using namespace std;

const long KOrg::kMidiBufferSize = 2048;
const long KOrg::kMaxValue = 127;
bool KOrg::is_initialized = false;

KOrg::KOrg()
{
  if (!is_initialized) {
    Pm_Initialize();
    is_initialized = true;
  }
  long num_devices = Pm_CountDevices();
  long device_id = -1;
  for (long i = 0; i < num_devices; i++) {
    const PmDeviceInfo* info = Pm_GetDeviceInfo(i);
    string name = info->name;
    if (name == "nanoKONTROL2 SLIDER/KNOB") {
      device_id = i;
    }
  }
  if (device_id == -1) {
    cerr << "KOrg nanoKontrol2 device not connected." << endl;
    exit(0);
  }
  Pm_OpenInput(&midi_stream, device_id, NULL, kMidiBufferSize, NULL, NULL);
}

void KOrg::update()
{
  PmEvent event;
  while(Pm_Poll(midi_stream)) {
    Pm_Read(midi_stream, &event, 1);
    PmMessage message = event.message;
    long status = Pm_MessageStatus(message);
    long control = Pm_MessageData1(message);
    long value = Pm_MessageData2(message);
    if (status == 176) {
      controller_state[control] = (double)value / (double)kMaxValue;
    } else {
      cout << "Unidentified MIDI status code: " << status << endl;
    }
  }
}

double KOrg::getSlider(long index) {
  return controller_state[index];
}

double KOrg::getKnob(long index) {
  return controller_state[16+index];
}
