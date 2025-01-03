# Std lib
import random
import time
import argparse
from typing import Tuple

# 3rd lib
import numpy as np

# User Package
from utils import loadMessage, showInfo
from impl import (
    encode_vigenere, decode_vigenere,
    encode_hill, decode_hill,
    encode_des,
    encode_aes,
    encode_md5,
    encode_sha1,
    encode_rsa, decode_rsa, modinv,
    encode_elgamal, decode_elgamal, find_generator,
)

DEFAULT = lambda message: "Not impl yet."

def run(type:str, message:str) -> Tuple[str, str]:
    if type == "vigenere":
        key = "key"
        encode = encode_vigenere(message, key)
        decode = decode_vigenere(encode, key)
    elif type == "hill":
        key = np.array([[3, 5], [1, 10]])
        encode = encode_hill(message, key)
        decode = decode_hill(encode, key)
    elif type == "des":
        key = "DesKey00"
        encode = encode_des(message, key)
        decode = DEFAULT(encode)
    elif type == "aes":
        key = [
            [0x2B, 0x28, 0xAB, 0x09],
            [0x7E, 0xAE, 0xF7, 0xCF],
            [0x15, 0xD2, 0x15, 0x4F],
            [0x16, 0xA6, 0x88, 0x3C]
        ]
        encode = encode_aes(message, key)
        decode = DEFAULT(encode)
    elif type == "md5":
        encode = encode_md5(message)
        decode = DEFAULT(encode)
    elif type == "sha1":
        encode = encode_sha1(message)
        decode = DEFAULT(encode)
    elif type == "rsa":
        p = 61
        q = 53
        n = p * q
        phi = (p - 1) * (q - 1)
        e = 17
        d = modinv(e, phi)
        encode = encode_rsa(message, e, n)
        decode = decode_rsa(encode, d, n)
    elif type == "elgamal":
        p = 100000007
        g = find_generator(p)
        x = random.randint(2, p - 2)
        y = pow(g, x, p)
        encode = encode_elgamal(message, p, g, y)
        decode = decode_elgamal(encode, p, x)
    else:
        encode = DEFAULT(message)
        decode = DEFAULT(encode)
    return (encode, decode)

if __name__ == "__main__":
    # Parse Args
    parser = argparse.ArgumentParser()
    parser.add_argument("--type", type=str, default="none")
    args = parser.parse_args()

    # Load Message
    MESSAGE_PATH = "message.txt"
    MESSAGE = loadMessage(MESSAGE_PATH)

    # Encode & Decode
    startTime = time.perf_counter()
    ENCODE, DECODE = run(args.type, MESSAGE)
    endTime = time.perf_counter()
    totalTime = endTime - startTime

    # Show Log
    showInfo(args.type, totalTime, MESSAGE, ENCODE, DECODE)
    exit(0)