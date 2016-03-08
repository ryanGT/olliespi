#!/usr/bin/env python
import os

emacs_files = ['spi_polling_isr.py',\
               'spi_polling_two_isrs/spi_polling_two_isrs.ino',\
               ]

emacs_files = ['notes.rst'] + emacs_files

emacs_str = ' '.join(emacs_files)

#emacs_cmd = 'emacsclient -n %s &' % emacs_str

#print(emacs_cmd)

#os.system(emacs_cmd)

for curfile in emacs_files:
    cmd = 'emacsclient %s &' % curfile
    os.system(cmd)
