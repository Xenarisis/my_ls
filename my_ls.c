#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define OPT_A (1<<0)
#define OPT_T (1<<1)

typedef struct listnode {
    int size;
    struct stat last_modif;
    char *str;
    struct listnode* next;
} list;

void print(list *head) {
    list *tmp = head;
    while(tmp->str != NULL) {
        printf("%s\n", tmp->str);
        tmp = tmp->next;
    }
    // printf("\n");
}

void swap_node_str(list *head) {
    char *str = head->str;

    head->str = head->next->str;

    head->next->str = str;
}

void swap_node_time(list *head) {
    struct stat st = head->last_modif;

    head->last_modif = head->next->last_modif;

    head->next->last_modif = st;
}

void alphabetical_sorting(list *head) {
    list *tmp = head;
    int index = 0;

    while(index < head->size) {
        while (tmp->str != NULL && tmp->next->str != NULL) {
            if (strcmp(tmp->str, tmp->next->str) > 0) {
                swap_node_str(tmp);
            }
            tmp = tmp->next;
        }
        index++;
        tmp = head;
    }
}

void time_modification_sorting(list *head) {
    list *tmp = head;
    int index = 0;

    while(index < head->size) {
        while(tmp->str != NULL && tmp->next && tmp->next->str != NULL) {
            time_t t1 = tmp->last_modif.st_mtime;
            time_t t2 = tmp->next->last_modif.st_mtime;
            long ns1 = tmp->last_modif.st_mtim.tv_nsec;
            long ns2 = tmp->next->last_modif.st_mtim.tv_nsec;

            if (t1 < t2 || (t1 == t2 && ns1 < ns2)) {
                swap_node_str(tmp);
                swap_node_time(tmp);
            } else if(t1 == t2 && ns1 == ns2 && strcmp(tmp->str, tmp->next->str) > 0) {
                swap_node_str(tmp);
                swap_node_time(tmp);
            }
            tmp = tmp->next;
        }
        index++;
        tmp = head;
    }
}


void sort(int flag, list *head) {
    if(flag & OPT_T) {
        time_modification_sorting(head);
    } else {
        alphabetical_sorting(head);
    }
}

void flag_a_or_not(int flag, list *head, char *directory) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(directory);

    list *tmp = head;
    int size = 0;

    while ((entry = readdir(dir)) != NULL) {
        if(flag & OPT_A) {
            tmp->str = strdup(entry->d_name);
            tmp->next = malloc(sizeof(list));
            tmp = tmp->next;
            tmp->str = NULL;
            tmp->next = NULL;
            size++;
        } else if(*entry->d_name != '.') {
            tmp->str = strdup(entry->d_name);
            tmp->next = malloc(sizeof(list));
            tmp = tmp->next;
            tmp->str = NULL;
            tmp->next = NULL;
            size++;
        }
    }

    head->size = size;

    closedir(dir);
}

void flag_t(int flag, list *head, char *directory) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(directory);

    list *tmp = head;
    char full_path[4096];

    while ((entry = readdir(dir)) != NULL && tmp != NULL) {
        if ((flag & OPT_A) || entry->d_name[0] != '.') {
            snprintf(full_path, sizeof(full_path), "%s/%s", directory, entry->d_name);
            stat(full_path, &tmp->last_modif);
            tmp = tmp->next;
        }
    }

    closedir(dir);
}


int get_flag(char *str) {
    int flag = 0;
    while(*str) {
        if(*str == 'a') {
            flag |= OPT_A;
        }
        if(*str == 't') {
            flag |= OPT_T;
        }
        str += 1;
    }

    return flag;
}

int get_opt(int ac, char **av) {
    int flag = 0;
    int index = 0;

    while(index < ac) {
        if(*av[index] == '-') {
            flag += get_flag(av[index] + 1);
        }
        index += 1;
    }

    return flag;
}

int count_dirs(int ac, char **av) {
    int count = 0;
    int index = 1;
    while(index < ac) {
        if (av[index][0] != '-') {
            count++; 
        }
        index++;
    }

    return count;
}

void find_directory(int ac, char **av, list *head, int flag) {
    head->size = count_dirs(ac, av);

    if (head->size == 0) {
        head->str = ".";
        return;
    }

    list *tmp = head;
    char full_path[4096];
    int index = 1;
    while(index < ac) {
        if (av[index][0] != '-') {
            tmp->str = av[index];
            if(flag & OPT_T) {
                if(av[index][0] != '.' && av[index][0] != '/') {
                    snprintf(full_path, sizeof(full_path), "./%s", av[index]);
                    stat(full_path, &tmp->last_modif);
                } else {
                    stat(av[index], &tmp->last_modif);
                }
            }
            tmp->next = malloc(sizeof(list));
            tmp = tmp->next;
            tmp->str = NULL;
            tmp->next = NULL;
        }
        index++;
    }
    return;
}

char *concatenation(char *directory) {
    char *path;

    if(directory[0] != '.' && directory[0] != '/') {
        path = malloc(sizeof(char) * (strlen(directory) + 3));
        strcpy(path, "./");
        strcat(path, directory);
    } else {
        path = directory;
    }

    return path;
}

void print_directories(char* directory, int flag) {
    list *head = malloc(sizeof(list));
    head->str = NULL;
    head->next = NULL;
    char *path = concatenation(directory);

    flag_a_or_not(flag, head, path);

    if(flag & OPT_T) {
        flag_t(flag, head, path);
    }

    sort(flag, head);

    print(head);

    free(head);
}

int main(int ac, char** av) {
    int flag = get_opt(ac, av);
    list *head = malloc(sizeof(list));
    find_directory(ac, av, head, flag);
    sort(flag, head);

    int index = 0;
    list *tmp = head;
    if(count_dirs(ac, av) < 2) {
        print_directories(tmp->str, flag);
    } else {
        while(tmp->str != NULL) {
            if(index > 0) {
                printf("\n");
            }
            printf("%s:\n", tmp->str);
            print_directories(tmp->str, flag);
            tmp = tmp->next;
            index++;
        }
    }

    free(head);
   
    return 0;
}
