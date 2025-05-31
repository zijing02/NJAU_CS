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
###############################################################################################################

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

# def fact1(n):
#     ans = 1
#     while n >= 1:
#         ans = ans*n
#         n -= 1
#         print(ans)
#
# fact1(8)
#
# def fact2(n):
#     if n == 1:
#         return 1
#     else:
#         return n*fact2(n-1)
#
# print(fact2(8))

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

# 元组_tuple
# t1 = (1, 'two')
# print(t1)
# t2 = (t1, 'd')
# print(t2)

# # search[]
# print((t1 + t2)[3])
# print((t1+t2)[2:5])


# lists    use[]

# Techs = ['m', 'n', 'g']
# print(Techs)
# sch = ['e', 'r', 't']
# All = Techs + sch
# print(All)
# Techs.append(sch)
# print(Techs)
# Techs.remove('m')
# print(Techs)

#

# list1 = [1, -2, 3, 4]
# list2 = [1, 2, 5, 6]


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

# dictionary{} 一一对应
# 元素可以是元组，不能是列表
# 提取用关键词

# month = {'jan': 1, 'feb': 2, 'mar': 3, 1: 'jan', 2: 'feb', 3: 'mar'}
# print(month['jan'])
# print(month[2])   #month[0] is wrong
#
# month['apr'] = 4
#
# dic = {}
# dic[1] = 1
# dic[1] = dic[1] + 1
# a = 2 in dic
# print(dic)
# print(a)
#
# collect = []
# for e in month:
#     collect.append(e)
# print(collect)

#
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

# list = [2, 5, 7, 8, 3]
# print(min(list))
#
# rectangle = [[2, 3], [2, 5], [4, 3], [4, 5], [3, 4]]
# print(max(rectangle[2][0], rectangle[2][1]))


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


# print('1' + '3')
# print('2')


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

# dic = {}
# dic[1] = 1
# dic[1] = dic[1] + 1
# a = 2 in dic
# print(dic)
# print(a)


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


# # 定义 --  类
# class Calculator:
#     # 固有属性
#     name = 'Good calculator'
#     price = 18
#
#     def __init__(self, name, price, height, weight, width=35):
#         # init前后双下划线    注意是init不是int
#         # 定义新属性
#         self.name = name
#         self.price = price
#         self.h = height
#         self.wi = width
#         self.we = weight
#         # self.add(1, 2)
#
#     def add(self, x, y):
#         print(self.name)  # 内部调用   加self
#         result = x + y
#         return result
#
#     @staticmethod
#     def minus(x, y):
#         result = x - y
#         return result
#
#     def times(self, x, y):
#         result = x * y
#         return result
#
#     def divide(self, x, y):
#         result = x / y
#         return result
#
#
# calcul = Calculator('bad calculator', 12, 15, 20)  # 上方width已经定义为固有属性，此处不需再定义
# print(str(calcul.name) + ' , ' + str(calcul.price))  # 外部调用   名称加.
# print(calcul.add(10, 11))
# print(Calculator.minus(10, 11))
# print(calcul.times(10, 11))
# print(int(calcul.divide(22, 11)))

# class Account:
#     __interest_rate = 0.1  #类变量利率
#
#     def __init__(self, owner, amount):   #对私有变量调用使用get  改变用set
#         self.owner = owner    #实例变量
#         self.__amount = amount #私有变量，仅可在函数内部用self.使用 外部无法访问
#         #类方法
#
#     @class-method

#     def interest_by(cls,amt):
#         return cls.__interest_rate*amt
#
#     def __get_info(self):
#         return "{0} 金额： {1} 利率 ：{2}。".format(self.owner, self.__amount, Account.__interest_rate)
#
#
# account = Account('tony', 8000.0)
# print(account.owner)          #调用实例变量用对象account
# #print(Account.__interest_rate)  #调用类变量必须用类名Account
#

# class Employee:
#     num_of_emps = 0
#     raise_amount = 1.04
#
#     def __init__(self, first, last, pay):
#         self.first = first
#         self.last = last
#         self.pay = pay
#         self.email = first + '.' + last + '@company.com'
#         Employee.num_of_emps += 1
#
#     def fullname(self):
#         return '{} {}'.format(self.first, self.last)
#
#     def apply_raise(self):
#         self.pay = int(self.pay * self.raise_amount)
#
#
# class Developer(Employee):
#     raise_amt = 1.1
#
#     def __init__(self, first, last, pay, prog_lang):
#         super().__init__(first, last, pay)
#         # Employee.__init__(self, first, last, pay)
#         self.prog_lang = prog_lang
#
#
# class Manager(Employee):
#     def __init__(self, first, last, pay, employees=None):
#         super().__init__(first, last, pay)
#         # Employee.__init__(self, first, last, pay)
#         if employees is None:
#             self.employees = []
#         else:
#             self.employees = employees
#
#     def add_emp(self, emp):
#         if emp not in self.employees:
#             self.employees.append(emp)
#
#     def remove_emp(self, emp):
#         if emp in self.employees:
#             self.employees.remove(emp)
#
#     def print_emps(self):
#         for emp in self.employees:
#             print(('-->', emp.fullname()))
#
#
# dev_1 = Developer('corey', 'schafer', 50000, 'python')
# dev_2 = Developer('test', 'employee', 60000, 'java')
# mar_1 = Manager('sue', 'smith', 90000, [dev_1])
#
#
# print(isinstance(mar_1, Developer))
# print(issubclass(Manager, Employee))

# @classmethod
# def set_raise_amt(cls, amount):
#     cls.raise_amount = amount
# @classmethod
# def from_string(cls, emp_str):
#     first, last, pay = emp_str1.split('-')
#     return cls(first, last, pay)
# @staticmethod
# def is_workday(day):
#     if day.weekday == 5 or day.weekday == 6:
#         return False
#     return True
# print(mar_1.email)
# mar_1.add_emp(dev_2)
# mar_1.remove_emp(dev_1)
# mar_1.print_emps()
# print(help(Developer))
# print(dev_1.pay)
# dev_1.apply_raise()
# print(dev_1.prog_lang)
# e1 = Employee('corey', 'schafer', 50000)
# e2 = Employee('test', 'user', 60000)
# e1.raise_amount = 1.09
# emp_str1 = 'john-doe-70000'
# emp_str2 = 'steve-smith-30000'
# emp_str3 = 'jane-doe-90000'
# first, last, pay = emp_str1.split('-')  # 插入拆分地点
# new_e1 = Employee.from_string(emp_str1)
# Employee.raise_amount = 1.06
# Employee.set_raise_amt(1.06)
# print(e1.__dict__)
# print(Employee.raise_amount)
# print(e1.pay)
# e1.apply_raise()
# print(e1.pay)
# print(e2.raise_amount)
# print(Employee.num_of_emps)
# print(new_e1.email)


# import datetime
# my_date = datetime.date(2016, 7, 11)
# print(Employee.is_workday(my_date))

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


# new_list = [1, 2, 3, 4]
# print(new_list[1])
# if 1 in new_list:
#     print(True)
#
# for n in new_list:
#     if n == 1:
#         print(True)
#
#         break
#
# new_list = [1, 2, 3, 4]
# # list.insert（位置， 数值)
# new_list.insert(4, 2)
# print(new_list)
# # list.append（数值）  加到末尾
# # 加列表
# # list。extend([])


# class Node:
#     data = None
#     next_node = None
#
#     def __init__(self, data):
#         self.data = data
#
#     def __repr__(self):
#         return '<Node data: %s>' % self.data
#
# # N1 = Node(10)
# # print(N1)
#
#
# class LinkedList:
#     def __init__(self):
#         self.head = None
#
#     def is_empty(self):
#         return self.head is None
#
#     def size(self):
#         current = self.head
#         count = 0
#
#         while current:
#             count += 1
#             current = current.next_node
#         return count
#
#     # 添加
#     def add(self, data):
#         # 在开头加数值
#         new_node = Node(data)
#         new_node.next_node = self.head
#         self.head = new_node
#
#     # 搜索
#     def search(self, key):
#         current = self.head
#         while current:
#             if current.data == key:
#                 return current
#             else:
#                 current = current.next_node
#             return None
#
#     # 插入
#     def insert(self, data, index):
#         if index == 0:
#             self.add(data)
#         if index > 0:
#             new = Node(data)
#             position = index
#             current = self.head
#
#             while position > 1:
#                 current = node.next_node
#                 position -= 1
#
#             prev_node = current
#             next_node = current.next_node
#
#             prev_node.next_node = new
#             new.next_node = next
#
#     # 删除
#     def remove(self, key):
#         current = self.head
#         previous = None
#         found = False
#         while current and not found:
#             if current.data == key and current is self.head:
#                 found = True
#                 self.head = current.next_node
#             elif current.data == key:
#                 found = True
#                 previous.next_node = current.next_node
#             else:
#                 previous = current
#                 current = current.next_node
#         return current
#
#     def __repr__(self):
#         nodes = []
#         current = self.head
#         while current:
#             if current is self.head:
#                 nodes.append('[Head: %s]' % current.data)
#             elif current.next_node is None:
#                 nodes.append('[Tail: %s]' % current.data)
#             else:
#                 nodes.append('[%s]' % current.data)
#
#             current = current.next_node
#         return '->'.join(nodes)
#
#
# l = LinkedList()
# l.add(12)
# l.add(13)
# l.add(23)
# n = l.search(13)
# print(n)


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


# Tree
# class TreeNode:
#     def __init__(self, data):
#         self.data = data
#         self.children = []
#         self.parent = None
#
#     def add_child(self, child):
#         child.parent = self
#         self.children.append(child)
#
#     def get_level(self):
#         level = 0
#         p = self.parent
#         while p:
#             level += 1
#             p = p.parent
#
#     def print_tree(self):
#         spaces = ' ' * self.get_level()
#         print(spaces + self.data)
#         if len(self.children) > 0:
#             for child in self.children:
#                 child.print_tree()
#
#
# def build_product_tree():
#     root = TreeNode("Electronics")
#
#     laptop = TreeNode("laptop")
#     laptop.add_child(TreeNode("Mac"))
#     laptop.add_child(TreeNode("surface"))
#
#     cellphone = TreeNode("cellphone")
#     cellphone.add_child(TreeNode("pixel"))
#     cellphone.add_child(TreeNode("oppo"))
#
#     tv = TreeNode("TV")
#     tv.add_child(TreeNode("samsung"))
#     tv.add_child(TreeNode("lg"))
#
#     root.add_child(laptop)
#     root.add_child(cellphone)
#     root.add_child(tv)
#
#     return root
#
#
# if __name__ == '__main__':
#     root = build_product_tree()
#     # print(root.get_level())
#     root.print_tree()
#     pass


# class Node(object):
#     def __init__(self, value):
#         self.value = value
#         self.left = None
#         self.right = None
#
#
# class BinaryTree(object):
#     def __init__(self, root):
#         self.root = Node(root)
#
#     def print_tree(self, traversal_type):
#         if traversal_type == 'preorder':
#             return self.preorder_print(tree.root, '')
#         elif traversal_type == 'inorder':
#             return self.inorder_print(tree.root, '')
#         elif traversal_type == 'postorder':
#             return self.postorder_print(tree.root, '')
#         else:
#             print('Traversal type' + str(traversal_type) + ' is not supported.')
#
#     def preorder_print(self, start, traversal):
#         if start:
#             traversal += (str(start.value) + '-')
#             traversal = self.preorder_print(start.left, traversal)
#             traversal = self.preorder_print(start.right, traversal)
#         return traversal
# def inorder_print(self, start, traversal):
#     if start:
#         traversal = self.inorder_print(start.left, traversal)
#         traversal += (str(start.value) + '-')
#         traversal = self.inorder_print(start.right, traversal)
#     return traversal
# def postorder_print(self, start, traversal):
#     if start:
#         traversal = self.postorder_print(start.left, traversal)
#         traversal = self.postorder_print(start.right, traversal)
#         traversal += (str(start.value) + '-')
#     return traversal

#
# tree = BinaryTree(1)
# tree.root.left = Node(2)
# tree.root.right = Node(3)
# tree.root.left.left = Node(4)
# tree.root.left.right = Node(5)
# tree.root.right.left = Node(6)
# tree.root.right.right = Node(7)
# tree.root.right.right.right = Node(8)


# print(tree.print_tree('preorder'))
# print(tree.print_tree('inorder'))
# print(tree.print_tree('postorder'))
# priority queue

#
# class Employee:
#     num_of_emps = 0
#     raise_amount = 1.04
#
#     def __init__(self, first, last, pay):
#         self.first = first
#         self.last = last
#         self.pay = pay
#         self.email = first + '.' + last + '@company.com'
#         Employee.num_of_emps += 1
#
#     def fullname(self):
#         return '{} {}'.format(self.first, self.last)
#
#     def apply_raise(self):
#         self.pay = int(self.pay * self.raise_amount)
#
#     def __repr__(self):
#         return 'Employee({}, {}, {})'.format(self.first, self.last, self.pay)
#     # def __str__(self):
#     #     pass
#
#
# emp_1 = Employee('corey', 'schafer', 50000)
# emp_2 = Employee('test', 'employee', 60000)
#
# print(emp_1.pay)
#
#
# class Node(object):
#     def __init__(self, value):
#         self.value = value
#         self.left = None
#         self.right = None
#
#
# class BinaryTree(object):
#     def __init__(self, root):
#         self.root = Node(root)
#
#     def print_tree(self, traversal_type):
#         if traversal_type == 'preorder':
#             return self.preorder_print(tree.root, '')
#         else:
#             print('Traversal type' + str(traversal_type) + ' is not supported.')
#
#     def preorder_print(self, start, traversal):
#         if start:
#             traversal += (str(start.value) + '-')
#             traversal = self.preorder_print(start.left, traversal)
#             traversal = self.preorder_print(start.right, traversal)
#         return traversal

# def find(self, start, value_to_find):
#     if start is None:
#         return False
#         elif start.value == value_to_find:
#             return True
#         else:
#             return self.find(start.left, value_to_find) or self.find(start.right, value_to_find)

# def get_height(self, start, height):
#     if start is None:
#         return height
#     elif start.left is None and start.right is None:
#         return height + 1
#     else:
#         height = max(self.get_height(start.left, height), self.get_height(start.right, height)) + 1
#         return height

# def get_node_count(self, start, node_count):
#     if start is None:
#         return node_count
#     elif start.left is None and start.right is None:
#         return node_count
#     elif start.left is not None or start.right is not None:
#         node_count = self.get_node_count(start.left, node_count) + self.get_node_count(start.right, node_count) + 2
#         return node_count
#
#     def get_node_count(self, start):
#         if start is None:
#             return 0
#         elif start.left is None and start.right is None:
#             return 1
#         else:
#             # left_node_count = 0
#             # right_node_count = 0
#             # if start.left is not None:
#             # left_node_count = self.get_node_count(start.left)
#             # if start.right is not None:
#             # right_node_count = self.get_node_count(start.right)
#             return self.get_node_count(start.left) + self.get_node_count(start.right) + 1
#     def get_height(self, start):
#         if start is None:
#             return 0
#         elif start.left is None and start.right is None:
#             return 1
#         else:
#             return max(self.get_height(start.left), self.get_height(start.right)) + 1
#
#     def count_leaves(self, start):
#         if start is None:
#             return 0
#         elif start.left is None and start.right is None:
#             return 1
#         else:
#             return self.count_leaves(start.left) + self.count_leaves(start.right)
#
#
# tree = BinaryTree(1)
# tree.root.left = Node(4)
# tree.root.right = Node(5)
# tree.root.left.left = Node(7)
# tree.root.left.right = Node(8)
# tree.root.right.left = Node(9)
# tree.root.right.right = Node(10)
# tree.root.left.left.left = Node(6)
# tree.root.left.right.right = Node(12)
# tree.root.left.left.right = Node(4)
#
#
# T = tree.print_tree('preorder')
# print(tree.find(tree.root, 200))
# print(tree.get_height(tree.root))
# print(tree.get_node_count(tree.root))
# print(tree.count_leaves(tree.root))
# print(T)
# li = T.split('-')
# print(li)
# def find_data(num):
#     for index in range(0, len(li) - 1):
#         if num != li[index]:
#             print('false')
#         else:
#             print('true')
# print(find_data('4'))
#
#
# def find_node(x):
#     if x == tree.left.left.left:
#         return True
#     else:
#         return False
# print(find_node(6))


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
