#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Define the maximum number of users the network can handle.
#define MAX_USERS 8 
#define INF 99999999 // Represents infinity for BFS distance

// --- 1. Data Structure Definitions ---

// Structure to hold information about a user (node in the graph).
typedef struct {
    int id;
    char name[20];
} User;

// Structure for the Graph, using an Adjacency Matrix representation.
// graph[i][j] = 1 means there is a relationship from User i to User j.
typedef struct {
    int num_users;
    int adj_matrix[MAX_USERS][MAX_USERS];
    User users[MAX_USERS];
} Graph;

// --- 2. Graph Initialization and Setup Functions ---

/**
 * @brief Initializes the graph structure.
 * Sets the adjacency matrix to all zeros and populates the user list.
 * @param g A pointer to the Graph structure.
 * @param users_list An array of User structures for the network.
 * @param count The number of users in the network.
 */
void initialize_graph(Graph* g, User users_list[], int count) {
    if (count > MAX_USERS) {
        printf("Error: Too many users defined. Max is %d.\n", MAX_USERS);
        g->num_users = 0;
        return;
    }

    g->num_users = count;
    // 1. Initialize Adjacency Matrix (no relationships yet)
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            g->adj_matrix[i][j] = 0;
        }
    }

    // 2. Populate the User list
    for (int i = 0; i < count; i++) {
        g->users[i] = users_list[i];
    }

    printf("Graph initialized with %d users.\n", count);
}

/**
 * @brief Adds a directed relationship (edge) between two users.
 * This simulates a 'Follows' or 'Sends Message To' relationship.
 * @param g A pointer to the Graph structure.
 * @param from_id The ID of the originating user.
 * @param to_id The ID of the target user.
 */
void add_relationship(Graph* g, int from_id, int to_id) {
    if (from_id >= 0 && from_id < g->num_users && to_id >= 0 && to_id < g->num_users) {
        // Since we are using IDs 0..N-1, they directly map to the matrix index.
        g->adj_matrix[from_id][to_id] = 1;
    } else {
        printf("Warning: Invalid user ID for adding relationship.\n");
    }
}

// --- 3. Social Network Analysis Algorithms ---

/**
 * @brief Calculates the Degree Centrality for all users.
 * Degree Centrality is simply the number of connections a node has.
 * In a directed graph, we often look at IN-Degree and OUT-Degree.
 * This function calculates the total degree (IN + OUT).
 * @param g A pointer to the Graph structure.
 */
void calculate_degree_centrality(Graph* g) {
    printf("\n--- 1. Degree Centrality (Influence Analysis) ---\n");
    printf("User\t\tIn-Degree\tOut-Degree\tTotal Degree\n");
    printf("------------------------------------------------------------\n");

    int max_degree = -1;
    int influential_user_id = -1;

    for (int i = 0; i < g->num_users; i++) {
        int in_degree = 0;  // Number of users connected TO User i
        int out_degree = 0; // Number of users User i connects TO

        // Calculate In-Degree (look down the column i)
        for (int j = 0; j < g->num_users; j++) {
            if (g->adj_matrix[j][i] == 1) {
                in_degree++;
            }
        }

        // Calculate Out-Degree (look across the row i)
        for (int j = 0; j < g->num_users; j++) {
            if (g->adj_matrix[i][j] == 1) {
                out_degree++;
            }
        }

        int total_degree = in_degree + out_degree;

        // Keep track of the most connected user
        if (total_degree > max_degree) {
            max_degree = total_degree;
            influential_user_id = i;
        }

        printf("%s\t\t%d\t\t%d\t\t%d\n", g->users[i].name, in_degree, out_degree, total_degree);
    }

    printf("\n--> Most Influential Node (highest Total Degree): %s (Total Degree: %d)\n",
           g->users[influential_user_id].name, max_degree);
}

/**
 * @brief Performs Breadth-First Search (BFS) to find the shortest path between two users.
 * BFS is ideal for finding shortest path in unweighted graphs (like a social network).
 * The path length represents the 'distance' or 'degree of separation'.
 * @param g A pointer to the Graph structure.
 * @param start_id The ID of the starting user.
 * @param target_id The ID of the target user.
 */
void breadth_first_search(Graph* g, int start_id, int target_id) {
    if (start_id == target_id) {
        printf("Start and target are the same user: %s\n", g->users[start_id].name);
        return;
    }

    printf("\n--- 2. Shortest Path / Relationship Distance (%s -> %s) ---\n", 
           g->users[start_id].name, g->users[target_id].name);

    // Array to store the distance from the start node
    int distance[MAX_USERS];
    // Array to store the predecessor of each node in the path (used to reconstruct path)
    int parent[MAX_USERS];
    // Queue for BFS traversal
    int queue[MAX_USERS];
    int front = 0, rear = 0;

    // Initialization
    for (int i = 0; i < g->num_users; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }

    // Start node setup
    distance[start_id] = 0;
    queue[rear++] = start_id;

    int found = 0;

    while (front < rear) {
        int u = queue[front++]; // Dequeue the current node (user)

        if (u == target_id) {
            found = 1;
            break;
        }

        // Explore neighbors of u (v)
        for (int v = 0; v < g->num_users; v++) {
            // Check if there is an edge (u -> v) AND v has not been visited (distance[v] == INF)
            if (g->adj_matrix[u][v] == 1 && distance[v] == INF) {
                distance[v] = distance[u] + 1; // Distance increases by 1
                parent[v] = u;                 // Record u as the parent (predecessor) of v
                queue[rear++] = v;             // Enqueue the neighbor
            }
        }
    }

    // Output the result
    if (found) {
        printf("Path found! Distance (Degrees of Separation): %d\n", distance[target_id]);
        
        // Path Reconstruction
        printf("Path: ");
        int path[MAX_USERS];
        int path_len = 0;
        int curr = target_id;

        while (curr != -1) {
            path[path_len++] = curr;
            curr = parent[curr];
        }

        // Print path in correct order (reverse of how we stored it)
        for (int i = path_len - 1; i >= 0; i--) {
            printf("%s", g->users[path[i]].name);
            if (i > 0) {
                printf(" -> ");
            }
        }
        printf("\n");
    } else {
        printf("No path exists from %s to %s.\n", g->users[start_id].name, g->users[target_id].name);
    }
}


// --- 4. Main Program and Simulation ---

int main() {
    // 1. Define the social network users
    // ID 0..7
    User social_users[MAX_USERS] = {
        {0, "arijit"},
        {1, "arge "},
        {2, "alim"},
        {3, "anirudda"},
        {4, "argha"},
        {5, "daverup"},
        {6, "prakar"},
        {7, "prithu"}
    };
    int user_count = MAX_USERS;

    // 2. Initialize the graph
    Graph social_network;
    initialize_graph(&social_network, social_users, user_count);

    // 3. Define the relationships (Edges)
    // NOTE: This is a directed network (A -> B means A follows B)
    add_relationship(&social_network, 0, 1); // Alice -> Bob
    add_relationship(&social_network, 0, 2); // Alice -> Charlie
    add_relationship(&social_network, 1, 3); // Bob -> David
    add_relationship(&social_network, 2, 3); // Charlie -> David
    add_relationship(&social_network, 3, 0); // David -> Alice (Forms a closed loop)
    add_relationship(&social_network, 3, 5); // David -> Frank
    add_relationship(&social_network, 4, 5); // Eve -> Frank
    add_relationship(&social_network, 5, 6); // Frank -> Grace
    add_relationship(&social_network, 6, 7); // Grace -> Henry
    add_relationship(&social_network, 7, 6); // Henry -> Grace (Mutual)
    add_relationship(&social_network, 2, 4); // Charlie -> Eve

    // --- EXECUTE ANALYSES ---

    // A. Analyze Influence: Calculate Degree Centrality
    // This tells us who is the most active and/or popular user.
    calculate_degree_centrality(&social_network);

    // B. Analyze Relationships: Find the shortest path (distance)

    // Example 1: Find the distance from Alice (0) to Frank (5)
    // Path: Alice -> Charlie -> Eve -> Frank (Distance 3)
    breadth_first_search(&social_network, 0, 5); 

    // Example 2: Find the distance from David (3) to Henry (7)
    // Path: David -> Frank -> Grace -> Henry (Distance 3)
    breadth_first_search(&social_network, 3, 7); 

    // Example 3: Find a path in the closed loop
    // Path: Alice -> Bob -> David -> Alice (Distance 3)
    breadth_first_search(&social_network, 0, 0); 
    
    // Example 4: Attempt to find a path to an unconnected part of the network
    // Henry (7) has no outgoing links to Alice (0)
    breadth_first_search(&social_network, 7, 0);

    return 0;
}