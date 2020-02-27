

# CS361 Lab 3
# Walker Mitchell

import sys

# Task 1: DP MCM
# This code is based on my class notes, the book's pseudocode, and https://www.geeksforgeeks.org/matrix-chain-multiplication-dp-8/

def dynamicMCM(chain, start, end):
    
    # Base case where multiplying one matrix has zero cost.
    if start == end:
        return 0
    
    # Initialize output
    cost = sys.maxsize
    
    # Do calculations
    for k in range(start, end):
        
        mult = (dynamicMCM(chain, start, k) + dynamicMCM(chain, k+1, end) + chain[start-1]*chain[k]*chain[end])
        
        # Keep lowest multiplication cost calculation
        if mult < cost:
            cost = mult
    
    # Return calculated multiplication cost
    return cost

# Generates a string of the parenthesized matrix chain, and returns it.
def dyParen(chain):
    outString = "("
    prev = 0
    next = 2
    openParens = [0 for i in range(len(chain)-1)] 
    closeParens = [0 for i in range(len(chain)-1)]
    while next < len(chain)-1:
        if chain[prev] > chain[next]:
            openParens[prev + 1] += 1
            closeParens[next] += 1
            next +=1
        else:
            openParens[prev] += 1
            closeParens[next - 1] += 1
            prev +=1
    
    for i in range(len(chain)-1):
        outString += "(" * openParens[i]
        outString += chr(65 + i)
        outString += ")" * closeParens[i]
        
    return outString + ")"
    
    
# Task 2: Memoized MCM
# This code is also based on my class notes, the book's pseudocode, and https://www.geeksforgeeks.org/printing-matrix-chain-multiplication-a-space-optimized-solution/
  
def memoMCM(p, n): 
  
    # Creating a matrix of order 
    # n*n in the memory. 
    m = [[0 for i in range(n)] for i in range (n)] 
  
    for l in range (2, n + 1): 
        for i in range (n - l + 1): 
            j = i + l - 1
  
            # Initializing infinity value. 
            m[i][j] = sys.maxsize 
            for k in range (i, j): 
                q = (m[i][k] + m[k + 1][j] + (p[i] * p[k + 1] * p[j + 1])); 
                if (q < m[i][j]): 
                    m[i][j] = q 
  
                    # Store k value in opposite index. 
                    m[j][i] = k + 1
    return m 
  
def memoParentheses(m, j, i ): 
  
    # Base Case
    if j == i: 
  
        # The first matrix is printed as 'A', next as 'B', and so on. The end clause here overrides the default newline print ending.
        print(chr(65 + j), end = "") 
        return
    
    # Recursive Step
    else: 
        print("(", end = "") 
  
        memoParentheses(m, m[j][i] - 1, i) 
  
        memoParentheses(m, j, m[j][i]) 
        print (")", end = "" ) 

# Task 5: Adjacency List BFS
# Based on class notes, the book's pseudocode, and https://www.geeksforgeeks.org/breadth-first-search-or-bfs-for-a-graph/

def listBFS(adjL, source):
    
    # Generate list to keep track of whether a vertex has been visited yet
    visited = [False] * (len(adjL))
    
    # Generate a list to keep track of the levels of each vertex in the tree being generated
    levels = [-1] * (len(adjL))
    
    # Generate a queue to perform BFS upon
    queue = []
    
    # Enqueue source node and mark it as visited
    queue.append(source)
    visited[source] = True
    levels[source] = 0
    
    print("List BFS: Vertices visited in the order")
    
    while queue != []:
        
        # Dequeue a vertex and print it
        vert = queue.pop(0)
        print(chr(65 + vert), end = ", ")
        
        # Get all adjacent vertices of the dequeued vertex. If an adjacent vertex 
        # hasn't been visited yet, mark it as visited and enqueue it.
        for i in adjL[vert]:
            if visited[i] == False:
                queue.append(i)
                visited[i] = True
                if (levels[i] == -1) or (levels[i] > levels[vert]+1):
                    levels[i] = levels[vert]+1
    
    print("")
    
    for i in range(max(levels)+1):
        print(" Level " + str(i) + " contains:", end = " ")
        for j in range(len(adjL)):
            if levels[j] == i:
                print(chr(65 + j), end = "  ")
        print("")
            
# Task 6: Adjacency Matrix BFS
def matrixBFS(adjM, source):
    
    L = len(adjM)
    
    # Generate list to keep track of whether a vertex has been visited yet
    visited = [False] * (len(adjM))
    
    # Generate a list to keep track of the levels of each vertex in the tree being generated
    levels = [-1] * (len(adjM))
    
    # Generate a queue to perform BFS upon
    queue = []
    
    # Enqueue source node and mark it as visited
    queue.append(source)
    visited[source] = True
    levels[source] = 0
    
    print("Matrix BFS: Vertices visited in the order")
    
    while queue != []:
        
        # Dequeue a vertex and print it
        vert = queue.pop(0)
        print(chr(65 + vert), end = ", ")
        
        # Get all adjacent vertices of the dequeued vertex. If an adjacent vertex 
        # hasn't been visited yet, mark it as visited and enqueue it.
        for i in range(L):
            if adjM[vert][i] == 1:
                if visited[i] == False:
                    queue.append(i)
                    visited[i] = True
                    if (levels[i] == -1) or (levels[i] > levels[vert]+1):
                        levels[i] = levels[vert]+1
    
    print("")
    
    for i in range(max(levels)+1):
        print(" Level " + str(i) + " contains:", end = " ")
        for j in range(L):
            if levels[j] == i:
                print(chr(65 + j), end = "  ")
        print("")

# DRIVER CODE TO FOLLOW

# Part 1: MCM

testChain = [30, 4, 8, 5, 10, 25, 15]

dyChain = testChain

print("") # Just making whitespace for formatting
print("DP MCM: ", dynamicMCM(dyChain, 1, len(dyChain)-1), " multiplications.")
print("Optimal Parenthesization is:", dyParen(dyChain))
print("") # Whitespace for formatting

memoChain = testChain
n = len(memoChain)-1  
m = memoMCM(memoChain, n)

print("Memoized MCM: ", m[0][n - 1], " multiplications.")
print("Optimal Parenthesization is: ", end = "") 

memoParentheses(m, n - 1, 0)  
print("\n\n") # Whitespace for formatting

# Part 2: BFS

# Hardcoded adjacency list from prompt graph
adjList = [[1,3],
           [0,2],
           [1,12,13],
           [0,4,5,6,12],
           [3,5],
           [3,4,7],
           [3,7,9],
           [5,6,8,10],
           [7,9],
           [6,8,10,13],
           [7,9,11],
           [10,12],
           [2,11,13],
           [2,3,9,12]]


# Use adjacency list to generate adjacency matrix
adjMatrix = [[0 for i in range(len(adjList))] for i in range(len(adjList))]

for i in range(len(adjList)):
    for j in range(len(adjList[i])):
        adjMatrix[i][adjList[i][j]] = 1


# Make sure the adjacency matrix filled correctly
#for i in range(len(adjMatrix)):
#    print(adjMatrix[i])

# Perform BFS on adjacency list with A as the source vertex
listBFS(adjList, 0)

print("") # Whitespace for formatting

# Perform BFS on adjacency matrix with A as the source vertex
matrixBFS(adjMatrix, 0)




