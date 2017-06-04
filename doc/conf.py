import sys
import os

sys.path.insert(0, os.path.abspath('.'))
extensions = ['doxy']

master_doc = 'index'
exclude_patterns = ['_build']

# Just copy all doxygen files over
html_extra_path = ['html']
