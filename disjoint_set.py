class DisjointSet:
    def __init__(self, size):
        self._parent = list(range(size))
        self._rank = [1] * size

    def find(self, vertex):
        if self._parent[vertex] != vertex:
            self._parent[vertex] = self.find(self._parent[vertex])
        return self._parent[vertex]

    def union(self, v1, v2):
        v1 = self.find(v1)
        v2 = self.find(v2)
        if v1 == v2:
            return
        elif self._rank[v1] < self._rank[v2]:
            self._parent[v1] = v2
            self._rank[v2] += self._rank[v1]
        else:
            self._parent[v2] = v1
            self._rank[v1] += self._rank[v2]

    def is_connected(self, v1, v2):
        return self.find(v1) == self.find(v2)
