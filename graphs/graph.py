from disjoint_set import DisjointSet
from priority_queue import PriorityQueue

INF = float('inf')

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

    @property
    def edges(self):
        return ((weight, start, end) for start in range(self.vertex_count) for end, weight in self._edges[start])

    def __repr__(self) -> str:
        return f"Graph({repr(self._edges)})"

    def __str__(self) -> str:
        return 'Graph {\n' + '\n'.join(f'  {start} -> {end} (weight {weight})' for weight, start, end in self.edges) + '\n}'

    def dfs(self, start=0, preorder=lambda x:None, postorder=lambda x:None):
        visited = [False] * self.vertex_count

        def visit(v):
            preorder(v)
            visited[v] = True
            for n in self.neighbours_of(v):
                if not visited[n]:
                    visit(n)
            postorder(v)

        if self.vertex_count > 0:
            visit(start)

        return visited

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
        return all(self.dfs())

    def spanning_tree_kruskal(self):
        ds = DisjointSet(self.vertex_count)
        tree = Graph.of_size(self.vertex_count)

        for weight, start, end in sorted(self.edges):
            if not ds.is_connected(start, end):
                ds.union(start, end)
                tree.add_bidirectional_edge(start, end, weight)
        return tree

    def spanning_tree_jarník(self):
        visited_from = [None] * self.vertex_count
        vertices = PriorityQueue(self.vertex_count)
        for i in range(self.vertex_count):
            vertices.add(i, INF)

        tree = Graph.of_size(self.vertex_count)
        vertices.set_priority(0, 0)

        while True:
            if vertices.empty():
                return tree
            if vertices.top()[1] == INF:
                return None

            current, weight = vertices.pop()
            if current != 0:
                tree.add_bidirectional_edge(visited_from[current], current, weight)

            for n, weight in self._edges[current]:
                if vertices.has(n) and weight < vertices.priority(n):
                    vertices.set_priority(n, weight)
                    visited_from[n] = current

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
        vertices.set_priority(start, 0)

        while not vertices.empty() and vertices.top()[1] != INF:
            current, distance = vertices.pop()

            if current == goal:
                return Graph._build_path(start, goal, visited_from)

            for n, weight in self._edges[current]:
                new_priority = distance + weight
                if vertices.has(n) and new_priority < vertices.priority(n):
                    vertices.set_priority(n, new_priority)
                    visited_from[n] = current
        return None

    # With heuristic is f(x) = 0, A* becomes Dijkstra
    def a_star(self, start: int, goal: int, heuristic):
        visited_from = [None] * self.vertex_count
        vertices = PriorityQueue(self.vertex_count)
        for i in range(self.vertex_count):
            vertices.add(i, INF)

        visited_from[start] = start
        vertices.set_priority(start, 0)

        while not vertices.empty() and vertices.top()[1] != INF:
            current, distance = vertices.pop()

            if current == goal:
                return Graph._build_path(start, goal, visited_from)

            for n, weight in self._edges[current]:
                new_priority = distance + weight + heuristic(goal, n)
                if vertices.has(n) and new_priority < vertices.priority(n):
                    vertices.set_priority(n, new_priority)
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

    def _vertex_with_no_outgoing_edges(self):
        return next(i for i in range(self.vertex_count) if not any(True for edges in self._edges for end, _ in edges if end == i))

    def topological_sort(self):
        vertices = []
        self.dfs(start=self._vertex_with_no_outgoing_edges(), postorder=vertices.append)
        vertices.reverse()
        return vertices

    def _run_bellman_ford(self, start: int):
        visited_from = [None] * self.vertex_count
        distance_to = [INF] * self.vertex_count
        visited_from[start] = start
        distance_to[start] = 0

        last_updated = None
        for _ in range(self.vertex_count):
            last_updated = None

            for weight, src, dst in self.edges:
                if distance_to[dst] > distance_to[src] + weight:
                    distance_to[dst] = distance_to[src] + weight
                    visited_from[dst] = src
                    last_updated = dst

        return visited_from, last_updated

    def bellman_ford(self, start: int, goal: int):
        visited_from, _ = self._run_bellman_ford(start)
        return Graph._build_path(start, goal, visited_from) if visited_from[goal] is not None else None

    def negative_cycle_from(self, start: int):
        visited_from, last = self._run_bellman_ford(start)
        if last is None:
            return None

        cycle = [last]
        current = visited_from[last]
        while current != last:
            cycle.append(current)
            current = visited_from[current]

        if len(cycle) > 1:
            cycle.append(last)
        cycle.reverse()
        return cycle

    def floyd_warshall(self):
        second_last_step = [[None] * self.vertex_count for _ in range(self.vertex_count)]
        distance_between = [[INF] * self.vertex_count for _ in range(self.vertex_count)]

        for i in range(self.vertex_count):
            distance_between[i][i] = 0
            second_last_step[i][i] = i

        for weight, start, end in self.edges:
            distance_between[start][end] = weight
            second_last_step[start][end] = start

        for k in range(self.vertex_count):
            for i in range(self.vertex_count):
                for j in range(self.vertex_count):
                     if distance_between[i][j] > distance_between[i][k] + distance_between[k][j]:
                        distance_between[i][j] = distance_between[i][k] + distance_between[k][j]
                        second_last_step[i][j] = second_last_step[k][j]

        return distance_between, second_last_step
    
    def sum_of_weights(self):
        return sum(w for w, _, _ in self.edges)


sample_graph = Graph([[], [[2, 100], [28, 40]], [[1, 100], [3, 100], [29, 40]], [[2, 100], [4, 100], [30, 40]], [[3, 100], [5, 100], [31, 40]], [[4, 100], [6, 100], [32, 40]], [[5, 100], [7, 100], [33, 40]], [[6, 100], [8, 100], [34, 40]], [[7, 100], [9, 100], [35, 40]], [[8, 100], [10, 100], [36, 40]], [[9, 100], [11, 100], [37, 40]], [[10, 100], [12, 100], [38, 40]], [[11, 100], [13, 100], [39, 40]], [[12, 100], [14, 100], [40, 40]], [[13, 100], [15, 100], [41, 40]], [[14, 100], [16, 100], [42, 40]], [[15, 100], [17, 100], [43, 40]], [[16, 100], [18, 100], [44, 40]], [[17, 100], [19, 100], [45, 40]], [[18, 100], [20, 100], [46, 40]], [[19, 100], [21, 100], [47, 40]], [[20, 100], [22, 100], [48, 40]], [[21, 100], [23, 100], [49, 40]], [[22, 100], [24, 100], [50, 40]], [[23, 100], [25, 100], [51, 40]], [[24, 100], [26, 100], [52, 40]], [[25, 100], [27, 100], [53, 40]], [[26, 100], [28, 100], [54, 40]], [[27, 100], [29, 100], [1, 40], [55, 40]], [[28, 100], [30, 100], [2, 40], [56, 40]], [[29, 100], [31, 100], [3, 40], [57, 40]], [[30, 100], [32, 100], [4, 40], [58, 40]], [[31, 100], [33, 100], [5, 40], [59, 40]], [[32, 100], [34, 100], [6, 40], [60, 40]], [[33, 100], [35, 100], [7, 40], [61, 40]], [[34, 100], [36, 100], [8, 40], [62, 40]], [[35, 100], [37, 100], [9, 40], [63, 40]], [[36, 100], [38, 100], [10, 40], [64, 40]], [[37, 100], [39, 100], [11, 40], [65, 40]], [[38, 100], [40, 100], [12, 40], [66, 40]], [[39, 100], [41, 100], [13, 40], [67, 40]], [[40, 100], [42, 100], [14, 40], [68, 40]], [[41, 100], [43, 100], [15, 40], [69, 40]], [[42, 100], [44, 100], [16, 40], [70, 40]], [[43, 100], [45, 100], [17, 40], [71, 40]], [[44, 100], [46, 100], [18, 40], [72, 40]], [[45, 100], [47, 100], [19, 40], [73, 40]], [[46, 100], [48, 100], [20, 40], [74, 40]], [[47, 100], [49, 100], [21, 40], [75, 40]], [[48, 100], [50, 100], [22, 40], [76, 40]], [[49, 100], [51, 100], [23, 40], [77, 40]], [[50, 100], [52, 100], [24, 40], [78, 40]], [[51, 100], [53, 100], [25, 40], [79, 40]], [[52, 100], [54, 100], [26, 40], [80, 40]], [[53, 100], [55, 100], [27, 40], [81, 40]], [[54, 100], [56, 100], [28, 40], [82, 40]], [[55, 100], [57, 100], [29, 40], [83, 40]], [[56, 100], [58, 100], [30, 40], [84, 40]], [[57, 100], [59, 100], [31, 40], [85, 40]], [[58, 100], [60, 100], [32, 40], [86, 40]], [[59, 100], [61, 100], [33, 40], [87, 40]], [[60, 100], [62, 100], [34, 40], [88, 40]], [[61, 100], [63, 100], [35, 40], [89, 40]], [[62, 100], [64, 100], [36, 40], [90, 40]], [[63, 100], [65, 100], [37, 40], [91, 40]], [[64, 100], [66, 100], [38, 40], [92, 40]], [[65, 100], [67, 100], [39, 40], [93, 40]], [[66, 100], [68, 100], [40, 40], [94, 40]], [[67, 100], [69, 100], [41, 40], [95, 40]], [[68, 100], [70, 100], [42, 40], [96, 40]], [[69, 100], [71, 100], [43, 40], [97, 40]], [[70, 100], [72, 100], [44, 40], [98, 40]], [[71, 100], [73, 100], [45, 40], [99, 40]], [[72, 100], [74, 100], [46, 40], [100, 40]], [[73, 100], [75, 100], [47, 40], [101, 40]], [[74, 100], [76, 100], [48, 40], [102, 40]], [[75, 100], [77, 100], [49, 40], [103, 40]], [[76, 100], [78, 100], [50, 40], [104, 40]], [[77, 100], [79, 100], [51, 40], [105, 40]], [[78, 100], [80, 100], [52, 40], [106, 40]], [[79, 100], [81, 100], [53, 40], [107, 40]], [[80, 100], [82, 100], [54, 40], [108, 40]], [[81, 100], [83, 100], [55, 40], [109, 40]], [[82, 100], [84, 100], [56, 40], [110, 40]], [[83, 100], [85, 100], [57, 40], [111, 40]], [[84, 100], [86, 100], [58, 40], [112, 40]], [[85, 100], [87, 100], [59, 40], [113, 40]], [[86, 100], [88, 100], [60, 40], [114, 40]], [[87, 100], [89, 100], [61, 40], [115, 40]], [[88, 100], [90, 100], [62, 40], [116, 40]], [[89, 100], [91, 100], [63, 40], [117, 40]], [[90, 100], [92, 100], [64, 40], [118, 40]], [[91, 100], [93, 100], [65, 40], [119, 40]], [[92, 100], [94, 100], [66, 40], [120, 40]], [[93, 100], [95, 100], [67, 40], [121, 40]], [[94, 100], [96, 100], [68, 40], [122, 40]], [[95, 100], [97, 100], [69, 40], [123, 40]], [[96, 100], [98, 100], [70, 40], [124, 40]], [[97, 100], [99, 100], [71, 40], [125, 40]], [[98, 100], [100, 100], [72, 40], [126, 40]], [[99, 100], [73, 40]], [[74, 40]], [[75, 40]], [[76, 40]], [[77, 40]], [[78, 40]], [[79, 40]], [[80, 40]], [[81, 40]], [[82, 40]], [[83, 40]], [[84, 40]], [[85, 40]], [[86, 40]], [[87, 40]], [[88, 40]], [[89, 40]], [[90, 40]], [[91, 40]], [[92, 40]], [[93, 40]], [[94, 40]], [[95, 40]], [[96, 40]], [[97, 40]], [[98, 40]], [[99, 40]], []])

sample_heuristic = lambda a, b: abs(a - b)
print('Dijkstra:', sample_graph.dijkstra(1, 126))
print('Greedy:  ', sample_graph.greedy(1, 126, sample_heuristic))
print('A star:  ', sample_graph.a_star(1, 126, sample_heuristic))
print()

sample_digraph = Graph([[[1, 31], [3, 13], [4, 31]], [[4, 13], [5, 31]], [[0, 13]], [], [[5, 13]], [[4, 31]]])
print(sample_digraph)
print('Topological sort: ', sample_digraph.topological_sort())
print()

sample_cycle = Graph([[[1, 1.5]], [[2, 0.2998]], [[3, -0.3119]], [[1, 0.005]]])
print(sample_cycle)
print('Cycle: ', sample_cycle.negative_cycle_from(0))

sample_undirected = Graph([[[5, 100], [6, 1]], [[5, 100], [7, 2]], [[5, 100], [8, 3]], [[5, 100], [9, 4]], [[5, 100], [10, 5]], [[1, 100], [2, 100], [3, 100], [4, 100], [6, 100], [7, 100], [8, 100], [0, 100], [9, 100], [10, 100], [11, 6], [12, 100], [13, 100], [14, 100], [15, 100], [16, 100], [17, 100], [18, 100], [19, 100], [20, 100], [21, 100], [22, 100], [23, 100], [24, 100], [25, 100], [26, 100], [27, 100], [28, 100], [29, 100], [30, 100], [31, 100], [32, 100], [33, 100], [34, 100], [35, 100], [36, 100], [37, 100], [38, 100], [39, 100], [40, 100], [41, 100], [42, 100], [43, 100], [44, 100], [45, 100], [46, 100], [47, 100], [48, 100], [49, 100], [50, 100], [51, 100], [52, 100], [53, 100], [54, 100], [55, 100], [56, 100], [57, 100], [58, 100], [59, 100], [60, 100], [61, 100], [62, 100], [63, 100], [64, 100], [65, 100], [66, 100], [67, 100], [68, 100], [69, 100], [70, 100], [71, 100], [72, 100], [73, 100], [74, 100], [75, 100], [76, 100], [77, 100], [78, 100], [79, 100], [80, 100], [81, 100], [82, 100], [83, 100], [84, 100], [85, 100], [86, 100], [87, 100], [88, 100], [89, 100], [90, 100], [91, 100], [92, 100], [93, 100], [94, 100], [95, 100], [96, 100], [97, 100], [98, 100], [99, 100], [100, 100], [101, 100], [102, 100], [103, 100], [104, 100], [105, 100], [106, 100], [107, 100], [108, 100], [109, 100], [110, 100], [111, 100], [112, 100], [113, 100], [114, 100], [115, 100], [116, 100], [117, 100], [118, 100], [119, 100], [120, 100], [121, 100], [122, 100], [123, 100], [124, 100], [125, 100], [126, 100], [127, 100]], [[5, 100], [0, 1], [12, 7]], [[5, 100], [1, 2], [13, 8]], [[5, 100], [2, 3], [14, 9]], [[5, 100], [3, 4], [15, 10]], [[5, 100], [4, 5], [16, 11]], [[5, 6], [17, 12]], [[5, 100], [6, 7], [18, 13]], [[5, 100], [7, 8], [19, 14]], [[5, 100], [8, 9], [20, 15]], [[5, 100], [9, 10], [21, 16]], [[5, 100], [10, 11], [22, 17]], [[5, 100], [11, 12], [23, 18]], [[5, 100], [12, 13], [24, 19]], [[5, 100], [13, 14], [25, 20]], [[5, 100], [14, 15], [26, 21]], [[5, 100], [15, 16], [27, 22]], [[5, 100], [16, 17], [28, 23]], [[5, 100], [17, 18], [29, 24]], [[5, 100], [18, 19], [30, 25]], [[5, 100], [19, 20], [31, 26]], [[5, 100], [20, 21], [32, 27]], [[5, 100], [21, 22], [33, 28]], [[5, 100], [22, 23], [34, 29]], [[5, 100], [23, 24], [35, 30]], [[5, 100], [24, 25], [36, 31]], [[5, 100], [25, 26], [37, 32]], [[5, 100], [26, 27], [38, 33]], [[5, 100], [27, 28], [39, 34]], [[5, 100], [28, 29], [40, 35]], [[5, 100], [29, 30], [41, 36]], [[5, 100], [30, 31], [42, 37]], [[5, 100], [31, 32], [43, 38]], [[5, 100], [32, 33], [44, 39]], [[5, 100], [33, 34], [45, 40]], [[5, 100], [34, 35], [46, 41]], [[5, 100], [35, 36], [47, 42]], [[5, 100], [36, 37], [48, 43]], [[5, 100], [37, 38], [49, 44]], [[5, 100], [38, 39], [50, 45]], [[5, 100], [39, 40], [51, 46]], [[5, 100], [40, 41], [52, 47]], [[5, 100], [41, 42], [53, 48]], [[5, 100], [42, 43], [54, 49]], [[5, 100], [43, 44], [55, 50]], [[5, 100], [44, 45], [56, 51]], [[5, 100], [45, 46], [57, 52]], [[5, 100], [46, 47], [58, 53]], [[5, 100], [47, 48], [59, 54]], [[5, 100], [48, 49], [60, 55]], [[5, 100], [49, 50], [61, 56]], [[5, 100], [50, 51], [62, 57]], [[5, 100], [51, 52], [63, 58]], [[5, 100], [52, 53], [64, 59]], [[5, 100], [53, 54], [65, 60]], [[5, 100], [54, 55], [66, 61]], [[5, 100], [55, 56], [67, 62]], [[5, 100], [56, 57], [68, 63]], [[5, 100], [57, 58], [69, 64]], [[5, 100], [58, 59], [70, 65]], [[5, 100], [59, 60], [71, 66]], [[5, 100], [60, 61], [72, 67]], [[5, 100], [61, 62], [73, 68]], [[5, 100], [62, 63], [74, 69]], [[5, 100], [63, 64], [75, 70]], [[5, 100], [64, 65], [76, 71]], [[5, 100], [65, 66], [77, 72]], [[5, 100], [66, 67], [78, 73]], [[5, 100], [67, 68], [79, 74]], [[5, 100], [68, 69], [80, 75]], [[5, 100], [69, 70], [81, 76]], [[5, 100], [70, 71], [82, 77]], [[5, 100], [71, 72], [83, 78]], [[5, 100], [72, 73], [84, 79]], [[5, 100], [73, 74], [85, 80]], [[5, 100], [74, 75], [86, 81]], [[5, 100], [75, 76], [87, 82]], [[5, 100], [76, 77], [88, 83]], [[5, 100], [77, 78], [89, 84]], [[5, 100], [78, 79], [90, 85]], [[5, 100], [79, 80], [91, 86]], [[5, 100], [80, 81], [92, 87]], [[5, 100], [81, 82], [93, 88]], [[5, 100], [82, 83], [94, 89]], [[5, 100], [83, 84], [95, 90]], [[5, 100], [84, 85], [96, 91]], [[5, 100], [85, 86], [97, 92]], [[5, 100], [86, 87], [98, 93]], [[5, 100], [87, 88], [99, 94]], [[5, 100], [88, 89], [100, 95]], [[5, 100], [89, 90], [101, 96]], [[5, 100], [90, 91], [102, 97]], [[5, 100], [91, 92], [103, 98]], [[5, 100], [92, 93], [104, 99]], [[5, 100], [93, 94], [105, 100]], [[5, 100], [94, 95], [106, 101]], [[5, 100], [95, 96], [107, 102]], [[5, 100], [96, 97], [108, 103]], [[5, 100], [97, 98], [109, 104]], [[5, 100], [98, 99], [110, 105]], [[5, 100], [99, 100], [111, 106]], [[5, 100], [100, 101], [112, 107]], [[5, 100], [101, 102], [113, 108]], [[5, 100], [102, 103], [114, 109]], [[5, 100], [103, 104], [115, 110]], [[5, 100], [104, 105], [116, 111]], [[5, 100], [105, 106], [117, 112]], [[5, 100], [106, 107], [118, 113]], [[5, 100], [107, 108], [119, 114]], [[5, 100], [108, 109], [120, 115]], [[5, 100], [109, 110], [121, 116]], [[5, 100], [110, 111], [122, 117]], [[5, 100], [111, 112], [123, 118]], [[5, 100], [112, 113], [124, 119]], [[5, 100], [113, 114], [125, 120]], [[5, 100], [114, 115], [126, 121]], [[5, 100], [115, 116], [127, 122]], [[5, 100], [116, 117]], [[5, 100], [117, 118]], [[5, 100], [118, 119]], [[5, 100], [119, 120]], [[5, 100], [120, 121]], [[5, 100], [121, 122]]])
