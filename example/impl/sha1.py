import struct

# 初始化常量
K = [  # 每轮的常数
    0x5A827999,  # 0 <= t <= 19
    0x6ED9EBA1,  # 20 <= t <= 39
    0x8F1BBCDC,  # 40 <= t <= 59
    0xCA62C1D6   # 60 <= t <= 79
]

def left_rotate(n, b):
    return ((n << b) | (n >> (32 - b))) & 0xFFFFFFFF

def pad_message(message):
    original_length = len(message) * 8
    message += b'\x80'
    while (len(message) * 8 + 64) % 512 != 0:
        message += b'\x00'
    message += struct.pack('>Q', original_length)
    return message

def encode_sha1(message:str) -> str:
    H = [
        0x67452301,
        0xEFCDAB89,
        0x98BADCFE,
        0x10325476,
        0xC3D2E1F0
    ]
    
    message = pad_message(bytes(message, 'ascii'))
    
    for chunk_start in range(0, len(message), 64):
        chunk = message[chunk_start:chunk_start + 64]
        
        W = list(struct.unpack('>16L', chunk))
        
        for t in range(16, 80):
            W.append(left_rotate(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1))
            
        A, B, C, D, E = H
        
        for t in range(80):
            if t < 20:
                F = (B & C) | ((~B) & D)
                Kt = K[0]
            elif t < 40:
                F = B ^ C ^ D
                Kt = K[1]
            elif t < 60:
                F = (B & C) | (B & D) | (C & D)
                Kt = K[2]
            else:
                F = B ^ C ^ D
                Kt = K[3]
            T = (left_rotate(A, 5) + F + E + Kt + W[t]) & 0xFFFFFFFF
            E = D
            D = C
            C = left_rotate(B, 30)
            B = A
            A = T
            
        H[0] = (H[0] + A) & 0xFFFFFFFF
        H[1] = (H[1] + B) & 0xFFFFFFFF
        H[2] = (H[2] + C) & 0xFFFFFFFF
        H[3] = (H[3] + D) & 0xFFFFFFFF
        H[4] = (H[4] + E) & 0xFFFFFFFF
        
    return f"{H[0]:08x}{H[1]:08x}{H[2]:08x}{H[3]:08x}{H[4]:08x}"