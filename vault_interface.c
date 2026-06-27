#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "secure_vault.h"

#ifdef _WIN32
    __declspec(dllexport) Vault* init_vault();
    __declspec(dllexport) void put_secret(Vault* vault, const char* key, const char* encrypted_val, int ts);
    __declspec(dllexport) const char* get_secret(Vault* vault, const char* key, int ts);
    __declspec(dllexport) void display_audit_trail(Vault* vault);
    __declspec(dllexport) void destroy_vault(Vault* vault);
#endif

Vault* init_vault() {
    Vault* vault = (Vault*)malloc(sizeof(Vault));
    vault->audit_root = NULL;
    for (int i = 0; i < TABLE_SIZE; i++) {
        vault->hash_table[i] = NULL;
    }
    return vault;
}

void put_secret(Vault* vault, const char* key, const char* encrypted_val, int ts) {
    unsigned long index = hash_function(key);
    HashNode* current = vault->hash_table[index];

    // Check if key already exists to update it
    while (current) {
        if (strcmp(current->key, key) == 0) {
            strncpy(current->encrypted_value, encrypted_val, sizeof(current->encrypted_value) - 1);
            char log_msg[128];
            snprintf(log_msg, sizeof(log_msg), "UPDATED key: %s", key);
            vault->audit_root = insert_audit_log(vault->audit_root, ts, log_msg);
            return;
        }
        current = current->next;
    }

    // Insert new secret block via chaining
    HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));
    strncpy(new_node->key, key, sizeof(new_node->key) - 1);
    strncpy(new_node->encrypted_value, encrypted_val, sizeof(new_node->encrypted_value) - 1);
    new_node->next = vault->hash_table[index];
    vault->hash_table[index] = new_node;

    char log_msg[128];
    snprintf(log_msg, sizeof(log_msg), "STORED key: %s", key);
    vault->audit_root = insert_audit_log(vault->audit_root, ts, log_msg);
}

const char* get_secret(Vault* vault, const char* key, int ts) {
    unsigned long index = hash_function(key);
    HashNode* current = vault->hash_table[index];

    char log_msg[128];
    snprintf(log_msg, sizeof(log_msg), "READ attempt key: %s", key);
    vault->audit_root = insert_audit_log(vault->audit_root, ts, log_msg);

    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->encrypted_value;
        }
        current = current->next;
    }
    return NULL;
}

void display_audit_trail(Vault* vault) {
    printf("\n--- SECURE AUDIT TRAIL TRAVERSAL (AVL TREE) ---\n");
    inorder_audit(vault->audit_root);
    printf("-----------------------------------------------\n");
}

void free_avl(AVLNode* node) {
    if (!node) return;
    free_avl(node->left);
    free_avl(node->right);
    free(node);
}

void destroy_vault(Vault* vault) {
    if (!vault) return;
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* current = vault->hash_table[i];
        while (current) {
            HashNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free_avl(vault->audit_root);
    free(vault);
}
