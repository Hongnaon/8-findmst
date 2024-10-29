#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define INF 1000

// ���� ����ü
typedef struct {
    int start, end, weight;
} Edge;

// �׷��� ����ü
typedef struct {
    int n;
    Edge edges[MAX_VERTICES];
} GraphType;

// �ּ� �� ����ü
typedef struct {
    Edge heap[MAX_VERTICES];
    int size;
} MinHeap;

// �׷��� �ʱ�ȭ
void graph_init(GraphType* g) {
    g->n = 0;
}

// ���� �׷����� �߰�
void insert_edge(GraphType* g, int start, int end, int w) {
    g->edges[g->n].start = start;
    g->edges[g->n].end = end;
    g->edges[g->n].weight = w;
    g->n++;
}

// �׷��� ����
void GenerateGraph(GraphType* g) {
    insert_edge(g, 1, 2, 3);
    insert_edge(g, 2, 3, 5);
    insert_edge(g, 1, 6, 11);
    insert_edge(g, 2, 6, 7);
    insert_edge(g, 5, 6, 9);
    insert_edge(g, 2, 5, 1);
    insert_edge(g, 2, 4, 4);
    insert_edge(g, 4, 5, 13);
    insert_edge(g, 5, 9, 18);
    insert_edge(g, 1, 7, 12);
    insert_edge(g, 3, 7, 6);
    insert_edge(g, 7, 8, 13);
    insert_edge(g, 3, 8, 5);
    insert_edge(g, 4, 8, 14);
    insert_edge(g, 8, 10, 15);
    insert_edge(g, 9, 10, 10);
    insert_edge(g, 4, 10, 16);
    insert_edge(g, 5, 10, 17);
    insert_edge(g, 3, 4, 2);
    insert_edge(g, 2, 7, 8);
}

// ���� ����ġ �� (����Ʈ���)
int compare(const void* a, const void* b) {
    Edge* x = (Edge*)a;
    Edge* y = (Edge*)b;
    return x->weight - y->weight;
}

// �θ� �迭
int parent[MAX_VERTICES];

// �θ� �ʱ�ȭ
void set_init(int n) {
    for (int i = 0; i < n; i++)
        parent[i] = -1;
}

// ���� ã��
int set_find(int curr) {
    if (parent[curr] == -1)
        return curr; // �θ� -1�̸� ���� ������ ��Ʈ
    return parent[curr] = set_find(parent[curr]);
}

// ���� ������ġ��
void set_union(int a, int b) {
    int root1 = set_find(a); // a ��Ʈ
    int root2 = set_find(b); // b ��Ʈ
    if (root1 != root2)
        parent[root1] = root2;
}

// QuickSort ��� Kruskal �˰���
void QuickKruskal(GraphType* g) {
    int edge_accepted = 0;
    int uset, vset;

    qsort(g->edges, g->n, sizeof(Edge), compare); // ����ġ ���� ����

    printf("QuickSort Based Kruskal\n");
    set_init(MAX_VERTICES);

    for (int i = 0; i < g->n; i++) {
        uset = set_find(g->edges[i].start);
        vset = set_find(g->edges[i].end);
        if (uset != vset) {
            // ����ġ�� ���
            printf("Edge (%d, %d) select %d\n", g->edges[i].start, g->edges[i].end, g->edges[i].weight);
            set_union(uset, vset);
            edge_accepted++;
        }
        if (edge_accepted == g->n - 1) // ��� ���� ���õ��� �� ����
            break;
    }

    printf("\n");
}

// �ּ� �� �ʱ�ȭ
void minHeapInit(MinHeap* h) {
    h->size = 0;
}

// �ּ� ���� ���� ����
void minHeapInsert(MinHeap* h, Edge e) {
    int i = h->size++;
    while (i > 0) {
        int parent = (i - 1) / 2; // �θ� ��� �ε���
        if (h->heap[parent].weight <= e.weight) // �θ� �� ������ ����
            break;
        h->heap[i] = h->heap[parent];
        i = parent; // ���� �ε��� �θ�� ����
    }
    h->heap[i] = e;
}

// �ּ� ������ ���� ����
Edge minHeapDelete(MinHeap* h) {
    Edge minEdge = h->heap[0]; // �ּ� ���� ��������
    Edge lastEdge = h->heap[--h->size];

    int parent = 0;
    int child = 1;
    while (child < h->size) {
        if (child + 1 < h->size && h->heap[child + 1].weight < h->heap[child].weight)
            child++;
        if (lastEdge.weight <= h->heap[child].weight) // ������ ������ �ڽĺ��� ������ ����
            break;
        h->heap[parent] = h->heap[child];
        parent = child;
        child = 2 * parent + 1;
    }
    h->heap[parent] = lastEdge;
    return minEdge;
}

// minheap��� ����
void MinHeapKruskal(GraphType* g) {
    MinHeap heap;
    minHeapInit(&heap);
    int edge_accepted = 0;
    int uset, vset;

    // MinHeap�� ��� ���� ����
    for (int i = 0; i < g->n; i++) {
        minHeapInsert(&heap, g->edges[i]);
    }

    printf("MinHeap Based Kruskal\n");
    set_init(MAX_VERTICES); // �θ��ʱ�ȭ

    while (heap.size > 0) {
        Edge edge = minHeapDelete(&heap);
        uset = set_find(edge.start);
        vset = set_find(edge.end);

        if (uset != vset) { // �� ������ ���� ������ �ƴ϶��
            // ����ġ�� ���
            printf("Edge (%d, %d) select %d\n", edge.start, edge.end, edge.weight);
            set_union(uset, vset); // ������ġ��
            edge_accepted++; // ���ÿ����� ����
        }
        if (edge_accepted == g->n - 1) // ��翧�����õǸ�
            break;
    }
}

int main(void) {
    GraphType* g;
    g = (GraphType*)malloc(sizeof(GraphType));
    graph_init(g);

    // �׷����� �����ϴ� �Լ�
    GenerateGraph(g);

    QuickKruskal(g); // Qsort ��� Kruskal
    MinHeapKruskal(g); // MinHeap ��� Kruskal

    free(g);
    return 0;
}
