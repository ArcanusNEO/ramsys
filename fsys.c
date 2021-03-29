#include "fsys.h"

char* path(TreeNode* root, TreeNode* i, char ans[_BUFFER_SIZE], char buffer[_BUFFER_SIZE]) {
    char* t;

    memset(ans, 0, sizeof(char) * _BUFFER_SIZE);
    memset(buffer, 0, sizeof(char) * _BUFFER_SIZE);
    for (; i != root; i = i->father) {
        ans[0] = '/';
        strcpy(ans + 1, i->metadata.name);
        strcat(ans, buffer);
        t = ans, ans = buffer, buffer = t;   
    }
    strcpy(ans, root->metadata.name);
    strcat(ans, buffer);
    return ans;
}

void print_name(TreeNode* x, int depth) {
    unsigned i, j, k;
    char* buffer = malloc(sizeof(char) * 64);

    strcpy(buffer, ctime(&(x->metadata.time)));
    *strchr(buffer, '\n') = '\0';
    for (i = 0, j = x->metadata.size, k = 0; j >= 1024; ++i) {
        k = (unsigned)((j & 1023) / 102.4);
        j >>= 10;
    }
    printf("%4u.%u%c  %s ", j, k, "BKMGTPEZY???"[i], buffer);
    for (i = 1; i <= depth; ++i) {
        printf("  ");
    }
    printf("%s", x->metadata.name);
    if (x->metadata.type == _DIR) {
        putchar('/');
    }
    puts("");
}

void list_dir_preorder(TreeNode* x, int depth) {
    if (x != NULL) {
        print_name(x, depth);
        if (x->metadata.type == _DIR) {
            list_dir_preorder(x->firstChild, depth + 1);
        }
        list_dir_preorder(x->nextSibling, depth);
    }
}

void free_tree(TreeNode* x) {
    if (x != NULL) {
        free_tree(x->firstChild);
        free_tree(x->nextSibling);
        free(x);
    }
}

void lsall(TreeNode* x) {
    list_dir_preorder(x->firstChild, 0);
}

void list_current_dir(TreeNode* x) {
    for (x != NULL && (x = x->firstChild); x != NULL; x = x->nextSibling) {
        print_name(x, 0);
    }
}

void touch(TreeNode* x, const char* name) {
    if (x != NULL && x->metadata.type == _DIR && strlen(name) < _MAX_FILE_NAME_LENGTH && strchr(name, '/') == NULL) {
        TreeNode* y;
        TreeNode* i;
        
        if (x->firstChild != NULL) {
            for (i = x->firstChild; i->nextSibling != NULL && strcmp(i->metadata.name, name) != 0; i = i->nextSibling) {
                NULL;
            }
            if (strcmp(i->metadata.name, name) == 0) {
                time(&(i->metadata.time));
                return;
            } 
        }
        y = malloc(sizeof(TreeNode));
        y->father = x;
        y->firstChild = NULL;
        y->this = y;
        y->nextSibling = NULL;
        y->blob.size = 0;
        y->blob.data = NULL;
        y->metadata.type = _FILE;
        y->metadata.size = sizeof(TreeNode) + sizeof(Metadata) + sizeof(Blob) + y->blob.size;
        strcpy(y->metadata.name, name);
        time(&(y->metadata.time));
        if (x->firstChild == NULL) {
            x->firstChild = y;
        } else {
            i->nextSibling = y;
        }
    }
}

void mkdir(TreeNode* x, const char* name) {
    if (x != NULL && x->metadata.type == _DIR && strlen(name) < _MAX_FILE_NAME_LENGTH && strchr(name, '/') == NULL) {
        TreeNode* y;
        TreeNode* i;

        if (x->firstChild != NULL) {
            for (i = x->firstChild; i->nextSibling != NULL && strcmp(i->metadata.name, name) != 0; i = i->nextSibling) {
                NULL;
            }
            if (strcmp(i->metadata.name, name) == 0) {
                return;
            } 
        }
        y = malloc(sizeof(TreeNode));
        y->father = x;
        y->firstChild = NULL;
        y->this = y;
        y->nextSibling = NULL;
        y->blob.size = 0;
        y->blob.data = NULL;
        y->metadata.type = _DIR;
        y->metadata.size = sizeof(TreeNode) + sizeof(Metadata) + sizeof(Blob);
        strcpy(y->metadata.name, name);
        time(&(y->metadata.time));
        if (x->firstChild == NULL) {
            x->firstChild = y;
        } else {
            i->nextSibling = y;
        }
    }
}

void cd_tree_node(TreeNode** pwd, TreeNode* x) {
    if (x != NULL && x->metadata.type == _DIR) {
        *pwd = x;
    }
}

TreeNode* cd_name(TreeNode** pwd, char* name) {
    TreeNode* i = NULL;

    if (pwd == NULL) {
        return i;
    }
    if (strcmp(name, ".") == 0) {
        i = (*pwd)->this;
    } else if (strcmp(name, "..") == 0) {
        i = *pwd = (*pwd == NULL ? NULL : (*pwd)->father);
    } else if (*pwd != NULL) {
        i = (*pwd)->firstChild;
        for (; i != NULL; i = i->nextSibling) {
            if (strcmp(i->metadata.name, name) == 0) {
                *pwd = i;
                break;
            }
        }
    }
    return i;
}

TreeNode* cd_path(TreeNode* root, TreeNode** pwd, const char* path) {
    int i = 0, j;
    size_t len;
    char* name = malloc(sizeof(char) * _MAX_FILE_NAME_LENGTH);

    len = strlen(path);
    if (pwd == NULL || path == NULL) {
        return NULL;
    }
    if (*pwd == NULL) {
        *pwd = root;
    }
    if (path[0] == '/') {
        *pwd = root;
        i = 1;
    } else if (strstr(path, root->metadata.name) == path) {
        *pwd = root;
        i = 7;
    }
    for (j = i; i < len; ++i) {
        if (path[i] == '/' || i == len - 1 && ++i) {
            memcpy(name, path + j, sizeof(char) * (i - j));
            name[i - j] = '\0';
            cd_name(pwd, name);
            j = i + 1;
        }
    }
    free(name);
    return *pwd;
}

unsigned size_dir(TreeNode* root, TreeNode* x) {
    if (x == NULL) {
        return 0;
    }
    unsigned tot;
    char* ans;
    char* buffer;

    tot = x->metadata.size + size_dir(root, x->firstChild) + size_dir(root, x->nextSibling);
    ans = malloc(sizeof(char) * _BUFFER_SIZE);
    buffer = malloc(sizeof(char) * _BUFFER_SIZE);
    printf("%-16u%s%c\n", tot, path(root, x, ans, buffer), " /"[x->metadata.type == _DIR]);
    free(ans);
    free(buffer);
    return tot;
}
