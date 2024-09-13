import numpy as np
import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt

from matplotlib.animation import FFMpegWriter

metadata = dict(title='Diffusion Test', artist='Patrin G.A.', comment='Well done!')
writer = FFMpegWriter(fps=5, metadata=metadata)

fig = plt.figure()
with open(r'output.raw','rb') as file:
    data_=file.read()
data = np.frombuffer(data_, dtype=np.double).reshape((20, 30, 40))

with writer.saving(fig, "output.mp4", 100):
  for i in range(np.shape(data)[2]):
    plt.imshow(data[:,:,i])
    writer.grab_frame()
