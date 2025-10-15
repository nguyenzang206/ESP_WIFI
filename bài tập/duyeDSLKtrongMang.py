class Node:
    def __init__(self, data):
        self.data = data
        self.next = None
    def recursive_traverse(node):
        #phần neo
        if node is None:
            return
        print(node.data, end = " -> ")
        #phần đệ quy
        Node.recursive_traverse(node.next)
head = Node(1)
head.next = Node(2)
head.next.next = Node(3)
head.next.next.next = Node(4)

# --- Gọi hàm đệ quy để in ---
Node.recursive_traverse(head)