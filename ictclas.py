#!/usr/bin/python
# -*- coding: utf-8 -*-
#  FileName    : ictclas.py 
#  Author      : Feather.et.ELF <andelf@139.com> 
#  Created     : Wed May 26 08:58:02 2010 by Feather.et.ELF 
#  Copyright   : Feather Workshop (c) 2010 
#  Description : ictclas python binding 
#  Time-stamp: <2010-05-26 09:17:48 andelf> 

from _ictclas import *
import os.path
import atexit

__all__ = ['ICT_POS_MAP_FIRST',
           'ICT_POS_MAP_SECOND',
           'PKU_POS_MAP_FIRST',
           'PKU_POS_MAP_SECOND',
           'add_user_word',
           'del_user_word',
           'file_proc',
           'finger_print',
           'ictclas',
           'import_user_dict',
           'keyword',
           'para_proc',
           'para_proc_a',
           'para_word_count',
           'save_user_dict',
           'set_pos_map']

init(os.path.dirname(__file__))
atexit.register(exit)

