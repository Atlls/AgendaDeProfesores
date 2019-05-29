#include <stdio.h> // v4.0 *Sin errores de compilación* *Indentado*
#include <string.h>
#define Max 50

struct Horario{
	int H, M, D;
	char T[3];
};

struct Profesor{
	char Nombre[25], Archivo[25];
};

struct Evento{
	struct Profesor P[Max];
	struct Horario Hrio[Max];
	char Nombre[20];
	int Cont; // Contador de ingresados en el registro //
};

int HoraAMin(int i, int j, char t[]); // v3.1  // De Hora a Minuto -> Minutos Equivalentes == H:M	// TraHora3v 
int RetornoPos(char N[], struct Evento Eventos[]);
void RegEvento(char N[], int NumE, struct Evento Eventos[], char Prof[25], int ConProf, int H, int Min, char T[], int Dur);
void RegGenPrf(int n, struct Profesor Profesores[]);												
void AsigDoce(int C, struct Profesor Profesores[], struct Evento Eventos[], int n, int *A, int *R);		
int Escritor(char Nom[25], int HL, int ML, char TL[3], int DL, char EvL[20]); 	
void Asignados(int H, int M, char T[3], int D, char Ev[20], int C, int a);
void Consular(struct Profesor Profesores[],int n);						int ComEx(char P[25], struct Profesor Profesores[], int n);	
void Eliminar(struct Profesor Profesores[],int n);						int Turno(char T[]);
void Leer(int *hh, int *mm, char t[], int *D, char E[]);

int main(){
	
	struct Evento Eventos[Max];
	struct Profesor Profesores[Max];
	
	FILE *A;	
	int i, j, Menu, N, Cant, Asignados = 0, Resagados = 0; 
	char Actividad[20];
	
	printf(" Digite la cantidad de profesores que quiere registrar: ");
	scanf("%i", &N);
	
	RegGenPrf(N, Profesores);
	
   do{ // Ciclo de Salida //
   	
	do{ //******* WHILE DE MENU *******//
	    printf("\n 1. Añadir Actividad a Docente \n");
		printf(" 2. Consultar Actividad de Docente \n");
		printf(" 3. Eliminar Actividad de Docente \n");
		printf(" 4. Asignar docente a un evento \n");
		printf(" 5. Reporte General por acitividad \n");
		printf(" 6. Resumen de asignacion \n");
		printf(" 7. Salir \n");
		printf(" Opcion: ");
		scanf("%i", &Menu);
	}while(Menu > 7 || Menu < 1);
	
	switch(Menu){
		case 1: // Añadir actividad a un docente //
			
			AsigDoce(1,Profesores,Eventos,N,&Asignados,&Resagados);
			
		break;	// Añadir actividad a un docente //
		case 2: // Consultar actividades de docente //
		
			Consular(Profesores,N);
		
		break;	// Consultar actividades de docente //
		case 3: // Eliminar una actividad a un docente //
		
			Eliminar(Profesores,N);
			
		break;	// Eliminar una actividad a un docente //
		case 4:	// Asignar docentes a un evento //
		 
			printf(" La cantidad de personas registradas es %i!", N);
			printf("\n Indique el numero de personas que deben ir al evento: ");
			
		   do{
			scanf("%i", &Cant);
		   }while(Cant > N);
			
			AsigDoce(Cant,Profesores,Eventos,N,&Asignados,&Resagados);
			
		break; 	// Asignar docentes a un evento //
		case 5: // Generar reporte de actividad //
		
			   do{
			   	printf("\n Digite el evento que desea Generar: ");
			   	scanf("%s", Actividad);
			   }while(RetornoPos(Actividad,Eventos) < 0); // Validador de existencia de Eventos
			   
				A = fopen("reporteActividad.txt","w");
				fprintf(A,"Evento %s:", Actividad);
					
				for(i = 1; i <= Max; i++){
					if(strcmp(Actividad,Eventos[i].Nombre) == 0){
						for(j = 1; j <= Eventos[i].Cont; j++){
						fprintf(A,"\n Docente: %s, ", Eventos[i].P[j].Nombre);
						fprintf(A,"%i:%i%s ", Eventos[i].Hrio[j].H, Eventos[i].Hrio[j].M, Eventos[i].Hrio[j].T);
						fprintf(A,"Dura %i", Eventos[i].Hrio[j].D);
						}		
					}	
				}
				
			   	fclose(A);
				printf(" El reporte de actividad se a registrado con exito!");
				
		break;	// Generar reporte de actividad //
		case 6:	// Mostrar asistencia e inasistencia de eventos //
		
				printf("\nEventos donde se puderon registrar: %i \nNumero de choques: %i", Asignados, Resagados);
				
		break;	// Mostrar asistencia e inasistencia de eventos //
	}
	
   }while(Menu != 7); /* / / / Ciclo de Salida / / / */
	
	return 0;
}

void RegGenPrf(int n, struct Profesor Profesores[]){// Crea archivos con los nombres de los profesores para usarlos en el programa,
	FILE *Entrada; 									// Tambien se engarga de resgistrar profesores para usar mecanismos de validación
	int i;
	
	for(i = 1; i <= n; i++){
		printf(" Diga el nombre del profesor %i: ", i);
		scanf("%s", Profesores[i].Nombre);
			
		strcpy(Profesores[i].Archivo,Profesores[i].Nombre);
		strcat(Profesores[i].Archivo, ".txt");	// Creado del archivo .txt con el Nombre del Profesor //
			
		Entrada = fopen(Profesores[i].Archivo,"w");

		printf(" °Se ha creado el registro de %s Exitosamente!° \n", Profesores[i].Nombre);
	
		fclose(Entrada);
	}
}

void RegEvento(char N[], int NumE, struct Evento Eventos[], char Prof[25], int ContProf, int H, int Min, char T[], int Dur){
	
	strcpy(Eventos[NumE].Nombre,N);
	Eventos[NumE].Hrio[ContProf].H = H;			 
	Eventos[NumE].Hrio[ContProf].M = Min;
	strcpy(Eventos[NumE].Hrio[ContProf].T,T);
	Eventos[NumE].Hrio[ContProf].D = Dur;
	strcpy(Eventos[NumE].P[ContProf].Nombre,Prof);
	Eventos[NumE].Cont++; // Cantidad de profesores y horarios, escencial en el bucle de reporteArcividad.txt
}

void AsigDoce(int C, struct Profesor Profesores[], struct Evento Eventos[], int n, int *A, int *R){//Principal algoritmo, responsable
	int i = 1, j, k, H, M, Dur, Asisten = 0;                                                       // Validar, Leer, escribir y
	char Time[3], Aux[25], Vacio[25] = "°°°", Evento[20], RegistroLoc[25][20]; 					   // Reportar Eventos
	
    for(i = 1; i <= C; i++){
	printf(" Indique el nombre del docente %i: ", i);
	
	   do{
	   	
		scanf("%s", RegistroLoc[i]);	
		
		strcpy(Aux,RegistroLoc[i]);
		strcpy(RegistroLoc[i],Vacio);
		
		k = 0;
		for(j = 1; j <= C; j++){
			
			if(strcmp(Aux,RegistroLoc[j]) == 0){
				printf(" Ese docente esta repetido \n");
				k = 1;
			}
		}
		
	   }while(k != 0 || ComEx(Aux, Profesores, n) != 0); // Debería ser && // Validador de existencia 2.1
		strcpy(RegistroLoc[i],Aux);
	}
	
	Leer(&H,&M,Time,&Dur,Evento);
	
	j = i + *A + *R; // Se reutiliza j, para controlar el registro de profesores de dicho evento //
	for(i = 1; i <= C; i++){ // Bucle para escribir //
		strcpy(Aux,RegistroLoc[i]);	
		k = Escritor(Aux,H,M,Time,Dur,Evento);	// k es 1 si se registró con exito, si no es 0;
		
		if(k == 1){
			*A = *A + k; // Asignado 
		}else{
			*R = *R + 1; // Resagados
		}
		
		RegEvento(Evento,j,Eventos,RegistroLoc[i],i,H,M,Time,Dur);
		Asisten = Asisten + k;// Cantidad de Profesores a Registrar para Asignados.txt
	}
	
	Asignados(H,M,Time,Dur,Evento,C,Asisten);
}

int ComEx(char P[25], struct Profesor Profesores[], int n){// Comprovante usado para validar profesores // 0 si existe y 1 si no
	int i, j = 0;
	
	for(i = 1; i <= n; i++){
		
		if(strcmp(P,Profesores[i].Nombre) == 0){
			j = 1;
		}
	}
	
	if(j == 1){
		return 0;
	}else{
		printf(" Ese nombre no esta registrado, digite otra vez: "); /**/
		return 1;
	}
}

int Escritor(char Nom[25],int HL, int ML, char TL[3], int DL, char EvL[20]){ //Escritor de archivos 2.4 *Indentado* 
	FILE *A;
	int B, HA, MA, DA, TiempoEL, TiempoEA;
	char Aux[20], TA[3], EvA[20], Profesor[25];
		
		strcpy(Profesor,Nom);
		strcat(Nom,".txt");
		
		A = fopen(Nom, "a+"); // Puede ser r+ //
		
		B = 0;
		TiempoEL = HoraAMin(HL,ML,TL);
		fscanf(A,"\n"); // Leer espacio para evitar bugs //

	while(!feof(A)){

		fscanf(A,"%i %i %s %i %s", &HA, &MA, TA, &DA, EvA);
		TiempoEA = HoraAMin(HA, MA, TA);
	
		if(Comparador(TiempoEL,DL,TiempoEA,DA) == 1){ // Comparador de choque: 1 si chocan y 0 si no //
			B = 1; strcpy(Aux,EvA); // Se "Resicla la variable Evl"
		}
		
	}
	
		if(B == 1){
			printf("\n%s no puede ir por %s ", Profesor, Aux);
			fclose(A);	return 0;
		}else{
			fprintf(A,"\n%i %i %s %i %s", HL, ML, TL, DL, EvL);
			printf("\n%s Puede ir al evento", Profesor);
			fclose(A);	return 1;	
		}
}

void Consular(struct Profesor Profesores[], int n){//Abre y lee el archivo de un profesor
	FILE *A;
	char Aux[25], T[3], Ev[20];
	int H, M, D;
	
   do{
   	printf(" Digite el nombre del profesor a consultar: "); scanf("%s",Aux); /**/
   }while(ComEx(Aux,Profesores,n) != 0);
   
   strcat(Aux,".txt");
   A = fopen(Aux,"r");
   
   fscanf(A,"\n"); // Leer espacio para evitar bugs //
   while(!(feof(A))){
   	fscanf(A, "%i %i %s %i %s", &H, &M, T, &D, Ev);
   	printf(" %i:%i%s Que dura: %i Por %s\n", H, M, T, D, Ev);
   }
   
	fclose(A);
	getch();
}

void Asignados(int H, int M, char T[], int D, char Ev[], int C, int a){//Reporta el moviemiento de los Eventos
	FILE *A;
	
	A = fopen("Asignados.txt","a");
	
	fprintf(A,"%i %i %s %i %s %i %i\n", H, M, T, D, Ev, C, a);
	
	fclose(A);
}

void Eliminar(struct Profesor Profesores[], int n){// Elimina un evento del archivo
	FILE *A, *B;
	int H, M, D, k = 0;
	char EvL[20], Ev[20],T[3], Nombre[25], Aux[] = "ooo.txt";
	
	printf(" Digite el nombre del Evento: "); scanf("%s", EvL);
	
   do{
  	printf(" Digite el nombre del Profesor: "); scanf("%s", Nombre);
   }while(ComEx(Nombre,Profesores,n) != 0);
  
	strcat(Nombre,".txt");
  	A = fopen(Nombre,"r"); B = fopen(Aux,"w");
  
  	fscanf(A,"\n");
	while(!(feof(A))){
		fscanf(A, "%i %i %s %i %s", &H, &M, T, &D, Ev);
		if(strcmp(Ev,EvL) == 1){
			fprintf(B,"\n%i %i %s %i %s", H, M, T, D, Ev);
		}else{
			k = 1; // Booleano para saber detecto el evento
		}
    }
  
	fclose(A); fclose(B);
  
	remove(Nombre);
	rename(Aux,Nombre);
  
	if(k == 1){
		printf("\nEl evento de %s, a sido eliminado", EvL);
	}else{
		printf(" Tal Evento no existe");
	}
}

int RetornoPos(char N[], struct Evento Eventos[]){
	int i;
	
	for(i = 1; i <= Max; i++){
		if(strcmp(N,Eventos[i].Nombre) == 0){
			return i; i = Max;
		}
	}
}

int Turno(char T[]){ // Validador del turno horario v1.2.3 *Solo puede validar de un tipo de letra* //

	if(strcmp(T,"AM") == 0 || strcmp(T,"am") == 0 || strcmp(T,"PM") == 0 || strcmp(T,"pm") == 0){
		return 1;
	}else{
		return 0;
	}
}

void Leer(int *hh, int *mm, char t[], int *D, char E[]){// Lee, guarda y valida la Hora, Minutos, y TiempoHorario. Evento y Duración
	
   do{
   	printf("Digite la Hora: ");
    scanf("%i:%i%s", *&hh, *&mm, t);
   }while((*hh<=00 || *hh >= 13) || (*mm<=-1 || *mm >= 60) || (Turno(t) != 1));
  	
  	printf("Digite la duracion: "); scanf("%i", *&D);
  	printf("Digite el evento: "); 	scanf("%s", E);
}

int HoraAMin(int i, int j, char t[]){// Traduce La hora en formato h:mt (t es AM o PM) a tiempo equevalente en minutos
	int k;
	
	if(i == 12 && (strcmp(t,"am") || strcmp(t,"AM"))){
		i = 0;
	}
	if(strcmp(t,"pm") == 0 || strcmp(t,"PM") == 0){
		i += 12;	
	}
	k = 0;
	k = (i * 60) + j;
	
	return k;
}

int Comparador(int TL, int DurL, int TA, int DurA){//Algoritmo de choque de horarios, Si choca retorna 1, sino 0
	int i, j, k;
	
	k = 0;
	for(i = TL - 1; i <= TL - 1 + DurL; i++){
		for(j = TA; j <= TA + DurA; j++){
			if(i == j){
				k = 1;
			}
		}
	}
	return k;
}
