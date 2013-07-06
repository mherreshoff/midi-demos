import pygame.midi as midi

def getCasioInput():
  midi.init()
  piano_input_id = None
  for device_id in range(midi.get_count()):
    interf, name, input, output, opened = midi.get_device_info(device_id)
    if "CASIO" in name and input == 1:
      piano_input_id = device_id
      break

  if piano_input_id is None:
    return None
  else:
    print "Piano is connected on interface: " + str(piano_input_id)

  return midi.Input(piano_input_id)

