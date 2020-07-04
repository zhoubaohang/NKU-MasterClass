
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 27 11:51:58 2018
@author: 安颖
"""
import numpy as np
 
 
#定义属性值
Outlook = ["Sunny","Overcast","Rain"]
Temperature = ["Hot","Mild","Cool"]
Humidity = ["High","Normal"]
Wind = ["Strong","Weak"]
Attri=[]
Attri.append(Outlook)
Attri.append(Temperature)
Attri.append(Humidity)
Attri.append(Wind)
 
#数据集
my_data = []
with open('data.txt', 'r') as data_txt:
    data = data_txt.readlines()
    for line in data:
        temp = line.split(',')
        my_data.append([temp[0],temp[1],temp[2],temp[3],int(temp[4])])
    my_data = np.array(my_data)
data_txt.close()
 
#初始化S和G集合
S = [["#","#","#","#"]]
G = [["?","?","?","?"]]
 
#判断实例是否属于某一概念
def is_range (d1,d2):
    flag = 1
    for i in range(4):
        if str(d2[i])!='?':
            if d1[i]!=d2[i]:
                flag = 0
                break
    return flag
 
#寻找S集的一般泛化式（将S一般化）
def find_s_min(data,S):
    min_s=[]
    s1=["#","#","#","#"]
    for j in range(4):
        if S[j]=='#':
            s1[j]=data[j]
        elif S[j]=='?':
            s1[j]=S[j]
        else:
            if S[j]!=data[j] :
                s1[j]='?'
            else:
                s1[j]=data[j]
    min_s.append(s1)
    return min_s
 
#寻找S集的一般泛化式（将G特殊化）
def find_g_min(data,S):
    min_g=[]
    for j in range(4):
        if str(S[j])=='?' or str(data[j])==str(S[j]) :
            for i in range(len(Attri[j])):
                if str(Attri[j][i])!=str(data[j]):
                    s1 = ["?","?","?","?"]
                    for k in range(4):
                        if k==j:
                            s1[k]=Attri[j][i]
                        else:
                            s1[k]=S[k]
                    min_g.append(s1)
            
    return min_g
 
#判断最小泛化函数是否可以加入S集合中
def vertificate_smin(S,G,min_s):
    flag = 1
    for i in range(len(min_s)) :
        #若min_s中包含S集合的某个概念 则删除该概念 并返回0
        for j in range(len(S)):
            if is_range(S[j],min_s[i])==1 :
                #del S[j]
                flag = 0
                break
        #若min_S比G集合一般 则返回0
        for j in range(len(G)):
            if is_range(min_s[i],G[j])==1:
                flag = 1
                break
            else:
                flag = 0
        if flag==1:
            S.append(min_s[i])
    
#判断最小泛化函数是否可以加入G集合中
def vertificate_gmin(S,G,min_g):
    flag = 1
    for i in range(len(min_g)):
        #若G集合包含该概念 则退出
        for j in range(len(G)):
            if is_range(G[j],min_g[i])==1:
                flag = 0
                break
        #判断S是否比G更特殊
        for j in range(len(S)):
            if is_range(S[j],min_g[i])==1 or str(S[j])=='#':
                flag = 1
        if flag==1:
           G.append(min_g[i])            
    
def Candidate(S,Q,data):
    if int(data[4]) == 1:
        
        #极大一般边界
        for i in range(len(G)):
            #若G中有不符合data的集合，则移除
            #print(str(i)+":"+str(len(G)))
            if i<len(G) and is_range(data,G[i])==0:
                del G[i]
            else :
                break
        #极大特殊边界
        for i in range(len(S)):
            #若S中有不符合data的概念s，则将s移除并将符合条件的最小一般泛化式放入
            if i<len(G) and is_range(data,S[i])==0:
                
                min_s = find_s_min(data,S[i])
                vertificate_smin(S,G,min_s)
                del S[i]
                
            else:
                break
 
    else:
        
        #极大特殊边界
        for i in range(len(S)):
            #若S中有符合data的集合，则移除
            if is_range(data,S[i])!=0:
                del S[i]
        #极大一般边界
        for i in range(len(G)):
            #若G中有不符合data的概念s，则将s移除并将符合条件的最小一般泛化式放入
            if is_range(data,G[i])!=0:
                s = G[i]
                del G[i]
                min_g = find_g_min(data,s)
                vertificate_gmin(S,G,min_g)
                #print("Glen"+str(len(G)))
                
if  __name__    ==   '__main__':
    for i in range(len(my_data)):
        print("第"+str(i+1)+"个示例："+str(my_data[i]))
        Candidate(S,G,my_data[i])
        print("S集合："+str(S))
        print("G集合："+str(G))
        print("________"*15)