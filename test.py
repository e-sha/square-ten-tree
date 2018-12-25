import tempfile
import numpy as np
import numpy.random as npr
import random
import subprocess
import os
import os.path as osp
import shutil

def is_ge(a, b):
    if a.size > b.size:
        return True
    if a.size < b.size:
        return False
    for i, j in zip(a, b):
        if i > j:
            return True
        if i < j:
            return False
    return True

def gen(k=100):
    n = random.randint(1, k)
    a = npr.randint(0, 10, size=n)
    n = random.randint(1, k)
    b = npr.randint(0, 10, size=n)
    if is_ge(a, b):
        a, b = b, a
    a = ''.join(map(str, a))
    b = ''.join(map(str, b))
    return a, b

def sure_dir_exists(d):
    p = osp.realpath(d)
    if not osp.isdir(p):
        os.makedirs(p)
    return d

tests_dir = sure_dir_exists('hard_tests')
n = 0

FNULL = open(os.devnull, 'w')

for i in range(100):
    a, b = gen()
    tfile, fn = tempfile.mkstemp(text=True)
    os.write(tfile, '{}\n'.format(a).encode())
    os.write(tfile, '{}\n'.format(b).encode())
    os.close(tfile)
    cat = subprocess.Popen(('cat', fn), stdout=subprocess.PIPE)
    gt = subprocess.check_output(('python3', '1.py'), stdin=cat.stdout)
    cat.wait()
    cat = subprocess.Popen(('cat', fn), stdout=subprocess.PIPE)
    pred = subprocess.check_output(('./1'), stdin=cat.stdout, stderr=FNULL)
    cat.wait()
    if gt != pred:
        shutil.copyfile(fn, osp.join(tests_dir, 'htest{}.txt'.format(n)))
        n += 1

FNULL.close()

print("{} hard tests written".format(n))
