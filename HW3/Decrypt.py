#!/usr/bin/env python
# coding: utf-8

# In[4]:


from PIL import Image
import os, random, struct
from Crypto.Cipher import AES

try:
    from Crypto.Util.Padding import pad, unpad
except ImportError:
    from Crypto.Util.py3compat import bchr, bord
    def unpad(padded_data, block_size):
        pdata_len = len(padded_data)
        if pdata_len % block_size:
            raise ValueError("Input data is not padded")
        padding_len = bord(padded_data[-1])
        if padding_len < 1 or padding_len > min(block_size, pdata_len):
            raise ValueError("Padding is incorrect.")
        if padded_data[-padding_len:] != bchr(padding_len) * padding_len:
            raise ValueError("PKCS#7 padding is incorrect.")
        return padded_data[:-padding_len]

def byte_XOR(byte_chunk, byte_iv):
    byte_final = bytes((a ^ b) for (a,b) in zip(byte_chunk, byte_iv))
    return byte_final

def decrypt_file(key, in_filename, mode, out_filename=None, chunksize=AES.block_size):
    if not out_filename:
        out_filename = "yourppm.ppm"
    decryptor = AES.new(key, AES.MODE_ECB) 
    
    if mode == "ECB":
        with open(in_filename, 'rb') as infile:
            with open(out_filename, 'wb') as outfile:
                #將ppm的資訊寫入新的圖片
                chunk = infile.read(15)
                outfile.write(chunk)
                pos = len(chunk)
                
                #提取存取在圖片中原始圖片size
                filesize = struct.unpack('<Q', infile.read(8))[0]
            
                 #將原本圖片中的像素資訊解密後寫入新的檔案
                while pos < filesize:
                    chunk = infile.read(chunksize)
                    pos += len(chunk)
                    plaintext = decryptor.decrypt(chunk)
                    #如果下一次進行解密時已經到圖片尾端，則做unpadding
                    if pos == filesize:
                        plaintext = unpad(plaintext, AES.block_size)
                    #做plaintext的寫入
                    outfile.write(plaintext)
                    
    elif mode == "CBC":
         with open(in_filename, 'rb') as infile:
            with open(out_filename, 'wb') as outfile:               
                #將ppm的資訊寫入新的圖片
                chunk = infile.read(15)
                outfile.write(chunk)
                pos = len(chunk)
                
                filesize = struct.unpack('<Q', infile.read(8))[0]
                #提取存取在原本圖片中的iv值
                iv = infile.read(16)
                
                #將原本圖片中的像素資訊解密後寫入新的檔案
                while pos < filesize:
                    chunk = infile.read(chunksize)
                    pos += len(chunk)
                    plaintext = decryptor.decrypt(chunk)
                    #做XOR
                    plaintext = byte_XOR(plaintext, iv)
                    #如果下一次進行解密時已經到圖片尾端，則做unpadding
                    if pos == filesize:
                        plaintext = unpad(plaintext, AES.block_size)
                    #做plainchunk的寫入
                    outfile.write(plaintext)
                    #更新iv成原本的chunk
                    iv = chunk
    
    elif mode == "XPCBC":
        with open(in_filename, 'rb') as infile:
            with open(out_filename, 'wb') as outfile:
                #將ppm的資訊寫入新的圖片
                chunk = infile.read(15)
                outfile.write(chunk)
                pos = len(chunk)
                
                filesize = struct.unpack('<Q', infile.read(8))[0]
                #提取存取在原本圖片中的iv值
                iv = infile.read(16)
                
                #將原本圖片中的像素資訊解密後寫入新的檔案
                while pos < filesize:
                    chunk = infile.read(chunksize)
                    pos += len(chunk)
                    newIV_element1 = chunk
                    #將chunk做AES
                    chunk = decryptor.decrypt(chunk)
                    newIV_element2 = chunk
                    #做XOR
                    plaintext = byte_XOR(chunk, iv)
                    #如果下一次進行解密時已經到圖片尾端，則做unpadding
                    if pos == filesize:
                        plaintext = unpad(plaintext, AES.block_size)
                    #做plaintext的寫入
                    outfile.write(plaintext)
                    #更新iv成兩個iv的element做XOR的結果
                    iv = byte_XOR(newIV_element1, newIV_element2)
                    
    return out_filename

def Decrypt(picture_file, decrype_mode, key_name):
    #將目前的ppm檔顯示出來
    im = Image.open(picture_file)
    im.show()
    im.close()
    #將目前的ppm檔進行解密
    decrypted_picture = decrypt_file(key_name.encode('utf-8'), picture_file, decrype_mode)
    #將解密完的ppm轉成jpg輸出
    ppmPicture = decrypted_picture
    im = Image.open(ppmPicture)
    im.save("./final.jpg", "JPEG")
    im.show()
    im.close()
    
if __name__=='__main__':
    
    jpg_name = input("請輸入檔案名稱: ") # ./myppm_ECB.ppm、./myppm_CBC.ppm、./myppm_XPCBC.ppm
    
    mode_name = (jpg_name.split(".")[-2]).split("_")[1] #ECB、CBC、XPCBC
    
    key_name = input("請輸入金鑰名稱: ") # keyskeyskeyskeys
    
    Decrypt(jpg_name, mode_name, key_name)


# In[ ]:




