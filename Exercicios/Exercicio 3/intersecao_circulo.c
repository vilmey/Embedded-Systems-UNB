#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

// CALCULA INTERSECAO CIRCULO //

int main()
{
	float raio1, raio2, centro_x, y_1, y_2, x_1, x_2;

	//Equacao da circunferencia
	y_1 = 0; y_2 = 0; raio1 = 3; raio2 = 6; centro_x = 6;

	x_1 = sqrt((raio1*raio1) - (y_1*y_1));
	x_2 = sqrt(pow((raio2) - (y_2*y_2),2)) - centro_x;

	printf("X1 = %f, X2 = %f\n", x_1, x_2);


	float i;
	for(i=0.1 ; i< 30; i+=0.0001)
	{
		y_1 = i; y_2 = i;

		//x1_old = x_1;
		//x2_old = x_2;

		x_1 = sqrt((raio1*raio1) - (y_1*y_1));
		x_2 = sqrt(pow((raio2) - (y_2*y_2),2)) - centro_x;
		//printf("Cir1 = (%f,%f), Cir2 = (%f,%f)\n", x_1, i, x_2, i);


		if(x_1 < (-x_2))
		{
			printf("Match Found !!!\n");
			printf("Cir1 = (%f,%f), Cir2 = (%f,%f)\n", x_1, i, x_2, i);
			break;
		}

		if(x_1 < 0 || x_2 > 0)
			break;

	}

	return 0;
}