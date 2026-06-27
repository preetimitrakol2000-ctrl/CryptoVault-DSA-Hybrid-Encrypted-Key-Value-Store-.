import ctypes
import os
import sys
import base64

class CryptoVaultBridge:
    def __init__(self):
        if not os.path.exists("./libvault.so") and not os.path.exists("./libvault.dll"):
            print("[*] Compiling algorithmic key-value storage core engines...")
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libvault.dll secure_vault.c vault_interface.c")
                lib_path = "./libvault.dll"
            else:
                os.system("gcc -shared -fPIC -o libvault.so secure_vault.c vault_interface.c")
                lib_path = "./libvault.so"
        else:
            lib_path = "./libvault.dll" if sys.platform.startswith("win") else "./libvault.so"

        self.lib = ctypes.CDLL(lib_path)
        
        # Mapping configurations
        self.lib.init_vault.restype = ctypes.c_void_p
        self.lib.put_secret.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
        
        self.lib.get_secret.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int]
        self.lib.get_secret.restype = ctypes.c_char_p
        
        self.lib.display_audit_trail.argtypes = [ctypes.c_void_p]
        self.lib.destroy_vault.argtypes = [ctypes.c_void_p]

        self.vault_ptr = self.lib.init_vault()

    def _encrypt_val(self, plaintext: str) -> str:
        # Obfuscation layer simulating data isolation boundary behaviors
        return base64.b64encode(plaintext.encode('utf-8')).decode('utf-8')

    def _decrypt_val(self, ciphertext: str) -> str:
        return base64.b64decode(ciphertext.encode('utf-8')).decode('utf-8')

    def store(self, key: str, value: str, timestamp: int):
        enc_value = self._encrypt_val(value)
        self.lib.put_secret(self.vault_ptr, key.encode('utf-8'), enc_value.encode('utf-8'), timestamp)

    def retrieve(self, key: str, timestamp: int) -> str:
        res = self.lib.get_secret(self.vault_ptr, key.encode('utf-8'), timestamp)
        if res:
            return self._decrypt_val(res.decode('utf-8'))
        return "Secret Key Not Found."

    def print_audit(self):
        self.lib.display_audit_trail(self.vault_ptr)

    def __del__(self):
        if hasattr(self, 'lib') and self.vault_ptr:
            self.lib.destroy_vault(self.vault_ptr)
            print("[+] Secure database memory space unmapped.")
