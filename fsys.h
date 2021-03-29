#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef _FSYS_H
#define _FSYS_H

#define _MAX_FILE_NAME_LENGTH 64
#define _BUFFER_SIZE 1024

typedef struct STreeNode TreeNode;
typedef struct SMetadata Metadata;
typedef struct SBlob Blob;
typedef enum EFileType FileType;

enum EFileType {_DIR = 1, _FILE, _LINK};

struct SMetadata {
    char name[_MAX_FILE_NAME_LENGTH];
    unsigned size;
    time_t time;
    FileType type;
};

struct SBlob {
    unsigned size;
    void* data;
};

struct STreeNode {
    Metadata metadata;
    Blob blob;
    TreeNode* firstChild;
    TreeNode* nextSibling;
    TreeNode* father;
    TreeNode* this;
};

extern char* path(TreeNode* root, TreeNode* i, char ans[_BUFFER_SIZE], char buffer[_BUFFER_SIZE]);
extern void print_name(TreeNode* x, int depth);
extern void list_dir_preorder(TreeNode* x, int depth);
extern void free_tree(TreeNode* x);
extern void lsall(TreeNode* x);
extern void list_current_dir(TreeNode* x);
extern void touch(TreeNode* x, const char* name);
extern void mkdir(TreeNode* x, const char* name);
extern void cd_tree_node(TreeNode** pwd, TreeNode* x);
extern TreeNode* cd_name(TreeNode** pwd, char* name);
extern TreeNode* cd_path(TreeNode* root, TreeNode** pwd, const char* path);
extern unsigned size_dir(TreeNode* root, TreeNode* x);

#endif