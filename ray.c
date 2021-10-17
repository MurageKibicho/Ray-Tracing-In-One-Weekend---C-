#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))
//Struct holding 3d vector
typedef struct vec *Vector;
struct vec
{
	float x;
	float y;
	float z;
};

typedef struct spherestruct *Sphere;
struct spherestruct
{
	Vector position;
	float radius;
	int material;
};

typedef struct raystruct *Ray;
struct raystruct
{
	Vector start;
	Vector direction;
};

typedef struct col *Color;
struct col
{
	float r;
	float g;
	float b;
};

typedef struct mat *Material;
struct mat
{
	Color diffuse;
	float reflection;
};

typedef struct li *Light;
struct li
{
	Vector position;
	Color intensity;
};

Vector CreateVector(float x, float y, float z)
{
	Vector newVector = malloc(sizeof(*newVector));
	newVector->x = x;
	newVector->y = y;
	newVector->z = z;
	return newVector;
}

Sphere CreateSphere(float positionX, float positionY,float positionZ, float radius,int material)
{
	Sphere newSphere = malloc(sizeof(*newSphere));
	newSphere->position = CreateVector(positionX, positionY,positionZ);
	newSphere->radius = radius;
	newSphere->material = material;
	return newSphere;
}

Ray CreateRay(float startX,float startY,float startZ, float directionX,float directionY,float directionZ)
{
	Ray newRay = malloc(sizeof(*newRay));
	newRay->start = CreateVector(startX, startY, startZ);
	newRay->direction = CreateVector(directionX, directionY,directionZ);
	return newRay;
}

Color CreateColor(float r, float g, float b)
{
	Color newColor = malloc(sizeof(*newColor));
	newColor->r = r;
	newColor->g = g;
	newColor->b = b;
	return newColor;
}

Light CreateLight(float x, float y, float z, float r, float g, float b)
{
	Light newLight = malloc(sizeof(*newLight));
	newLight->position = CreateVector(x,y,z);
	newLight->intensity = CreateColor(r,g,b);
	return newLight;
}

Material CreateMaterial(float r, float g, float b, float reflection)
{
	Material newMaterial = malloc(sizeof(*newMaterial));
	newMaterial->diffuse = CreateColor(r,g,b);
	newMaterial->reflection = reflection;
	return newMaterial;
}


Vector AddVectors(Vector one, Vector two)
{
	Vector result = CreateVector((one->x + two->x),(one->y + two->y),(one->z + two->z));
	return result;
}

Vector SubtractVectors(Vector one, Vector two)
{
	Vector result = CreateVector((one->x - two->x),(one->y - two->y),(one->z - two->z));
	return result;
}

Vector ScaleVector(float scale, Vector one)
{
	Vector result = CreateVector((one->x * scale),(one->y * scale),(one->z * scale));
	return result;
}

float DotProduct(Vector one, Vector two)
{
	float result = (one->x * two->x) + (one->y * two->y) + (one->z * two->z);
	return result;
}

int TestRaySphereIntersection(Ray ray, Sphere sphere, float *t)
{
	int toReturn = 0;
	float directionDotProduct = DotProduct(ray->direction, ray->direction);
	Vector distanceRaySphere = SubtractVectors(ray->start, sphere->position);
	
	float B = 2 * DotProduct(ray->direction, distanceRaySphere);
	float C = DotProduct(distanceRaySphere, distanceRaySphere) - (sphere->radius * sphere->radius);
	float discriminant = B * B - 4 * directionDotProduct * C;
	if(discriminant < 0)
	{
		toReturn = 0;
	}
	else
	{
		float squareRootDiscriminant = sqrtf(discriminant);
		float t0 = (-B + squareRootDiscriminant) / 2;
		float t1 = (-B - squareRootDiscriminant) / 2;
		if(t0 > t1)
		{
			t0 = t1;
		}
		if((t0 > 0.001f) && (t0 < *t))
		{
			*t = t0;
			toReturn = 1;
		}
		else
		{
			toReturn = 0;
		}
	}
	return toReturn;
}

void PrintAsciiSphere()
{
	Sphere newSphere = CreateSphere(20,20,20,10,0);
	Ray newRay = CreateRay(0,0,0,0,0,1);
	for(int y = 0; y < 40; y++)
	{
		newRay->start->y = y;
		for(int x = 0; x < 40; x++)
		{
			newRay->start->x = x;
			float t = 2000;
			int hit = TestRaySphereIntersection(newRay,newSphere, &t);
			if(hit == 1)
			{
				printf("+");
			}
			else
			{
				printf("-");
			}
		}
		putchar('\n');
	}
}

void CreatePPM(char *fileName, unsigned char *imageData, int imageWidth, int imageHeight)
{
	FILE *imageFile = fopen(fileName, "wb");
	fprintf(imageFile,"P6 %d %d %d\n",imageWidth,imageHeight,255);
	fwrite(imageData,3,imageWidth*imageHeight,imageFile);
	fclose(imageFile);
}

void PrintRGBSphere()
{
	Ray newRay = CreateRay(0,0,0,0,0,1);
	Material materialsOne = CreateMaterial(1,0,0,0.2);
	Material materialsTwo = CreateMaterial(0,1,0,0.5);
	Material materialsThree = CreateMaterial(0,0,1,0.9);
	Material *materials = malloc(3 * sizeof(Material));
	materials[0] = materialsOne;
	materials[1] = materialsTwo;
	materials[2] = materialsThree;
	
	Sphere sphereOne = CreateSphere(200,300,0,100,0);
	Sphere sphereTwo = CreateSphere(400,400,0,100,1);
	Sphere sphereThree = CreateSphere(500,140,0,100,2);
	Sphere *spheres = malloc(3 * sizeof(Sphere));
	spheres[0] = sphereOne;
	spheres[1] = sphereTwo;
	spheres[2] = sphereThree;
	
	Light lightOne = CreateLight(0,240,-100,1,1,1);
	Light lightTwo = CreateLight(3200,3000,-1000,0.6,0.7,1);
	Light lightThree = CreateLight(600,0,-100,0.3,0.5,1);
	Light *lights = malloc(3 * sizeof(Light));
	lights[0] = lightOne;
	lights[1] = lightTwo;
	lights[2] = lightThree;
	
	int width = 800;
	int height = 600;
	unsigned char image[3*width*height];
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			float red = 0;
			float green = 0; 
			float blue = 0; 
			int level = 0; 
			float coef = 1.0;
			newRay->start->x = x;
			newRay->start->y = y;
			newRay->start->z = -2000;
			newRay->direction->x = 0;
			newRay->direction->y = 0;
			newRay->direction->z = 1;
			do
			{
				float t = 20000.0f;
				int currentSphere = -1;
				for(unsigned int i = 0; i < 3; i++)
				{
					if((TestRaySphereIntersection(newRay, spheres[i], &t)) == 1)
					{
						currentSphere = i;
					}
				}
				if(currentSphere == -1)
				{
					break;
				}
				Vector scaled = ScaleVector(t, newRay->direction);
				Vector newStart = AddVectors(newRay->start, scaled);
				Vector normal = SubtractVectors(newStart,spheres[currentSphere]->position);
				float temp = DotProduct(normal,normal);
				if(temp == 0)
				{
					break;
				}
				temp = 1.0f / sqrtf(temp);
				normal = ScaleVector(temp, normal);
				Material currentMaterial = materials[spheres[currentSphere]->material];
				for(unsigned int j = 0; j < 3; j++)
				{
					Light currentLight = lights[j];
					Vector distance = SubtractVectors(currentLight->position, newStart);
					if(DotProduct(normal, distance) < 0.0f)
					{
						continue;
					}
					float t = sqrtf(DotProduct(distance,distance));
					
					Ray lightRay = CreateRay(0,0,0,0,0,0);
					lightRay->start = newStart;
					lightRay->direction = ScaleVector((1/t), distance);
					
					float lambert = DotProduct(lightRay->direction, normal) * coef;
					red += (lambert * (currentLight->intensity->r) * (currentMaterial->diffuse->r));
					green += lambert * currentLight->intensity->g * currentMaterial->diffuse->g;
					blue += lambert * (currentLight->intensity->b) * (currentMaterial->diffuse->b);
				}
				coef *= currentMaterial->reflection;
				newRay->start = newStart;
				float reflect = 2.0f * DotProduct(newRay->direction, normal);
				Vector temporaryVector= ScaleVector(reflect, normal);
				newRay->direction = SubtractVectors(newRay->direction, temporaryVector);
				level++;
			}
			while((coef > 0.0f) && (level < 15));
			image[(x + y *width)*3 + 0] = (unsigned char)min(red*255.0f,255.0f);
			image[(x + y *width)*3 + 1] = (unsigned char)min(green*255.0f,255.0f);
			image[(x + y *width)*3 + 2] = (unsigned char)min(blue*255.0f,255.0f);
		}
	}
	CreatePPM("image.ppm", image, width, height);	
}

int main()
{
	//PrintAsciiSphere();
	PrintRGBSphere();
	return 0;
}
