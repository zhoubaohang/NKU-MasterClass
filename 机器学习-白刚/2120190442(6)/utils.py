import numpy as np

def get_pi_idx(val, pdf):
    '''
        获取对应分布
    '''
    N = pdf.size
    accumulate = 0
    for i in range(0, N):
        accumulate += pdf[i]
        if accumulate >= val:
            return i

def generate_ensemble(out_pi, out_mu, out_sigma, M=10):
    '''
        采样混合分布
    '''
    NTEST = x.shape[1]
    result = np.random.rand(NTEST, M)
    rn = np.random.randn(NTEST, M)
    mu = 0
    std = 0
    idx = 0

    # transforms result into random ensembles
    for j in range(0, M):
        for i in range(0, NTEST):
            idx = get_pi_idx(result[i, j], out_pi[i])
            mu = out_mu[i, idx]
            std = out_sigma[i, idx]
            result[i, j] = mu + rn[i, j]*std
    return result