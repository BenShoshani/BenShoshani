#include "header1.h"

void main()
{

	bool stop = false;
	int ans, sizeOfAllSegments = 0, temp;
	char fileName[101], compressedNewFileName[101], compressedMaxGrayLevel = 0;
	grayImage *pgmNoColor = NULL, *pgmWithColor=NULL;
	unsigned char threshold;
	imgPosList ** segments=NULL;

	while (stop == false)
	{
		printMenu();
		scanf("%d", &ans);
		switch (ans)
		{
		case 1:
		{
			printf("Please enter the PGM file name\n ");
			scanf("%s", fileName);
			pgmNoColor = readPGM(fileName);
			printPixels(pgmNoColor);
			break;
		}
		case 2:
		{
			if (pgmNoColor == NULL)
			{
				printf("Please enter the PGM file name\n ");
				scanf("%s", fileName);
				pgmNoColor = readPGM(fileName);
				printPixels(pgmNoColor);

			}
			pgmWithColor = pgmNoColor;
			printf("Please enter the threshold for the segemnts :\n");
			scanf("%d", &temp);
			threshold = temp;
			segments = (imgPosList **)calloc(1, sizeof(imgPosList *));
			if (segments == NULL)
				exit(1);
			sizeOfAllSegments = findAllSegments(pgmWithColor, threshold, segments);
			if (sizeOfAllSegments != 0)
				printf("All the segments in the picture were found\n\n");
			break;
		}
		case 3:
		{
			if (sizeOfAllSegments == 0)
			{
				if (pgmNoColor == NULL)
				{
					printf("Please enter the PGM file name\n ");
					scanf("%s", fileName);
					pgmNoColor = readPGM(fileName);
					printPixels(pgmNoColor);

				}
				pgmWithColor = pgmNoColor;
				printf("Please enter the threshold for the segemnts :\n");
				scanf("%d", &temp);
				threshold = temp;
				segments = (imgPosList **)calloc(1, sizeof(imgPosList *));
				if (segments == NULL)
					exit(1);
				sizeOfAllSegments = findAllSegments(pgmWithColor, threshold, segments);
				if (sizeOfAllSegments != 0)
					printf("All the segments in the picture were found\n\n");
			}
			pgmWithColor = colorSegments(*segments, sizeOfAllSegments);
			if (pgmWithColor != NULL)
			{
				printf("Coloring the segment in the picture was successfully completed\n\n");
				printf("The PGM matrix with the colord segments:\n");
				printPixels(pgmWithColor);
			}
				break;
		}
		case 4:
		{
			if (sizeOfAllSegments == 0)
			{
				if (pgmNoColor == NULL)
				{
					printf("Please enter the PGM file name\n ");
					scanf("%s", fileName);
					pgmNoColor = readPGM(fileName);
					printPixels(pgmNoColor);

				}
				pgmWithColor = pgmNoColor;
				printf("Please enter the threshold for the segemnts :\n");
				scanf("%d", &temp);
				threshold = temp;
				segments = (imgPosList **)calloc(1, sizeof(imgPosList *));
				if (segments == NULL)
					exit(1);
				sizeOfAllSegments = findAllSegments(pgmWithColor, threshold, segments);
				if (sizeOfAllSegments != 0)
					printf("All the segments in the picture were found\n\n");
				pgmWithColor = colorSegments(*segments, sizeOfAllSegments);
				if (pgmWithColor != NULL)
				{
					printf("Coloring the segment in the picture was successfully completed\n\n");
					printf("The PGM matrix with the colord segments:\n");
					printPixels(pgmWithColor);
				}

			}
			printf("Please enter a binary file name to save the compressed colored image:\n ");
			scanf("%s", compressedNewFileName);
			printf("Please enter the max gray level (0-127):\n ");
			scanf("%d", &temp);
			compressedMaxGrayLevel = temp;
			saveCompressed(compressedNewFileName, pgmWithColor, compressedMaxGrayLevel);
			printf("Compressing the colored picture was successfully completed\n\n");
			break;
		}
		case 5:
		{
			printf("Please enter the PGM file name\n ");
			scanf("%s", fileName);
			pgmNoColor = readPGM(fileName);
			printf("Please enter a new max gray level: ");
			scanf("%d", &temp);
			printf("Please enter a binary file name to save the compressed colored image::\n ");
			scanf("%s", compressedNewFileName);
			compressedMaxGrayLevel = temp;
			saveCompressed(compressedNewFileName, pgmNoColor, compressedMaxGrayLevel);
			printf("Compressing the picture into file was successfully completed\n\n");
			break;
		}
		case 6:
		{
			printf("Please enter a binary file name that contains a compressed image:\n ");
			scanf("%s", compressedNewFileName);
			printf("Please enter a PGM file name to save the compressed image:\n");
			scanf("%s", fileName);
			convertCompressedImageToPGM(compressedNewFileName,fileName);
			printf("Converting the compressed bianry file into PGM was successfully completed\n\n");
			break;
		}
		case 7:
		{
			freeAll(pgmNoColor, pgmWithColor, segments, sizeOfAllSegments);
			exit(1);
		}		
		default:
			break;
		}
	}
}