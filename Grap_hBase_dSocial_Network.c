#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 8
#define INF 99999999  // Infinite distance for BFS

// -------------------- STRUCTURE DEFINITIONS --------------------

typedef struct {
    int id;
    char name[20];
} User;

typedef struct {
    int num_users;
    int adj_matrix[MAX_USERS][MAX_USERS];
    User users[MAX_USERS];
} Graph;

// -------------------- GRAPH INITIALIZATION --------------------

void initialize_graph(Graph *g, User users_list[], int count) {
    if (count > MAX_USERS) {
        printf("Error: Too many users! Max allowed = %d\n", MAX_USERS);
        g->num_users = 0;
        return;
    }

    g->num_users = count;

    // Initialize adjacency matrix
    for (int i = 0; i < count; i++)
        for (int j = 0; j < count; j++)
            g->adj_matrix[i][j] = 0;

    // Copy user info
    for (int i = 0; i < count; i++)
        g->users[i] = users_list[i];

    printf("Graph initialized with %d users.\n", count);
}

void add_relationship(Graph *g, int from_id, int to_id) {
    if (from_id >= 0 && from_id < g->num_users && to_id >= 0 && to_id < g->num_users)
        g->adj_matrix[from_id][to_id] = 1;
    else
        printf("Invalid relationship between %d and %d.\n", from_id, to_id);
}

// -------------------- DEGREE CENTRALITY --------------------

void calculate_degree_centrality(Graph *g) {
    printf("\n--- DEGREE CENTRALITY (Influence Analysis) ---\n");
    printf("User\t\tIn-Degree\tOut-Degree\tTotal\n");
    printf("---------------------------------------------\n");

    int max_degree = -1, top_user = -1;

    for (int i = 0; i < g->num_users; i++) {
        int in_deg = 0, out_deg = 0;

        for (int j = 0; j < g->num_users; j++) {
            if (g->adj_matrix[j][i]) in_deg++;
            if (g->adj_matrix[i][j]) out_deg++;
        }

        int total = in_deg + out_deg;
        printf("%-10s\t%d\t\t%d\t\t%d\n", g->users[i].name, in_deg, out_deg, total);

        if (total > max_degree) {
            max_degree = total;
            top_user = i;
        }
    }

    printf("\nMost Influential User: %s (Total Degree: %d)\n",
           g->users[top_user].name, max_degree);
}

// -------------------- BREADTH FIRST SEARCH --------------------

void breadth_first_search(Graph *g, int start_id, int target_id) {
    if (start_id == target_id) {
        printf("\nStart and target are the same: %s\n", g->users[start_id].name);
        return;
    }

    printf("\n--- SHORTEST PATH (%s → %s) ---\n",
           g->users[start_id].name, g->users[target_id].name);

    int distance[MAX_USERS], parent[MAX_USERS], queue[MAX_USERS];
    int front = 0, rear = 0;

    for (int i = 0; i < g->num_users; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }

    distance[start_id] = 0;
    queue[rear++] = start_id;
    int found = 0;

    while (front < rear) {
        int u = queue[front++];
        if (u == target_id) { found = 1; break; }

        for (int v = 0; v < g->num_users; v++) {
            if (g->adj_matrix[u][v] && distance[v] == INF) {
                distance[v] = distance[u] + 1;
                parent[v] = u;
                queue[rear++] = v;
            }
        }
    }

    if (found) {
        printf("Path found! Distance = %d\nPath: ", distance[target_id]);
        int path[MAX_USERS], len = 0, curr = target_id;

        while (curr != -1) {
            path[len++] = curr;
            curr = parent[curr];
        }

        for (int i = len - 1; i >= 0; i--) {
            printf("%s", g->users[path[i]].name);
            if (i > 0) printf(" -> ");
        }
        printf("\n");
    } else {
        printf("No path exists between %s and %s.\n",
               g->users[start_id].name, g->users[target_id].name);
    }
}

// -------------------- MAIN PROGRAM --------------------

int main() {
    User users[MAX_USERS] = {
        {0, "arijit"}, {1, "arge"}, {2, "alim"}, {3, "anirudda"},
        {4, "argha"}, {5, "daverup"}, {6, "prakar"}, {7, "prithu"}
    };

    Graph network;
    initialize_graph(&network, users, MAX_USERS);

    // Relationships (directed)
    add_relationship(&network, 0, 1);
    add_relationship(&network, 0, 2);
    add_relationship(&network, 1, 3);
    add_relationship(&network, 2, 3);
    add_relationship(&network, 3, 0);
    add_relationship(&network, 3, 5);
    add_relationship(&network, 4, 5);
    add_relationship(&network, 5, 6);
    add_relationship(&network, 6, 7);
    add_relationship(&network, 7, 6);
    add_relationship(&network, 2, 4);

    // Analyses
    calculate_degree_centrality(&network);
    breadth_first_search(&network, 0, 5);
    breadth_first_search(&network, 3, 7);
    breadth_first_search(&network, 0, 0);
    breadth_first_search(&network, 7, 0);

    return 0;
}
