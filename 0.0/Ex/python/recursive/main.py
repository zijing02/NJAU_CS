# def recursiveMultiply(a, b):
#     if b == 1:
#         return a
#     else:
#         return a + recursiveMultiply(a, b-1)
#
# print(str(recursiveMultiply(36, 5)))

# def recursive_f(n):
#     if 0 < n <= 2:
#         return 1
#     else:
#         return recursive_f(n-1) + recursive_f(n-2)
#
# print(recursive_f(8))
# def printMove(fr, to):
#     print(str(fr) + ' -> ' + str(to))
#
# def hanoiTowers(n, fr, to, sp):
#     if n == 1:
#         printMove(fr, to)
#     else:
#         hanoiTowers(n-1, fr, sp, to)
#         hanoiTowers(1, fr, to, sp)
#         hanoiTowers(n-1, sp, to, fr)
#
# hanoiTowers(4, "FROM", "TO", "SPARE")

# def pali(s):
#     s = s.lower()
#     if len(s) == 1:
#         return True
#     elif len(s) == 2 and s[0] == s[1]:
#         return True
#     elif len(s) == 2 and s[0] != s[1]:
#         return False
#     elif len(s) > 2:
#         if s[0] == s[-1]:
#             return pali(s[1: -1])
#         else:
#             return False
#
#
# print(pali('a aaaaA'))

# def pali(s):
#     s_len = len(s)
#     if s_len == 1:
#         return True
#     elif s_len == 2:
#         return s[0] == s[1]
#     else:
#         return s[0] == s[-1] and pali(s[1:-1])
#
# print(str(pali('232')))

