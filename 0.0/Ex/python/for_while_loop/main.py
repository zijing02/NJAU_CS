################################################################
# x = float(input('a number: '))
# if x**2 - 2 <=0.01:
#     if x**2 - 2 <= 0.001:
#         print('good')
#     else:
#         print('not yet')
# elif x**2 - 2 >= 0.1:
#     print('far not')

################################################################

# iteration
#
# x = 3
# ans = 0
# itersLeft = x
# while(itersLeft !=0):
#     ans = ans + x
#     itersLeft = itersLeft - 1
# print(str(x) + '*' + str(x) + '=' + str(ans))

# cube root
#
# x = int(input('Enter an int: '))
# ans = 0
# while ans * ans * ans < x:
#     ans = ans + 1
# if ans * ans * ans != x:
#     print(str(x) + ' is not a perfect cube')
# else:
#     print(ans)
#
########################################################

# x = int(input('Enter an int: '))
# for ans in range(0 , abs(x)+1):          #generate
#     if ans * ans * ans == abs(x):
#         break               #cheak
# if ans * ans * ans != abs(x):
#     print(str(x) + ' is not a perfect cube')
# elif x < 0:
#     ans = -ans
# print('Cube root of ' + str(x) + ' is ' + str(ans))

# 近似

# x = int(input('number: '))
# epsilon = 0.01
# step = epsilon**2
# numGuesses = 0
# ans = 0.0
# while (abs(ans**2-x)) >= epsilon and ans <= x:
#     ans += step
#     numGuesses += 1
# print('numGuesses = ' + str(numGuesses))
# if abs(ans**2-x) >= epsilon:
#     print('Failed on square root of ' + str(x))
# else:
#     print(str(ans) + ' is close to the square root of ' + str(x))

# binary search

# x = int(input('number: '))
# head = 0
# end = x
# mid = 0
# epsilon = 0.01
# numGuesses = 0
# while (abs(mid**2-x)) >= epsilon and mid <= x:
#     mid = (head + end) / 2.0
#     print('head: ' + str(head) + ' mid: ' + str(mid) + ' end: ' + str(end))
#     if mid**2 > x:
#         end = mid
#     else:
#         head = mid
# print('numGuesses = ' + str(numGuesses))
# if abs(mid**2-x) >= epsilon:
#     print('Failed on square root of ' + str(x))
# else:
#     print(str(mid) + ' is close to the square root of ' + str(x))

######################################################################

# x = float(input('a number: '))
# p = int(input('an int: '))
# result = 1
# for turn in range(p):
#     print('iteration: ' + ' ' + str(turn) +\
#           ' current result: ' + str(result))
#     result = result*x

#####################################################################

# def iterativePower(x, p):
#     result = 1
#     for turn in range(p):
#         print('iteration:'+str(turn)+' current result:'+str(result))
#         result = result*x
#     print('result is ' + str(result))
#     return result
# iterativePower(2, 4)
######################################################################
#
# def Fibonacci_while(n):
#     if n <= 2:
#         return 1
#     else:
#         pre = 1
#         cur = 1
#         next = pre + cur
#         currentIndex = 2
#         while n > currentIndex:
#             pre = cur
#             cur = next
#             next = pre + cur
#             currentIndex += 1
#     print('The Nth Fibonacci num is ' + str(cur))
#
# Fibonacci_while(9)
#
# def Fibonacci_for(n):
#     if n <= 2:
#         return 1
#     else:
#         pre = 1
#         cur = 1
#         for turn in range(n-2):
#             next = pre + cur
#             pre = cur
#             cur = next
#         print('is ' + str(cur))
#
# Fibonacci_for(4)
#
############################################################################################################
#
# def printA(n):
#     for turn in range(n):
#         print('a'*n)
#
# printA(5)
#############################################################################################################
# def lower_triangle(n):
#     for turn in range(n):
#         print('a'*(turn + 1))
#
# lower_triangle(6)

# def upper_triangle(n):
#     for turn in range(n):
#         print(' '*turn + 'a'*(n - turn))
#
# upper_triangle(6)
#############################################################################################################
# def lower_triangle(n):
#     index = 0
#     while index <= n:
#         print('a'*index)
#         index += 1
#
# lower_triangle(20)

# def upper_triangle(n):
#     index = n
#     while 1 <= index:
#         print(' '*(n-index) + 'a'*index)
#         index -= 1
#
# upper_triangle(40)
# def triangle(x, y, z):
#     if x + y > z and x + z > y and z + y > x:
#         print('triangle')
#     else:
#         print('not')
#
# triangle(3, 4, 5)

# import math
#
# def f(a, b, c):
#     epsilon = 0.0000000000000001
#     if abs(a) < epsilon and abs(b) < epsilon:
#         print('no')
#     elif a == 0 and b != 0:
#         print('one is ' + str(-c/b))
#     elif b ** 2 < 4 * a * c:
#         print('no')
#     elif abs(b**2 - 4*a*c) < epsilon:
#         root = -b/(2*a)
#         print('one is ' + str(root))
#     else:
#         root1 = (-b + math.sqrt(b**2 - 4*a*c))/(2*a)
#         root2 = (-b - math.sqrt(b**2 - 4*a*c))/(2*a)
#         print('one is ' + str(root1) + ' another is ' + str(root2))
#
# f(5, -7, -6)
########################################################################
#


# def remove(l1, l2):
#    for e1 in l1:
#        if e1 in l2:
#           l1.remove(e1)
#
#
# remove(list1, list2)
# print(list1)

# l = [1, -2, 3, 4]
#
#
# def square(x):
#     return x*x
#
# ans = square(3)
#
# print(square(3))


# def applyToEach(l, f):    # definition
#   for i in range(len(l)):
#       l[i] = f(l[i])
#
# applyToEach(l, square)       # call
#
# print(l)

# def wash_bowls(name):
#     return name + ' is washing bowls'
#
#
# def brush_teeth(name):
#     return name + ' is brushing teeth'
#
# def daily_life(ListOperation, name):
#     for m in ListOperation:
#         print(m(name))
#
#
# daily_life([brush_teeth, wash_bowls], 'zijing')


# def get_digits(number):
#     all_number_list = []
#     while 1 <= number:
#         all_number_list.append(number % 10)
#         number = int(number/10)
#
#     all_number_list.reverse()
#     return all_number_list
#
# print(get_digits(12345))
#
#
# def isNumberQualified(number):
#     sum = 0
#     for num in get_digits(number):
#         sum = num**3 + sum
#     return sum == number
#
#
# ret = isNumberQualified(1634)
# print(ret)
#
#
# def getAllDaffodils():
#     start = 100
#     while start <= 100000:
#         if isNumberQualified(start):
#             print(start)
#         start += 1
#
#
# getAllDaffodils()

# def linear_search(num, x):
#     for e in num:
#         if e == x:
#             return True
#
#
# print(linear_search([1, 2, 3, 4, 5], 6))


# def smallest_while(numbers):
#     index = 0
#     numbers_length = len(numbers)
#     minimum = numbers[0]
#     while index < numbers_length:
#         a = numbers[index]
#         if minimum >= a:
#             minimum = a
#         index += 1
#     return minimum
#
#
# print(smallest_while([8, 10, 20, 3, 1, 5, 0, -2]))

#
# def smallest_for1(numbers):
#     minimum = numbers[0]
#     numbers_length = len(numbers)
#     for index in range(0, numbers_length + 1):
#         a = numbers[index]
#         if minimum >= a:
#             minimum = a
#     return minimum
#
#
# print(smallest_for1([8, 10, 20, 3, 1, 5, 0, -2]))
#
# smallest = [8, 10, 20, 3, 5]
# print(len(smallest))
# print(min(1, 2, 6, 9))


# def smallest_for(numbers):
#     minimum = numbers[0]
#     for number in numbers:
#         if minimum >= number:
#             minimum = number
#     return minimum
#
#
# print(smallest_for([8, 10, 20, 3, 1, 5, 0, -2]))


# def my_sort(numbers):
#
#
#
# sorted_answer = my_sort([10, 8, 20, 1, 3, 5])
# numbers = [1, 3, -1, 4, -8]
# numbers.sort()
# print(numbers)

# numbers = ['a', 'b', 'd', 'r']


# def bubble_sort(numbers):
#     new_list = list(numbers)
#     numbers_length = len(numbers)
#     for outer_index in range(0, numbers_length):
#         print('1')
#         for inner_index in range(numbers_length - outer_index - 1):
#             if new_list[inner_index] > new_list[inner_index + 1]:
#                 (new_list[inner_index], new_list[inner_index + 1]) =
#                     (new_list[inner_index + 1], new_list[inner_index])
#                 print(str(outer_index) + str(inner_index) + str(new_list))
#
#     return new_list
#
#
# print(bubble_sort([5, 1, 4, 2, 8]))

#
# def intersect(l1, l2):
#     tmp = []
#     for e1 in l1:
#         for e2 in l2:
#             if e1 == e2:
#                 tmp.append(e1)
# 一下感觉多余
#     res = []
#     for e in tmp:
#         if not (e in res):
#             res.append(e)
#     return res
#
#
# print(intersect([1, 3, 5, 7, 0], [2, 4, 6, 8, 0]))


# def plus_function(a):
#     if 0 < a < 10:
#         ans = a*4 + a*10*3 + a*100*2 + a*1000
#         return ans
#     else:
#         return 'no'
#
#
# print(plus_function(8))


# matrix = [[1, 2, 3, 4, 5],
#           [3, 5, 7, 8, 9],
#           [4, 2, 6, 575, 232],
#           [1, 2, 5, 6, 7]]
# print(matrix[2][1])


# def get_row_sum(matrix):
#     new_list = []
#     for li in matrix:
#         sum = 0
#         for number in li:
#             sum = sum + number
#         new_list.append(sum)
#     return new_list
#
#
# print(get_row_sum([[1, 2, 3, 4, 5],
#                    [3, 5, 7, 8, 9],
#                    [4, 2, 6, 575, 232],
#                    [1, 2, 5, 6, 7]]))


#
# def get_column_sum(matrix):
#     new_list = []
#     for li in matrix:
#         index1 = 0
#         for index1 in range(0, len(li)):
#             index2 = 0
#             sum = 0
#             for index2 in range(0, len(matrix)):
#                 sum = sum + matrix[index2][index1]
#                 # print(new_list)
#             new_list.append(sum)
#     return new_list
#
#
# print(get_column_sum([[1, 2, 3, 4, 5],
#                       [3, 5, 7, 8, 9],
#                       [4, 2, 6, 575, 232],
#                       [1, 2, 5, 6, 7]]))

# def get_column_sum(matrix):
#     new_list = []
#     for index1 in range(0, len(matrix[0])):
#         sum = 0
#         for index2 in range(0, len(matrix)):
#             sum = sum + matrix[index2][index1]
#             # print(new_list)
#         new_list.append(sum)
#     return new_list
#
#
# print(get_column_sum([[1, 2, 3, 4, 5],
#                       [3, 5, 7, 8, 9],
#                       [4, 2, 6, 575, 232],
#                       [1, 2, 5, 6, 7]]))

# def print_multi_table():
#     for i in range(1, 10):
#         column = ''
#         for j in range(1, i+1):
#             ans = i*j
#             column = column + str(i) + '*' + str(j) + '=' + str(ans) + ' '
#
#         print(column)
#
#
# print_multi_table()

# def print_multi_table():
#     column = ''
#     for i in range(1, 10):
#         for j in range(1, i+1):
#             ans = i*j
#             column = column + str(i) + '*' + str(j) + '=' + str(ans) + ' '
#         column = column + '\n'
#     return column
#
# ans = print_multi_table()
# print(ans)


# def get_digits(number):
#     all_number_list = []
#     while 1 <= number:
#         all_number_list.append(number % 10)
#         number = int(number/10)
#
#     return all_number_list
#
#
# def reverse_number(number):
#     li = get_digits(number)
#     final = 0
#     for index1, num in enumerate(li):
#         a = num*(10**(len(li) - (index1 + 1)))
#         final = a + final
#     return final
#
#
# ans = reverse_number(12345)
# print(ans)


# def reverse_number(number):
#     ret = 0
#     while number > 0:
#         ret = ret * 10 + number % 10
#         number = int(number / 10)
#
#     return ret
#
#
# ans = reverse_number(0)
# print(ans)


# def get_available_route_count(matrix):
#     c = matrix
#     c[0][0] = matrix[0][0]
#     for index2 in range(1, len(c[0])):
#         if c[0][index2]*c[0][index2 - 1] == 1:
#             c[0][index2] = 1
#         else:
#             c[0][index2] = 0
#
#     for index1 in range(1, len(matrix)):
#         if c[index1][0]*c[index1 - 1][0] == 1:
#             c[index1][0] = 1
#         else:
#             c[index1][0] = 0
#
#     for index1 in range(1, len(c)):
#         for index2 in range(1, len(c[0])):
#             if c[index1][index2] != 0:
#                 c[index1][index2] = c[index1][index2 - 1] + c[index1 - 1][index2]
#
#     return c[-1][-1]
#
#
# count = get_available_route_count([[1, 0, 1, 1, 1, 1, 1, 1],
#                                    [1, 0, 1, 1, 1, 1, 1, 1],
#                                    [1, 0, 1, 1, 1, 1, 1, 1],
#                                    [1, 0, 1, 1, 1, 1, 1, 1],
#                                    [1, 1, 1, 1, 1, 1, 1, 1],
#                                    [0, 1, 1, 1, 1, 1, 1, 1]])
# print(count)


# def get_available_route_count(matrix):
#     c = matrix
#     c[0][0] = matrix[0][0]
#     for index2 in range(1, len(c[0])):
#         c[0][index2] = min(c[0][index2], c[0][index2 - 1])
#     for index1 in range(1, len(matrix)):
#         c[index1][0] = min(c[index1][0], c[index1 - 1][0])
#     for index1 in range(1, len(c)):
#         for index2 in range(1, len(c[0])):
#             if c[index1][index2] != 0:
#                 c[index1][index2] = c[index1][index2 - 1] + c[index1 - 1][index2]
#
#     return c[-1][-1]
#
#
# count = get_available_route_count([[1, 0, 1, 1, 1, 1, 1, 1],
#                                    [1, 0, 1, 1, 1, 1, 1, 1],
#                                    [1, 0, 1, 1, 1, 1, 1, 1],
#                                    [1, 0, 1, 1, 1, 1, 1, 1],
#                                    [1, 0, 1, 1, 1, 1, 1, 1],
#                                    [0, 1, 1, 1, 1, 1, 1, 1]])
# print(count)


# def find_duplicates(li):
#     new_list = []
#     for index1 in range(0, len(li) - 1):
#         for index2 in range(0, len(li) - 1):
#             if index2 != index1 and li[index2] == li[index1]:
#                 ans = li[index1]
#                 print(ans)
#                 new_list.append(ans)
#     return new_list
#
#
# print(find_duplicates([-1, 1, 1, 8, -1, 3]))


# def find_duplicates(li):
#     item_count = {}
#     new_list = []
#     for number in li:
#         if number in item_count:
#             item_count[number] = item_count[number] + 1
#         else:
#             item_count[number] = 1
#     print('item_count = ' + str(item_count))
#     for key, value in item_count.items():
#         if value >= 2:
#             new_list.append(key)
#     return new_list
#
#
# print(find_duplicates([-1, 1, 1, 8, -1, 3]))


# def point_in_the_rectangle(rectangle, point):
#     # rectangle => [[,],[,],[,],[,]]
#     x_min = rectangle[0][0]
#     x_max = rectangle[0][0]
#     y_min = rectangle[0][1]
#     y_max = rectangle[0][1]
#     for rect_point in rectangle:
#         # rect_point => [,]
#         x_min = min(x_min, rect_point[0])
#         x_max = max(x_max, rect_point[0])
#         y_min = min(y_min, rect_point[1])
#         y_max = max(y_max, rect_point[1])
#     print(str(x_min) + ' ' + str(x_max) + ' ' + str(y_min) + ' ' + str(y_max))
#     return x_min < point[0] < x_max and y_min < point[1] < y_max
#
#
# print(point_in_the_rectangle([[2, 3], [2, 5], [4, 3], [4, 5]], [3, 4]))

#
# rectangle = [[2, 3], [2, 5], [4, 3], [4, 5], [3, 4]]
# print(max(rectangle[2][0], rectangle[2][1]))


