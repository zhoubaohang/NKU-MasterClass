import os
import argparse
import numpy as np
from utils import load_r, load_block
from sme import SparseMatrixEncoding as SME

parser = argparse.ArgumentParser()
parser.add_argument('--threshold', type=float, default=1e-10)
parser.add_argument('--beta', type=float, default=0.85)
parser.add_argument('--save', type=bool, default=True)
parser.add_argument('--top', type=int, default=100)
parser.add_argument('--block', type=int, default=1000)
args = parser.parse_args()

sme = SME()

block_size = args.block

errors = 0

with open('WikiData.txt', 'r') as fp:
    block = []
    for line in fp:
        data = line.strip('\n').split('\t')
        block.append(data)
    sme.fit(block)

# 创建r_new向量
index = 0
with open('r_new.txt', 'w') as fp:
    while index <= sme.nodesize:
        fp.write(f"{index} {0.}\n")
        index += 1

# 创建r_old向量
index = 0
with open('r_old.txt', 'w') as fp:
    while index <= sme.nodesize:
        fp.write(f"{index} {1./sme.nodesize}\n")
        index += 1
        errors += 1. / sme.nodesize

# 稀疏矩阵分块
print("正在分块....")
sme.break_stripe(block_size)

iter_cnt = 0
print("开始计算....")
while errors > args.threshold:
    iter_cnt += 1
    r_new_sum = 0
    
    for r_new, block in zip(load_r('r_new.txt', block_size), load_block()):
        r_new_tmp = r_new
        for r_old in load_r('r_old.txt', block_size):
            r_new_tmp = sme.matmul(r_old, r_new_tmp, block)
            
        with open('r_new_tmp.txt', 'a+') as fp:
            for k, v in r_new_tmp.items():
                fp.write(f"{k} {v}\n")

    for r_new_tmp in load_r('r_new_tmp.txt', block_size):
        r_new_sum += sum(r_new_tmp.values())

    clip = (1. - r_new_sum) / sme.nodesize
    ss = 0
    
    for r_new in load_r('r_new_tmp.txt', block_size):
        with open('r_new_clip.txt', 'a+') as fp:
            for k, v in r_new.items():
                fp.write(f"{k} {v+clip}\n")
                

    errors = 0
    index = 0
    with open('r_new_clip.txt', 'r') as f1:
        with open('r_old.txt', 'r') as f2:
            for l1, l2 in zip(f1, f2):
                new = float(l1.strip('\n').split(' ')[1])
                old = float(l2.strip('\n').split(' ')[1])
                errors += abs(new - old)
                index += 1

    os.remove('r_old.txt')
    os.remove('r_new_tmp.txt')
    os.rename('r_new_clip.txt', 'r_old.txt')

    print(f"{iter_cnt} {errors}")

if args.save:
    result = np.loadtxt('r_old.txt', delimiter=' ')
    index = np.argsort(result[:,1])[-args.top:]
    with open('result.txt', 'w') as fp:
        for i in reversed(index):
            fp.write(f"{int(result[i][0])} {result[i][1]}\n")