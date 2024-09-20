import numpy as np
import matplotlib
from tqdm import trange

matplotlib.use("Agg")
import matplotlib.pyplot as plt

from matplotlib.animation import FFMpegWriter

metadata = dict(title='Diffusion Test', artist='Patrin G.A.', comment='Well done!')
writer = FFMpegWriter(fps=5, metadata=metadata)

fig = plt.figure()
total_iter_num = 3000
iter_step = 10
Nt = int(total_iter_num / iter_step)
with writer.saving(fig, "home_output.mp4", 100):
  for i in trange(Nt):
    with open(f'output_{int(i*iter_step)}.raw','rb') as file:
      data_=file.read()
      data = np.frombuffer(data_, dtype=np.float32).reshape((301, 301))
    plt.imshow(data)
    writer.grab_frame()
