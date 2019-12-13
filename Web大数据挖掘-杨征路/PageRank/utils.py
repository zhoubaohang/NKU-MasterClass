import os

def load_block():
    blocks = [i for i in os.listdir('.') if 'block' in i]

    for block in blocks:
        with open(block, 'r') as fp:
            yield fp.readlines()

def load_r(filename, block_size):
    index = 0
    cache = {}
    with open(filename, 'r') as fp:
        for line in fp:
            k, v = line.strip('\n').split(' ')
            
            if index < block_size:
                cache[int(k)] = float(v)
                index += 1
            else:
                yield cache
                cache.clear()
                index = 0
        yield cache