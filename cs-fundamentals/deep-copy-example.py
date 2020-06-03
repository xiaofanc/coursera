# 20200514 discussion on deep copy

def log_fname(fn):
    def wrapper(*arg):
        print(f"{fn.__name__}{arg}")
        return fn(*arg)
    return wrapper

@log_fname
def ex1():
    a = [ [1] ]*5
    print('a =', a) #[[1], [1], [1], [1], [1]]
    a[0][0] = 2
    print('a =', a) #[[2], [2], [2], [2], [2]]
    a[0] = [3]
    print('a =', a) #[[3], [2], [2], [2], [2]]

@log_fname
def ex2():
    a = [1, [2], [3, [4]], 5]
    b = a # this is obviously not deep copy
    a[0]= 6
    print('a =', a) #[6, [2], [3, [4]], 5]
    print('b =', b) #[6, [2], [3, [4]], 5]
    b = a[:] # is this deep copy?
    print('a =', a) #[6, [2], [3, [4]], 5]
    print('b =', b) #[6, [2], [3, [4]], 5]
    a[1] = [7] # 
    print('a =', a) #[6, [7], [3, [4]], 5]
    print('b =', b) #[6, [2], [3, [4]], 5]
    a[2][0] = 8 # still share the memory 
    print('a =', a) #[6, [7], [8, [4]], 5]
    print('b =', b) #[6, [2], [8, [4]], 5]

@log_fname
def deepcopy(nums):
    return [n if isinstance(n, int) else deepcopy(n) for n in nums]

@log_fname
def ex3(): # completely separate memory
    a = [1, [2], [3, [4]], 5]
    b = deepcopy(a)
    print('a =', a) #[1, [2], [3, [4]], 5]
    print('b =', b) #[1, [2], [3, [4]], 5]
    a[1][0] = 80
    print('a =', a) #[1, [80], [3 , [4]], 5]
    print('b =', b) #[1, [2], [3, [4]], 5]
    a[2][1][0] = 0
    print('a =', a) #[1, [80], [3, [0]], 5]
    print('b =', b) #[1, [2], [3, [4]], 5]


if __name__ == '__main__':
    ex1()
    ex2()
    ex3()