def encode_vigenere(message:str, key:str) -> str:
    encrypted_message = []
    key_length = len(key)
    for i, char in enumerate(message):
        if char.isalpha():
            shift = ord(key[i % key_length].upper()) - ord('A')
            if char.isupper():
                encrypted_char = chr((ord(char) - ord('A') + shift) % 26 + ord('A'))
            else:
                encrypted_char = chr((ord(char) - ord('a') + shift) % 26 + ord('a'))
            encrypted_message.append(encrypted_char)
        else:
            encrypted_message.append(char)
    return ''.join(encrypted_message)


def decode_vigenere(ciphertext:str, key:str) -> str:
    decrypted_message = []
    key_length = len(key)
    for i, char in enumerate(ciphertext):
        if char.isalpha():
            shift = ord(key[i % key_length].upper()) - ord('A')
            if char.isupper():
                decrypted_char = chr((ord(char) - ord('A') - shift) % 26 + ord('A'))
            else:
                decrypted_char = chr((ord(char) - ord('a') - shift) % 26 + ord('a'))
            decrypted_message.append(decrypted_char)
        else:
            decrypted_message.append(char)
    return ''.join(decrypted_message)