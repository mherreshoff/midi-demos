import math
import numpy as np
import pygame.mixer as mixer
import pygame.sndarray as sndarray

MAX_AMP = 2**15 - 1
def as_sound(vector):
  scaled = vector * MAX_AMP/4
  two_channel = np.array(np.transpose(np.asmatrix([[1], [1]]) * scaled), dtype=np.int16).copy(order='C')
  return sndarray.make_sound(array=two_channel)
# Note: The copy(order='C') is here to prevent the error message "ValueError: ndarray is not C-contiguous"
# As documented here: https://groups.google.com/forum/#!msg/briansupport/YmhR4rXLcBI/n-_F-nwEI6YJ

def pure_tone_wave(freq):
  """Make a two channel sine wave."""
  samples_per_second, sample_size, channels = mixer.get_init()
  length = 1.0 # One second
  t = np.linspace(0, length, length*samples_per_second)
  return np.sin(t * freq * 2 * math.pi) 

def pure_tone(freq):
  wave = pure_tone_wave(freq)
  return as_sound(wave)

def harmonic_tone_wave(freq, harmonics):
  waves = [h*pure_tone_wave(freq*(i+1)) for i,h in enumerate(harmonics)]
  wave = np.sum(waves, axis=0)
  return wave

def harmonic_tone(freq, harmonics):
  wave = harmonic_tone_wave(freq, harmonics)
  return as_sound(wave)
