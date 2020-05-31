# encoding=utf-8
def add(A, B, a, b, M):
    if A == (0, 0): return B
    if B == (0, 0): return A

    x1, y1 = A
    x2, y2 = B

    if A != B:
        λ = (y2 - y1) * pow((x2 - x1), M-2, M)
    else:
        λ = (x1*x1*3 + a) * pow(2*y1, M-2, M)
    
    x3 = λ * λ - x1 - x2
    y3 = λ * (x1 - x3) - y1
    
    return (x3 % M, y3 % M)

def blasting(base, pub, M):
    X = (0, 0) # 初始值
    for i in range(M):
        if X == pub:
            secret = i
            break
        X = add(X, base, a, b, M)

    return secret if X == pub else -1

def decrypt(a, b, M, base, pub, cipher):
    secret = blasting(base, pub, M)
    print(secret)
    xC1 = (0, 0)
    c1, c2 = cipher[0], cipher[1]
    for i in range(secret):
        xC1 = add(xC1, c1, a, b, M)
    
    plaintext = add(c2, (xC1[0], -xC1[1]), a, b, M)
    return plaintext
    # print("x+y=%d"%(plaintext[0]+plaintext[1]))

if __name__ == "__main__":
    a = 1234577
    b = 3213242
    M = 7654319
    base = (5234568, 2287747)
    pub = (2366653, 1424308)
    cipher = [(5081741, 6744615), (610619, 6218)]
    plaintext = decrypt(a, b, M, base, pub, cipher)
    print("x+y=%d"%(plaintext[0]+plaintext[1]))