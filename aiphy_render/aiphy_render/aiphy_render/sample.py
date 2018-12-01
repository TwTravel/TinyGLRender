#import cv2
#import matplotlib.pyplot as plt
import numpy as np
#from sample1 import imgcpt
from  aiphy_render.render_api import modelset
from aiphy_render.render_api  import modelrender
#src = cv2.imread("./pic_objs.bmp")
modelset("./bunny.stl", 80.0, 1.0, 1.0, 0, 0.1, 0, 0, 0, 0, 0, 4)
modelset("./forearm.stl", 80.0, 1.0, 1.0, 0, 0.2, 0, 0, 0, 0, 0, 2)
modelset("./bunny.stl", 240.0, 1.0, 1.0, 0.1,-0.1, 0, 0, 0, 0, 0, 8.5)
modelrender()
#print(imgcpt(src))
