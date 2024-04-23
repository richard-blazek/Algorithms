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

    def priority(self, idx: int):
        return self._heap[self._map[idx]][1]

    def decrease_priority(self, idx: int, priority: float):
        self._heap[self._map[idx]][1] = priority
        self._bubble_up(self._map[idx])

    def top(self):
        return self._heap[0]

    def pop(self):
        self._swap(0, len(self._heap) - 1)
        index, priority = self._heap.pop()
        self._map[index] = None
        self._bubble_down(0)
        return index, priority

    def add(self, idx: int, priority: float):
        self._heap.append([idx, priority])
        self._map[idx] = len(self._heap) - 1
        self._bubble_up(len(self._heap) - 1)

    def empty(self):
        return not self._heap

    def has(self, index: int):
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

    def edge_weight(self, start: int, end: int):
        return next((weight for vertex, weight in self._edges[start] if vertex == end), None)

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
    def _build_path(start: int, end: int, visited_from: list[int]):
        path = [end]
        while end != start:
            end = visited_from[end]
            path.append(end)
        path.reverse()
        return path

    def path_length(self, path: list[int]) -> float:
        return sum(self.edge_weight(src, dst) for src, dst in zip(path, path[1:]))

    def dijkstra(self, start: int, goal: int):
        visited_from = [None] * self.vertex_count
        vertices = PriorityQueue(self.vertex_count)
        for i in range(self.vertex_count):
            vertices.add(i, INF)

        visited_from[start] = start
        vertices.decrease_priority(start, 0)

        while not vertices.empty() and vertices.top()[1] != INF:
            current, distance = vertices.pop()

            if current == goal:
                return Graph._build_path(start, goal, visited_from)

            for n, weight in self._edges[current]:
                if vertices.has(n) and distance + weight < vertices.priority(n):
                    vertices.decrease_priority(n, distance + weight)
                    visited_from[n] = current
        return None

    def greedy(self, start: int, goal: int, heuristic):
        visited_from = [None] * self.vertex_count
        vertices = PriorityQueue(self.vertex_count)

        visited_from[start] = start
        vertices.add(start, 0)

        while not vertices.empty():
            current, _ = vertices.pop()

            if current == goal:
                return Graph._build_path(start, goal, visited_from)

            for n, _ in self._edges[current]:
                if visited_from[n] is None:
                    visited_from[n] = current
                    vertices.add(n, heuristic(goal, n))
        return None

    def a_star(self, start: int, goal: int, heuristic):
        visited_from = [None] * self.vertex_count
        vertices = PriorityQueue(self.vertex_count)
        for i in range(self.vertex_count):
            vertices.add(i, INF)

        visited_from[start] = start
        vertices.decrease_priority(start, 0)

        while not vertices.empty() and vertices.top()[1] != INF:
            current, distance = vertices.pop()

            if current == goal:
                return Graph._build_path(start, goal, visited_from)

            for n, weight in self._edges[current]:
                new_priority = distance + weight + heuristic(goal, n)
                if vertices.has(n) and new_priority < vertices.priority(n):
                    vertices.decrease_priority(n, new_priority)
                    visited_from[n] = current
        return None

    def __repr__(self) -> str:
        return f"Graph({repr(self._edges)})"

    def __str__(self) -> str:
        s = f'Graph of {self.vertex_count} vertices\n'
        s += 'Edges:\n'
        s += '\n'.join(f'    {start} -> {end} (weight {weight})' for weight, start, end in self.edges)
        return s

sample_graph = Graph([[], [[2, 100], [28, 40]], [[1, 100], [3, 100], [29, 40]], [[2, 100], [4, 100], [30, 40]], [[3, 100], [5, 100], [31, 40]], [[4, 100], [6, 100], [32, 40]], [[5, 100], [7, 100], [33, 40]], [[6, 100], [8, 100], [34, 40]], [[7, 100], [9, 100], [35, 40]], [[8, 100], [10, 100], [36, 40]], [[9, 100], [11, 100], [37, 40]], [[10, 100], [12, 100], [38, 40]], [[11, 100], [13, 100], [39, 40]], [[12, 100], [14, 100], [40, 40]], [[13, 100], [15, 100], [41, 40]], [[14, 100], [16, 100], [42, 40]], [[15, 100], [17, 100], [43, 40]], [[16, 100], [18, 100], [44, 40]], [[17, 100], [19, 100], [45, 40]], [[18, 100], [20, 100], [46, 40]], [[19, 100], [21, 100], [47, 40]], [[20, 100], [22, 100], [48, 40]], [[21, 100], [23, 100], [49, 40]], [[22, 100], [24, 100], [50, 40]], [[23, 100], [25, 100], [51, 40]], [[24, 100], [26, 100], [52, 40]], [[25, 100], [27, 100], [53, 40]], [[26, 100], [28, 100], [54, 40]], [[27, 100], [29, 100], [1, 40], [55, 40]], [[28, 100], [30, 100], [2, 40], [56, 40]], [[29, 100], [31, 100], [3, 40], [57, 40]], [[30, 100], [32, 100], [4, 40], [58, 40]], [[31, 100], [33, 100], [5, 40], [59, 40]], [[32, 100], [34, 100], [6, 40], [60, 40]], [[33, 100], [35, 100], [7, 40], [61, 40]], [[34, 100], [36, 100], [8, 40], [62, 40]], [[35, 100], [37, 100], [9, 40], [63, 40]], [[36, 100], [38, 100], [10, 40], [64, 40]], [[37, 100], [39, 100], [11, 40], [65, 40]], [[38, 100], [40, 100], [12, 40], [66, 40]], [[39, 100], [41, 100], [13, 40], [67, 40]], [[40, 100], [42, 100], [14, 40], [68, 40]], [[41, 100], [43, 100], [15, 40], [69, 40]], [[42, 100], [44, 100], [16, 40], [70, 40]], [[43, 100], [45, 100], [17, 40], [71, 40]], [[44, 100], [46, 100], [18, 40], [72, 40]], [[45, 100], [47, 100], [19, 40], [73, 40]], [[46, 100], [48, 100], [20, 40], [74, 40]], [[47, 100], [49, 100], [21, 40], [75, 40]], [[48, 100], [50, 100], [22, 40], [76, 40]], [[49, 100], [51, 100], [23, 40], [77, 40]], [[50, 100], [52, 100], [24, 40], [78, 40]], [[51, 100], [53, 100], [25, 40], [79, 40]], [[52, 100], [54, 100], [26, 40], [80, 40]], [[53, 100], [55, 100], [27, 40], [81, 40]], [[54, 100], [56, 100], [28, 40], [82, 40]], [[55, 100], [57, 100], [29, 40], [83, 40]], [[56, 100], [58, 100], [30, 40], [84, 40]], [[57, 100], [59, 100], [31, 40], [85, 40]], [[58, 100], [60, 100], [32, 40], [86, 40]], [[59, 100], [61, 100], [33, 40], [87, 40]], [[60, 100], [62, 100], [34, 40], [88, 40]], [[61, 100], [63, 100], [35, 40], [89, 40]], [[62, 100], [64, 100], [36, 40], [90, 40]], [[63, 100], [65, 100], [37, 40], [91, 40]], [[64, 100], [66, 100], [38, 40], [92, 40]], [[65, 100], [67, 100], [39, 40], [93, 40]], [[66, 100], [68, 100], [40, 40], [94, 40]], [[67, 100], [69, 100], [41, 40], [95, 40]], [[68, 100], [70, 100], [42, 40], [96, 40]], [[69, 100], [71, 100], [43, 40], [97, 40]], [[70, 100], [72, 100], [44, 40], [98, 40]], [[71, 100], [73, 100], [45, 40], [99, 40]], [[72, 100], [74, 100], [46, 40], [100, 40]], [[73, 100], [75, 100], [47, 40], [101, 40]], [[74, 100], [76, 100], [48, 40], [102, 40]], [[75, 100], [77, 100], [49, 40], [103, 40]], [[76, 100], [78, 100], [50, 40], [104, 40]], [[77, 100], [79, 100], [51, 40], [105, 40]], [[78, 100], [80, 100], [52, 40], [106, 40]], [[79, 100], [81, 100], [53, 40], [107, 40]], [[80, 100], [82, 100], [54, 40], [108, 40]], [[81, 100], [83, 100], [55, 40], [109, 40]], [[82, 100], [84, 100], [56, 40], [110, 40]], [[83, 100], [85, 100], [57, 40], [111, 40]], [[84, 100], [86, 100], [58, 40], [112, 40]], [[85, 100], [87, 100], [59, 40], [113, 40]], [[86, 100], [88, 100], [60, 40], [114, 40]], [[87, 100], [89, 100], [61, 40], [115, 40]], [[88, 100], [90, 100], [62, 40], [116, 40]], [[89, 100], [91, 100], [63, 40], [117, 40]], [[90, 100], [92, 100], [64, 40], [118, 40]], [[91, 100], [93, 100], [65, 40], [119, 40]], [[92, 100], [94, 100], [66, 40], [120, 40]], [[93, 100], [95, 100], [67, 40], [121, 40]], [[94, 100], [96, 100], [68, 40], [122, 40]], [[95, 100], [97, 100], [69, 40], [123, 40]], [[96, 100], [98, 100], [70, 40], [124, 40]], [[97, 100], [99, 100], [71, 40], [125, 40]], [[98, 100], [100, 100], [72, 40], [126, 40]], [[99, 100], [73, 40]], [[74, 40]], [[75, 40]], [[76, 40]], [[77, 40]], [[78, 40]], [[79, 40]], [[80, 40]], [[81, 40]], [[82, 40]], [[83, 40]], [[84, 40]], [[85, 40]], [[86, 40]], [[87, 40]], [[88, 40]], [[89, 40]], [[90, 40]], [[91, 40]], [[92, 40]], [[93, 40]], [[94, 40]], [[95, 40]], [[96, 40]], [[97, 40]], [[98, 40]], [[99, 40]], []])

sample_heuristic = lambda a, b: abs(a - b)
