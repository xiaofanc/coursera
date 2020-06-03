from collections import deque

class TreeNode:
    def __init__(self, val, left = None, right = None):
        self.val = val
        self.left = left
        self.right = right

    def __str__(self):
        return 'N(%s, %s, %s)' % (self.val, self.left or '', self.right or '')

def createcompletetree(nums):
    size = len(nums)
    root = TreeNode(nums[0])
    q = [root]
    i = 1
    while q:
        node = q.pop(0)
        if i <  size:   # append left
            left = TreeNode(nums[i])
            node.left = left
            q.append(left)
            i += 1
        else: break
        if i <  size:   # append right
            right = TreeNode(nums[i])
            node.right = right
            q.append(right)
            i += 1    
        else: break   
    return root 

def createcompletetree(nums):
    size = len(nums)
    root = TreeNode(nums[0])
    q = deque([root])
    i = 1
    while q:
        node = q.popleft()
        if i <  size:   # append left
            left = TreeNode(nums[i])
            node.left = left
            q.append(left)
            i += 1
        else: break
        if i <  size:   # append right
            right = TreeNode(nums[i])
            node.right = right
            q.append(right)
            i += 1    
        else: break   
    return root 

if __name__ == '__main__':
    root = createcompletetree([1,2,3,4,5,6,7,8,9])
    print(root)


