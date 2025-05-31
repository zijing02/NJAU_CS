# YouTube
# def linear_search(list, target, none=None):
#     for i in range(0, len(list)):
#         if list[i] == target:
#             return i
#         else:
#             return None
#
#
# def verify(index):
#     if index is not None:
#         print('target found at index: ', index)
#     else:
#         print('target not found in list')
#
#
# numbers = [1, 2, 3, 4, 5, 6]
#
#
# result = linear_search(numbers, 12)
# print(result)


# def binary_search(list, target):
#     first = 0
#     last = list[len(list) - 1]
#
#     while first <= last:
#         mid = (first + last)//2
#
#         if list[mid] == target:
#             return mid
#         elif list[mid] < target:
#             first = mid + 1
#         else:
#             last = mid - 1
#
#     return None
#
#
# print(binary_search([1, 2, 3, 4, 5, 6, 7], 4))


# def recursive_binary_search(list, target):
#     if len(list) == 0:
#         return False
#     else:
#         mid = len(list) // 2
#         if list[mid] == target:
#             return True
#         else:
#             if list[mid] < target:
#                 return recursive_binary_search(list[mid + 1:], target)
#             else:
#                 return recursive_binary_search(list[:mid], target)
#
#
# def verify(result):
#     print('target', result)
#
#
# numbers = [1, 2, 3, 4, 5, 6, 7, 8]
# result = recursive_binary_search(numbers, 12)
# verify(result)




# 归并排序
# 类似二分，分解问题，分而治之
# 复杂 O(log)
# def merge_sort(list):
#     # sorts a list in ascending order
#     if len(list) <= 1:
#         return list
#
#     left_half, right_half = split(list)
#     left = merge_sort(left_half)
#     right = merge_sort(right_half)
#     return merge(left, right)
#
#
# def split(list):
#     mid = len(list) // 2
#     left = list[:mid]
#     right = list[mid:]
#     return left, right
#
#
# def merge(left, right):
#     l = []
#     i = 0
#     j = 0
#     while i < len(left) and j < len(right):  # 两个长度相同
#         if left[i] < right[j]:
#             l.append(left[i])
#             i += 1
#         else:
#             l.append(right[j])
#             j += 1
#     while i < len(left):
#         l.append(left[i])
#         i += 1
#     while j < len(right):
#         l.append(left[j])
#         j += 1
#     return l
#
#
# def verify_sorted(list):
#     n = len(list)
#     if n <= 1:
#         return True
#     return list[0] < list[1] and verify_sorted(list[1:])
#
#
# alist = [54, 26, 93, 17, 77, 31, 44, 55, 20]
# l = merge_sort(alist)
# print(l)
# print(verify_sorted(alist))

