#include <stdlib.h>
#include <stdio.h>
 
typedef struct {
    unsigned int first;
    unsigned int second;
    unsigned int weight;
} weighted_edge;
 
/* Check if the tour already contains an edge */
static unsigned int tour_contains(const weighted_edge *tour, unsigned int t, 
        const weighted_edge *edge)
{
    unsigned int contains = 0;
    unsigned int i;
    for (i = 0; i < t && !contains; i++) {
        contains = tour[i].first == edge->first 
            && tour[i].second == edge->second;
    }
    return contains;
}
 
/* Find the edge to v's nearest neighbour not in the tour already */
static unsigned int nearest_neighbour_edge(const weighted_edge *edges, unsigned int size, 
        const weighted_edge *tour, unsigned int t, unsigned int v)
{
    unsigned int min_distance = 0;
    unsigned int nearest_neighbour;
    unsigned int i;
    for (i = 0; i < size; i++) {
        if ((edges[i].first == v || edges[i].second == v)
                && (min_distance == 0 || edges[i].weight < min_distance)
                && !tour_contains(tour, t, &edges[i]))
        {
            min_distance = edges[i].weight;
            nearest_neighbour = i;
        }
    }
    return nearest_neighbour;
}
 
weighted_edge *nearest_neighbour_tsp(const weighted_edge *edges, unsigned int size,
        unsigned int order)
{
    unsigned int t, v = 0;
    weighted_edge *tour = malloc(order * sizeof(weighted_edge));
    if (tour == NULL) {
        return NULL;
    }
    for (t = 0; t < order; t++) {
        unsigned int e = nearest_neighbour_edge(edges, size, tour, t, v);
        tour[t] = edges[e];
        v = edges[e].first == v ? edges[e].second : edges[e].first;
    }
    return tour;
}

/* Connect two edges */
void weighted_edge_connect(weighted_edge *edges, unsigned int first, unsigned int second,
        unsigned int weight, unsigned int *pos)
{
    edges[*pos].first = first;
    edges[*pos].second = second;
    edges[*pos].weight = weight;
    (*pos)++;
}

void print_edges(const weighted_edge *edges, unsigned int n)
{
    unsigned int e;
    for (e = 0; e < n; e++) {
        printf("(%u, %u, %u) ", edges[e].first, edges[e].second, edges[e].weight);
    }
    putchar('\n');
}

int main(void)
{
    unsigned int i = 0;
    const unsigned int size = 20; /* Edges */
    const unsigned int order = 5; /* Vertices */
    weighted_edge *edges = malloc(size * sizeof(weighted_edge));
    weighted_edge *tour;

    weighted_edge_connect(edges, 0, 1, 20, &i);
    weighted_edge_connect(edges, 0, 2, 30, &i);
    weighted_edge_connect(edges, 0, 3, 10, &i);
    weighted_edge_connect(edges, 0, 4, 11, &i);
    weighted_edge_connect(edges, 1, 0, 15, &i);
    weighted_edge_connect(edges, 1, 2, 16, &i);
    weighted_edge_connect(edges, 1, 3, 4, &i);
    weighted_edge_connect(edges, 1, 4, 2, &i);
    weighted_edge_connect(edges, 2, 0, 3, &i);
    weighted_edge_connect(edges, 2, 1, 5, &i);
    weighted_edge_connect(edges, 2, 3, 2, &i);
    weighted_edge_connect(edges, 2, 4, 4, &i);
    weighted_edge_connect(edges, 3, 0, 19, &i);
    weighted_edge_connect(edges, 3, 1, 6, &i);
    weighted_edge_connect(edges, 3, 2, 18, &i);
    weighted_edge_connect(edges, 3, 4, 3, &i);
    weighted_edge_connect(edges, 4, 0, 16, &i);
    weighted_edge_connect(edges, 4, 1, 4, &i);
    weighted_edge_connect(edges, 4, 2, 7, &i);
    weighted_edge_connect(edges, 4, 3, 16, &i);

    tour = nearest_neighbour_tsp(edges, size, order);
    print_edges(tour, order);

    free(tour);
    free(edges);
    return 0;
}
