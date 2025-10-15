def luy_thua(a, n):
    if n == 0:
        return 1
    else:
        return a * luy_thua(a, n-1)
    
a = int(input("Nhập một số nguyên không âm: "))
n = float(input("Nhập một số bất kì: "))
print(luy_thua(a, n))