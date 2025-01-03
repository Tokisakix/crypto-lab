import numpy as np

def prepare_text(text, size):
    padding_length = (size - len(text) % size) % size
    padded_text = text + '@' * padding_length
    return padded_text

def text_to_num_matrix(text, size):
    num_list = []
    for char in text:
        if char.islower():
            num_list.append(ord(char) - ord('a'))
        elif char == ' ':
            num_list.append(26)
        elif char == '@':
            num_list.append(27)
        else:
            raise ValueError(f"Unsupported character: {char}")
    return np.array(num_list).reshape(-1, size).T

def num_matrix_to_text(matrix):
    num_list = matrix.T.flatten()
    text = []
    for num in num_list:
        if 0 <= num <= 25:
            text.append(chr(num + ord('a')))
        elif num == 26:
            text.append(' ')
        elif num == 27:
            text.append('@')
        else:
            raise ValueError(f"Invalid number: {num}")
    return ''.join(text)

def encode_hill(message, key):
    size = key.shape[0]
    padded_message = prepare_text(message, size)
    message_matrix = text_to_num_matrix(padded_message, size)
    encrypted_matrix = np.dot(key, message_matrix) % 28
    return num_matrix_to_text(encrypted_matrix)

def decode_hill(ciphertext, key):
    size = key.shape[0]
    det = int(np.round(np.linalg.det(key)))
    det_inv = pow(det, -1, 28)
    key_inv = (
        det_inv * np.round(det * np.linalg.inv(key)).astype(int) % 28
    )
    ciphertext_matrix = text_to_num_matrix(ciphertext, size)
    decrypted_matrix = np.dot(key_inv, ciphertext_matrix) % 28
    return num_matrix_to_text(decrypted_matrix).rstrip('@')