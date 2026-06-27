from vault_bridge import CryptoVaultBridge
import time

def main():
    print("=== CRYPTOVAULT-DSA: HYBRID AUTONOMIC INDEX KEY STORE ===")
    vault = CryptoVaultBridge()

    # Storing credentials out-of-order to force AVL tree balancing operations
    print("\n[*] Populating secure hash tables and indexing chronological audit trails...")
    vault.store("root_password", "AdminSecured2026!", timestamp=105)
    vault.store("api_jwt_token", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9", timestamp=101)
    vault.store("db_connection", "postgresql://db_user:pass@localhost:5432/prod", timestamp=103)

    # Executing transactional data lookups
    print("\n[*] Fetching secrets down the execution layer...")
    secret_key = "root_password"
    retrieved = vault.retrieve(secret_key, timestamp=106)
    print(f" -> Query Key [{secret_key}] Output Plaintext Result: {retrieved}")

    # Display chronological immutable records
    vault.print_audit()

if __name__ == "__main__":
    main()
