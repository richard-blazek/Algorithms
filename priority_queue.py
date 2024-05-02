class PriorityQueue:
    def __init__(self, capacity: int):
        self._heap = []
        self._map = [None] * capacity

    def _swap(self, i: int, j: int):
        self._map[self._heap[i][0]], self._map[self._heap[j][0]] = self._map[self._heap[j][0]], self._map[self._heap[i][0]]
        self._heap[i], self._heap[j] = self._heap[j], self._heap[i]

    def _bubble_up(self, i: int):
        while (parent := (i - 1) // 2) >= 0 and self._heap[i][1] <= self._heap[parent][1]:
            self._swap(i, parent)
            i = parent

    def _bubble_down(self, i: int):
        while (child := i * 2 + 1) < len(self._heap):
            if child + 1 < len(self._heap) and self._heap[child + 1][1] < self._heap[child][1]:
                child += 1

            if self._heap[child][1] >= self._heap[i][1]:
                break

            self._swap(i, child)
            i = child

    def priority(self, identity: int):
        return self._heap[self._map[identity]][1]

    def _decrease_priority(self, identity: int, priority: float):
        self._heap[self._map[identity]][1] = priority
        self._bubble_up(self._map[identity])

    def _increase_priority(self, identity: int, priority: float):
        self._heap[self._map[identity]][1] = priority
        self._bubble_down(self._map[identity])

    def set_priority(self, identity: int, priority: float):
        if priority < self.priority(identity):
            self._decrease_priority(identity, priority)
        else:
            self._increase_priority(identity, priority)

    def top(self):
        return self._heap[0]

    def pop(self):
        self._swap(0, len(self._heap) - 1)
        identity, priority = self._heap.pop()
        self._map[identity] = None
        self._bubble_down(0)
        return identity, priority

    def add(self, identity: int, priority: float):
        self._heap.append([identity, priority])
        self._map[identity] = len(self._heap) - 1
        self._bubble_up(len(self._heap) - 1)

    def empty(self):
        return not self._heap

    def has(self, identity: int):
        return self._map[identity] is not None
