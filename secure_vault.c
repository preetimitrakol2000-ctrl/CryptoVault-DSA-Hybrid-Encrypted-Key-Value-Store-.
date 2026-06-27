#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "secure_vault.h"

// Simple, effective string hashing function (DJB2)
unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

// AVL Tree Helper: Get Node Height
int get_height(AVLNode* node) {
    return node ? node->height : 0;
}

// AVL Tree Helper: Get Balance Factor
int get_balance(AVLNode* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

// AVL Tree Rotation: Right Rotate
AVLNode* right_rotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = 1 + (get_height(y->left) > get_height(y->right) ? get_height(y->left) : get_height(y->right));
    x->height = 1 + (get_height(x->left) > get_height(x->right) ? get_height(x->left) : get_height(x->right));
    return x;
}

// AVL Tree Rotation: Left Rotate
AVLNode* left_rotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = 1 + (get_height(x->left) > get_height(x->right) ? get_height(x->left) : get_height(x->right));
    y->height = 1 + (get_height(y->left) > get_height(y->right) ? get_height(y->left) : get_height(y->right));
    return y;
}

// Inserts an access log into the AVL Tree ordered by timestamp index
AVLNode* insert_audit_log(AVLNode* node, int timestamp, const char* action) {
    if (!node) {
        AVLNode* new_node = (AVLNode*)malloc(sizeof(AVLNode));
        new_node->timestamp = timestamp;
        strncpy(new_node->action, action, sizeof(new_node->action) - 1);
        new_node->left = new_node->right = NULL;
        new_node->height = 1;
        return new_node;
    }

    if (timestamp < node->timestamp)
        node->left = insert_audit_log(node->left, timestamp, action);
    else
        node->right = insert_audit_log(node->right, timestamp, action);

    node->height = 1 + (get_height(node->left) > get_height(node->right) ? get_height(node->left) : get_height(node->right));
    int balance = get_balance(node);

    // Left Left Case
    if (balance > 1 && timestamp < node->left->timestamp) return right_rotate(node);
    // Right Right Case
    if (balance < -1 && timestamp > node->right->timestamp) return left_rotate(node);
    // Left Right Case
    if (balance > 1 && timestamp > node->left->timestamp) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    // Right Left Case
    if (balance < -1 && timestamp < node->right->timestamp) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }
    return node;
}

// In-order traversal to print audit records cleanly
void inorder_audit(AVLNode* root) {
    if (root) {
        inorder_audit(root->left);
        printf("[Audit Log] Timestamp: %d | Action: %s\n", root->timestamp, root->action);
        inorder_audit(root->right);
    }
}
