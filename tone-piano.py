#!/usr/bin/python2.7

import casio
import tone

import numpy as np
import math
import pygame.midi as midi
import pygame.mixer as mixer
import pygame.sndarray as sndarray
import pygame.time as time
import sys

casio.init()
mixer.init()

def note_to_frequency(n):
  return 440 * (2**((n-57.0) / 12.0))

scale_names = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C"]
def note_to_string(n):
  return "Octave %d note %s freq=%lf." % (int(n / 12), scale_names[n%12], note_to_frequency(n))

harmonics = [float(x) for x in sys.argv[1:]]
print harmonics
notes = [tone.harmonic_tone(note_to_frequency(n), harmonics) for n in range(150)]
def play_note(n):
  notes[n].play()

while True:
  time.wait(10)
  if casio.has_note():
    status, note = casio.read_note()
    if status == "On":
      print "Turned on note (%s)" % note_to_string(note)
      play_note(note)
    elif status == "Off":
      print "Turned off note (%s)" % note_to_string(note)
    else:
      print "Unrecognized Status: %d" % status

