with open("boot.bin", "rb") as f:
    data = f.read()

with open("boot.h", "w") as f:
    f.write("unsigned char boot[] = {\n")
    for i in range(0, len(data), 16):
        buffer = data[i:i+16]
        f.write("  " + ", ".join(f"0x{i:02x}" for i in buffer) + ",\n")
    f.write("};\n")

# ik i can do it manually but im a lazy ass
