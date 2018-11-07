#ifndef _HEADER_H
#define _HEADER_H

#include <string.h>
#include <stdio.h>
#include<stdlib.h>
#include <stdbool.h>
typedef unsigned short imgPos[2];

typedef struct treeNodeListCell treeNodeListCell;

typedef struct treeNode
{
	imgPos position;
	treeNodeListCell *next_possible_position;
}treeNode;

typedef struct grayImage
{
	unsigned short rows, cols;
	unsigned char **pixels;
}grayImage;

typedef struct treeNodeListCell
{
	treeNode * node;
	struct treeNodeListCell *next;
}treeNodeListCell;


typedef struct segment
{
	treeNode *root;
}Segment;

typedef struct imgPosCell
{
	imgPos position;
	struct imgPosCell *next, *prev;
}imgPosCell;

typedef struct imgPosList
{
	imgPosCell *head, *tail;
}imgPosList;

#define MAX_FILE_NAME 100

//1
grayImage *readPGM(char *fname);
//2
Segment findSingleSegment(grayImage *img, imgPos start, unsigned char threshold);
void findSingleSegmentRec(grayImage *img, imgPos pos, int threshold, treeNode * currentNode, bool * boolBoard, int val);
bool squareIsBlocked(grayImage *img, treeNode * currentNode, bool * boolBoard);
treeNode * createNodeAndInsertToEnd(treeNodeListCell ** possiblePos, imgPos pos);
Segment findSingleSegmentInAllFunction(grayImage *img, imgPos start, unsigned char threshold, bool **board);
void findSingleSegmentInAllFunctionRec(grayImage *img, imgPos pos, int threshold, treeNode * currentNode, bool * boolBoard, int val);
//3
int findAllSegments(grayImage *img, unsigned char threshold, imgPosList ** segment);
bool * fillBoard(bool *board, Segment temp, int rows);
bool * fillBoardTreeRec(bool *board, treeNode *t, int rows);
bool * fillBoardList(bool * board, treeNodeListCell * node, int rows);
imgPosList  buildListOfSegments(Segment temp);
void buildListRec(imgPosCell ** node, treeNode * root, imgPosList * lst);
void buildNodeRec(imgPosCell ** node, treeNodeListCell * listCell, imgPosList * lst);
void printBoolBoard(bool *board, int rows, int cols);
void buildPrevsAndTail(imgPosList * node);
//4
grayImage *colorSegments(imgPosList *segments, int size);
void findMaxRowAndCol(imgPosList * segment, int size, unsigned short * maxRow, unsigned short * maxCol);
grayImage * updatePixels(imgPosList *segment, int size, int current, grayImage * image);

//5
void saveCompressed(char *file_name, grayImage *image, unsigned char max_gray_level);
int howManyChars(int bitsNum, unsigned short numOfPixels);
unsigned char makePrev(unsigned char temp, int numOfBitsToCopy);
void initialBoot(FILE *file, grayImage *image, unsigned char max_gray_level);
//6
void convertCompressedImageToPGM(char *compressed_file_name, char *pgm_file_name);
unsigned char makePrevForExtract(unsigned char temp, int numOfBitsToCopy);
void  copyContentToPgm(grayImage* Img, FILE* fp, unsigned char max);
//7
void printMenu();
void freeAll(grayImage  *pgmNoColor, grayImage *pgmWithColor, imgPosList **list, int sizeOfList);
void freeAllListsHelp(imgPosCell *head);
void freeAllLists(imgPosList **list, int size);
void freeNode(treeNodeListCell * node);
void freeSegRec(treeNode * seg);
void freeSeg(Segment seg);
//Generic functions
void printSeg(Segment seg);
void printSegRec(treeNode * t);
void printNode(treeNodeListCell * node);
void printPixels(grayImage * img);
void printAllLists(imgPosList **list, int size);
void printAllListsHelp(imgPosCell *head);
int countBits(unsigned char max_gray_level);
int countBitsRec(int num);

#endif