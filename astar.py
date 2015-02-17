from Queue import PriorityQueue
import time

def getIndex(x, y, width):
    return x + y*width

def getIndexStr(x, y, width):
    return '%d' % (getIndex(x, y, width))

def getAdjacencies(x, y, width, height):
    adjacencies = []

    is_upper_left_corner  = ((x == 0) and (y == 0))
    is_lower_left_corner  = ((x == 0) and (y == height - 1))
    is_upper_right_corner = ((x == width - 1) and (y == 0))
    is_lower_right_corner = ((x == width - 1) and (y == height - 1))
    is_x_edge = (x == 0) or (x == width - 1)
    is_y_edge = (y == 0) or (y == height - 1)

    if is_upper_left_corner:
        adjacencies.append(getIndexStr(x + 1, y, width))
        adjacencies.append(getIndexStr(x, y + 1, width))

    elif is_lower_left_corner:
        adjacencies.append(getIndexStr(x + 1, y, width))
        adjacencies.append(getIndexStr(x, y - 1, width))

    elif is_upper_right_corner:
        adjacencies.append(getIndexStr(x - 1, y, width))
        adjacencies.append(getIndexStr(x, y + 1, width))

    elif is_lower_right_corner:
        adjacencies.append(getIndexStr(x - 1 , y, width))
        adjacencies.append(getIndexStr(x, y - 1, width))

    elif is_x_edge:
        adjacencies.append(getIndexStr(x, y - 1, width))
        adjacencies.append(getIndexStr(x, y + 1, width))

    elif is_y_edge:
        adjacencies.append(getIndexStr(x - 1, y, width))
        adjacencies.append(getIndexStr(x + 1, y, width))

    else:
        adjacencies.append(getIndexStr(x, y - 1, width))

        adjacencies.append(getIndexStr(x - 1, y, width))
        adjacencies.append(getIndexStr(x + 1, y, width))

        adjacencies.append(getIndexStr(x, y + 1, width))

    return adjacencies

class Graph:

    def __init__(self, size):

        width = size
        height = size

        self.nodes = {}
        self.edges = {}

        for x in range(0, width):
            for y in range(0, height):
                index_str = getIndexStr(x, y, width)
                self.nodes[index_str] = (x, y)
                self.edges[index_str] = getAdjacencies(x, y, width, height)

    def __str__(self):
        output = ""
        for edge in self.edges:
            output += "'%s': %s\n" % (edge, self.edges[edge])

        return output

def heuristic(a, b):
    (x1, y1) = a
    (x2, y2) = b
    return abs(x1 - x2) + abs(y1 - y2)

def reconstruct_path(came_from, current):
    total_path = [current]
    while current in came_from:
        # print "Current = %s" % came_from[current]
        current = came_from[current]
        total_path.append(current)

    total_path.reverse()

    return total_path

def a_star(graph, start, goal_id):
    # Unoptimized A* for proof of concept
    frontier = PriorityQueue()
    frontier.put((0, start))
    visited = []
    came_from = {}

    goal_node = graph.nodes[goal_id]
    steps = 0
    while not frontier.empty():
        steps += 1
        # Get the node with the lowest cost
        current = frontier.get()
        node_id = current[1]
        visited.append(node_id)
        # If this node is the goal_id, finish
        if node_id == goal_id:
            print "Found path from %s to %s in %d steps!" % (start, goal_id, steps)
            return reconstruct_path(came_from, goal_id)

        # Otherwise add all of the neighbors that haven't
        # been visited to the frontier.
        for neighbor_id in graph.edges[node_id]:
            neighbor_node = graph.nodes[neighbor_id]
            cost = heuristic(neighbor_node, goal_node)
            if neighbor_id not in visited:
                came_from[neighbor_id] = node_id
                frontier.put((cost, neighbor_id))

    print "Steps: %d" % steps

    return path

t0 = time.clock()
graph = Graph(512)
t1 = time.clock()
print "Initialized graph in %f seconds." % (t1 - t0)

a = getIndexStr(2, 3, 512)
b = getIndexStr(511, 511, 512)

t0 = time.clock()
path = a_star(graph, a, b)
t1 = time.clock()
print "Took %f seconds to run the search." % (t1 - t0)
print path
