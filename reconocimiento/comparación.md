# Comparación: Eigenfaces, Fisherfaces y LBPH frente a CNN

Nombre: José Antonio Medina Ayala
Matrícula: 22121289


## Funcionamiento general

En el proyecto se utilizan fotografías de rostros en escala de grises con un tamaño de 100×100 píxeles. Eigenfaces, Fisherfaces y LBPH transforman esas imágenes en una representación numérica que permite compararlas con los rostros registrados.

El método predict devuelve una etiqueta y un valor llamado confidence. En estos reconocedores de OpenCV ese valor representa una distancia, no un porcentaje de seguridad. Una distancia menor indica mayor semejanza con los ejemplos conocidos.

El umbral permite decidir si se acepta la identificación o se muestra “Desconocido”. Cada algoritmo trabaja con una escala diferente, por lo que no se puede utilizar el mismo umbral para todos.

## Eigenfaces (PCA)

Eigenfaces utiliza PCA, o análisis de componentes principales, para reducir la cantidad de información necesaria para representar una cara.

Una imagen de 100×100 contiene 10,000 valores. En lugar de comparar directamente todos los píxeles, PCA encuentra las direcciones donde existe mayor variación entre las imágenes y representa cada rostro mediante sus componentes principales.

### Ejemplo con las fotografías del proyecto

Estos son cuatro píxeles de la fila 50, entre las columnas 48 y 51:

| Fotografía | Columna 48 | Columna 49 | Columna 50 | Columna 51 |
| ---------- | ---------- | ---------- | ---------- | ---------- |
| cesare_1249.jpg | 135 | 133 | 134 | 133 |
| joseT_1308.jpg | 157 | 148 | 142 | 135 |
| kike_1249.jpg | 125 | 126 | 126 | 124 |
| Promedio | 139.0 | 135.7 | 134.0 | 130.7 |

Primero se obtiene el rostro promedio y se resta a cada imagen. Para los cuatro valores de cesare, el resultado aproximado es:

[-4.0, -2.7, 0.0, 2.3]

Después se proyecta la imagen sobre los componentes principales. En el análisis reportado, el primer componente explica el 31.4% de la variación y con 63 componentes se alcanza aproximadamente el 90%.

Esto permite pasar de 10,000 valores a una representación mucho más pequeña. Sin embargo, reducir la representación de cada rostro no significa que el archivo completo del modelo vaya a ser pequeño.

Los primeros cinco valores de una imagen de cesare fueron:

[-3176, -1666, 2034, -711, -446]

Mientras que para una imagen de kike fueron:

[-963, 213, -9, -294, 261]

La diferencia entre estas representaciones permite distinguir los rostros.

**Ventaja:** reduce la cantidad de dimensiones y su funcionamiento se puede explicar mediante el promedio y las proyecciones.

**Desventaja:** es sensible a cambios de iluminación y alineación. PCA busca la mayor variación general, aunque esa variación no sea la más útil para identificar personas.

## Fisherfaces (PCA + LDA)

Fisherfaces combina PCA con LDA, o análisis discriminante lineal.

Primero se reduce la dimensión de las imágenes y después se utilizan las etiquetas de las personas para buscar una representación donde las fotografías de la misma persona estén más juntas y las de personas diferentes queden más separadas.

Con siete personas, LDA puede producir hasta seis componentes discriminantes. En este proyecto, cada rostro queda representado mediante seis valores.

El modelo reportado ocupa aproximadamente 2 MB y tarda 0.52 ms por predicción. Su entrenamiento fue el más lento, con unos 27 minutos.

**Ventaja:** considera la identidad de las personas para construir la representación y puede manejar mejor ciertas variaciones de iluminación que Eigenfaces.

**Desventaja:** necesita ejemplos que representen las variaciones de cada persona y, en esta prueba, requirió más tiempo de entrenamiento. Su resultado también depende de la calidad de las fotografías.

## LBPH (patrones binarios locales)

LBPH analiza las texturas de pequeñas regiones del rostro.

En su versión básica, compara un píxel central con sus ocho vecinos. Si el vecino tiene un valor mayor o igual al centro, se asigna un 1; si es menor, se asigna un 0.

En el ejemplo del proyecto, el píxel central tiene un valor de 78. Al recorrer sus vecinos en sentido horario desde la esquina superior izquierda se obtiene:

1, 0, 1, 1, 1, 1, 1, 0

El código binario resultante es 10111110, equivalente a 190 en decimal.

Después se divide el rostro en regiones, se calcula un histograma de los patrones de cada una y se juntan los histogramas para comparar las caras.

Como utiliza comparaciones locales, puede tolerar ciertos cambios uniformes de iluminación. Esto no significa que sea inmune a sombras o cambios fuertes de luz.

**Ventaja:** su entrenamiento fue muy rápido y obtuvo la mejor exactitud de los tres métodos en esta prueba.

**Desventaja:** conserva histogramas asociados a las imágenes de entrenamiento. Por eso el tamaño del modelo aumenta al agregar fotografías.

Con las 17,317 imágenes, el archivo llegó a aproximadamente 1.4 GB y se reportó que OpenCV no pudo cargarlo.

## Comparación con los datos del proyecto

La prueba se planteó con aproximadamente 500 fotografías por persona, utilizando el 80% para entrenamiento y el 20% para evaluación.

El documento reporta 2,798 fotografías de entrenamiento y 700 de prueba para siete personas. El total de entrenamiento es ligeramente menor que las 2,800 que resultarían de utilizar exactamente 400 por persona.

| Característica | LBPH | Eigenfaces | Fisherfaces |
| -------------- | ---- | ---------- | ----------- |
| Exactitud | 95.0% | 93.4% | 90.7% |
| Tiempo de entrenamiento | 3 s | 1,248 s, aproximadamente 21 min | 1,592 s, aproximadamente 27 min |
| Tiempo de predicción por foto | 38.4 ms | 20.5 ms | 0.52 ms |
| Tamaño del modelo | 244 MB | 870 MB | 2 MB |
| Aciertos con tony | 67 de 100 | 56 de 100 | 49 de 100 |
| Umbral de ejemplo del curso | 70 | 2800 | 500 |
| Resultado observado con cámara | Funcionó bien | Asignó otro nombre | Reconoció bien |

### Interpretación de los resultados

LBPH obtuvo la mayor exactitud y entrenó en mucho menos tiempo. Sin embargo, su predicción fue más lenta que la de los otros métodos y su archivo ocupó bastante espacio.

Fisherfaces tuvo la menor exactitud de la evaluación, pero produjo el modelo más pequeño y fue el más rápido al predecir. Además, en la prueba con cámara se observó un buen reconocimiento.

Eigenfaces quedó en segundo lugar en exactitud, aunque en vivo llegó a identificar el rostro con otro nombre.

Los tres presentaron más dificultades con las imágenes etiquetadas como tony. El análisis señala confusiones con kike y pau, lo que sugiere que las representaciones utilizadas no separaron suficientemente esos rostros en las condiciones evaluadas.

### Limitaciones de la prueba

Los tres métodos se ejecutaron al mismo tiempo durante las mediciones. Esto pudo generar competencia por los recursos y afectar los tiempos registrados.

También se utilizaron fotografías de las mismas sesiones para entrenamiento y prueba. Al compartir iluminación, cámara y condiciones similares, la evaluación puede resultar más favorable que una prueba con imágenes tomadas en otro momento.

Por eso sería conveniente evaluar también con sesiones separadas, cambios de iluminación y distintas posiciones del rostro.

## Comparación con CNN

Una CNN, o red neuronal convolucional, utiliza filtros que recorren la imagen para extraer características.

Cada filtro contiene pesos que se multiplican por los valores de una región de la imagen. Los resultados se suman para generar un mapa de características.

Durante el entrenamiento, la red ajusta esos pesos para aprender patrones útiles para la tarea.

### Ejemplo de convolución

Un filtro de 3×3 donde todos los pesos son 1/9 calcula el promedio de cada región.

En el parche de 4×4 del proyecto, usando un desplazamiento de un píxel y sin relleno, el filtro puede colocarse en cuatro posiciones. La salida es de 2×2 y sus valores aproximados son:

75.6, 76.2, 83.0 y 83.1.

Este filtro suaviza la imagen. Otros filtros pueden resaltar bordes y diferentes patrones.

En una CNN entrenada, los filtros se aprenden a partir de los datos, en lugar de definir manualmente todos sus valores.

| Aspecto | Eigenfaces, Fisherfaces y LBPH | CNN |
| ------- | ----------------------------- | --- |
| Extracción de características | Proyecciones estadísticas o patrones locales definidos | Filtros aprendidos durante el entrenamiento |
| Información espacial | Eigenfaces y Fisherfaces aplanan la imagen; LBPH conserva información por regiones | Aprovecha la estructura espacial mediante convoluciones |
| Datos necesarios | Pueden utilizarse en proyectos pequeños, con ejemplos representativos | Entrenar desde cero suele requerir más datos; un modelo preentrenado reduce esa necesidad |
| Recursos | En este proyecto se ejecutaron en CPU | Dependen del tamaño de la red; una GPU puede acelerar el entrenamiento |
| Cambios de luz y posición | Su respuesta depende del método y de las condiciones de captura | Puede lograr mayor robustez con entrenamiento y datos adecuados |
| Incorporación de personas | Eigenfaces y Fisherfaces normalmente requieren reentrenamiento; LBPH admite actualización en OpenCV | Con embeddings se pueden registrar personas sin reentrenar la red |
| Explicación | Sus transformaciones son más sencillas de describir | La interpretación completa de sus decisiones es más compleja |

Una CNN no garantiza eliminar las confusiones. Su rendimiento depende de la calidad de los datos, del entrenamiento y de las condiciones donde se utilice.

También hay que distinguir entre una CNN que clasifica directamente un conjunto fijo de personas y una red que genera embeddings. En el segundo caso, se obtiene un vector del rostro y se compara con los vectores registrados.

## Conclusión

Para las condiciones de este proyecto, LBPH resulta una opción práctica porque entrenó en pocos segundos y consiguió la mayor exactitud. Su principal inconveniente es el crecimiento del archivo al aumentar las fotografías.

Fisherfaces sería una alternativa cuando se busca un modelo pequeño y predicciones rápidas, aunque su entrenamiento fue más lento y su exactitud de prueba quedó por debajo de los otros dos.

Eigenfaces permite entender la reducción de dimensiones y la representación de los rostros, pero mostró dificultades cuando cambiaron las condiciones de la prueba en vivo.

Como siguiente paso se podría evaluar una CNN preentrenada que genere embeddings. Para compararla de forma justa, habría que utilizar las mismas condiciones de evaluación y fotografías de sesiones distintas a las de entrenamiento.
