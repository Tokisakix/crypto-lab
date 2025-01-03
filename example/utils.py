def loadMessage(messagePath:str) -> str:
    with open(messagePath, "r", encoding="utf-8") as file:
        return file.read()

def showInfo(type:str, time, message:str, encode:str, decode:str) -> None:
    print(f"[+] Type: {type}")
    print(f"[+] Time: {time:.3f}s\n")
    print(f"[+] Raw Message   : {message}\n")
    print(f"[+] Encode Message: {encode}\n")
    print(f"[+] Decode Message: {decode}\n")
    print("[+] Done!")
    return