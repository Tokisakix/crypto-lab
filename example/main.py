import time
import argparse

from utils import loadMessage, showInfo

ALL_IMPL = {
    "None": (lambda message: message, lambda message: message),
}

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--type", type=str, default="None")
    args = parser.parse_args()

    MESSAGE_PATH = "message.txt"
    MESSAGE = loadMessage(MESSAGE_PATH)

    startTime = time.perf_counter()
    encodeAlgorithm, decodeAlgorithm = ALL_IMPL[args.type]
    ENCODE = encodeAlgorithm(MESSAGE)
    DECODE = decodeAlgorithm(ENCODE)
    endTime = time.perf_counter()
    totalTime = endTime - startTime

    showInfo(args.type, totalTime, MESSAGE, ENCODE, DECODE)
    exit(0)