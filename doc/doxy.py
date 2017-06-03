from __future__ import print_function
import subprocess
import jinja2
import conf

def setup(app):
    env = jinja2.Environment(
            autoescape=True,
            loader=jinja2.FileSystemLoader('.'))
    header = env.get_template('header.html')
    context = getattr(conf, 'html_context', dict())
    subprocess.call(['cat', 'header.html'])
    with open('_header.html', 'w') as f:
        f.write(header.render(context))
    subprocess.call(['cat', '_header.html'])
    with open('../Doxyfile') as f1:
        with open('_Doxyfile', 'w') as f2:
            for line in f1:
                if line.startswith('HTML_HEADER '):
                    f2.write('HTML_HEADER = doc/_header.html')
                else:
                    f2.write(line)
    subprocess.call(['doxygen', 'doc/_Doxyfile'], cwd='..')
