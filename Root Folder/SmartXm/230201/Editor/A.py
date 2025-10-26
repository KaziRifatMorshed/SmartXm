# Allocate approximately 1 GB of memory using a bytearray

size_in_bytes = 1024 * 1024 * 1024  # 1 GB = 1024^3 bytes
arr = bytearray(size_in_bytes)

print("Allocated ~1 GB of memory successfully!")
