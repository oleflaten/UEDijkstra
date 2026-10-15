# UEDijkstra
Testing simple Dijkstra implementation in Unreal Engine.

**NB: Just a skeleton - algorithm not implemented yet**

# How to use
The class **DijNode** is the *nodes* in the graph, making a visual cube with a *Cost* number on it.

The class **DijSpawner**, spawns a grid of nodes, size set in it's *Rows* and *Columns* variables.
The DijSpawner also has a function *StepDijkstra()* where the algorithm should be run, one step at the time, so we can visualize how it progresses.
The time between each step is set with the *StepDelay* varialble.

When you run the program the nodes are spawned in a grid.

# Input
- LMB to set start
- RMB to set end
- MMB to make walls
- Space to start the stepping.

### Idea:
The color should tell the state of the Node
- White = unvisited
- Blue = visited
- Green = start node
- Red = end node
- Yellow = part of the final path, shown after a path is found
- Black = wall   set to cost = -1, you cannot traverse this node
