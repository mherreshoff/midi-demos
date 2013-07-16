# This file reads midi events from a Casio CTK-2100 piano.
import pygame.midi as midi

casio_input = None
def init():
  global casio_input
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

  casio_input = midi.Input(piano_input_id)


def has_note():
  return casio_input.poll()

def read_note():
  event = casio_input.read(1)[0]
  data, timestamp = event
  status, note, data2, data3 = data

  if status == 144:
    status_name = "On"
  elif status == 128:
    status_name = "Off"
  else:
    status_name = "Unknown"
  return (status_name, note)
