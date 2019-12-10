#!/usr/bin/env python
# coding: utf-8

import random


small_prime = [3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199,
211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397,
401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599,
601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797,
809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997]


def Miller_Robin(N, times=10):

    # Implementation uses the Miller-Rabin Primality Test
    # The optimal number of rounds for this test is 10
    m = N - 1
    k = 0
    while True:
        m = m // 2
        k = k + 1
        if (m % 2) == 1:
            break
    
    a = random.randint(2, N-2)
    b = SAM(a, m, N)
    
    if (b != 1) & (b != (N-1)):
        i = 1
        while (i < k) & (b != (N-1)):
            b = (b ** 2) % N
            if (b == 1):
                return False
            i = i + 1
        if b != (N - 1):
            return False
    return True



def GCD(integer1, integer2):  
    if integer1 == 0 : 
        return integer2 
    return GCD(integer2 % integer1, integer1)


def EEA(integer1, integer2):
    x,y, u,v = 0,1, 1,0
    while integer1 != 0:
        q, r = (integer2 // integer1), (integer2 % integer1)
        m, n = (x - u * q), (y - v * q)
        integer2, integer1, x, y, u, v = integer1, r, u, v, m, n
    gcd = integer2
    return gcd, x, y



def SAM(base, exp, n):
    bin_exp = bin(exp)
    value = base
    for i in range(3, len(bin_exp)):
        value = (value ** 2) % n
        if(bin_exp[i:i+1] == '1'):
            value = (value * base) % n
    return value


def Encryption(plaintext, e, n):
    ciphertext = []
    for plain_alphabet in plaintext:
        plain_ascii = ord(plain_alphabet)
        encrypt_ascii = SAM(plain_ascii, e, n)
        ciphertext.append(encrypt_ascii)
    return ciphertext


def CRT(base, exp, prime_p, prime_q):
    exp_p = exp % (prime_p - 1)
    exp_q = exp % (prime_q - 1)
    q_inv = EEA(prime_q, prime_p)[1] % prime_p
    m1 = SAM(base, exp_p, prime_p)
    m2 = SAM(base, exp_q, prime_q)
    h = (q_inv * (m1 - m2)) % prime_p
    value = m2 + h * prime_q
    return value


def Decryption(ciphertext, d, p, q):
    ciphertext = ciphertext.split(",")
    plaintext = "" 
    for cipher_ascii in ciphertext:
        cipher_ascii = int(cipher_ascii)
        decrypt_ascii = CRT(cipher_ascii, d, p, q)
        decrypt_alphabet = chr(decrypt_ascii) 
        plaintext += decrypt_alphabet
    return plaintext


if __name__ == '__main__':
    
    # Step0: Initiate the all procedures
    while True :
        start = input("\n請輸入 {0:s} 以初始化加解密用的金鑰: ".format("\"Init\""))
        if(start == "Init"): break
    
    # Step1: Decide primes p and q, using  Miller-Robin Test.
    while True:
        p = random.randint(2 ** 512, 2 ** 513 - 1)
        check = False
        
        for prime in small_prime:
            if p % prime == 0: 
                check = True
                break
                
        if check == True: continue
        elif Miller_Robin(p) == True: break
            
    while True:
        q = random.randint(2 ** 512, 2 ** 513 - 1)
        check = False
        
        for prime in small_prime:
            if q % prime == 0: 
                check = True
                break
                
        if check == True: continue
        elif (q != p) & Miller_Robin(q): break

    print("The two primes are =>\n\t(1){0:d}\n\t(2){1:d}".format(p, q))
    
    # Step2: Decide RSA keys, using Extended Euclidean Algorithm (EEA)
    n = p * q
    
    phi_n = (p-1) * (q-1)

    while True:
        e = random.randint(1, phi_n - 1)
        if GCD(e, phi_n) == 1:
            d = EEA(e, phi_n)[1]
            d = d % phi_n
            break

    print("The phi is =>\n\t{0:d}\nThe encrypt exponent is =>\n\t{1:d}          \nThe decrypt exponent is =>\n\t{2:d}".format(phi_n, e, d))
    
    # Step3: Let users choose working mode, encryption or decryption 
    while True :
        mode = input("\n請輸入要執行的工作模式, 加密請輸入 \"Encrypt\" , 解密請輸入 \"Decrypt\" ，結束請輸入 \"End\" : ")
        if (mode == "Encrypt") | (mode == "Decrypt"):
            
    # Step4: Design encrypting procedure, using Square-and-Multiply(SAM)
            if mode == "Encrypt":
                plaintext = input("Please input your plaintext: ")
                ciphertext = Encryption(plaintext, e, n)
                print("Your final ciphertext is {0}".format(ciphertext))
                                  
    # Step5: Design decrypting procedure, using Chinese-Remainder-Theorem(CRT)
            elif mode == "Decrypt":
                ciphertext = input("Please input your ciphertext: ")
                plaintext = Decryption(ciphertext, d, p, q)
                print("Your final plaintext is \"{0:s}\"".format(plaintext))
                                  
        elif mode == "End": break