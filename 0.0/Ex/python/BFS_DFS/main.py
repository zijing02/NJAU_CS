# graph = {"A": ["B", "C"],  # 连接点
#          "B": ["A", "C", "D"],
#          "C": ["A", "B", "D", "E"],
#          "D": ["B", "C", "E", "F"],
#          "E": ["C", "D"],
#          "F": ["D"]}


# def DFS(graph, s):
#     stack = [s]
#     seen = set()
#     seen.add(s)
#     while len(stack) > 0:
#         vertex = stack.pop()  # 从栈中弹出最后一个数据
#         nodes = graph[vertex]
#         for w in nodes:
#             if w not in seen:
#                 stack.append(w)
#                 seen.add(w)
#         print(vertex)
# DFS(graph, "E")
# 第一个错误自己
# def BFS(graph, p):
#     li = [p]
#     seen = set()
#     seen.add(p)
#     while len(li) > 0:
#         vertex = li[len(li) - 1]
#         nodes = graph[vertex]
#         for n in nodes:
#             if n not in seen:
#                 li.append(n)
#                 seen.add(n)
#         print(vertex)
# # BFS(graph, "A")
# def BFS(graph, s):
#     queue = [s]  # queue.pop()弹出指定数据
#     seen = set()  # 集合
#     seen.add(s)
#     parent = {s: None}
#     while len(queue) > 0:
#         vertex = queue.pop(0)
#         nodes = graph[vertex]
#         for w in nodes:
#             if w not in seen:
#                 queue.append(w)
#                 seen.add(w)
#                 parent[w] = vertex
#         print(vertex)
#     return parent
# parent = BFS(graph, "E")
# print(parent)
# # for key in parent:
# #     print(key, parent[key])
# v = 'D'
# while v is not None:
#     print(v)
#     v = parent[v]


print(5)
