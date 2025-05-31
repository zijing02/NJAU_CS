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


# 链表
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
# today is 7.19