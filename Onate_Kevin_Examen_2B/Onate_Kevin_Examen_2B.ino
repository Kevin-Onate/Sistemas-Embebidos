#define fil 197
#define col 4
extern float entrenamiento [fil][col];
float P0 = random(0, 100) / 100;
float P1 = random(0, 100) / 100;
float P2 = random(0, 100) / 100;
float P3 = random(0, 100) / 100;
int iteraciones = 0;              //contador de iteraciones
int cont_error = 0;               //contar errores
float Sum [fil];                   //sumatoria para cada registro de entrada de la bas de datos
float Pred [fil];                 // valor de prediccion para cada reistro de la base de datos, en un valor de pesos especifico
float vec_error[fil];             // almacena un valor entre 1-1 y 0, de acuerdo en que el 0 indica que no hay error caso contrario, si lo hay
int i;
int j;                            // variables de iteraciones para los ciclos for
int k;
int m;
int l;
int pos_error = 0;                // indica si para cierto valor de pesos este o no error en las predicciones
                                  // si pos_error es 0 no hay errores, caso contrario indica que si hay errores
float n = 0.1;                    // valor en base al cual cambian los pesos, cuando hay errores en la base de datos

void setup() {
  Serial.begin(9600);
}

void loop() {
  while (iteraciones < 100) {     // en base a 8 iteraciones como maximo, intenta calcular los peesos adecuados
    cont_error = 0;
    for (i = 0; i < fil; i++) { //Ecuación del sistema     Sum = X0P0 + X1P1 + X2P2 + ...
      Sum[i] = ((entrenamiento[i][0] * P3) + (entrenamiento[i][1] * P1) + (entrenamiento[i][2] * P2) + (entrenamiento[i][3] * P0));      // acaba de almacenar los valores de sumatoria para cada registro de
                                                                                                                                         //la base de datos con los pesos actuales
    }
    for (j = 0; j < fil; j++) {     //PREDICCIONES
      if (Sum[j] >= 0) {
        Pred[j] = 1;
      } else {
        Pred[j] = 0;
      }                             // acaba de almacenar las predicciones para todos los registros de la base de datos con los pesos actuales
    }

    for (int h = 0; h < fil; h++) {     //VERIFICACIÓN DE ERROR
      vec_error[h] = entrenamiento[h][3] - Pred[h];
    }
    // acaba de almacenar los valores de error para cada registro de la base de datos
    // con los pesos actuales, entendiendo de que si alamacena un valor diferente de cero
    // aun hay error en los valores de los pesos


    for (l = 0; l < fil; l++) {     //ENCONTRAR EL ERROR
      if (vec_error[l] != 0) {      // Busca si hay valores diferentes de 0.
        pos_error = l;              // Extrae su posición.
        Serial.print(entrenamiento[l][0]);
        Serial.print(",");
        Serial.print(entrenamiento[l][1]);
        Serial.print(",");
        Serial.print(entrenamiento[l][2]);
        Serial.print(",");
        Serial.println(entrenamiento[l][3]);
        
        break;
      }
    }

    //indicando en la variable pos_error si hubo o no errores, con los pesos actuales
    // RECALCULAR PESOS
    P0 = P0 + n * (vec_error[pos_error]) * 1; // es el peso de la etiqueta
    P1 = P1 + n * (vec_error[pos_error]) * (entrenamiento[pos_error][1]);            // se acaba de variar los valores de los pesos, en un valor del factor de aprendizaje
    P2 = P2 + n * (vec_error[pos_error]) * (entrenamiento[pos_error][2]);            // igual a n, solo en caso de que haya habido errores
    P3 = P3 + n * (vec_error[pos_error]) * (entrenamiento[pos_error][0]);
    iteraciones++;      // Cuenta la iteración.


    for (m = 0; m < fil; m++) {           //CANTIDAD DE ERRORES
      if (vec_error[m] != 0)              // Cuenta los valores diferentes de cero.
        cont_error++;                     // acaba de contar el numero de errores para los valores de pesos en esta iteracion
    }

    Serial.println("------ITERACION # "+String(iteraciones)+"----");    
    //Serial.print("Iteracion: ");
    //Serial.println(iteraciones);
    Serial.print("Peso P0(etiqueta):");
    Serial.println(P0);
    Serial.print(" Peso P1: ");
    Serial.println(P1);
    Serial.print(" Peso P2: ");
    Serial.println(P2);
    Serial.print(" Peso P3: ");
    Serial.println(P3);
    Serial.print("# de errores:          ");
    Serial.println(cont_error);
    Serial.println(" ");
  }
}
