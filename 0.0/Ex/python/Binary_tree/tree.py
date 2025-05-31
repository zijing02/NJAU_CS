class BinaryTree(object):

    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None

    def preorder_print(self):
        li = []
        if self is None:
            return None
        if self is not None:
            li.append(self.value)
        if self.left is not None:
            li = li + self.left.preorder_print()
        if self.right is not None:
            li = li + self.right.preorder_print()
        return li

    def is_same_with(self, tree2):
        l = False
        r = False
        if self is None and tree2 is None:
            return True
        if self is not None and tree2 is not None:
            # check if the left tree is the same and save the value to l
            if self.left is not None:
                l = self.left.is_same_with(tree2.left)
            else:
                l = tree2.left is None

            if self.right is not None:
                r = self.right.is_same_with(tree2.right)
            else:
                r = tree2.right is None

            return l and r and self.value == tree2.value
        else:
            return False

    def is_symmetric(self, tree):
        l = False
        r = False
        if self is None:
            return True
        if self is not None:
            if self.left is not None:
                l = self.left.is_symmetric(tree.right)
            else:
                l = tree.right is None
            if self.right is not None:
                r = self.right.is_symmetric(tree.left)
            else:
                r = tree.left is None
            return l and r and self.value == tree.value
        else:
            return False

    def get_node_count(self):
        l = 0
        r = 0
        if self is None:
            return 0
        if self.left is not None:
            l = self.left.get_node_count()
        if self.right is not None:
            r = self.right.get_node_count()
        return l + r + 1

    def find(self, value_to_find):
        l = False
        r = False
        if self is None:
            return False
        elif self.value == value_to_find:
            return True
        if self.left is not None:
            l = self.left.find(value_to_find)
        if self.right is not None:
            r = self.right.find(value_to_find)
        return l or r

    def get_height(self):
        L = 0
        R = 0
        if self is None:
            return 0
        if self.left is not None:
            L = self.left.get_height()
        if self.right is not None:
            R = self.right.get_height()
        return max(L, R) + 1

    def count_leaves(self):
        l = 0
        r = 0
        if self is None:
            return 0
        if self.right is None and self.left is None:
            return 1
        if self.left is not None:
            l = self.left.count_leaves()
        if self.right is not None:
            r = self.right.count_leaves()
        return l + r

    def level_order_traversal(self):
        queue = [self]
        li = []
        while len(queue) > 0:
            vertex = queue.pop(0)
            if vertex.left is not None:
                queue.append(vertex.left)
            if vertex.right is not None:
                queue.append(vertex.right)
            li.append(vertex.value)
        return li

    def level_order_traversal_zig_zag(self):
        queue = [self]
        li = [self]
        tempList = []
        leftToRight = False
        while len(queue) > 0 or len(tempList) > 0:
            if len(queue) > 0:
                top_node = queue.pop(0)
                if top_node.left is not None:
                    tempList.append(top_node.left)
                if top_node.right is not None:
                    tempList.append(top_node.right)
            else:
                if leftToRight:
                    li = li + tempList
                else:
                    tempList.reverse()
                    li = li + tempList
                leftToRight = not leftToRight
                tempList.reverse()
                queue = tempList
                tempList = []

        return list(map(lambda x: x.value, li))


tree = BinaryTree(1)
tree.left = BinaryTree(4)
tree.right = BinaryTree(5)
tree.left.left = BinaryTree(7)
tree.left.right = BinaryTree(8)
tree.right.left = BinaryTree(9)
tree.right.right = BinaryTree(10)
tree.left.left.left = BinaryTree(6)
tree.left.right.right = BinaryTree(12)


# tree2 = BinaryTree(1)
# tree2.left = BinaryTree(4)
# tree2.right = BinaryTree(5)
# tree2.left.left = BinaryTree(7)
# tree2.left.right = BinaryTree(8)
# tree2.right.left = BinaryTree(9)
# tree2.right.right = BinaryTree(10)
# tree2.left.left.left = BinaryTree(6)
# tree2.left.right.right = BinaryTree(12)
print(tree.level_order_traversal())
print(tree.level_order_traversal_zig_zag())

# print(tree.is_same_with(tree2))
# print(tree.is_symmetric(tree))
# T = tree.preorder_print()
# print(T)
# print(tree.find(40))
# print(tree.get_height())
# print(tree.get_node_count())
# print(tree.count_leaves())

#
# def is_same(tree1, tree2):
#     if tree1 is None and tree2 is None:
#         return True
#     if tree1 is not None and tree2 is not None:
#         return tree1.value == tree2.value and is_same(tree1.left, tree2.left) and is_same(tree1.right, tree2.right)
#     else:
#         return False

# def is_symmetric(l, r):
#     if l is None and r is None:
#         return True
#     if l is not None and r is not None:
#         return l.value == r.value and is_symmetric(l.right, r.left) and is_symmetric(l.left, r.right)
#     else:
#         return False





