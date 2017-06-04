import sys
import os

sys.path.insert(0, os.path.abspath('.'))
extensions = ['doxy']

exclude_patterns = ['_build']
html_extra_path = ['html']
