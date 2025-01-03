def loadMessage(messagePath:str) -> str:
    with open(messagePath, "r", encoding="utf-8") as file:
        return file.read()

def showInfo(type:str, time, message:str, encode:str, decode:str) -> None:
    print(f"type: {type}")
    print(f"time: {time:.3f}s\n")
    print(f"raw message   : {message}\n")
    print(f"encode message: {encode}\n")
    print(f"decode message: {decode}\n")
    print("done!")
    return