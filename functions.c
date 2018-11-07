#include "header1.h"

void printSeg(Segment seg)
{
	printSegRec(seg.root);
}
void printSegRec(treeNode * t)
{
	if (t == NULL)
		return;
	else
	{
		printf("(%d,%d) ", t->position[0], t->position[1]);
		if (t->next_possible_position == NULL)
			return;
		printSegRec(t->next_possible_position->node);
		printNode(t->next_possible_position->next);
	}
}
void printNode(treeNodeListCell * node)
{
	if (node == NULL)
		return;
	else
	{
		printSegRec(node->node);
		printNode(node->next);
	}
}
Segment findSingleSegment(grayImage *img, imgPos start, unsigned char threshold)//îåöà ñâîðè àçã ìôé ð÷åãú ééçåñ
{
	bool *isFlaged = (bool *)calloc((img->rows) * (img->cols), sizeof(bool));
	int val;
	Segment * res = (Segment *)calloc(1, sizeof(Segment));
	res->root = (treeNode *)calloc(1, sizeof(treeNode));
	if (isFlaged == NULL || res == NULL || res->root == NULL)
		exit(1);
	res->root->position[0] = start[0];
	res->root->position[1] = start[1];
	isFlaged[start[0] * img->cols + start[1]] = true;
	val = img->pixels[0][start[0] * img->cols + start[1]];
	findSingleSegmentRec(img, res->root->position, (int)(threshold), res->root, isFlaged, val);
	return *res;
}
void findSingleSegmentRec(grayImage *img, imgPos pos, int threshold, treeNode * currentNode, bool * boolBoard, int val)
{
	int num,x,y;
	if (squareIsBlocked(img, currentNode, boolBoard))
		return;

	for (x = currentNode->position[0] - 1; x <= currentNode->position[0] + 1; x++)
	{

		for (y = currentNode->position[1] - 1; y <= currentNode->position[1] + 1; y++)
		{
			if (y >= 0 && y < img->cols && x >= 0 && x < img->rows)
			{
				if (boolBoard[x*img->cols + y] == false)
				{
					num = img->pixels[0][x*img->cols + y];
					if (num <= val + threshold && num >= val - threshold)
					{
						imgPos newPos;
						newPos[0] = x;
						newPos[1] = y;
						boolBoard[x*img->cols + y] = true;
						treeNode *last = createNodeAndInsertToEnd(&(currentNode->next_possible_position), newPos);
						findSingleSegmentRec(img, newPos, threshold, last, boolBoard, val);
					}
					boolBoard[x*img->cols + y] = true;
				}
			}
		}
	}

}
bool squareIsBlocked(grayImage *img, treeNode * currentNode, bool * boolBoard)
{
	int counter = 0;
	int x;
	int y;
	for (x = currentNode->position[0] - 1; x <= currentNode->position[0] + 1; x++)
	{
		if (x < 0 || x >= img->rows)
		{
			counter += 3;
		}
		else
		{
			for (y = currentNode->position[1] - 1; y <= currentNode->position[1] + 1; y++)
			{
				if (y < 0 || y >= img->cols)
					counter++;
				else
					if (boolBoard[x*img->cols + y] == true)
						counter++;
			}
		}
	}
	if (counter == 9)
		return true;
	else
		return false;
}
treeNode * createNodeAndInsertToEnd(treeNodeListCell ** possiblePos, imgPos pos)
{
	if (*possiblePos == NULL)
	{
		*possiblePos = (treeNodeListCell *)calloc(1, sizeof(treeNodeListCell));
		(*possiblePos)->node = (treeNode *)calloc(1, sizeof(treeNode));
		if (*possiblePos == NULL || (*possiblePos)->node == NULL)
			exit(1);
		(*possiblePos)->node->position[0] = pos[0];
		(*possiblePos)->node->position[1] = pos[1];
		return (*possiblePos)->node;

	}
	else
	{
		treeNodeListCell * run = *possiblePos;
		while (run->next != NULL)
			run = run->next;
		run->next = (treeNodeListCell *)calloc(1, sizeof(treeNodeListCell));
		if (run->next == NULL)
			exit(1);
		run = run->next;
		run->node = (treeNode *)calloc(1, sizeof(treeNode));
		if (run->node == NULL)
			exit(1);
		run->node->position[0] = pos[0];
		run->node->position[1] = pos[1];
		return run->node;
	}

}
int findAllSegments(grayImage *img, unsigned char threshold, imgPosList ** segment)
{
	int i, j, size = 1;
	imgPos check;
	bool *board = (bool *)calloc((img->cols)*(img->rows), sizeof(bool));
	if (board == NULL)
		exit(1);
	for (i = 0;i < img->rows;i++)
	{
		check[0] = i;
		for (j = 0;j < img->cols;j++)
		{
			check[1] = j;
			if (board[i*img->cols + j] == false)
			{
				(*segment) = (imgPosList *)realloc((*segment), size * sizeof(imgPosList));
				if ((*segment) == NULL)
					exit(1);
				Segment temp = findSingleSegmentInAllFunction(img, check, threshold, &board);
				segment[0][size - 1] = buildListOfSegments(temp);
				freeSeg(temp);
				size++;
			}
		}
	}
	free(board);
	return size - 1;
}
Segment findSingleSegmentInAllFunction(grayImage *img, imgPos start, unsigned char threshold, bool **board)//îåöà ñâîðè àçã ìôé ð÷åãú ééçåñ åîòãëï àú äìåç
{
	int val;
	Segment res;
	res.root = (treeNode *)calloc(1, sizeof(treeNode));
	if (res.root == NULL)
		exit(1);
	res.root->position[0] = start[0];
	res.root->position[1] = start[1];
	(*board)[start[0] * img->cols + start[1]] = true;
	val = img->pixels[0][start[0] * img->cols + start[1]];
	findSingleSegmentInAllFunctionRec(img, res.root->position, (int)(threshold), res.root, (*board), val);
	return res;
}
void findSingleSegmentInAllFunctionRec(grayImage *img, imgPos pos, int threshold, treeNode * currentNode, bool * boolBoard, int val)
{
	int num,x,y;
	if (squareIsBlocked(img, currentNode, boolBoard))
		return;

	for (x = currentNode->position[0] - 1; x <= currentNode->position[0] + 1; x++)
	{

		for (y = currentNode->position[1] - 1; y <= currentNode->position[1] + 1; y++)
		{
			if (y >= 0 && y < img->cols && x >= 0 && x < img->rows)
			{
				if (boolBoard[x*img->cols + y] == false)
				{
					num = img->pixels[0][x*img->cols + y];
					if (num <= val + threshold && num >= val - threshold)
					{
						imgPos newPos;
						newPos[0] = x;
						newPos[1] = y;
						boolBoard[x*img->cols + y] = true;
						treeNode *last = createNodeAndInsertToEnd(&(currentNode->next_possible_position), newPos);
						findSingleSegmentInAllFunctionRec(img, newPos, threshold, last, boolBoard, val);
					}
				}
			}
		}
	}

}
bool * fillBoard(bool *board, Segment temp, int cols)
{
	return fillBoardTreeRec(board, temp.root, cols);
}
bool * fillBoardTreeRec(bool *board, treeNode *t, int cols)
{
	if (t == NULL)
		return NULL;
	else
	{
		board[t->position[0] * cols + t->position[1]] = 1;
		if (t->next_possible_position == NULL)
			return board;
		board = fillBoardTreeRec(board, t->next_possible_position->node, cols);
		board = fillBoardList(board, t->next_possible_position->next, cols);
		return board;
	}

}
void printBoolBoard(bool *board, int rows, int cols)
{
	int i, j;
	for (i = 0;i < rows;i++)
	{
		for (j = 0;j < cols;j++)
		{
			if (board[i * cols + j] == 1)
				printf("1 ");
			else
				printf("0 ");
		}
		printf("\n");
	}
}
bool * fillBoardList(bool * board, treeNodeListCell * node, int cols)
{
	if (node == NULL)
		return board;
	else
	{
		board = fillBoardTreeRec(board, node->node, cols);
		board = fillBoardList(board, node->next, cols);
		return board;
	}
}
imgPosList  buildListOfSegments(Segment temp)
{
	imgPosList res;
	buildListRec(&(res.head), temp.root, &res);
	if (res.head == NULL)
		exit(1);
	buildPrevsAndTail(&(res));
	return res;
}
void buildPrevsAndTail(imgPosList * node)
{
	imgPosCell *run1, *run2;
	run1 = run2 = (node)->head;
	while (run2 != NULL)
	{
		run2 = run2->next;
		if (run2 != NULL)
			run2->prev = run1;
		else
			(node)->tail = run1;
		run1 = run2;
	}
}
void buildListRec(imgPosCell ** node, treeNode * root, imgPosList * lst)
{
	if (root == NULL)
		return;
	else
	{
		(*node) = (imgPosCell *)calloc(1, sizeof(imgPosCell));
		if ((*node) == NULL)
			exit(1);
		(*node)->position[0] = root->position[0];
		(*node)->position[1] = root->position[1];
		if ((*lst).head == NULL)
		{
			((*lst).head = (*node));
			((*lst).tail = (*node));
		}
		else
			((*lst).tail = (*node));
		if (root->next_possible_position == NULL)
			return;
		buildListRec(&((*node)->next), root->next_possible_position->node, &(*lst));
		if ((*node)->next != NULL)
			buildNodeRec(&((*lst).tail), root->next_possible_position->next, &(*lst));
		else
			buildNodeRec(&((*node)->next), root->next_possible_position->next, &(*lst));
	}
}
void buildNodeRec(imgPosCell ** node, treeNodeListCell * listCell, imgPosList * lst)
{
	if (node == NULL || listCell == NULL)
		return;
	else
	{
		imgPosCell *run = (*node);
		buildListRec(&((*node)->next), listCell->node, lst);
		buildNodeRec(&((*node)->next), listCell->next, lst);
	}
}
grayImage *readPGM(char *fname)//î÷áìú ùí ùì ÷åáõ áôåøîè ôéâéàí åáåðä âøééàéîâ
{
	grayImage * newPGM;
	int num, i, j;
	newPGM = (grayImage *)calloc(1, sizeof(grayImage));
	if (newPGM == NULL)
		exit(1);
	newPGM->pixels = (unsigned char **)calloc(1, sizeof(unsigned char*));
	if (newPGM->pixels == NULL)
		exit(1);
	FILE * file = fopen(fname, "r");
	if (file == NULL)
	{
		printf("There is no such file");
		return NULL;
	}
	fseek(file, 2 * sizeof(char), SEEK_CUR);

	fscanf(file, "%d ", &(num));
	newPGM->cols = num;
	fscanf(file, "%d ", &(num));
	newPGM->rows = num;
	fscanf(file, "%d ", &(num));
	*(newPGM->pixels) = (unsigned char *)calloc((newPGM->rows)*(newPGM->cols), sizeof(unsigned char));
	if (*(newPGM->pixels) == NULL)
		exit(1);
	for (i = 0;i < newPGM->rows;i++)
	{
		for (j = 0;j < newPGM->cols;j++)
		{
			fscanf(file, "%d ", &(num));
			newPGM->pixels[0][i*(newPGM->cols) + j] = num;
		}
	}
	printf("PGM file successfully loaded into memory\n");
	fclose(file);
	return newPGM;
}
grayImage *colorSegments(imgPosList *segments, int size)
{
	int i;	
	grayImage * resImg = (grayImage *)calloc(1, sizeof(grayImage));
	if (resImg == NULL)
		exit(1);
	findMaxRowAndCol(segments, size, &resImg->rows, &resImg->cols);
	resImg->pixels = (unsigned char **)calloc(1, sizeof(unsigned char *));
	*(resImg->pixels) = (unsigned char *)calloc((resImg->rows) * (resImg->cols), sizeof(unsigned char));
	if (resImg->pixels == NULL || *(resImg->pixels) == NULL)
		exit(1);
	for (i = 0; i < size; i++)
		resImg = updatePixels(segments + i, size, i, resImg);
	return resImg;
}
grayImage * updatePixels(imgPosList *segment, int size, int current, grayImage * image)
{
	double newGrey;
	newGrey = (current * (255.0 / (size - 1)));
	imgPosCell *run = segment->head;
	int col = image->cols;
	while (run != NULL)
	{
		image->pixels[0][run->position[0] * col + run->position[1]] = (int)newGrey;
		run = run->next;
	}
	return image;
}
void findMaxRowAndCol(imgPosList * segment, int size, unsigned short * maxRow, unsigned short * maxCol)
{
	int i;	
	(*maxRow) = (unsigned short)segment[0].head->position[0];
	(*maxCol) = (unsigned short)segment[0].head->position[1];
	imgPosCell * traveler;
	for (i = 0; i < size; i++)
	{
		traveler = segment[i].head;
		while (traveler != NULL)
		{
			if (traveler->position[0] >(*maxRow))
				(*maxRow) = traveler->position[0];
			if (traveler->position[1] > (*maxCol))
				(*maxCol) = traveler->position[1];
			traveler = traveler->next;
		}
	}
	(*maxRow) += 1;
	(*maxCol) += 1;
}
void printPixels(grayImage * img)
{
	int i,j;	
	for (i = 0; i < img->rows; i++)
	{
		for (j = 0; j < img->cols; j++)
			printf("%4d ", img->pixels[0][i * img->cols + j]);
		printf("\n");
	}
}
void printAllLists(imgPosList **list, int size)
{
	int i;
	for (i = 0;i < size;i++)
	{
		printf("List number %d is:", i + 1);
		printAllListsHelp(list[0][i].head);
		printf("\n");
	}
}
void printAllListsHelp(imgPosCell *head)
{
	while (head != NULL)
	{
		printf("(%d,%d) ", head->position[0], head->position[1]);
		head = head->next;
	}
}
void saveCompressed(char *file_name, grayImage *image, unsigned char max_gray_level)
{
	int i, j, nextAP = 8, leftOverBits = 0, numOfBits;
	unsigned char write = 0, temp, leftOver = 0;
	numOfBits = countBits(max_gray_level);
	FILE* binFile = fopen(file_name, "wb");
	if (binFile == NULL)
	{
		printf("didnt open");
		exit(1);
	}
	initialBoot(binFile, image, max_gray_level);
	for (i = 0; i<image->rows; i++)
	{
		for (j = 0; j < image->cols; j++)
		{
			if (leftOverBits != 0)
			{
				write = write | leftOver;
				nextAP -= leftOverBits;
				leftOverBits = 0;
			}
			temp = (image->pixels[0][i*image->cols + j] * max_gray_level) / 255;
			if (nextAP >= numOfBits)
			{
				temp = temp << (nextAP - numOfBits);
				write = temp | write;
				nextAP -= numOfBits;
			}
			else
			{
				leftOverBits = numOfBits - nextAP;
				leftOver = makePrev(temp, leftOverBits);
				temp = temp >> (leftOverBits);
				write = write | temp;
				nextAP = 0;
			}
			if (nextAP < 1)
			{
				fwrite(&write, sizeof(unsigned char), 1, binFile);
				nextAP = 8;
				write = 0;
			}
		}
	}
	if (nextAP == 8)
		fwrite(&leftOver, sizeof(unsigned char), 1, binFile);
	else
		fwrite(&write, sizeof(unsigned char), 1, binFile);
	fclose(binFile);
}
void initialBoot(FILE *file, grayImage *image, unsigned char max_gray_level)
{
		fwrite(&(image->cols), sizeof(unsigned short), 1, file);
		fwrite(&(image->rows), sizeof(unsigned short), 1, file);
		fwrite(&max_gray_level, sizeof(unsigned char), 1, file);
		fwrite("\n", sizeof(unsigned char), 1, file);
}
unsigned char makePrev(unsigned char temp, int numOfBitsToCopy)
{
	unsigned char mask = 255;
	mask = mask >> (8 - numOfBitsToCopy);
	unsigned char res = mask & temp;
	res = res << (8 - numOfBitsToCopy);
	return res;
}

int countBits(unsigned char max_gray_level)
{
	int num = max_gray_level;
	return countBitsRec(num);
}
int countBitsRec(int num)
{
	int count;
	if (num == 1 || num == 0)
		return 1;
	else
	{
		count = countBitsRec(num / 2);
		return count + 1;
	}
}
void convertCompressedImageToPGM(char *compressed_file_name, char *pgm_file_name)
{
	int i, j,bitsNum, indexL = 8, indexR;
	unsigned char maxGrayLevel, temp, leftOver = 0, write = 0;
	grayImage *picture;
	FILE* binFile = fopen(compressed_file_name, "rb");     
	FILE* pgm = fopen(pgm_file_name, "w");
	if (binFile == NULL)
	{
		printf("File Didnt open");
		exit(1);
	}
	if (pgm == NULL)
	{
		printf("File Didnt open");
		exit(1);
	}
	picture = (grayImage *)calloc(1, sizeof(grayImage));
	if (picture == NULL)
		exit(1);

	fread(&(picture->cols), sizeof(unsigned short), 1, binFile);
	fread(&(picture->rows), sizeof(unsigned short), 1, binFile);
	fread(&(maxGrayLevel), sizeof(unsigned char), 1, binFile);
	fseek(binFile, 1, SEEK_CUR);

	bitsNum = countBits(maxGrayLevel);
	indexR = 8 - bitsNum;

	picture->pixels = (unsigned char**)calloc(1, sizeof(unsigned char*));
	*(picture->pixels) = (unsigned char*)calloc(picture->rows*picture->cols, sizeof(unsigned char));
	if (picture->pixels == NULL)
	{
		exit(1);
	}
	if (*(picture->pixels) == NULL)
	{
		exit(1);
	}

	fread(&temp, sizeof(unsigned char), 1, binFile);
	for (i = 0; i < picture->rows; i++)
	{
		for (j = 0; j < picture->cols; j++)
		{
			if (indexL - indexR<bitsNum)
				fread(&temp, sizeof(unsigned char), 1, binFile);

			if (indexL - indexR > 0)
			{
				if (indexL - indexR == bitsNum)
					leftOver = 0;
				write = temp;
				write = write << (8 - indexL);
				write = write >> (8 - indexL + indexR);

				leftOver = makePrevForExtract(leftOver, indexR); 
				leftOver = leftOver << indexL;
				write = write | leftOver;

			}
			else
			{
				write = leftOver << (bitsNum - indexL);
				leftOver = temp >> indexR;
				write = write | leftOver;
			}
			leftOver = makePrevForExtract(temp, indexR);
			picture->pixels[0][i*picture->cols + j] = (write * 255) / maxGrayLevel;
			indexL = indexR;
			indexR -= bitsNum;
			if (indexR < 0)
				indexR = 8 + indexR;
		}
	}
	copyContentToPgm(picture, pgm, maxGrayLevel);
	free(*(picture->pixels));
	free(picture->pixels);
	free(picture);
	fclose(pgm);
	fclose(binFile);

}
unsigned char makePrevForExtract(unsigned char temp, int numOfBitsToCopy)
{
	unsigned char mask = 255;
	mask = mask >> (8 - numOfBitsToCopy);
	unsigned char res = mask & temp;
	return res;
}
void  copyContentToPgm(grayImage* Img, FILE* file, unsigned char max)
{
	fputs("P2\n", file);
	fprintf(file, "%d %d\n", Img->cols, Img->rows);
	fprintf(file, "%d\n", max);
	int i, j;
	for (i = 0; i < Img->rows; i++)
	{
		for (j = 0; j < Img->cols; j++)
		{
			fprintf(file, "%2d ", Img->pixels[0][i*Img->cols + j]);
		}
		fputs("\n", file);
	}
}
void printMenu()
{
	printf("Please choose one of the options:\n\n");
	printf("1 .Read an image file in PGM format\n");
	printf("2 .find all segments\n");
	printf("3 .Color the segments\n");
	printf("4 .Save the colored image in a compressed format\n");
	printf("5 .Compress and save the original image in a compressed format\n");
	printf("6 .Convert a compressed image to PGM format\n");
	printf("7 .Exit\n\n");
}
void freeAll(grayImage  *pgmNoColor, grayImage *pgmWithColor, imgPosList **list,int sizeOfList)
{
	if (pgmNoColor != NULL)
	{
		free(*(pgmNoColor->pixels));
		free(pgmNoColor->pixels);
		free(pgmNoColor);
	}
	if (pgmWithColor != NULL)
	{
		free(*(pgmWithColor->pixels));
		free(pgmWithColor->pixels);
		free(pgmWithColor);
	}
	if (list != NULL)
	{
		freeAllLists(list,sizeOfList);
	}
	
}

void freeAllLists(imgPosList **list, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		freeAllListsHelp(list[0][i].head);
	}
	free(*list);
	free(list);
}
void freeAllListsHelp(imgPosCell *head)
{
	if (head == NULL)
		free(head);
	else
	{
		freeAllListsHelp(head->next);
		free(head);
	}
}

void freeSeg(Segment seg)
{
	freeSegRec(seg.root);
	
}
void freeSegRec(treeNode * seg)
{
	if (seg == NULL)
		free(seg);
	else
	{
		if (seg->next_possible_position == NULL)
		{
			free(seg->next_possible_position);
			free(seg);
			return;
		}
		freeSegRec(seg->next_possible_position->node);
		freeNode(seg->next_possible_position->next);
		free(seg->next_possible_position);
		free(seg);
	}
}
void freeNode(treeNodeListCell * node)
{
	if (node == NULL)
		free(node);
	else
	{
		freeSegRec(node->node);
		freeNode(node->next);
		free(node);
	}
}
