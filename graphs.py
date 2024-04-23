INF = float('inf')

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

class IndexHeap:
    def __init__(self, size: int, default: float):
        self._heap = [[i, default] for i in range(size)]
        self._map = list(range(size))

    def _swap(self, i, j):
        self._map[self._heap[i][0]], self._map[self._heap[j][0]] = self._map[self._heap[j][0]], self._map[self._heap[i][0]]
        self._heap[i], self._heap[j] = self._heap[j], self._heap[i]

    def _bubble_up(self, i):
        while (parent := (i - 1) // 2) >= 0 and self._heap[i][1] <= self._heap[parent][1]:
            self._swap(i, parent)
            i = parent

    def _bubble_down(self, i):
        while (child := i * 2 + 1) < len(self._heap):
            if child + 1 < len(self._heap) and self._heap[child + 1][1] < self._heap[child][1]:
                child += 1

            if self._heap[child][1] >= self._heap[i][1]:
                break

            self._swap(i, child)
            i = child

    def update(self, index, new_value):
        i = self._map[index]
        old_value = self._heap[i][1]
        self._heap[i][1] = new_value
        if new_value < old_value:
            self._bubble_up(i)
        elif new_value > old_value:
            self._bubble_down(i)

    def value_of(self, index):
        return self._heap[self._map[index]][1]

    def pop(self):
        self._swap(0, len(self._heap) - 1)
        index, value = self._heap.pop()
        self._map[index] = None
        self._bubble_down(0)
        return index, value

    def empty(self):
        return not self._heap
    
    def has(self, index):
        return self._map[index] is not None

class Graph:
    def __init__(self, edges: list[list[tuple[int, float]]]):
        self._edges = edges

    @staticmethod
    def from_matrix(matrix: list[list[bool]], weights: list[list[float]]):
        edges = [[] for _ in range(len(matrix))]

        for start in range(len(matrix)):
            for end in range(len(matrix)):
                if matrix[start][end]:
                    edges[start].append([end, weights[start][end]])

        return Graph(edges)

    @staticmethod
    def of_size(size: int):
        return Graph([[] for _ in range(size)])

    @property
    def vertex_count(self):
        return len(self._edges)

    def neighbours_of(self, vertex: int):
        return [neighbour for neighbour, _ in self._edges[vertex]]

    def add_edge(self, start: int, end: int, weight: float = 1.0):
        for edge in self._edges[start]:
            if edge[0] == end:
                edge[1] = weight
                return
        self._edges[start].append([end, weight])

    def add_bidirectional_edge(self, v1: int, v2: int, weight: float = 1.0):
        self.add_edge(v1, v2, weight)
        self.add_edge(v2, v1, weight)

    def dfs(self, function):
        visited = [False] * self.vertex_count

        def visit(v):
            function(v)
            visited[v] = True
            for n in self.neighbours_of(v):
                if not visited[n]:
                    visit(n)

        if self.vertex_count > 0:
            visit(0)

    def bfs(self, function):
        if self.vertex_count <= 0:
            return

        visited = [False] * self.vertex_count
        to_visit = [0]
        visited[0] = True

        i = 0
        while i < len(to_visit):
            function(to_visit[i])
            for n in self.neighbours_of(to_visit[i]):
                if not visited[n]:
                    visited[n] = True
                    to_visit.append(n)

    def is_connected(self):
        count = [0]

        def inc(_):
            count[0] += 1

        self.dfs(inc)
        return count == self.vertex_count

    @property
    def edges(self):
        return [(weight, start, end) for start in range(self.vertex_count) for end, weight in self._edges[start]]

    # Kruskal's algorithm
    def min_spanning_tree(self):
        ds = DisjointSet(self.vertex_count)

        edges = [[] for _ in range(self.vertex_count)]
        for weight, start, end in sorted(self.edges):
            if not ds.is_connected(start, end):
                ds.union(start, end)
                edges[start].append((end, weight))

        return Graph(edges)
    
    @staticmethod
    def _build_path(end: int, start: int, visited_from: list[int], distance: float):
        path = [end]
        while end != start:
            end = visited_from[end]
            path.append(end)
        path.reverse()
        return path, distance

    def dijkstra(self, start, end):
        visited_from = [-1] * self.vertex_count

        vertices = IndexHeap(self.vertex_count, INF)
        vertices.update(start, 0)

        while True:
            if vertices.empty():
                return None

            current, distance = vertices.pop()
            if distance == INF:
                return None

            if current == end:
                return Graph._build_path(end, start, visited_from, distance)

            for n, weight in self._edges[current]:
                if vertices.has(n) and distance + weight < vertices.value_of(n):
                    vertices.update(n, distance + weight)
                    visited_from[n] = current

    def __repr__(self) -> str:
        return f"Graph({repr(self._edges)})"

    def __str__(self) -> str:
        s = f'Graph of {self.vertex_count} vertices\n'
        s += 'Edges:\n'
        s += '\n'.join(f'    {start} -> {end} (weight {weight})' for weight, start, end in self.edges)
        return s
