def tong(n):
    if n == 1:
        return 1
    else:
        return n + tong(n-1)

n = int(input("Nhập một số nguyên không âm : "))
print (tong(n))