#!/usr/bin/python
# -*- coding: utf-8 -*-

import ictclas


print __file__

print "OK"
data = u"教育部启动中外合作办学评估试点工作"
for tok in ictclas.para_proc_a(data):
    print tok[0]
