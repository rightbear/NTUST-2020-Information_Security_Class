#!/usr/bin/env python
# coding: utf-8

# In[7]:


from random import randrange
from hashlib import sha1
from gmpy2 import xmpz, to_binary, is_prime


# In[8]:


#用 extended euclidean algorithm 找 mod 反元素
def EEA(integer1, integer2):
    x,y, u,v = 0,1, 1,0
    while integer1 != 0:
        q, r = (integer2 // integer1), (integer2 % integer1)
        m, n = (x - u * q), (y - v * q)
        integer2, integer1, x, y, u, v = integer1, r, u, v, m, n
    gcd = integer2
    return gcd, x, y

#用 square and multiply 加速指數運算
def SAM(base, exp, n):
    bin_exp = bin(exp)
    value = base
    for i in range(3, len(bin_exp)):
        value = (value ** 2) % n
        if(bin_exp[i:i+1] == '1'):
            value = (value * base) % n
    return value


# In[9]:


#代入指定的p, q長度以生成p,q,alpha
def generate_params(p_bits, q_bits):
    prime_p, prime_divisor_q = generate_p_q(p_bits, q_bits)
    integer_alpha = generate_alpha(prime_p, prime_divisor_q)
    return prime_p, prime_divisor_q, integer_alpha

#生成公鑰中的p, q
def generate_p_q(p_bits, q_bits):
    g = q_bits  # g >= 160
    n = (p_bits - 1) // g
    b = (p_bits - 1) % g
    while True:
        # 生成p
        while True:
            s = xmpz(randrange(1, 2 ** (g)))
            a = sha1(to_binary(s)).hexdigest()
            zz = xmpz((s + 1) % (2 ** g))
            z = sha1(to_binary(zz)).hexdigest()
            U = int(a, 16) ^ int(z, 16)
            mask = 2 ** (q_bits - 1) + 1
            prime_divisor_q = U | mask
            if is_prime(prime_divisor_q, 20):
                break
        # 生成q
        i = 0  # counter
        j = 2  # offset
        while i < 4096:
            V = []
            for k in range(n + 1):
                arg = xmpz((s + j + k) % (2 ** g))
                zzv = sha1(to_binary(arg)).hexdigest()
                V.append(int(zzv, 16))
            W = 0
            for qq in range(0, n):
                W += V[qq] * 2 ** (160 * qq)
            W += (V[n] % 2 ** b) * 2 ** (160 * n)
            X = W + (2 ** (p_bits - 1))
            c = X % (2 * prime_divisor_q)
            prime_p = X - c + 1  # p = X - (c - 1)
            if prime_p >= (2 ** (p_bits - 1)):
                if is_prime(prime_p, 10):
                    return prime_p, prime_divisor_q
            i += 1
            j += n + 1

#生成公鑰中的alpha
def generate_alpha(prime_p, prime_divisor_q):
    while True:
        integer_h = randrange(2, prime_p - 1)
        exp = xmpz((prime_p - 1) // prime_divisor_q)
        integer_alpha = SAM(integer_h, exp, prime_p) #用SAM加速
        if integer_alpha > 1:
            break
    return integer_alpha

# 生成公鑰中的beta與私鑰
def generate_keys(integer_alpha, prime_p, prime_divisor_q):
    #生成d
    integer_d = randrange(2, prime_divisor_q)  # x < q
    #生成beta
    integer_beta = SAM(integer_alpha, integer_d, prime_p) #用SAM加速
    return integer_d, integer_beta


# In[10]:


#簽章的流程
def Signing(message, prime_p, prime_divisor_q, alpha, integer_d):
    kE = randrange(2, prime_divisor_q)
    kE_inv = EEA(kE, prime_divisor_q)[1] % prime_divisor_q
    signature_r = SAM(alpha, kE, p) % q #用SAM加速
    
    # 建立 SHA1 物件
    hash_message = sha1(message.encode()).hexdigest()   # 將Hash的結果以16進位的整數表示
    hash_message = int(hash_message, 16)  # 將16進位的整數轉成10進位的整數
    
    signature_s = ((hash_message + integer_d * signature_r) * kE_inv) % prime_divisor_q
    
    return signature_r, signature_s


# In[11]:


#驗章的流程
def Verifying(message, signature_r, signature_s, prime_p, prime_divisor_q, alpha, beta):
    w = EEA(signature_s, prime_divisor_q)[1] % prime_divisor_q
    
    # 建立 SHA1 物件
    hash_message = sha1(message.encode()).hexdigest()
    hash_message = int(hash_message, 16)
    
    u1 = (w * hash_message) % prime_divisor_q
    u2 = (w * signature_r) % prime_divisor_q
    verification_v = ((SAM(alpha, u1, prime_p) * SAM(beta, u2, prime_p)) % prime_p) % prime_divisor_q #用SAM加速

    if (verification_v % prime_divisor_q) == (signature_r % prime_divisor_q): result = "valid"
    else: result = "invalid"
    
    return result


# In[12]:


if __name__ == '__main__':
    
    # Step0: Initiate the all procedures
    while True :
        start = input("\n請輸入 {0:s} 以初始化Key: ".format("\"Init\""))
        if(start == "Init"): break
    
    # Step1: Decide public keys(p, q, Alpha, Beta) and private key(d)
    p_length = 1024
    q_length = 160
    p, q, Alpha = generate_params(p_length, q_length)
    d, Beta = generate_keys(Alpha, p, q)

    print("\nThe public keys are =>\n\t(1) p: {0:d}\n\t(2) q: {1:d}\n\t(3) Alpha: {2:d}\n\t(4) Beta: {3:d}".format(p, q, Alpha, Beta))
    print("\nThe private key is =>\n\t{0}".format(d))
    
    # Step3: Let users choose working mode, signing or verification 
    while True :
        mode = input("\n請輸入要執行的工作模式, 簽署請輸入 \"Sign\" , 驗證請輸入 \"Verify\" ，結束請輸入 \"End\" : ")
        if (mode == "Sign") | (mode == "Verify"):
            
    # Step4: Design signing procedure
            if mode == "Sign":
                message = input("\nPlease input your message: ")
                r, s = Signing(message, p, q, Alpha, d)
                print("\nYour final signature are =>\n\t(1) r: {0}\n\t(2) s: {1}".format(r, s))
                                  
    # Step5: Design verifying procedure
            elif mode == "Verify":
                message = input("\nPlease input your message: ")
                result = Verifying(message, r, s, p, q, Alpha, Beta)
                print("\nYour final verification is \"{0:s}\"".format(result))
                                  
        elif mode == "End": break

