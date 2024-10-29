#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100
#define INF 1000

// 엣지 구조체
typedef struct {
    int start, end, weight;
} Edge;

// 그래프 구조체
typedef struct {
    int n;
    Edge edges[MAX_VERTICES];
} GraphType;

// 최소 힙 구조체
typedef struct {
    Edge heap[MAX_VERTICES];
    int size;
} MinHeap;

// 그래프 초기화
void graph_init(GraphType* g) {
    g->n = 0;
}

// 엣지 그래프에 추가
void insert_edge(GraphType* g, int start, int end, int w) {
    g->edges[g->n].start = start;
    g->edges[g->n].end = end;
    g->edges[g->n].weight = w;
    g->n++;
}

// 그래프 생성
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

// 엣지 가중치 비교 (퀵솔트사용)
int compare(const void* a, const void* b) {
    Edge* x = (Edge*)a;
    Edge* y = (Edge*)b;
    return x->weight - y->weight;
}

// 부모 배열
int parent[MAX_VERTICES];

// 부모 초기화
void set_init(int n) {
    for (int i = 0; i < n; i++)
        parent[i] = -1;
}

// 집합 찾기
int set_find(int curr) {
    if (parent[curr] == -1)
        return curr; // 부모 -1이면 현재 정점이 루트
    return parent[curr] = set_find(parent[curr]);
}

// 집합 히ㅏㅂ치기
void set_union(int a, int b) {
    int root1 = set_find(a); // a 루트
    int root2 = set_find(b); // b 루트
    if (root1 != root2)
        parent[root1] = root2;
}

// QuickSort 기반 Kruskal 알고리즘
void QuickKruskal(GraphType* g) {
    int edge_accepted = 0;
    int uset, vset;

    qsort(g->edges, g->n, sizeof(Edge), compare); // 가중치 기준 정렬

    printf("QuickSort Based Kruskal\n");
    set_init(MAX_VERTICES);

    for (int i = 0; i < g->n; i++) {
        uset = set_find(g->edges[i].start);
        vset = set_find(g->edges[i].end);
        if (uset != vset) {
            // 가중치만 출력
            printf("Edge (%d, %d) select %d\n", g->edges[i].start, g->edges[i].end, g->edges[i].weight);
            set_union(uset, vset);
            edge_accepted++;
        }
        if (edge_accepted == g->n - 1) // 모든 엣지 선택됐을 때 정지
            break;
    }

    printf("\n");
}

// 최소 힙 초기화
void minHeapInit(MinHeap* h) {
    h->size = 0;
}

// 최소 힙에 엣지 삽입
void minHeapInsert(MinHeap* h, Edge e) {
    int i = h->size++;
    while (i > 0) {
        int parent = (i - 1) / 2; // 부모 노드 인덱스
        if (h->heap[parent].weight <= e.weight) // 부모가 더 작으면 정지
            break;
        h->heap[i] = h->heap[parent];
        i = parent; // 현재 인덱스 부모로 변경
    }
    h->heap[i] = e;
}

// 최소 힙에서 엣지 삭제
Edge minHeapDelete(MinHeap* h) {
    Edge minEdge = h->heap[0]; // 최소 엣지 저저ㅏㅇ
    Edge lastEdge = h->heap[--h->size];

    int parent = 0;
    int child = 1;
    while (child < h->size) {
        if (child + 1 < h->size && h->heap[child + 1].weight < h->heap[child].weight)
            child++;
        if (lastEdge.weight <= h->heap[child].weight) // 마지막 엣지가 자식보다 작으면 종료
            break;
        h->heap[parent] = h->heap[child];
        parent = child;
        child = 2 * parent + 1;
    }
    h->heap[parent] = lastEdge;
    return minEdge;
}

// minheap기반 구현
void MinHeapKruskal(GraphType* g) {
    MinHeap heap;
    minHeapInit(&heap);
    int edge_accepted = 0;
    int uset, vset;

    // MinHeap에 모든 엣지 삽입
    for (int i = 0; i < g->n; i++) {
        minHeapInsert(&heap, g->edges[i]);
    }

    printf("MinHeap Based Kruskal\n");
    set_init(MAX_VERTICES); // 부모초기화

    while (heap.size > 0) {
        Edge edge = minHeapDelete(&heap);
        uset = set_find(edge.start);
        vset = set_find(edge.end);

        if (uset != vset) { // 두 정점이 같은 집합이 아니라면
            // 가중치만 출력
            printf("Edge (%d, %d) select %d\n", edge.start, edge.end, edge.weight);
            set_union(uset, vset); // 정점합치기
            edge_accepted++; // 선택엣지수 증가
        }
        if (edge_accepted == g->n - 1) // 모든엣지선택되면
            break;
    }
}

int main(void) {
    GraphType* g;
    g = (GraphType*)malloc(sizeof(GraphType));
    graph_init(g);

    // 그래프를 생성하는 함수
    GenerateGraph(g);

    QuickKruskal(g); // Qsort 기반 Kruskal
    MinHeapKruskal(g); // MinHeap 기반 Kruskal

    free(g);
    return 0;
}
