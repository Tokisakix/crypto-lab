import random

def is_prime(n):
    if n < 2:
        return False
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            return False
    return True

def find_generator(p):
    if p == 2:
        return 1
    factors = set()
    phi = p - 1
    n = phi
    
    for i in range(2, int(n**0.5) + 1):
        while n % i == 0:
            factors.add(i)
            n //= i
    if n > 1:
        factors.add(n)
        
    for g in range(2, p):
        if all(pow(g, phi // f, p) != 1 for f in factors):
            return g
    return None

def extended_gcd(a, b):
    if a == 0:
        return b, 0, 1
    else:
        g, y, x = extended_gcd(b % a, a)
        return g, x - (b // a) * y, y

def modinv(a, m):
    g, x, y = extended_gcd(a, m)
    if g != 1:
        raise Exception('Modular inverse does not exist')
    else:
        return x % m

def encode_elgamal(message, p, g, y):
    message_bytes = message.encode()
    
    block_size = (p.bit_length() - 1) // 8
    encrypted_blocks = []
    for i in range(0, len(message_bytes), block_size):
        block = message_bytes[i:i + block_size]
        m = int.from_bytes(block, 'big')
        k = random.randint(2, p - 2)
        c1 = pow(g, k, p)
        c2 = (m * pow(y, k, p)) % p
        encrypted_blocks.append((c1, c2))
    return encrypted_blocks

def decode_elgamal(encrypted_blocks, p, x):
    decrypted_bytes = b''
    for c1, c2 in encrypted_blocks:
        s = pow(c1, x, p)
        s_inv = modinv(s, p)
        m = (c2 * s_inv) % p
        block_size = (p.bit_length() - 1) // 8
        decrypted_bytes += m.to_bytes(block_size, 'big')
    return decrypted_bytes.decode().rstrip('\x00')