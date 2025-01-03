import struct
import math

# 初始化常量
# 64 个常数
T = [
    int(2 ** 32 * abs(math.sin(i + 1)))
    for i in range(64)
]

# 每轮的循环左移位数
S = [
    [7, 12, 17, 22],
    [5,  9, 14, 20],
    [4, 11, 16, 23],
    [6, 10, 15, 21],
]

def left_rotate(x, n):
    return ((x << n) | (x >> (32 - n))) & 0xFFFFFFFF

def pad_message(message):
    original_length = len(message) * 8
    message += b'\x80'
    while (len(message) * 8 + 64) % 512 != 0:
        message += b'\x00'
    message += struct.pack('<Q', original_length)
    return message

def encode_md5(message:str) -> str:
    A = 0x67452301
    B = 0xEFCDAB89
    C = 0x98BADCFE
    D = 0x10325476
    
    message = pad_message(bytes(message, 'ascii'))

    for chunk_start in range(0, len(message), 64):
        chunk = message[chunk_start:chunk_start + 64]

        M = list(struct.unpack('<16I', chunk))

        AA, BB, CC, DD = A, B, C, D

        for i in range(64):
            if i < 16:
                F = (B & C) | ((~B) & D)
                g = i
            elif i < 32:
                F = (D & B) | ((~D) & C)
                g = (5 * i + 1) % 16
            elif i < 48:
                F = B ^ C ^ D
                g = (3 * i + 5) % 16
            else:
                F = C ^ (B | (~D))
                g = (7 * i) % 16
            F = (F + A + T[i] + M[g]) & 0xFFFFFFFF
            A = D
            D = C
            C = B
            B = (B + left_rotate(F, S[i // 16][i % 4])) & 0xFFFFFFFF

        A = (A + AA) & 0xFFFFFFFF
        B = (B + BB) & 0xFFFFFFFF
        C = (C + CC) & 0xFFFFFFFF
        D = (D + DD) & 0xFFFFFFFF

    return f"{A:08x}{B:08x}{C:08x}{D:08x}"