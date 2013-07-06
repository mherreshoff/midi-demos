#!/usr/bin/python2.7

import piano

import numpy as np
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


bowl = mixer.Sound("bowla.wav")
samples_per_second = pygame.mixer.get_init()[0]
def make_note(n):
  a1 = sndarray.array(bowl)


notes = [make_note(n) for n in range(150)]
def play_note(n):
  notes[n].play()

while True:
  time.wait(10)
  if piano_input.poll():
    event = piano_input.read(1)[0]
    data, timestamp = event
    status, note, data2, data3 = data
    if status == 144: # Note turns on.
      print "Turned on note %s" % note_to_string(note)
#      play_note(note)
    elif status == 128: # Note turns off.
      print "Turned off note %s" % note_to_string(note)
    else:
      print "Unrecognized Status: %d" % status

