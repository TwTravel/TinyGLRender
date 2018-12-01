#import cv2
#import matplotlib.pyplot as plt
import numpy as np
#from sample1 import imgcpt
from  aiphy_render import AddStlModel, Render3dModel
# from aiphy_render.render_api  import modelrender
#src = cv2.imread("./pic_objs.bmp")
AddStlModel("./bunny.stl", 80.0, 1.0, 1.0, 0, 0.1, 0, 0, 0, 0, 0, 4)
AddStlModel("./forearm.stl", 80.0, 1.0, 1.0, 0, 0.2, 0, 0, 0, 0, 0, 2)
AddStlModel("./bunny.stl", 240.0, 1.0, 1.0, 0.1,-0.1, 0, 0, 0, 0, 0, 8.5)
Render3dModel(0)
#print(imgcpt(src))
