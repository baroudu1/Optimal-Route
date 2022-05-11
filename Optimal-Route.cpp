#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define infinie 1316134911

typedef struct mat_Ville
{
	int nbr_v;
	float **mat;
	char **l_v;
} mat_Ville;

typedef struct ville
{
	//
	float d;
	int i_v;
	char *n_v;
	struct ville *suc;
} Ville;

typedef Ville *listVille;

mat_Ville creermatVil(int nbr_v)
{
	mat_Ville *mv = (mat_Ville *)malloc(sizeof(mat_Ville));

	mv->nbr_v = nbr_v;
	mv->l_v = (char **)malloc(nbr_v * sizeof(char *));
	mv->mat = (float **)malloc(nbr_v * sizeof(float *));

	for (int i = 0; i < nbr_v; i++)
	{
		(mv->mat)[i] = (float *)malloc(nbr_v * sizeof(float));
	}

	return *mv;
}

Ville *creerVille(float d, char *n_V, int i_V)
{
	Ville *v = (Ville *)malloc(sizeof(Ville));

	v->d = d;
	v->n_v = n_V;
	v->i_v = i_V;
	v->suc = NULL;

	return v;
}

Ville *ajouter_Ville_A_list(float d, char *n_V, int i_V, listVille *lV)
{
	Ville *v = *lV;

	if (v == NULL)
	{
		*lV = creerVille(d, n_V, i_V);
		return *lV;
	}

	while (v->suc)
	{
		v = v->suc;
	}
	v->suc = creerVille(d, n_V, i_V);

	return v->suc;
}

bool exist_Ville_Dans_list(listVille lV, int i_v)
{
	Ville *v = lV;

	while (v)
	{
		if (v->i_v == i_v)
			return true;

		v = v->suc;
	}
	return false;
}

int i_v_proche(mat_Ville mv, listVille lV, int i_Depart)
{
	float minDes = infinie;
	int i_Min;

	for (int i = 0; i < mv.nbr_v; i++)
	{
		if (minDes >= mv.mat[i_Depart][i] && i_Depart != i && !exist_Ville_Dans_list(lV, i))
		{
			minDes = mv.mat[i_Depart][i];
			i_Min = i;
		}
	}

	return i_Min;
}

listVille H_Algorithm(mat_Ville mv, int nbr)
{
	listVille lV = NULL;

	Ville *v = ajouter_Ville_A_list(0, mv.l_v[nbr], nbr, &lV);

	int i_v_proch;

	for (int i = 0; i < mv.nbr_v; i++)
	{
		if (i != nbr)
		{
			i_v_proch = i_v_proche(mv, lV, v->i_v);

			v->d = mv.mat[v->i_v][i_v_proch];

			v = ajouter_Ville_A_list(0, mv.l_v[i_v_proch], i_v_proch, &lV);
		}
	}

	v->d = mv.mat[v->i_v][0];

	return lV;
}

void afficherl_v(listVille lV)
{
	Ville *v = lV;
	while (v)
	{
		printf("%s ----- %.1f ----- ", v->n_v, v->d);
		v = v->suc;
	}
}

void ligne_des_Villes(mat_Ville *mv, FILE **fp)
{
	char *n_v;

	for (int i = 0; i < mv->nbr_v; i++)
	{
		n_v = (char *)malloc(60 * sizeof(char));

		if (i < mv->nbr_v - 1)
			fscanf(*fp, "%s\t", n_v);
		else
			fscanf(*fp, "%s\n", n_v);

		mv->l_v[i] = n_v;
	}
}

void lire_les_Villes(mat_Ville *mv, FILE **fp, int i_vle)
{
	float d;

	for (int i = 0; i < mv->nbr_v; i++)
	{

		if (i < mv->nbr_v - 1)
			fscanf(*fp, "%f\t", &d);
		else
			fscanf(*fp, "%f\n", &d);

		mv->mat[i_vle][i] = d;
	}
}

int lireFichier(mat_Ville *mv, FILE **fp, char *n_Fichier)
{
	*fp = fopen(n_Fichier, "r");
	int nbr_v;

	int i = 0;

	if (*fp != NULL)
	{
		fscanf(*fp, "%d\n", &nbr_v);

		*mv = creermatVil(nbr_v);

		ligne_des_Villes(mv, fp);
		while (!feof(*fp))
		{
			lire_les_Villes(mv, fp, i);
			i++;
		}
		return nbr_v;
	}
	return 0;
}

int main()
{
	srand(time(0));
	mat_Ville MV;
	FILE *fp;
	// lire le fichier qui contient le graphe
	char file[] = "ville.txt";
	// appliquer Ford Filkurson
	int k = lireFichier(&MV, &fp, file);
	for (int i = 0; i < k; i++)
	{
		printf("\t%s", MV.l_v[i]);
	}
	for (int i = 0; i < k; i++)
	{
		printf("\n%s", MV.l_v[i]);

		for (int j = 0; j < k; j++)
		{
			printf("\t%.1f", MV.mat[i][j]);
		}
	}
	printf("\n\n");

	for (int i = 0; i < k; i++)
	{
		listVille list_Ville = NULL;
		float val_cir = 0;
		list_Ville = H_Algorithm(MV, i);
		printf("\n  ");
		afficherl_v(list_Ville);
		printf("%s\n", list_Ville->n_v);

		while (list_Ville)
		{
			val_cir += list_Ville->d;
			list_Ville = list_Ville->suc;
		}
		printf("\n  La valeur de circuit :  %.1f", val_cir);
		printf("\n\n");
	}

	system("pause");
}
