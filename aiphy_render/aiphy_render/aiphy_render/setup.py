# setup.py
from distutils.core import setup, Extension

tinygl_src_file_list = ["api.cpp", "clip.cpp", "glu.cpp",  "init.cpp", "list.cpp",  "misc.cpp", "select.cpp", "vertex.cpp", "zline.cpp", "arrays.cpp",  "error.cpp",  "glx.cpp",  "kh_project.cpp", "matrix.cpp",  "msghandling.cpp",  "specbuf.cpp",  "zbuffer.cpp",  "zmath.cpp",
"clear.cpp",   "get.cpp",    "image_util.cpp",  "light.cpp",  "memory.cpp",  "oscontext.cpp",   "texture.cpp", "zdither.cpp", "ztriangle.cpp"]

tinygl_src_file_path_list = [ "aiphy_render/tinygl/src/" + src_file for src_file in tinygl_src_file_list]

sample_src_file_list = ['DrawObj.cpp']
sample_src_file_path_list = ["aiphy_render/sample/src/" + src_file for src_file in sample_src_file_list]


setup(name="aiphy_render", ext_modules=[
	Extension("aiphy_render.render_api", tinygl_src_file_path_list +  sample_src_file_path_list, include_dirs = [ 'aiphy_render/tinygl/include', 'aiphy_render/sample/include'], language="c++",  extra_compile_args=['-fpermissive','-fPIC'])]
)
