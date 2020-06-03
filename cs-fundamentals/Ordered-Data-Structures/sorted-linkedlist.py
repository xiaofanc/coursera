class ListNode:
    def __init__(self, val, next=None, prev=None):
        self.val = val
        self.next = next
        self.prev = prev

    def __str__(self):
        return '%s->%s' % (self.val, self.next or '')

    def __gt__(self, other):
        return True

    def __lt__(self, other):
        return False

    @classmethod
    def from_list(cls, nums):
        if nums == []: return None
        else: return cls(nums[0], cls.from_list(nums[1:]))


def insert(head, newVal):
    prehead = ListNode(float('-inf'))
    prehead.next = head
    newNode = ListNode(newVal)
    prev, cur = prehead, head
    while cur:
        if cur.val >= newVal:
            prev.next = newNode
            newNode.prev = prev
            newNode.next = cur
            cur.prev = newNode
            break
        prev, cur = prev.next, cur.next
    if prev.val < newVal:
        prev.next = newNode
        newNode.prev = prev
    res = prehead.next
    prehead.next = None
    return res

def insert(head, newVal):
    prehead = ListNode(float('-inf'))
    prehead.next = head
    newNode = ListNode(newVal)
    cur = prehead
    while cur.next:
        if cur.next.val >= newVal:
            newNode.prev = cur
            newNode.next = cur.next
            cur.next.prev = newNode
            cur.next = newNode
            break
        cur = cur.next
    if cur.val < newVal:
        cur.next = newNode
        newNode.prev = cur
    res = prehead.next
    prehead.next = None
    return res


if __name__ == '__main__':
    l = ListNode.from_list([1,2,3,4,5])
    print(l)
    l = insert(l,3)
    l = insert(l,3.1)
    print(l)
    l = insert(l,0)
    l = insert(l,10)
    print(l)