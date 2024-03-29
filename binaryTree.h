#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef __DEBUG
#define __DEBUG 
#endif

#define DEBUG if(__DEBUG)
#define LOG printf

/**
 * key:char* the string
 * left:BinaryTreeNode* the left node
 * right:BinaryTreeNode* the right node
*/
struct BinaryTreeNode {
    char *key;
    struct BinaryTreeNode *left, *right;
};

// Function prototypes
struct BinaryTreeNode* newNodeCreate(char* value);
struct BinaryTreeNode* insertNode(struct BinaryTreeNode* node, char* value);
void preOrder(struct BinaryTreeNode* root);
struct BinaryTreeNode* buildBalancedBST(char **words, int start, int end);
struct BinaryTreeNode* searchBST(struct BinaryTreeNode* root, char* value);
int searchDict(struct BinaryTreeNode *tree,struct BinaryTreeNode *treeCaps, char* word);
int searchHyphenatedWord(struct BinaryTreeNode *tree, struct BinaryTreeNode *treeCaps, char *word);
int matchTree(struct BinaryTreeNode *tree, struct BinaryTreeNode *treeCaps, char *word);
int checkUppercase(struct BinaryTreeNode *tree, struct BinaryTreeNode *treeCaps, char *word);
int searchPartOfHyphenWord(struct BinaryTreeNode *tree, struct BinaryTreeNode *treeCaps, char* word);


#endif // BINARYTREE_H
