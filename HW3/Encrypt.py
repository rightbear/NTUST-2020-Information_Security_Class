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
    def pad(data_to_pad, block_size):
        padding_len = block_size-len(data_to_pad) % block_size
        padding = bchr(padding_len) * padding_len
        return data_to_pad + padding

def byte_XOR(byte_chunk, byte_iv):
    byte_final = bytes((a ^ b) for (a,b) in zip(byte_chunk, byte_iv))
    return byte_final

def encrypt_file(key, in_filename, mode, out_filename=None, chunksize=AES.block_size):
    if not out_filename:
        #將目前的ppm檔名轉成: "舊檔名 + mode名 + .ppm"
        out_filename = in_filename[:-4] + "_" + mode + '.ppm'
    encryptor = AES.new(key, AES.MODE_ECB)
    filesize = os.path.getsize(in_filename)
    #IV預設為"0000000000000000"
    #iv = os.urandom(16).encode('utf-8')
    iv = ("0" * chunksize).encode('utf-8')
    
    if mode == "ECB":
        with open(in_filename, 'rb') as infile:
            with open(out_filename, 'wb') as outfile:
                #將ppm的資訊(15 bytes)寫入新的圖片
                chunk = infile.read(15)
                outfile.write(chunk)
                pos = len(chunk)
                
                #將原本圖片的size寫入新圖片
                outfile.write(struct.pack('<Q', filesize))
                    
                 #將原本圖片中的像素資訊加密後寫入新的檔案
                while pos < filesize:
                    chunk = infile.read(chunksize)
                    pos += len(chunk)
                    #如果下一次進行加密時已經到圖片尾端且chunk不夠長，則做padding
                    if pos == filesize:
                        chunk = pad(chunk, AES.block_size)
                    #將chunk做AES
                    ciphertext = encryptor.encrypt(chunk)
                    #做cipherchunk的寫入
                    outfile.write(ciphertext)
                    
    elif mode == "CBC":
         with open(in_filename, 'rb') as infile:
            with open(out_filename, 'wb') as outfile:
                #將ppm的資訊寫入新的圖片
                chunk = infile.read(15)
                outfile.write(chunk)
                pos = len(chunk)
                
                outfile.write(struct.pack('<Q', filesize))
                 #隨機產生16位元的IV
                iv = os.urandom(16)
                outfile.write(iv)
                
                #將原本圖片中的像素資訊加密後寫入新的檔案
                while pos < filesize:
                    chunk = infile.read(chunksize)
                    pos += len(chunk)
                    #如果下一次進行加密時已經到圖片尾端且chunk不夠長，則做padding
                    if pos == filesize:
                        chunk = pad(chunk, AES.block_size)
                    #做XOR
                    chunk = byte_XOR(chunk, iv)
                    #將chunk做AES
                    ciphertext = encryptor.encrypt(chunk)
                    #做cipherchunk的寫入
                    outfile.write(ciphertext)
                    #更新iv成原本的ciphertext
                    iv = ciphertext
    
    elif mode == "XPCBC":
        with open(in_filename, 'rb') as infile:
            with open(out_filename, 'wb') as outfile:
                #將ppm的資訊寫入新的圖片
                chunk = infile.read(15)
                outfile.write(chunk)
                pos = len(chunk)
                
                outfile.write(struct.pack('<Q', filesize))
                 #隨機產生16位元的IV
                iv = os.urandom(16)
                outfile.write(iv)
                
                #將原本圖片中的像素資訊加密後寫入新的檔案
                while pos < filesize:
                    chunk = infile.read(chunksize)
                    pos += len(chunk)
                    #如果下一次進行加密時已經到圖片尾端且chunk不夠長，則做padding
                    if pos == filesize:
                        chunk = pad(chunk, AES.block_size)
                    #做XOR
                    chunk = byte_XOR(chunk, iv)
                    newIV_element1 = chunk
                    #將chunk做AES
                    ciphertext = encryptor.encrypt(chunk)
                    newIV_element2 = ciphertext
                    #做cipherchunk的寫入
                    outfile.write(ciphertext)
                    #更新iv成兩個iv的element做XOR的結果
                    iv = byte_XOR(newIV_element1, newIV_element2)
                    
    return out_filename

def Encrypt(picture_file, encrype_mode, key_name):
    #將目前的圖片檔轉成.ppm的格式
    ppmPicture = "./myppm.ppm"
    im = Image.open(picture_file)
    im.show()
    im.save(ppmPicture)   
    #將目前的ppm檔進行加密
    encrypted_picture = encrypt_file(key_name.encode('utf-8'), ppmPicture, encrype_mode)
    im.close()   
    #將加密完的ppm顯示出來
    im = Image.open(encrypted_picture)
    im.show()
    im.close()
    
if __name__=='__main__':
    
    jpg_name = input("請輸入檔案名稱: ") # ./start.jpg
    
    mode_name = input("請輸入加密方式: ") # ECB、CBC、XPCBC
    
    key_name = input("請輸入金鑰名稱: ") # keyskeyskeyskeys
    
    Encrypt(jpg_name, mode_name, key_name)


# In[ ]:




