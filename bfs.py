# graph is in adjacent list representation
graph = {
        '1': ['2', '3', '4'],
        '2': ['5', '6'],
        '5': ['9', '10'],
        '4': ['7', '8'],
        '7': ['11', '12']
        }

def getIndex(x, y, width):
    return x + y*width

def getIndexStr(x, y, width):
    return '%d' % (x + y*width)

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
        adjacencies.append(getIndexStr(x - 1, y - 1, width))
        adjacencies.append(getIndexStr(x + 0, y - 1, width))
        adjacencies.append(getIndexStr(x + 1, y - 1, width))

        adjacencies.append(getIndexStr(x - 1, y, width))
        adjacencies.append(getIndexStr(x + 1, y, width))

        adjacencies.append(getIndexStr(x - 1, y + 1, width))
        adjacencies.append(getIndexStr(x + 0, y + 1, width))
        adjacencies.append(getIndexStr(x + 1, y + 1, width))


    return adjacencies


def generateGraph(size):
    width = size
    height = size

    graph = {}

    for x in range(0, width):
        for y in range(0, height):
            index_str = getIndexStr(x, y, width)
            graph[index_str] = getAdjacencies(x, y, width, height)

    return graph

def bfs(graph, start, end):
    # maintain a queue of paths
    queue = []
    # push the first path into the queue
    queue.append([start])
    while queue:
        # get the first path from the queue
        path = queue.pop(0)
        # get the last node from the path
        node = path[-1]
        # path found
        if node == end:
            return path
        # enumerate all adjacent nodes, construct a new path and push it into the queue
        for adjacent in graph.get(node, []):
            new_path = list(path)
            new_path.append(adjacent)
            queue.append(new_path)

print bfs(graph, '1', '11')

big_graph = generateGraph(512)
big_result = bfs(big_graph, '514', '516')
