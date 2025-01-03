# Std lib
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
)

DEFAULT = lambda message: "Not impl yet."

def run(type:str, message:str) -> Tuple[str, str]:
    if type == "vigenere":
        key = "vigenereExampleKey"
        encode = encode_vigenere(message, key)
        decode = decode_vigenere(encode, key)
    if type == "hill":
        key = np.array([[3, 5], [1, 10]])
        encode = encode_hill(message, key)
        decode = decode_hill(encode, key)
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

    # sHOW Log
    showInfo(args.type, totalTime, MESSAGE, ENCODE, DECODE)
    exit(0)