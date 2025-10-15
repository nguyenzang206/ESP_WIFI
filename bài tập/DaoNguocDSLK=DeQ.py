class Node:
    def __init__(self, data):
        self.data = data
        self.next = None
    def reverse_linked_list(current, prev = None):
        #Phần neo - đến cuối danh sách
        if current is None:
            return prev
        #lưu node kế tiếp
        next_node = current.next
        #đảo chiều liên kết
        current.next = prev
        #phần đệ quy
        return Node.reverse_linked_list(next_node, current)
    
head = Node(1)
head.next = Node(2)
head.next.next = Node(3)
head.next.next.next = Node(4)

# --- Gọi hàm đệ quy để đảo ngược ---
head = Node.reverse_linked_list(head)
current = head
while current:
    print(current.data, end=" -> ")
    current = current.next
print("None")