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
        self._heap = [(i, default) for i in range(size)]
        self._map = list(range(size))

    def _bubble_up(self, i):
        while (parent := (i - 1) // 2) >= 0 and self._heap[i][1] <= self._heap[parent][1]:
            self._heap[i], self._heap[parent] = self._heap[parent], self._heap[i]
            self._map[self._heap[i][0]] = i
            self._map[self._heap[parent][0]] = parent
            i = parent

    def _bubble_down(self, i):
        while (child := i * 2 + 1) < len(self._heap):
            if child + 1 < len(self._heap) and self._heap[child + 1][1] < self._heap[child][1]:
                child += 1

            if self._heap[child][1] >= self._heap[i][1]:
                break

            self._heap[i], self._heap[child] = self._heap[child], self._heap[i]
            self._map[self._heap[i][0]] = i
            self._map[self._heap[child][0]] = child
            i = child

    def top(self):
        return self._heap[0][0]

    def update(self, index, new_value):
        i = self._map[index]
        old_value = self._heap[i][1]
        self._heap[i][1] = new_value
        if new_value < old_value:
            self._bubble_up(i)
        elif new_value < old_value:
            self._bubble_down(i)

    def value_of(self, index):
        return self._heap[self._map[index]][1]

class Graph:
    def __init__(self, edges: list[list[tuple[int, float]]]):
        self._edges = edges

    @staticmethod
    def from_matrix(matrix: list[list[bool]], weights: list[list[float]]):
        edges = [[] for _ in range(len(matrix))]

        for start in range(len(matrix)):
            for end in range(len(matrix)):
                if matrix[start][end]:
                    edges[start].append((end, weights[start][end]))

        return Graph(edges)

    @property
    def vertex_count(self):
        return len(self._edges)

    def neighbours_of(self, vertex: int):
        return [neighbour for neighbour, _ in self._edges[vertex]]

    def edge_weight(self, start: int, end: int):
        return self._edges[start][end][1]

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

    # Kruskal's algorithm
    def min_spanning_tree(self):
        all_edges = sorted((weight, start, end) for start in range(self.vertex_count) for end, weight in self._edges[start])
        ds = DisjointSet(self.vertex_count)

        edges = [[] for _ in range(self.vertex_count)]
        for weight, start, end in all_edges:
            if not ds.is_connected(start, end):
                ds.union(start, end)
                edges[start].append((end, weight))

        return Graph(edges)
    
    def dijkstra_tree(self, start):
        edge_start = [-1] * self.vertex_count
        edge_weight = [-1] * self.vertex_count
        edges = [[] for _ in range(self.vertex_count)]

        visited = [False] * self.vertex_count
        vertices = IndexHeap(self.vertex_count, INF)
        vertices.update(start, 0)

        while vertices.value_of(vertices.top()) < INF:
            current = vertices.top()
            visited[current] = True
            vertices.update(current, INF)

            if edge_start[current] != -1:
                edges[edge_start[current]].append((current, edge_weight[current]))

            for n, weight in self._edges[current]:
                if not visited[n] and vertices.value_of(current) + weight < vertices.value_of(n):
                    vertices.update(n, vertices.value_of(current) + weight)
                    edge_start[n] = current
                    edge_weight[n] = weight

        return Graph(edges)
