# Micro Code
Micro Code est bytecode basé sur une pile personnalisé écrit en C.

| Instruction | OpCode | Signification |
|-------------|--------|---------------|
| HALT        | 0x00   | Stop le prog
|             |        |
| ADD         | 0x01   | Additionne les deux dernières valeurs de la pile
| SUB         | 0x02   | Soustrais les deux dernières valeurs de la pile
| MUL         | 0x03   | Multiplie les deux dernières valeurs de la pile
| DIV         | 0x04   | Divise les deux dernières valeurs de la pile
|             |        |
| PUSH        | 0x10   | Ajoute une valeur sur la pile
| DROP        | 0x11   | Retire la dernière valeur de la pile
| SWAP        | 0x12   | Swap les deux dernières valeurs 
| OVER        | 0x13   | Push l'avant dernière valeur
| DUP         | 0x14   | Duplique la dernière valeur
|             |        |
| DELAY       | 0x20   | Fait un delay de x ms présent sur la pile
|             |        |
| JMP         | 0x30   | Saute à l'adresse qui est sur la pile
| JZ          | 0x31   | Saute à addr si stack[sp] == 0
| JNZ         | 0x32   | Saute à addr si stack[sp] != 0
| EQ          | 0x33   | Pousse 1 si a == b, sinon 0
| LT          | 0x34   | Pousse 1 si a < b, sinon 0
| GT          | 0x35   | Pousse 1 si a > b, sinon 0
|             |        |
| STORE       | 0x40   | Store a à l'adresse x
| LOAD        | 0x41   | Load à l'adresse x et push sur la pile
|             |        |
| PRINT       | 0x50   | Affiche la valeur de la pile
| PUTC        | 0x51   | Affiche le caractère correspondant à la pile
