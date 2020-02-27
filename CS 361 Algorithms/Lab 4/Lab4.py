


# CS361 Lab 4
# Walker Mitchell

import sys

# Task 1: DFA
# For this task my sources are the textbook and my class notes.

def DFA(checkString):
    
    states = ("S", "Q1", "Q2", "R1", "R2")  # This is Q
    
    currentState = states[0]                # This is q1
    
    for i in range(0, len(checkString)):    # Process the string according to given table
        if checkString[i] not in ['a','b']: # If string has a character not in DFA's alphabet, reject it.
            return False
        
        if checkString[i] == 'a' and currentState in states[:3]:
            currentState = states[1]
        
        if checkString[i] == 'a' and currentState in states[3:]:
            currentState = states[4]

        if checkString[i] == 'b' and (currentState == states[0] or currentState in states[3:]):
            currentState = states[3]

        if checkString[i] == 'b' and currentState in states[1:3]:
            currentState = states[2]
    
    if currentState == states[1] or currentState == states[3]:  # Checks if DFA is in an accept state after processing string
        return True
        
    else: 
        return False
    
 
# Task 3: Bellman-Ford
# For this task my sources are class notes and the textbook.

def bellmanFord(adjList, sourceVert):
    
    # Initialize Single-Source
    predecessors = [None for i in range(len(adjList))]
    distances = [sys.maxsize for i in range(len(adjList))]
    distances[sourceVert] = 0
    
    # Actual Bellman-Ford logic
    for i in range(len(adjList)-1):
        for j in range(len(adjList)):
            for k in range(len(adjList[j])):
                relax(adjList, distances, predecessors, j, k)
    
    # Final check for negative weight cycle
    negCycle = False
    for j in range(len(adjList)):
            for k in range(len(adjList[j])):
                if distances[adjList[j][k][0]] > distances[j] + adjList[j][k][1]:
                    negCycle = True
    
    # Print distances, predecessors, and whether or not a negative weight cycle was found.
    print("Using vertex " + chr(65 + sourceVert) + " as the source vertex:")
    
    for i in range(len(adjList)):
        if(predecessors[i] == None):
            print("Vertex " + chr(65 + i) + " has distance " + str(distances[i]) + " and it has no parent.")
        else:
            print("Vertex " + chr(65 + i) + " has distance " + str(distances[i]) + 
                  " and its parent is vertex " + chr(65 + predecessors[i]))
    
    if(negCycle == False):
        print("This graph has no negative weight cycle.")
    else:
        print("A negative weight cycle was found.")
    
def relax(adjList, dist, pred, startVert, edgeIndex):
    endVert = adjList[startVert][edgeIndex][0]          # Pull edge's destination vertex out of adjacency list
    edgeWeight = adjList[startVert][edgeIndex][1]       # Pull edge's weight out of adjacency list
    if dist[endVert] > dist[startVert] + edgeWeight:
        dist[endVert] = dist[startVert] + edgeWeight
        pred[endVert] = startVert

# DRIVER CODE TO FOLLOW

print("Task 2: Testing DFA code")

testString = "ababa"
print("Testing the string " + testString + ":")
print(DFA(testString))

testString = "baba"
print("Testing the string " + testString + ":")
print(DFA(testString))

testString = "aababaab"
print("Testing the string " + testString + ":")
print(DFA(testString))

testString = "babaaabaaabb"
print("Testing the string " + testString + ":")
print(DFA(testString))

testString = ""
print("Testing the empty string:")
print(DFA(testString))



print("\nTask 4: Testing Bellman-Ford code")

# Each tuple represents an outgoing edge, with the first element being the 
# destination vertex and the second element being the weight of the edge.
testGraphAdjList = [[(3,3)],                            # A
                    [(0,-2)],                           # B
                    [(1,1), (12,3)],                    # C
                    [(4,2), (5,6), (6,-1), (13,-1)],    # D
                    [(5,3)],                            # E
                    [(7,-2)],                           # F
                    [(7,1), (9,3)],                     # G
                    [(10,-1)],                          # H
                    [(7,-4)],                           # I
                    [(8,2), (10,3)],                    # J
                    [],                                 # K
                    [(10,2)],                           # L
                    [(11,-4)],                          # M
                    [(2,-3), (9,5), (12,8)]]            # N

bellmanFord(testGraphAdjList, 0)

