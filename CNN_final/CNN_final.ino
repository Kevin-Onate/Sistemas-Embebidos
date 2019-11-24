#include "matriz_entrenamiento.h"
float datos_prueba[5]={6.2,2.9,4.3,1.3,2}; //6,2  2,9 4,3 1,3
int respuesta;

void setup() {
 Serial.begin(9600);
}



void loop() {
  cnn(3,5,120);
  delay(5000);
}

int knn (int k,int etiquetas, int tam_col, int tam_fil){
  int col;
  int fil;
  int i=0;
  int j; 
  int etiqueta;
  float aux;
  float aux_eti;
  float sumatoria=0; // sumatoria de la elevación al cuadrado de cada col
  float distancia=0; // raiz de sumatoria
  
  /*matriz de k-vecinos
  |1|2|3| -> numero de vecinos
  |2|1|1| -> asginacion del vecino por etiqueta
  |0.1|0.2|0.3| -> distancia
  */
 float matriz_k [3][k];
 
  /* etiquetas
  |1|2|3| -> etiquetas que existe dentro de la base de datos
  |2|1|0| -> conteo de etiquetas dentro de los k-vecinos
   */
 int matriz_etiq[2][etiquetas];
  // asignar numero de vecinos y enceramos asignación de etiqueta

   for(;i<k;i++){
      matriz_k[0][i]=i+1.0;
      matriz_k[1][i]=0;
      matriz_k[2][i]=3000.0+i;
    /*
     |1|2|3|
     |0|0|0|
     |3000|3001|3002|
     */
    }
   // asignar etiquetas y enceramos su contador
   for(i=0;i<etiquetas;i++){
    matriz_etiq[0][i]=i+1;
    matriz_etiq[1][i]=0;
    /*
     |1|2|3| 
     |0|0|0|
     */
    }

    /*PASOS PARA K-NN
     * Leer cada fila de la matriz de entrenamiento
     * distancia entre la fila de la matriz con la nueva instancia (vector de prueba)
     * ordenar la matriz matriz_k (determinar los k vecinos de menor distancia)
     * contar las etiquetas en la matriz_k y asignar en matriz etiquetas
     * eligo al mayor número de matriz etiquetas
     * retorno la etiqueta resultante
     * 
     */

     for(fil=0;fil<tam_fil;fil++){
         for(col=0;col<tam_col-1;col++){ // menos 1 por ultima columna es etiqueta
             sumatoria=sumatoria+pow(matriz[fil][col]-datos_prueba[col],2);
          }
             distancia=sqrt(sumatoria);
             // comparación de nuevo dato con solo la distacia mayor almacenada en matriz_k
              if(distancia<matriz_k[2][k-1]){
                 matriz_k[2][k-1]=distancia;
                 matriz_k[1][k-1]=matriz[fil][tam_col-1];
                 //ordenar por metodo burbuja
                 for (i=0;i<k;i++){
                     for(j=i+1;j<k;j++){
                          if(matriz_k[2][i]>matriz_k[2][j]){
                            //distancia
                             aux=matriz_k[2][i];
                             matriz_k[2][i]=matriz_k[2][j];
                             matriz_k[2][j]=aux;
                             // etiqueta
                              aux_eti=matriz_k[1][i];
                             matriz_k[1][i]=matriz_k[1][j];
                             matriz_k[1][j]=aux_eti;
                            } //end if
                      } //end for
                  }
                 //fin ordenamiento 
              } 
              //fin condicion de numero menor
              sumatoria=0;
        }
        // fin de lectura de matriz
        // contar por columna, cuantos vecinos hay por etiqueta
        for(i=0;i<etiquetas;i++){
          for(j=0;j<k;j++){
              if(matriz_etiq[0][i]==matriz_k[1][j]){
                 matriz_etiq[1][i]++;
                }
            }
          }

         // buscar la etiqueta con mayor numero de vecinos

        for(i=0;i<etiquetas-1;i++){
            if(matriz_etiq[1][i]<matriz_etiq[1][i+1])
               etiqueta=matriz_etiq[0][i+1];
            else
              etiqueta=matriz_etiq[0][i];
          }
        
        return etiqueta;    
     }


void cnn (int etiquetas, int tam_col, int tam_fil){
  int i;
  int j;
  int l;
  int f;
  int c;
  int x;
  float centroid=0;
  float promedio=0;
  float distancia_menor=3000;
  float distancia=0;
  int label;
  int s=0;      //contar los datos en sourse
/*  
 *   centroides -> promedio
 *                 promedio por columna y por etiqueta
 *                 almaceno vectores 
 *                 knn entre vectores y matriz de entrenamiento
 */

 float centroides [etiquetas][tam_col];
 for(i=0;i<etiquetas;i++){
   for(j=0;j<tam_col;j++){
    if(j=tam_col-1){
     centroides[i][j]=i+1;        //llenas etiqueta a cada centroide
    }
    else{
      centroides[i][j]=0;         //Leno de ceros al resto de la matriz
    }
    }
  }

   for(l=0;l<etiquetas;l++){
     for(i=0;i<tam_col-1;i++){
       for(j=(40*l);j<40*(l+1);j++){
          centroid=centroid+matriz[j][i];
        }
          centroides[l][i]=centroid/40;
          Serial.print(centroides[l][i],4);
          Serial.print(',');
          centroid=0;
      }
          Serial.println(centroides[l][tam_col-1]);
   } 
    
    //encontrar la distancia entre los centroides y la matriz de entrenamiento 
    //para comparar si clasifican adecuadamente 

        //for -> filas de matriz de entrenamiento
          //for -> filas de matriz centroides
            //for -> columnas
  for(i=0;i<tam_fil;i++){
    for(j=0;j<etiquetas;j++){
      for(l=0;l<tam_col-1;l++){
        promedio=promedio+pow(centroides[j][l]-matriz[i][l],2);    
      }
      distancia=sqrt(promedio);
      if(distancia<distancia_menor){
        distancia_menor=distancia;
        label=centroides[j][tam_col-1];
      }
      promedio=0;
    }
    distancia_menor=3000;
    if(matriz[i][tam_col-1]==label){
      //Serial.print(String(i)+":");
      //Serial.println("D");
    }
    else{
      //Serial.print(String(i)+":");
      //Serial.println("s"); 
      int pos[s]={};     //almacena posiciones de S
      s++; 
      pos[s-1]=i;       //vector posiciones donde hay S
      float matriz_s[s+etiquetas][tam_col];
      for(x=0;x<=s;x++){
        for(f=0;f<s+etiquetas;f++){
          for(c=0;c<tam_col;c++){
            matriz_s[f][c]=centroides[f][c];
            matriz_s[etiquetas][c]=matriz[pos[s-1]][c];
            //Serial.print(matriz_s[f][c]=matriz[i][c]);
            //Serial.print(',');
          }
          Serial.println(' ');
        }
      }
      for(f=0;f<s+etiquetas;f++){
        for(c=0;c<tam_col;c++){
          Serial.print(matriz_s[f][c]);
          Serial.print(' ');          
        }
        Serial.println(' ');
      }
    }
  }
  Serial.println(s);
}
     
