import numpy as np

def FindS(data):
    # 目标概念
    h = None
    for i, d in enumerate(data):
        # 若为正例样本
        if d[-1] == 'Yes':
            if h is None:
                h = d[:-1]
            else:
                for j, v in enumerate(d[:-1]):
                    # 若不符合约束，替换为更宽泛的约束
                    if h[j] != v:
                        h[j] = '?'
            print(f"Data: {d}\nh: {h}")
            print("..................................................")
    return h

class CandidateElimination(object):

    def __init__(self, data):
        self.data = data
        self.attr = self.collect_attr(data)
        self.S = [['o'] * len(self.attr)]
        self.G = [['?'] * len(self.attr)]

    def __call__(self):
        '''
            算法主入口
        '''
        for d in self.data:
            # 特征
            x = d[:-1]
            # 标签
            y = d[-1]

            if y == 'Yes':
                # 从G中移去与d不一致的假设
                tmp = []
                for g in self.G:
                    if self.consistent(g, x):
                        tmp.append(g)
                self.G = tmp
                # 对S进行一般化
                self.findMinNormal(x)
            else:
                # 从S中移去与d不一致的假设
                tmp = []
                for s in self.S:
                    if not self.consistent(s, d):
                        tmp.append(s)
                self.S = tmp
                # 对G进行特殊化
                self.findMinSpecial(x)
        
            print(f"Data: {d}")
            print(f"S:{self.S}")
            print(f"G:{self.G}")
            print("..................................................")

    def findMinSpecial(self, d):
        '''
            寻找极小特殊化式加入G
        '''
        tmp = []

        for g in self.G:
            # 若d符合假设g
            if not self.consistent(g, d):
                tmp.append(g)
            else:
            # 若d不符合假设g
                i = 0
                for x, y in zip(d, g):
                    if y == '?' or x == y:
                        # 根据属性构造极小特殊化式h
                        for a in self.attr[i]:
                            if a != x:
                                h = g[:]
                                h[i] = a
                                # 若h与d一致
                                if not self.consistent(h, d):
                                    flag = False
                                    # S的某个成员比h更特殊
                                    for s in self.S:
                                        flag = self.normal(h, s)
                                        if flag:
                                            break
                                    
                                    if flag:
                                        tmp.append(h)
                    i += 1
        G = []
        # 遍历添加极小化特殊式到G
        for i, g1 in enumerate(tmp):
            flag = True
            for j, g2 in enumerate(tmp):
                if i != j:
                    # 判断g是否比G中另一假设更特殊
                    flag = self.normal(g1, g2)
                    if not flag:
                        break
            # 若g满足条件则不被移去
            if flag:
                G.append(g1)
        
        self.G = G

    def findMinNormal(self, d):
        '''
            寻找极小一般化式加入S
        '''
        tmp = []

        for s in self.S:
            # 若d符合假设s
            if self.consistent(s, d):
                tmp.append(s)
            else:
            # 若d不符合假设s
                if s[0] == 'o': 
                    # 若S为最特殊假设
                    tmp.append(d)
                else:
                    # 根据样本d构造极小一般化式h
                    h = d[:]
                    for i, v in enumerate(s):
                        # 不符合的属性替换为一般约束'?'
                        if v != h[i]:
                            h[i] = '?'
                    # 若h与d一致
                    if self.consistent(h, d):
                        flag = False
                        for g in self.G:
                            # 若G的某个成员比h更一般
                            flag = self.normal(g, h)
                            if flag:
                                break
                        if flag:
                            tmp.append(h)
        
        S = []
        # 遍历添加极小一般化式到S
        for i, s1 in enumerate(tmp):
            flag = False
            for j, s2 in enumerate(tmp):
                if i != j:
                    # 判断s是否比S中另一假设更一般
                    flag = self.normal(s1, s2)
                    if flag:
                        break
            # 若s满足条件则不被移去
            if not flag:
                S.append(s2)
        
        self.S = S

    def normal(self, g, h):
        '''
            判断假设g是否比h更一般
        '''
        flag = True
        # 考虑假设g中的一般性
        n_g = len([1 for i in g if i == '?'])
        # 考虑假设g中的最特殊性
        o_g = len([1 for i in g if i == 'o'])
        # 考虑假设h中的一般性
        n_h = len([1 for i in h if i == '?'])
        # 考虑假设h中的最特殊性
        o_h = len([1 for i in h if i == 'o'])
        
        # 若g更特殊
        if o_g > 0 and o_h == 0:
            flag = False
        # 若h更特殊
        elif o_g == 0 and o_h > 0:
            flag = True
        # 若h与g都为特殊
        elif o_g > 0 and o_g == o_h:
            flag = True
        # 若g更一般
        elif n_g > n_h:
            flag = True
        # 若h更一般
        elif n_g < n_h:
            flag = False
        elif n_g == n_h:
            flag = True
            if g == h:
                flag = False
        
        return flag

    def consistent(self, h, d):
        '''
            判断样本d是否符合假设h
        '''
        flag = True
        # 遍历样本和假设的属性
        for x, y in zip(h, d):
            # 若假设h为最特殊假设
            if x == 'o':
                flag = False
                break
            # 若样本d不满足假设h
            if x != '?' and x != y:
                flag = False
                break
        
        return flag
    
    def collect_attr(self, data):
        '''
            统计样本的所有属性
        '''
        attr = []
        data = np.array(data)
        n_attr = data.shape[1] - 1

        for i in range(n_attr):
            attr.append(np.unique(data[:,i]).tolist())
        
        return attr


def CE(data):
    # 特殊边界S
    s = None
    # 一般边界G
    g = []

    for d in data:
        # 若为正例
        if d[-1] == 'Yes':
            # 一般化特殊边界S
            if s is None:
                s = d[:-1]
            else:
                for j, v in enumerate(d[:-1]):
                    if s[j] != v:
                        s[j] = '?'
            
            # 从G中移去与样本不相符的假设
            if len(g):
                tmp = []
                for t in g:
                    flag = True
                    for x, y in zip(d[:-1], t):
                        if y != '?' and x != y:
                            flag = False
                            break
                    if flag:
                        tmp.append(t)
                g = tmp
        # 若为负例
        else:
            if s is not None:
                # 根据负例特殊化边界G
                for i in range(len(s)):
                    if s[i] != '?' and d[i] != s[i]:
                        tmp = ['?'] * len(s)
                        tmp[i] = s[i]
                        g.append(tmp)

    return s, g