#include "main.h"
#include <elf.h>

/**
 * print_elf_header - Prints information from the ELF header
 * @ehdr: Pointer to the ELF header
 */
void print_elf_header(Elf64_Ehdr *ehdr)
{
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (int i = 0; i < EI_NIDENT; i++)
        printf("%02x%c", ehdr->e_ident[i], (i == EI_NIDENT - 1) ? '\n' : ' ');
    printf("  Class:                             %s\n", (ehdr->e_ident[EI_CLASS] == ELFCLASS64) ? "ELF64" : "ELF32");
    printf("  Data:                              %s\n", (ehdr->e_ident[EI_DATA] == ELFDATA2LSB) ? "2's complement, little endian" : "Unknown");
    printf("  Version:                           %d (current)\n", ehdr->e_ident[EI_VERSION]);
    printf("  OS/ABI:                            %s\n", (ehdr->e_ident[EI_OSABI] == ELFOSABI_SYSV) ? "UNIX - System V" : "Unknown");
    printf("  ABI Version:                       %d\n", ehdr->e_ident[EI_ABIVERSION]);
    printf("  Type:                              %s\n", (ehdr->e_type == ET_EXEC) ? "EXEC (Executable file)" : "Unknown");
    printf("  Entry point address:               0x%lx\n", (unsigned long)ehdr->e_entry);
}

/**
 * main - Entry point
 * @argc: Argument count
 * @argv: Argument vector
 * Return: 0 on success, 98 on error
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        return (98);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return (98);
    }

    Elf64_Ehdr ehdr;
    ssize_t bytes_read = read(fd, &ehdr, sizeof(ehdr));
    if (bytes_read == -1)
    {
        perror("read");
        close(fd);
        return (98);
    }

    if (bytes_read != sizeof(ehdr) || ehdr.e_ident[EI_MAG0] != ELFMAG0 || ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 || ehdr.e_ident[EI_MAG3] != ELFMAG3)
    {
        fprintf(stderr, "%s: Not an ELF file\n", argv[1]);
        close(fd);
        return (98);
    }

    print_elf_header(&ehdr);
    close(fd);
    return (0);
}
