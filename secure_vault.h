#ifndef SECURE_VAULT_H
#define SECURE_VAULT_H

#define TABLE_SIZE 127 // Small prime for clean demo hash scaling

typedef struct HashNode {
    char key[64];
    char encrypted_value[256];
    struct HashNode* next;
} HashNode;

typedef struct AVLNode {
    int timestamp;
    char action[128];
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

typedef struct Vault {
    HashNode* hash_table[TABLE_SIZE];
    AVLNode* audit_root;
} Vault;

unsigned long hash_function(const char* str);
AVLNode* insert_audit_log(AVLNode* node, int timestamp, const char* action);
void inorder_audit(AVLNode* root);

#endif // SECURE_VAULT_H
