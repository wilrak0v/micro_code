instructions1 = [0x4D, 0x42, 0x04, 0x00, 0x40, 0x00, 0x01,
                 0x00,]
instructions2 = [
    0x01,
    0x00,
    0x00,
    0x03,
]

with open("test.bin", "wb") as f:
    f.write(bytes(instructions1))

# with open("test2.bin", "wb") as f:
#    f.write(bytes(instructions2))

print("Fichier(s) de tests binaires générés !")
