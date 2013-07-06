#!/usr/bin/python2.7

import piano

import numpy as np
import math
import pygame.midi as midi
import pygame.mixer as mixer
import pygame.sndarray as sndarray
import pygame.time as time
import sys

casio_input = piano.getCasioInput();

mixer.init()

scale_names = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C"]

def note_to_string(n):
  return "Octave %d note %s." % (int(n / 12), scale_names[n%12])

def make_tone(n):
  samples_per_second, sample_size, channels = mixer.get_init()
  assert(sample_size < 0)
  max_amp = 2**(-sample_size - 1) - 1
  length = 1.0 # One second
  t = np.transpose(np.asmatrix([[1], [1]]) * np.linspace(0, length, length*samples_per_second))
  freq = 440 * (2**((n-45.0) / 12.0))
  wave = np.array(np.sin(t * freq * 2 * math.pi) * max_amp, dtype=np.int16)
  return sndarray.make_sound(wave)


notes = [make_tone(n) for n in range(150)]
def play_note(n):
  notes[n].play()

while True:
  time.wait(10)
  if casio_input.poll():
    event = casio_input.read(1)[0]
    data, timestamp = event
    status, note, data2, data3 = data
    if status == 144: # Note turns on.
      print "Turned on note %s" % note_to_string(note)
      play_note(note)
    elif status == 128: # Note turns off.
      print "Turned off note %s" % note_to_string(note)
    else:
      print "Unrecognized Status: %d" % status

