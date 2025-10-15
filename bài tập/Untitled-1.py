def factorial(n):
    #Phần neo
    if n == 0 or n == 1:
        return 1
    #phần đệ quy
    else:
        return n * factorial(n-1)

print n = 4