#include <stdio.h>
#include <stdlib.h>

void CreatePPM(unsigned char *buf,int wrap,int xsize,int ysize, char *filename)
{
	FILE *f;
	int i;
	printf("\n\nWrap: %d\n\n", wrap);
	f = fopen(filename, "w");
	fprintf(f,"P5\n%d %d\n%d\n",xsize,ysize,255);
	for(i = 0; i < ysize; i++)
	{
		unsigned char *ch = (buf+i*wrap);
		fwrite(ch,1,xsize,f);
	}
	fclose(f);
}

int main()
{
	int imageWidth = 256;
	int imageHeight = 256;
	FILE *imageFile = fopen("image.ppm", "w");
	fprintf(imageFile,"P3\n%d %d\n%d\n",imageWidth,imageHeight,255);
	for(int j = imageHeight-1; j >= 0; j--)
	{
		for(int i = 0; i < imageWidth; i++)
		{
			double r = (double)i / (imageWidth - 1);
			double g = (double)j / (imageHeight - 1);
			double b = 0.25;
			
			unsigned char ir = (unsigned char) (255.99 * r);
			unsigned char ig = (unsigned char) (255.99 * g);
			unsigned char ib = (unsigned char) (255.99 * b);	
			fprintf(imageFile,"%d %d %d\n",ir,ig,ib);
		}
	}
	
	return 0;
}
