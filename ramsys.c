#include "fsys.h"

int main(int argc, char* argv[]) {
    TreeNode* root = malloc(sizeof(TreeNode));
    TreeNode* pwd;
    char* buffer = malloc(sizeof(char) * _BUFFER_SIZE);
    char* arg = malloc(sizeof(char) * _BUFFER_SIZE);

    srand((unsigned)time(NULL)), rand(), rand(), rand();
    strcpy(root->metadata.name, "rootfs");
    root->metadata.type=_DIR;
    root->father = root;
    root->firstChild = NULL;
    root->nextSibling = NULL;
    root->this = root;
    root->metadata.size = 1;
    time(&(root->metadata.time));
    cd_tree_node(&pwd, root);

    while (1) {
        memset(buffer, 0, sizeof(char) * _BUFFER_SIZE);
        memset(arg, 0, sizeof(char) * _BUFFER_SIZE);
        printf("> ");
        fgets(buffer, _BUFFER_SIZE, stdin);
        if (~sscanf(buffer, "%s", arg)) {
            if (strcmp(arg, "exit") == 0) {
                break;
            } else if (strcmp(arg, "lsall") == 0) {
                lsall(pwd);
            } else if (strcmp(arg, "mkdir") == 0) {
                if (~sscanf(buffer + strlen(arg), "%s", arg)) {
                    mkdir(pwd, arg);
                }
            } else if (strcmp(arg, "touch") == 0) {
                if (~sscanf(buffer + strlen(arg), "%s", arg)) {
                    touch(pwd, arg);
                }
            } else if (strcmp(arg, "cd") == 0) {
                if (~sscanf(buffer + strlen(arg), "%s", arg)) {
                    cd_path(root, &pwd, arg);
                }
            } else if (strcmp(arg, "pwd") == 0) {
                puts(path(root, pwd, buffer, arg));
            } else if (strcmp(arg, "du") == 0) {
                size_dir(root, pwd);
            } else if (strcmp(arg, "ls") == 0) {
                list_current_dir(pwd);
            }
        }
    }

    free_tree(root);
    free(buffer);
    free(arg);
    return 0;
}