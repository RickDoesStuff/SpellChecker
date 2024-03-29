#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "spchk.h"
#include "linestream.h"
#include "binaryTree.h"
#include "filestream.h"

/**
 * basic string compare function used in qsort
 */
int compare(const void *num1, const void *num2)
{
    return strcmp(*(char **)num1, *(char **)num2);
}
int main(int argc, char **argv)
{
    // assume no miss spelled Words
    int missSpelledWord = 1;

    // file extention
    const char *fileExt = ".txt";
    // to keep track of how many files were found
    int numFiles=0;

    // file names
    char *fname = argc > 1 ? argv[1] : "dict.txt";
    char *fparagraph = argc > 2 ? argv[2] : "paragraph.txt";
    char *fdirectory = argc > 3 ? argv[3] : NULL;

    // open dictionary file
    int fileDesc = open(fname, O_RDONLY);
    if (fileDesc < 0)
    {
        printf("%s\n",fname);
        exit(EXIT_FAILURE);
    }

    // open paragraph file
    int fileDesc2 = open(fparagraph, O_RDONLY);
    if (fileDesc2 < 0)
    {
        printf("%s\n",fparagraph);
        exit(EXIT_FAILURE);
    }

    // create lines struct and innit
    lines_t lines;
    ldinit(&lines, fileDesc);

    // create the current line pointer
    char *curLine;

    // word counter
    int wordCount = 0;

    // location of word in paragraph

    int arrsize = 100;
    char **wordArr = malloc(arrsize * sizeof(char *));
    char **wordArrCAPS = malloc(arrsize * sizeof(char *));

    // load the dictionary
    while ((curLine = next_word(&lines, NULL, NULL, NULL, NULL)))
    {
        if (curLine == NULL)
        {
            // curline is a blank line in the file
            printf("blank line\n");
            continue;
        }

        DEBUG LOG("word:::%d:%s\n", wordCount, curLine);
        // or make an insert fuction to insert directly into the tree

        wordArr[wordCount] = strdup(curLine);

        // make the words capital as well
        char tempWord[4096];
        int i;
        for (i = 0; curLine[i]; i++)
        {
            char chr = curLine[i];
            tempWord[i] = toupper(chr);
        }
        tempWord[strlen(curLine)] = '\0';
        wordArrCAPS[wordCount] = strdup(tempWord);

        wordCount++;

        // if we run out of room in our array
        if (wordCount == arrsize)
        {
            DEBUG LOG("Bigger*2\n");
            arrsize = arrsize * 2;
            wordArrCAPS = realloc(wordArrCAPS, sizeof(char *) * arrsize);
            wordArr = realloc(wordArr, sizeof(char *) * arrsize);
        }
    }
    lddestroy(&lines);

    DEBUG LOG("\n\n\n");
    // create the binary tree
    // printf("\nThe non sorted array: ");
    // printf("\n[");
    // for(int i = 0; i < wordCount; i++)
    // {
    //     printf("%s, ", wordArr[i]);
    // }
    // printf("]");
    qsort(wordArr, wordCount, sizeof(char*), compare);
    qsort(wordArrCAPS, wordCount, sizeof(char*), compare);

    // printf("\nThe sorted array: ");
    // printf("\n[ %s", wordArr[0]);
    // for(int i = 1; i < wordCount; i++)
    // {
    //     printf(", %s", wordArr[i]);
    // }
    // printf(" ]\n");

    struct BinaryTreeNode *tree = buildBalancedBST(wordArr, 0, wordCount - 1);
    DEBUG preOrder(tree);
    DEBUG printf("\n\n\n");
    // create the CAPS binary tree

    struct BinaryTreeNode *treeCaps = buildBalancedBST(wordArrCAPS, 0, wordCount - 1);
    DEBUG preOrder(treeCaps);
    DEBUG printf("\n\n\n");

    // read the paragraph file
    missSpelledWord = wordDictCompare(tree,treeCaps,fparagraph);

    if (fdirectory != NULL)
    {
        // load all the file paths found from the subdirectories of the given directory        
        char **filePaths = searchFiles(fdirectory, fileExt, &numFiles);
        // filePaths are not null (there are file paths)
        if (filePaths != NULL)
        {
            // loops through each file opening and reading it
            for (int i = 0; i < numFiles; i++)
            {
                DEBUG LOG("Checking path:%s\n",filePaths[i]);
                // if there is atleast 1 miss spelled word, set the boolean to false
                if ((wordDictCompare(tree,treeCaps,filePaths[i])) == 0)
                {
                    missSpelledWord = 0;
                }
                
            }
        } else {
            DEBUG LOG("file path is null!!\n");
        }
    }

    DEBUG LOG("\n\n\n");
    DEBUG preOrder(tree);
    
    if (missSpelledWord){
        //printf("\n\nsuccess");
        return(EXIT_SUCCESS);
    }
        //printf("\n\nfailure(miss spelled words)");
    return(EXIT_FAILURE);
    //return EXIT_SUCCESS;
}


/**
 * return 1 if all words are spelled correctly
*/
int wordDictCompare(struct BinaryTreeNode *tree, struct BinaryTreeNode *treeCaps, char* path){
    int row = 1;
    int col = 1;
    int actualRow = 1;
    int actualCol = 1;

    char *curLine;
    int spelledCorrect = 1;

    int fileDesc = open(path, O_RDONLY);
    if (fileDesc < 0)
    {
        printf("%s\n",path);
        exit(EXIT_FAILURE);
    }

    lines_t lines;
    ldinit(&lines, fileDesc);

    while ((curLine = next_word(&lines, &row, &col, &actualRow, &actualCol)))
    {
        if (strlen(curLine) == 0)
        {
            // curline is a blank line in the file
            DEBUG printf("blank line\n");
            continue;
        }
        DEBUG printf("\n**********\nword:::%s\n", curLine);
        if (searchDict(tree, treeCaps, curLine) == 0)
        {
            // my_files/baz/bar.txt (8,19): almost-correkt
            printf("%s (%i,%i): %s\n", path, row, col, curLine);
            spelledCorrect = 0;
        }
    }
    lddestroy(&lines);

    return spelledCorrect;
}