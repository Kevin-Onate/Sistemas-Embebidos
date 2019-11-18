#include "matriz_entrenamiento.h"
float datos_prueba[5]={6.2,2.9,4.3,1.3,2}; //6,2  2,9 4,3 1,3
int respuesta;
int cont = 0;

void setup() {
 Serial.begin(9600);
}



void loop() {
  int val=cont_eti(120,4)+1;
  cnn(3,5,120,val);
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


void cnn (int etiquetas, int tam_col, int tam_fil, int resp){
  int i;
  int j;
  int l;
  float centroid=0;
  int fil;
  int col;
  float s1;
  float s2;
  float s3;
  float dst1;
  float dst2;
  float dst3;
  float comp_eti=0;
  float centroides[etiquetas][tam_col];
  float matriz_aux[tam_fil][tam_col-1];
  float eti_res[tam_fil];
  float cent_1[tam_col-1]={};
  float cent_2[tam_col-1]={};
  float cent_3[tam_col-1]={};
  
 for(i=0;i<etiquetas;i++){
   for(j=0;j<tam_col;j++){
     centroides[i][j]=0;
    }
  }

   for(l=0;l<etiquetas;l++){
     for(i=0;i<tam_col-1;i++){
       for(j=(resp*l);j<resp*(l+1);j++){
          centroid=centroid+matriz[j][i];
        }
          centroides[l][i]=centroid/40;
          //Serial.print(centroides[l][i],4);
          //Serial.print(',');
          centroid=0;
      }
          //Serial.println(' ');
   }

    for(j=0;j<tam_col-1;j++){
      cent_1[j]=centroides[0][j];
      cent_2[j]=centroides[1][j];
      cent_3[j]=centroides[3][j];
    }
    for(fil=0; fil<tam_fil; fil++){
      for(col=0; col<tam_col-1;col++){
        s1=s1+pow(matriz[fil][col]-cent_1[col],2);
        s2=s2+pow(matriz[fil][col]-cent_2[col],2);
        s3=s3+pow(matriz[fil][col]-cent_3[col],2);
      }
    dst1=sqrt(s1);
    dst2=sqrt(s2);
    dst3=sqrt(s3);
    s1=0;
    s2=0;
    s3=0;

    if(dst1<dst2 && dst1<dst3){
        comp_eti=1;
        eti_res[fil]=comp_eti;
    }
    else{
      if(dst2<dst1 && dst2<dst3){
        comp_eti=2;
        eti_res[fil]=comp_eti;
      }
      else{
        if(dst3<dst1 && dst3<dst2){
          comp_eti=3;
          eti_res[fil]=comp_eti;
        }  
      }
      if(matriz[fil][4]==eti_res[i]){
        matriz_aux[fil][4]=10;    //D
      }
      else{
        matriz_aux[fil][4]=12;    //S  
      }  
    }
  }
  for(i=0;i<tam_fil;i++){
    for(j=0;j<tam_col-1;j++){
      matriz_aux[i][j]=matriz[i][j];
    }
  } 
  for(i=0;i<tam_fil;i++){
    for(j=0;j<tam_col-1;j++){
      Serial.print(matriz_aux[i][j]);
      Serial.println(' ');
    }
    Serial.println(' ');
  }
  Serial.println("FIN");
}



int cont_eti(int tam_fil, int col_etiq){
  int resultado;
  for(int i=0; i<tam_fil-1; i++){
    if(matriz[i][col_etiq]==matriz[i+1][col_etiq]){
      cont++;
      resultado=cont;
      delay(200);
    }
    else{
      cont=0;
      return resultado;  
    }
  }
}
     
