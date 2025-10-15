def mang(a, start = 0, end = None):
    if end is None:
        end = len(a) - 1
    #Phần neo - điều kiện dừng
    if start >= end:
        return a
    #phần đệ quy - hoán đổi phần tử
    a[start], a[end] = a[end], a[start]
    return mang(a, start + 1, end - 1)

print (mang([1, 2, 3, 4, 5, 6]))