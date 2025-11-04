# graph-base-social-network-analysis
A C program that models a social network using a graph and analyzes relationships using degree centrality and shortest path (BFS).
# 🧩 Social Network Graph Analysis in C

This project simulates a simple **social network** using **graph data structures** in C.  
Each user is represented as a node, and directed relationships (like "follows" or "sends message to") are represented as edges between nodes.  
The program performs **social network analysis** using graph algorithms such as **Degree Centrality** and **Breadth-First Search (BFS)**.

---

## 🚀 Features

- **Graph Representation:**  
  Users and their relationships are stored using an **adjacency matrix**.

- **Degree Centrality Calculation:**  
  Finds the **most influential user** based on incoming and outgoing connections.

- **Shortest Path (BFS):**  
  Uses **Breadth-First Search** to find the shortest connection path (degrees of separation) between two users.

- **Custom Network Setup:**  
  You can modify user names and relationships to test your own network structure.

---

## 🧮 Algorithms Used

| Function | Purpose |
|-----------|----------|
| `calculate_degree_centrality()` | Calculates each user's influence (In-degree + Out-degree) |
| `breadth_first_search()` | Finds the shortest path between two users using BFS |
| `add_relationship()` | Creates a directed edge between users |
| `initialize_graph()` | Builds the network with all users and empty relationships |

---

## 🧑‍💻 Example Output

