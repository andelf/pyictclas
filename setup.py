#  FileName    : setup.py 
#  Author      : Feather.et.ELF <andelf@gmail.com> 
#  Created     : Wed May 26 08:55:04 2010 by Feather.et.ELF 
#  Copyright   : Feather Workshop (c) 2010 
#  Description : setup script 
#  Time-stamp: <2010-05-26 08:57:36 andelf> 


from distutils.core import setup, Extension

print "Download libs at: http://ictclas.org/Down_share.html"

module1 = Extension('_ictclas',
                    sources = ['ictclas.cpp'],
                    libraries = ['ICTCLAS30'],
                    library_dirs = ['./'])

setup (name = 'ictclas',
       version = '3.0',
       description = 'ICTCLAS30 python binding, a segmentation lib for chinese.',
       author = 'Andelf (Feather)',
       author_email = 'andelf@gmail.com',
       long_description = '''
Download libs at: http://ictclas.org/Down_share.html
Then ./setup.py install
Tested with win32 version.
''',
       py_modules=['ictclas'],
       ext_modules = [module1])

