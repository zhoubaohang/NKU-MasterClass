import logging
import numpy as np
from itertools import chain
try:
    from mpi4py import MPI
    MPI_INSTALLED = True
except ImportError:
    MPI_INSTALLED = False

class MPIUtil(object):
    def __init__(self):
        logger_name = f'{self.__class__.__name__}'
        self._logger = logging.getLogger(logger_name)

    # Wrapper for common MPI interfaces.
    def barrier(self):
        if MPI_INSTALLED:
            mpi_comm = MPI.COMM_WORLD
            mpi_comm.barrier()

    @property
    def rank(self):
        if MPI_INSTALLED:
            mpi_comm = MPI.COMM_WORLD
            return mpi_comm.Get_rank()
        else:
            return 0

    @property
    def size(self):
        if MPI_INSTALLED:
            mpi_comm = MPI.COMM_WORLD
            return mpi_comm.Get_size()
        else:
            return 1

    @property
    def is_master(self):
        return self.rank == 0

    def split_size(self, size):
        '''
        Split a size number(int) to sub-size number.
        '''
        splited_idx = {}
        idx = np.arange(size)
        if size < self.size:
            return idx
        else:
            rank_id = 0
            inc = size//self.size
            for i in range(0, size, inc):
                splited_idx[rank_id] = idx[i:] if i + inc >= size\
                                                 else idx[i:i+inc]
                rank_id += 1
            return splited_idx[self.rank]

    def merge_seq(self, seq):
        '''
        Gather data in sub-process to root process.
        '''
        if self.size == 1:
            return seq
        mpi_comm = MPI.COMM_WORLD
        merged_seq = mpi_comm.allgather(seq)
        return merged_seq