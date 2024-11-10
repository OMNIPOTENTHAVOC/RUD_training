import pygame
import heapq

# Initialize Pygame
pygame.init()

# Screen dimensions
WIDTH, HEIGHT = 600, 600
ROWS, COLS = 20, 20  # Grid size
CELL_SIZE = WIDTH // COLS

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
GRAY = (200, 200, 200)

# Set up display
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("D* Lite Pathfinding")

# Node class
class Node:
    def _init_(self, x, y):
        self.x = x
        self.y = y
        self.g = float('inf')
        self.rhs = float('inf')
        self.cost = 1
        self.parent = None
        self.is_obstacle = False
    
    def draw(self, color):
        pygame.draw.rect(screen, color, (self.y * CELL_SIZE, self.x * CELL_SIZE, CELL_SIZE, CELL_SIZE))
        pygame.draw.rect(screen, BLACK, (self.y * CELL_SIZE, self.x * CELL_SIZE, CELL_SIZE, CELL_SIZE), 1)

# Helper functions
def h(n, goal):
    return abs(n.x - goal.x) + abs(n.y - goal.y)

def k(n, goal):
    return (min(n.g, n.rhs) + h(n, goal), min(n.g, n.rhs))

def neighbors(n):
    nbs = []
    for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
        nx, ny = n.x + dx, n.y + dy
        if 0 <= nx < ROWS and 0 <= ny < COLS:
            nbs.append(grid[nx][ny])
    return nbs

def update_v(n, goal):
    if n != goal:
        n.rhs = min(get_cost(nb) + nb.g for nb in neighbors(n))
    if n.g != n.rhs:
        heapq.heappush(open_set, (k(n, goal), n))

def d_star(start, goal):
    start.rhs = 0
    heapq.heappush(open_set, (k(start, goal), start))
    
    while open_set:
        curr = heapq.heappop(open_set)[1]

        if curr.g == curr.rhs:
            if curr == goal:
                return path(goal)

            for nb in neighbors(curr):
                update_v(nb, goal)
        else:
            curr.g = curr.rhs
            for nb in neighbors(curr):
                update_v(nb, goal)
    
    return None

def get_cost(n):
    return n.cost

def path(goal_node):
    p = []
    current = goal_node
    while current:
        p.append((current.x, current.y))
        current = current.parent
    return p[::-1]

def draw_grid():
    for row in range(ROWS):
        for col in range(COLS):
            grid[row][col].draw(WHITE if not grid[row][col].is_obstacle else BLACK)

def draw_path(path):
    for (x, y) in path:
        grid[x][y].draw(GREEN)

# Define the grid and nodes
grid = [[Node(x, y) for y in range(COLS)] for x in range(ROWS)]
start_node = grid[0][0]
goal_node = grid[ROWS - 1][COLS - 1]
start_node.g = start_node.rhs = 0
open_set = []

# Main loop
def main_loop():
    running = True
    while running:
        screen.fill(WHITE)
        draw_grid()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

            # Place obstacle with mouse click
            if pygame.mouse.get_pressed()[0]:
                pos = pygame.mouse.get_pos()
                x, y = pos[1] // CELL_SIZE, pos[0] // CELL_SIZE
                grid[x][y].is_obstacle = True
                grid[x][y].cost = float('inf')  # Infinite cost for obstacles

        # Compute path
        path_result = d_star(start_node, goal_node)
        if path_result:
            draw_path(path_result)

        pygame.display.flip()

# Start the main loop
main_loop()
pygame.quit()
