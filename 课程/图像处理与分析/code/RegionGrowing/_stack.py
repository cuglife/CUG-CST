class Stack(object):
    def __init__(self):
        self._item = []

    def push(self, value):
        self._item.append(value)

    def pop(self):
        return self._item.pop()

    def size(self):
        return len(self._item)

    def is_empty(self):
        return True if self.size() == 0 else False

    def empty(self):
        self._item = []
