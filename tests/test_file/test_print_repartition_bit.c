#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../include/gestion_tag.h"
#include "../../include/sub_include/data_container.h"
#include "../../include/sub_include/print_val.h"

static int contains_text(const char* haystack, const char* needle)
{
    return strstr(haystack, needle) != NULL;
}

static int capture_print_repartition_output(char* out_buf, size_t out_size)
{
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return -1;
    }

    fflush(stdout);
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout < 0) {
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
        close(saved_stdout);
        close(pipefd[0]);
        close(pipefd[1]);
        return -1;
    }

    close(pipefd[1]);

    init_data_container();
    init_tag_manager();

    if (set_tag("a1", 2) <= 0 ||
        set_tag("agh5", 5) <= 0 ||
        set_tag("patap", 16) <= 0 ||
        set_tag("patat", 12) <= 0 ||
        set_tag("uni1", 1) <= 0 ||
        set_tag("triot", 3) <= 0) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        close(pipefd[0]);
        return -1;
    }

    print_repartition_bit();
    fflush(stdout);

    if (dup2(saved_stdout, STDOUT_FILENO) < 0) {
        close(saved_stdout);
        close(pipefd[0]);
        return -1;
    }
    close(saved_stdout);

    size_t total = 0;
    while (total + 1 < out_size) {
        ssize_t n = read(pipefd[0], out_buf + total, out_size - total - 1);
        if (n <= 0) {
            break;
        }
        total += (size_t)n;
    }
    close(pipefd[0]);
    out_buf[total] = '\0';

    return (int)total;
}

int main__(void)
{
    char output[8192];
    int n = capture_print_repartition_output(output, sizeof(output));
    if (n <= 0) {
        fprintf(stderr, "[FAIL] capture output failed\n");
        return 1;
    }

    if (!contains_text(output, "=== REPARTITION DES BITS DANS LA TRAME CAN ===")) {
        fprintf(stderr, "[FAIL] header missing\n");
        return 1;
    }

    if (!contains_text(output, "Legende des tags:")) {
        fprintf(stderr, "[FAIL] legend title missing\n");
        return 1;
    }

    if (!contains_text(output, "a1 : [a]") ||
        !contains_text(output, "agh5 : [b]") ||
        !contains_text(output, "patap : [c]") ||
        !contains_text(output, "patat : [d]") ||
        !contains_text(output, "uni1 : [e]") ||
        !contains_text(output, "triot : [f]")) {
        fprintf(stderr, "[FAIL] legend entries missing\n");
        return 1;
    }

    for (int i = 0; i < N_BYTES; i++) {
        char pattern[32];
        snprintf(pattern, sizeof(pattern), " [%d] | ", i);
        if (!contains_text(output, pattern)) {
            fprintf(stderr, "[FAIL] missing byte row: %d\n", i);
            return 1;
        }
    }

    printf("[PASS] print_repartition_bit: structure de sortie valide\n");
    return 0;
}
