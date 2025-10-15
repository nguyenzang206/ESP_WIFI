def tim_kiem(a, n, x):
    if n == 0:
        return -1
    if a[n-1] == x:
        return n - 1
    return tim_kiem(a, n-1, x)
a = [1, 2, 3, 4, 5, 6]
x = int(input("Nhập số cần tìm"))
kq = tim_kiem(a, len(a), x)
if kq != -1:
    print(f"im thay {x} tai vi tri {kq}")
else:
    print("Không tim thay {x}")
    