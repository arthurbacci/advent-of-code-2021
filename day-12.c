#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    size_t num_relations;
    size_t *relations;
    bool big;
    char name[100];
} Node;

bool is_big(char *s) {
    if (*s >= 65 && *s <= 90)
        return 1;
    return 0;
}



unsigned int num_paths(size_t index, Node *nodes,
                       size_t *history, size_t history_len, int two) {
    history = realloc(history, (++history_len) * sizeof(*history));
    history[history_len - 1] = index;

    unsigned int ret = 0;
    for (size_t i = 0; i < nodes[index].num_relations; i++) {
        Node n = nodes[nodes[index].relations[i]];
        if (nodes[index].relations[i] == 1) {
            ret++;
            for (size_t j = 0; j < history_len; j++)
                printf("%s -> ", nodes[history[j]].name);
            printf("%s\n", n.name);
        } else {
            bool visited = 0;
            for (size_t j = 0; j < history_len; j++) {
                if (history[j] == nodes[index].relations[i]) {
                    visited = 1;
                    break;
                }
            }
            
            if (nodes[index].relations[i] != 0) {
                bool v = visited == 0 || n.big;
                if (v || !two) {
                    size_t *hist = malloc(history_len * sizeof(*hist));
                    memcpy(hist, history, history_len * sizeof(*hist));
                    ret += num_paths(nodes[index].relations[i], nodes,
                                    hist, history_len, v ? two : 1);
                }
            }
        }
    }
    free(history);
    return ret;
}

int main() {
    FILE *fp = fopen("input/day-12.input", "r");
    
    size_t num_nodes = 2;
    Node *nodes = malloc(num_nodes * sizeof(*nodes));
    {
        Node start = {0, NULL, 0, "start"};
        nodes[0] = start;
        Node end = {0, NULL, 0, "end"};
        nodes[1] = end;
    }

    while (1) {
        bool passed_first = 0;
        char buf[2][100];
        size_t len[2] = {0, 0};

        char c;
        while ((c = fgetc(fp)) != EOF && c != '\n') {
            if (c == '-')
                passed_first = 1;
            else
                buf[passed_first][len[passed_first]++] = c;
        }
        if (len[0] && len[1]) {
            buf[0][len[0]] = '\0';
            buf[1][len[1]] = '\0';

            ssize_t in_nodes[2] = {-1, -1};
            for (size_t i = 0; i < num_nodes; i++)
                for (size_t j = 0; j < 2; j++)
                    if (strcmp(buf[j], nodes[i].name) == 0)
                        in_nodes[j] = i;
            
            for (size_t fs = 0; fs < 2; fs++) {
                if (in_nodes[fs] != -1) {
                    nodes[in_nodes[fs]].relations = realloc(
                        nodes[in_nodes[fs]].relations,
                        (++nodes[in_nodes[fs]].num_relations) * sizeof(Node *)
                    );
                } else {
                    Node n = {
                        1,
                        malloc(sizeof(Node *)),
                        is_big(buf[fs]),
                        ""
                    };

                    memcpy(n.name, buf[fs], len[fs] + 1);

                    nodes = realloc(nodes, (++num_nodes) * sizeof(Node));
                    nodes[num_nodes - 1] = n;
                    in_nodes[fs] = num_nodes - 1;
                }

            }
            

            for (size_t fs = 0; fs < 2; fs++) {
                nodes[in_nodes[fs]].relations[
                    nodes[in_nodes[fs]].num_relations - 1
                ] = in_nodes[!fs];
            }

        }
        if (c == EOF)
            break;
    }



    for (size_t i = 0; i < num_nodes; i++) {
        printf("%s:\n", nodes[i].name);
        for (size_t j = 0; j < nodes[i].num_relations; j++) {
            Node n = nodes[nodes[i].relations[j]];
            printf("\t%s %c\n", n.name, n.big ? 'B' : 's');
        }
    }

    printf("%d\n", num_paths(0, nodes, NULL, 0, 0));

    for (size_t i = 0; i < num_nodes; i++)
        free(nodes[i].relations);
    free(nodes);
    fclose(fp);
    return 0;
}