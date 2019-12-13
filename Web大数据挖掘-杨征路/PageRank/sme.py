import os
import numpy as np

class SparseMatrixEncoding(object):

    def __init__(self):
        self.filename = "SME.txt"

    def fit(self, data):
        matrix = {}

        for s, d in data:
            s = int(s)
            d = int(d)
            if s not in matrix:
                matrix[s] = []
            if d not in matrix:
                matrix[d] = []
            matrix[s].append(d)
        
        self.N = max(matrix.keys()) + 1
        
        if not os.path.exists(self.filename):
            lines = [' '.join(map(str, [s, *d]))+'\n' for s, d in matrix.items()]
            with open(self.filename, 'a+') as fp:
                fp.writelines(lines)
    
    def break_stripe(self, block_size):
        index = 0
        while index <= self.nodesize:
            block = []
            with open(f"block_{index}.txt", 'w') as fw:
                with open(self.filename, 'r') as fr:
                    for line in fr:
                        src, *dst = line.strip('\n').split(' ')
                        
                        tmp = index
                        flag = False
                        for i in range(tmp, tmp+block_size):
                            if str(i) in dst:
                                flag = True
                                break
                        
                        if flag:
                            block.append(line)
                fw.writelines(block)
            index += block_size
    
    def matmul(self, r_old, r_new, block, beta=0.85):
        
        for line in block:
            src, *dst = line.strip('\n').split(' ')
            for j in range(len(dst)):
                if int(dst[j]) in r_new.keys():
                    r_new[int(dst[j])] += beta * r_old.get(int(src), 0) / len(dst)

        return r_new

    @property
    def nodesize(self):
        return self.N
