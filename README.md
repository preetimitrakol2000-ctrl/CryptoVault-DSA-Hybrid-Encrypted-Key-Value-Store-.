# CryptoVault-DSA // Hybrid Encrypted Key-Value Store

An engineered dual-engine storage vault combining a low-latency Hash Table with a self-balancing AVL Tree structure to achieve performance lookups coupled with structured cryptographic auditing timelines.

## Data Structures Strategy
- **Secret Lookup Data Tables ($O(1)$):** Custom implementation of a native C Hash Table with **Linked List Chaining collision resolution wrappers** to retain secrets safely inside process margins.
- **Immutable Audit Streams ($O(\log n)$):** Employs an structural **AVL Tree Index**, self-balancing across rotations to prevent linear performance degradation under heavy indexing loads.

## Structural Directory
- `secure_vault.c`: Hash Table mechanics and AVL core balancing logic routines.
- `vault_interface.c`: Direct memory map bindings and system registration layers.
- `secure_vault.h`: Component definition headers mapping structural dimensions.
- `vault_bridge.py`: Encoding processors, type declarations, and bridge linkages.
- `main_app.py`: Verification run execution entry point.

## Execution Requirements
Build and run the stack instantly with:
```bash
python main_app.py
