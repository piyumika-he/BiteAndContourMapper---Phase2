# BiteAndContourMapper---Phase2
Expanding upon the foundation laid in phase1, this project introduces dynamic maze construction with connections and advanced pathfinding capabilities. Using Qt and C++, it explores algorithms for shortest paths, minimal bites, and uphill traversal in a grid-based visualization of bites and contours.

# Project Overview
In this phase, the phase---1 system is expanded to include:
  Dynamic Maze Construction: Connections link bites to create pathways within the grid-based maze.
  Pathfinding Algorithms: Implemented functions include:
  Breadth-First Search (BFS): Used in the path_any and path_least_bites functions. It explores the maze level by level, finding any valid path or the one with the fewest bites.
  Dijkstra's Algorithm: Used in path_shortest. It employs a priority queue to find the shortest path by expanding the least costly paths first, optimizing based on distance.
  Custom Hills Minimization (Modified Dijkstra): Used in path_least_uphill. It modifies Dijkstra's algorithm to prioritize paths with fewer uphill climbs, considering terrain height.

Each algorithm uses a different strategy to explore paths, either prioritizing distance, number of intermediate points, or minimizing elevation gain.

# Key Features
Connections and Paths
Establish direct or meandering connections between bites while adhering to constraints like no diagonal paths.
Visualize connections based on the contours or ground level.
Calculate path lengths using the Manhattan distance.

Advanced Pathfinding
Support for shortest paths, minimal intermediate nodes (bites), and paths minimizing uphill sections.
Handles loops, cycle checks, and custom rules for paths across varied terrains.

Algorithmic Optimization
Efficient handling of graph traversal and pathfinding using suitable STL containers and algorithms.
Designed with performance optimization in mind to handle large datasets.

Data Management Enhancements
Added functions to manage connections, including clearing all connections and retrieving paths.
Supports bite removal with automatic cleanup of associated connections.

Technologies Used
Programming Language: C++17
Libraries: Qt framework for visualization, STL for optimized data handling
Algorithms: Graph traversal, Manhattan distance calculations, and pathfinding heuristics

Grade
Achieved a top grade (5/5) in the module for demonstrating advanced concepts and optimization techniques.
