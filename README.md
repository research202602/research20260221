# Paired 2-Disjoint Path Cover Enumeration and Verification

This project enumerates a specific class of graph structures (three complete subgraphs connected by perfect matchings) and verifies whether, for every choice of four distinct vertices `(s1, t1, s2, t2)`, there exist two vertex-disjoint paths `s1→t1` and `s2→t2` that together cover all vertices of the graph (i.e., they form a **paired 2-disjoint path cover**).

## Background

This verification is motivated by research on the fault tolerance of **BCube networks** under the partitioned edge fault model. For the case `k=2, n=3`, BCube(3,0) consists of three complete graphs K₃, with each pair of subgraphs connected by a perfect matching. This program enumerates all possible combinations of matchings (a total of `6×6×6 = 216` graphs) and checks whether each graph satisfies the required path cover property.

## Features

- Generates all possible graph structures: each subgraph is a complete graph internally, and between subgraphs a random perfect matching (permutation) is chosen.
- For each graph, enumerates all possible quadruples `(s1, t1, s2, t2)` (all distinct).
- Uses depth‑first search (DFS) to attempt to find two vertex‑disjoint paths that cover all vertices.
- If a counterexample is found, it immediately outputs the edge set of the graph and the failing quadruple; if all graphs pass the verification, it prints a success message.

## Compilation and Execution

### Requirements

- A C++11 (or later) compiler (e.g., g++, clang)
- Standard library headers: `<iostream>`, `<vector>`, `<algorithm>`, `<bitset>`, `<set>`, `<queue>`, `<functional>`

### Compile

```bash
g++ -std=c++11 -O2 main.cpp -o verify
```

### Run

```bash
./verify
```

The program redirects its output to a file named `2-dpc.out` (as set by `freopen` in the code). You can also run it without redirection to see the output on the terminal.

**Note**: Because the program enumerates all quadruples and performs DFS for each, the runtime may be long (especially given the number of graphs). It is recommended to run it on a reasonably fast machine and be patient.

## Output Description

- The program first prints the total number of generated graphs (should be 216).
- It then verifies each graph one by one, printing progress.
- If a graph passes verification, it outputs `"Valid"`. If a counterexample is found, it immediately prints the details (the four vertex indices and the edges of the graph) and stops.
- If all graphs pass, it prints a final confirmation that the theorem holds.

### Example Output (partial)

```
Generating all possible graphs...
Total graphs: 216
Verifying graph 1/216... success to found!
s1=0, t1=1, s2=2, t2=3
Graph edges:
0 - 1
0 - 2
...
p1:
0->...->1
p2:
2->...->3
Valid
Verifying graph 2/216... Valid
...
```

## Code Structure Overview

- `Graph` structure: stores an adjacency matrix using `bitset<N>`, with methods to add edges, check adjacency, and get neighbors.
- `generate_all_graphs()`: enumerates all matching combinations and constructs the 216 graphs.
- `find_two_paths()`: given four vertices, tries to find two vertex‑disjoint paths covering all vertices.
- `verify_graph()`: iterates over all quadruples for a single graph, calling `find_two_paths` for each.
- `main()`: generates all graphs and verifies them sequentially.
