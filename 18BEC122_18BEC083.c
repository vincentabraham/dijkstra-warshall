#include<stdio.h>
#include<string.h>
#define INF 99999 // Represents Infinity

void freeAllocatedMemory(int **array, int nrows);
int ** Allocate2D(int **array, int n);
int * floydWarshall(int **G, int n, int start, char **node_labels);
int * dijk(int **G, int n, int start, char **node_labels);

int main(void)
{
    int **TIME, **DIST, n;
    int *distance, *time; // Stores the shortest distances and time taken
    int *route; // Overall shortest path
    char **node_labels;
    char start_label[70]; // Label for the Starting Node
    int i, j, start;
    printf(" --------------------------------------------------- \n");
    printf("| COMPUTING THE SHORTEST ROUTE FOR A ROAD NETWORK  |\n");
    printf(" --------------------------------------------------- \n\n");

    printf("Enter the Number of Locations in the Road Network: ");
    scanf("%d", &n);

    // Allocating Memory for storing the Node Labels
    node_labels = (char **)malloc(n * sizeof(char*));
    route = (int *)malloc((n - 1) * sizeof(int));
    // Checking for memory validity
    if(node_labels == NULL){
        free(node_labels);
        printf("Failed to Allocate Memory.");
        exit(0);
    }

    // Now in each row, allocating the memory for the columns
    for(i = 0; i < n; i++){
        node_labels[i] = (char *)malloc(70 * sizeof(char)); // Max. Characters = 70
        // Checking the memory validity
        if(node_labels[i] == NULL){
            freeAllocatedMemory(node_labels, i);
            printf("Failed to Allocate Memory.");
            exit(0);
        }
    }

    // Now allocating memory for the Adjacency Matrix storing the distance Initializing it
    DIST = Allocate2D(DIST, n);
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            DIST[i][j] = -1;
        }
    }

    // Allocating memory for the Adjacency Matrix storing the time Initializing it
    TIME = Allocate2D(TIME, n);
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            TIME[i][j] = -1;
        }
    }

    // Scanning the Node Values for the Road Network
    for(i = 0; i < n; i++){
        printf("Enter the Name of Location %d: ", i);
        scanf(" %[^\n]%*c", node_labels[i]);
    }

    printf("\nEnter the Distances in the Road Network. (Enter 0 if the roads aren't connected): \n\n");
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(i == j){
                DIST[i][j] = 0; // Weight of a node to itself would be zero
            }
            else if(DIST[i][j] == -1){
                printf("Enter the Distance Between %s & %s (km): ", node_labels[i], node_labels[j]);
                scanf("%d", &DIST[i][j]);
                DIST[j][i] = DIST[i][j];
            }
        }
        printf("\n");
    }

    printf("\nEnter the Time Taken in Travelling Through the Road Network. (Enter 0 if the roads aren't connected): \n\n");
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(i == j){
                TIME[i][j] = 0; // Weight of a node to itself would be zero
            }
            else if(TIME[i][j] == -1){
                printf("Enter the Time Taken From %s to %s (mins): ", node_labels[i], node_labels[j]);
                scanf("%d", &TIME[i][j]);
                TIME[j][i] = TIME[i][j];
            }
        }
        printf("\n");
    }

    printf("Enter the Starting Location: ");
    scanf(" %[^\n]%*c", start_label);

    // Searching for the Index of the Starting Node
    i = 0;
    while(strcmp(start_label, node_labels[i]) != 0){
        i++;
        if(i > n - 1){
        printf("Invalid Starting Point.");
        exit(0);
    }
    }
    start = i;
    distance = floydWarshall(DIST, n, start, node_labels);

    printf("\n");
    time = dijk(TIME, n, start, node_labels);

    printf("\nDestination | Distance | Time |\n");
    for(i = 0; i <= (n - 1); i++)
    {
        route[i] = distance[i] + time[i];
        if(i != start){
            printf("%11s | %8d | %4d |\n", node_labels[i], distance[i], time[i]);
        }
    }

    // Finding out the minimum value
    int min = (start == 0) ? 1 : 0;
    for(i = 0; i <= (n-1); i++){
        if((route[i] < route[min]) && (start != i)){
            min = i;
        }
    }

    printf("\nOverall Shortest Route: %s to %s", node_labels[start], node_labels[min]);

    free(DIST);
    free(TIME);
    free(node_labels);

    return 0;
}

void freeAllocatedMemory(int **array, int nrows)
{
    /* This function frees the memory allocated (nrows) rows of an array */
    for (int i = 0; i < nrows; i++)
    {
        free(array[i]); // Free the memory allocated to the row
    }
    free(array); // Free the memory allocated to the array
}

int ** Allocate2D(int **array, int n)
{
    /* This function allocates memory for an n x n 2D array */

    int i, j;
    // First allocating the memory for (n) rows
    array = (int **)malloc(n * sizeof(int*));
    // Checking for memory validity
    if(array == NULL){
        free(array);
        printf("Failed to Allocate Memory.");
        exit(0);
    }

    // Now in each row, allocating the memory for the columns
    for(i = 0; i < n; i++){
        array[i] = (int *)malloc(n * sizeof(int));
        // Checking the memory validity
        if(array[i] == NULL){
            freeAllocatedMemory(array, i);
            printf("Failed to Allocate Memory.");
            exit(0);
        }
    }

    return array;

}

int * floydWarshall(int **G, int n, int start, char **node_labels)
{
    int i, j, k, temp;
    int **dist, **next; // Stores the distance & path for each of the nodes
    int *shortest_dist; // Stores the shortest distances from the starting node
    int count = 0;



    // Allocating Memory for the arrays
    dist = Allocate2D(dist, n);
    next = Allocate2D(next, n);
    shortest_dist = (int *)malloc((n - 1) * sizeof(int));

    // Initialising the (distance) matrix
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j ++){
            if(i != j && G[i][j] == 0) // No edge connecting the two nodes
                dist[i][j] = INF;
            else
                dist[i][j] = G[i][j];
        }
    }

    // Initialising the (next) matrix
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(dist[i][j] == INF){
                next[i][j] = -1; // No path between the nodes
            }
            else{
                next[i][j] = j;
            }
        }
    }

    // Computes the shortest distance from node (i) to (j) through intermediate node (k)
    for(k = 0; k < n; k++){
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                if(dist[i][k] + dist[k][j] < dist[i][j]){
                    // If the intermediate node provides the shortest path, update the distance & path
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    printf("\n");

    count = 0; // Index of the return array
    // Displaying the distance & path from the start node to all the nodes
    for(i = 0; i < n; i++){
        if(i != start){
            printf("\nThe Shortest Distance From %s to %s is %d km.", node_labels[start], node_labels[i], dist[start][i]);
            shortest_dist[count] = dist[start][i];
            count++;
            printf("\nPath: ");
            if(next[start][i] != -1){
                printf("%s --> ", node_labels[start]);
                temp = start;
                while(temp != i){
                    temp = next[temp][i];
                    printf("%s --> ", node_labels[temp]);
                }
                printf("END.");
                printf("\n");
            }
        }
        else{
                shortest_dist[count] = 0;
                count++;
            }
    }

    free(dist);
    free(next);

    return shortest_dist;
}

int * dijk(int **G, int n, int start, char **node_labels)
{
    int **cost, *dist, *visited, *pred;
    int i, j, count, mindist, nextnode;
    int * shortest_time; // Stores the shortest time from the starting node
    int m = 0;

    // Allocating memory for the arrays
    cost = Allocate2D(cost, n);
    dist = (int *)malloc(n * sizeof(int));
    visited = (int *)malloc(n * sizeof(int));
    pred = (int *)malloc(n * sizeof(int));
    shortest_time = (int *)malloc((n - 1) * sizeof(int));

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            if(G[i][j] == 0){
                 cost[i][j] = INF;
            }
            else
                cost[i][j] = G[i][j];
        }
    }

    for(i = 0; i < n; i++){
        dist[i] = cost[start][i];
        pred[i] = start;
        visited[i] = 0;
    }

    dist[start] = 0;
    visited[start] = 1;
    count = 1;

    while(count < n - 1){

        mindist = INF;

        for(i = 0; i < n; i++){
            if(dist[i] < mindist && !visited[i]){
                mindist = dist[i];
                nextnode = i;
            }
        }

        visited[nextnode] = 1;

        for(i = 0; i < n; i++){
            if(!visited[i]){
                if((mindist + cost[nextnode][i]) < dist[i]){
                    dist[i] = mindist + cost[nextnode][i];
                    pred[i] = nextnode;
                }
            }
        }
        count++;
    }

    m = 0; // Index of the return array

    for(i = 0; i < n; i++){
            if(i != start){
                printf("The Shortest Time Taken From %s to %s is %d minutes.\n", node_labels[start],node_labels[i], dist[i]);
                printf("Path: %s", node_labels[i]);;
                shortest_time[m] = dist[i];
                m++;
                j = i;
                do{
                    j = pred[j];
                    printf(" <-- %s ", node_labels[j]);
                }while(j != start);
            }
            else{
                shortest_time[m] = 0;
                m++;
            }
            printf("\n");
        }

    free(cost);
    free(dist);
    free(pred);
    free(visited);

    return shortest_time;
}
