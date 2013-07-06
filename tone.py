import math
import numpy as np
import pygame.mixer as mixer
import pygame.sndarray as sndarray

def make_tone(freq):
  """Make a two channel sine wave."""
  samples_per_second, sample_size, channels = mixer.get_init()
  assert(sample_size < 0)
  max_amp = 2**(-sample_size - 1) - 1
  length = 1.0 # One second
  t = np.transpose(np.asmatrix([[1], [1]]) * np.linspace(0, length, length*samples_per_second))
  wave = np.array(np.sin(t * freq * 2 * math.pi) * max_amp/5, dtype=np.int16)
  return sndarray.make_sound(wave)
