#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SECTOR_SIZE 512

// La matriz oficial estricta de las 35 palabras clave del Python actual
const char *PYTHON_KEYWORDS[] = {
    "False", "None", "True", "and", "as", "assert", "async", "await", "break", "class", 
    "continue", "def", "del", "elif", "else", "except", "finally", "for", "from", "global", 
    "if", "import", "in", "is", "lambda", "nonlocal", "not", "or", "pass", "raise", 
    "return", "try", "while", "with", "yield"
};

int main(int argc, char *argv[]) {
    char *archivo_entrada = NULL;
    char *archivo_salida = "boot.bin";

    if (argc < 2) {
        printf("==================================================\n");
        printf("    PCC - Python Code Compiler (BIOS v0.1)        \n");
        printf("==================================================\n");
        printf("Uso correcto: pcc.exe <archivo.py> [-o <salida.bin>]\n");
        return 1;
    }

    archivo_entrada = argv[1];

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && (i + 1) < argc) {
            archivo_salida = argv[i + 1];
            break;
        }
    }

    FILE *fp_in = fopen(archivo_entrada, "r");
    if (fp_in == NULL) {
        printf("[PCC ERROR] No se pudo abrir el archivo de origen '%s'.\n", archivo_entrada);
        return 1;
    }

    unsigned char buffer_bios[SECTOR_SIZE];
    memset(buffer_bios, 0, SECTOR_SIZE);
    int bytes_codigo = 0;
    
    // Buffer de línea de 256 caracteres para evitar desbordamientos en la RAM
    char linea[256]; 
    int numero_linea = 1;

    printf("[PCC v0.1] Analizando las 35 palabras clave en '%s'...\n", archivo_entrada);

    while (fgets(linea, sizeof(linea), fp_in)) {
        // Quitar saltos de línea para facilitar la comparación de texto
        linea[strcspn(linea, "\r\n")] = 0;
        
        char *ptr = linea;
        // Limpiar espacios en blanco iniciales (soportar la indentación de Python)
        while (isspace((unsigned char)*ptr)) ptr++;
        
        // Ignorar líneas vacías o comentarios puros
        if (strlen(ptr) == 0 || *ptr == '#') {
            numero_linea++;
            continue;
        }

        // --- BUCLE DE ESCANEO DE LAS 35 KEYWORDS ---
        for (int k = 0; k < 35; k++) {
            if (strstr(ptr, PYTHON_KEYWORDS[k]) != NULL) {
                printf("[PCC KEYWORD] Línea %d: Detectado token oficial '%s'\n", numero_linea, PYTHON_KEYWORDS[k]);
                
                // Mapeo físico directo para tokens de control básicos en v0.1
                if (strcmp(PYTHON_KEYWORDS[k], "pass") == 0) {
                    if (bytes_codigo < 510) buffer_bios[bytes_codigo++] = 0x90; // Opcode x86: NOP
                }
                else if (strcmp(PYTHON_KEYWORDS[k], "while") == 0 && strstr(ptr, "True")) {
                    if (bytes_codigo < 509) {
                        buffer_bios[bytes_codigo++] = 0xEB; // Opcode x86: JMP short
                        buffer_bios[bytes_codigo++] = 0xFE; // Salto relativo a sí mismo
                    }
                }
            }
        }

        // --- INYECTOR CLÁSICO ORIGINAL DE PRINT("H") ---
        if (strstr(ptr, "print(\"H\")") != NULL || strstr(ptr, "print('H')") != NULL) {
            if (bytes_codigo < 504) {
                buffer_bios[bytes_codigo++] = 0xB4; // MOV AH, 0x0E (Modo Teletexto)
                buffer_bios[bytes_codigo++] = 0x0E;
                buffer_bios[bytes_codigo++] = 0xB0; // MOV AL, 'H'
                buffer_bios[bytes_codigo++] = 'H';
                buffer_bios[bytes_codigo++] = 0xCD; // INT 0x10 (Interrupción de video BIOS)
                buffer_bios[bytes_codigo++] = 0x10;
            }
        }
        
        numero_linea++;
    }
    fclose(fp_in);

    if (bytes_codigo > 510) {
        printf("[PCC ERROR] El código binario excede los 510 bytes permitidos por la BIOS.\n");
        return 1;
    }

    // Inyección automática clásica de la firma mágica del sector de arranque (MBR) [1.1]
    buffer_bios[510] = 0x55;
    buffer_bios[511] = 0xAA;

    FILE *fp_out = fopen(archivo_salida, "wb");
    if (fp_out == NULL) {
        printf("[PCC ERROR] No se pudo crear el archivo de salida '%s'.\n", archivo_salida);
        return 1;
    }

    fwrite(buffer_bios, 1, SECTOR_SIZE, fp_out);
    fclose(fp_out);

    printf("[PCC SUCCESS] Compilación terminada. Generado '%s' (%d bytes ocupados por código).\n", archivo_salida, bytes_codigo);
    return 0;
}
note:The code can have error, I am a small dev :c
