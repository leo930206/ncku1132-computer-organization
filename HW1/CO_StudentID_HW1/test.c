#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Split the linked list into two parts
void splitList(Node *head, Node **firstHalf, Node **secondHalf) {
    asm volatile(
        "mv t0, %0\n"           // t0 = head
        "li t1, 0\n"            // t1 = firstHalf (初始化為 NULL)
        "li t2, 0\n"            // t2 = secondHalf (初始化為 NULL)
        
        // 設置慢指針 (t3) 和快指針 (t4)
        "mv t3, t0\n"           // t3 = slow pointer
        "mv t4, t0\n"           // t4 = fast pointer

        // 快慢指針循環，快指針每次走兩步，慢指針每次走一步
        "loop:\n"
        "beqz t4, done\n"       // 如果 fast pointer 為 NULL，結束循環
        "lw t5, 4(t4)\n"        // 加載快指針的下一個節點
        "beqz t5, done\n"       // 如果快指針的下一個節點為 NULL，結束循環
        "lw t5, 4(t5)\n"        // 加載快指針的再下一個節點
        "beqz t5, done\n"       // 如果快指針的再下一個節點為 NULL，結束循環
        "mv t4, t5\n"           // 更新快指針為下一個節點
        "addi t3, t3, 4\n"      // 更新慢指針為下一個節點
        "b loop\n"

        "done:\n"
    );
}



// Merge two sorted linked lists
Node *mergeSortedLists(Node *a, Node *b) {
    Node *result = NULL;
    Node *tail = NULL;

    asm volatile(
        "li t0, 0\n"           // result = NULL
        "li t1, 0\n"           // tail = NULL
        
        // 檢查哪一個鏈結串列的頭節點小，並將其加到結果中
        "loop:\n"
        "beqz %0, done\n"      // 如果 a 為 NULL，結束循環
        "beqz %1, done\n"      // 如果 b 為 NULL，結束循環
        "lw t2, 0(%0)\n"       // 加載 a 的數據
        "lw t3, 0(%1)\n"       // 加載 b 的數據
        "blt t2, t3, merge_a\n"  // 如果 a 的數據小於 b 的數據，將 a 加入結果
        "merge_b:\n"
        "lw t4, 4(%1)\n"       // 取出 b 的下一個節點
        "b loop\n"

        "merge_a:\n"
        "lw t5, 4(%0)\n"       // 取出 a 的下一個節點
        "b loop\n"

        "done:\n"
    );
    return result;
}



// Merge Sort function for linked list
Node *mergeSort(Node *head)
{
    if (!head || !head->next)
        return head; // Return directly if there is only one node

    Node *firstHalf, *secondHalf;
    splitList(head, &firstHalf,
              &secondHalf); // Split the list into two sublists

    firstHalf = mergeSort(firstHalf);   // Recursively sort the left half
    secondHalf = mergeSort(secondHalf); // Recursively sort the right half

    return mergeSortedLists(firstHalf, secondHalf); // Merge the sorted sublists
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }
    int list_size;
    fscanf(input, "%d", &list_size);
    Node *head = (list_size > 0) ? (Node *)malloc(sizeof(Node)) : NULL;
    Node *cur = head;
    for (int i = 0; i < list_size; i++) {
        fscanf(input, "%d", &(cur->data));
        if (i + 1 < list_size)
            cur->next = (Node *)malloc(sizeof(Node));
        cur = cur->next;
    }
    fclose(input);

    // Linked list sort
    head = mergeSort(head);

    cur = head;
    while (cur) {
        printf("%d ", cur->data);
        asm volatile(
            "addi %0, %0, 4\n"      // 移動指針到下一個節點
        );
        
    }
    printf("\n");
    return 0;
}
