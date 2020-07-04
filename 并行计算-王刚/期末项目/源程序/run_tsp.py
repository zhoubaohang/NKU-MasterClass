# -*- coding: utf-8 -*-
"""
Created on Fri Dec 21 22:03:10 2018

@author: 周宝航
"""
import argparse
import numpy as np
from tsp import TSP
from genetic_algorithm import TSPGA, ParallelTSPGA

np.random.seed(0)

parser = argparse.ArgumentParser()
parser.add_argument('--parallel',    '-L', type=bool,  default=False, help='run the parallel GA for TSA')
parser.add_argument('--cities',      '-N', type=int,   default=20,    help='the number of cities in TSP')
parser.add_argument('--cross_rate',  '-C', type=float, default=0.01,  help='the cross rate in GA')
parser.add_argument('--mutate_rate', '-M', type=float, default=0.005, help='the mutate rate in GA')
parser.add_argument('--population',  '-P', type=int,   default=500,   help='the population size in GA')
parser.add_argument('--generation',  '-G', type=int,   default=100,   help='the genetaion size in GA')
args = parser.parse_args()

N_CITIES        = args.cities
POP_SIZE        = args.population
CROSS_RATE      = args.cross_rate
MUTATE_RATE     = args.mutate_rate
N_GENERATIONS   = args.generation

tsp_model = TSP(N_CITIES)

if args.parallel:
    ga    = ParallelTSPGA(N_CITIES, POP_SIZE, tsp_model,
                          cross_rate=CROSS_RATE, mutation_rate=MUTATE_RATE,
                          n_generations=N_GENERATIONS)
else:
    ga    = TSPGA(N_CITIES, POP_SIZE, tsp_model,
                  cross_rate=CROSS_RATE, mutation_rate=MUTATE_RATE,
                  n_generations=N_GENERATIONS)

ga.evolve()