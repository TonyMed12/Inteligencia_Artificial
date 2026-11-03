           ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
            EXAMEN TEÓRICO — PERCEPTRÓN, MLP Y CNN (GRUPO B)

                            Eduardo Alcaraz
           ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


1 Datos del alumno
══════════════════

  ━━━━━━━━━━━
   Nombre    José Antonio Medina Ayala
   Grupo   B 
   Fecha    03 de Noviembre de 2026
  ━━━━━━━━━━━


2 Pregunta 1 — Por qué no basta “una sola neurona”
══════════════════════════════════════════════════

  1. Resume el funcionamiento de un perceptrón como “regla de decisión”.
  2. Explica la noción de *frontera lineal* sin escribir fórmulas.
  3. Da un ejemplo (aunque sea inventado) donde esa frontera *no*
     alcanza.


2.0.1 Respuesta
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌
1. Recibe varios valores de entrada y cada uno tiene su importancia o peso sináptico, con la combinacion de estos aplica reglas para definir a que clase pertenece,
2. La frontera lineal es la separación de clases, como el ejemplo de los tacos y el refresco, el estado de ánimo era separable linealmente dejando una clase de un lado y otra del otro.
3. Cuando hicimos el ejemplo de las XOR que quedaban mezcladas ambas clases, y teniamos que añadir una capa más para poder realizar el procesamiento, no puede separarlos ninguna linea.

3 Pregunta 2 — CNN: de píxeles a mapas
══════════════════════════════════════

  1. ¿Qué problema de *estructura espacial* ignora un MLP si aplana la
     foto?
  2. ¿Qué significa que un mismo filtro se aplique en toda la imagen?
  3. Además de clasificación de objetos, cita *dos* usos donde la CNN
     encaja bien.


3.0.1 Respuesta
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌

 1. Los patrones locales, porque los convierte en vector y ya no verifica la cercania de los pixeles.
 2. Que usa los mismos pesos en varias regiones, desplazandolo para buscar un patron.
 3. Usos medicos como la detección del cancer que nos explicó y defectos en productos industriales como los tornillos.


4 Pregunta 3 — Softmax, clases y decisiones
═══════════════════════════════════════════

  1. Diferencia un problema *binario* de uno *multiclase* en términos de
     salida de la red.
  2. ¿Qué lectura le das a las probabilidades que produce un softmax?
  3. Imagina tres carpetas de imágenes (tres riesgos o tres animales):
     ¿qué arquitectura de *salida* usarías y por qué?


4.0.1 Respuesta
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌
1. El binario tiene dos clases, y el multiclase requiere de una salida diferente, como la softmax que era para este tipo, a diferencia de la sigmoide que se usaba cuando solo teniamos dos clases.
2. Valores entre 0 y 1 con las probabilidades.
3. La softmax, con una neurona por clase para que cada una se encargue de lo suyo y porque este tipo de neurona de salida nos permite clasificar a partir de 2 clases.


5 Pregunta 4 — MLP: profundidad con sentido
═══════════════════════════════════════════

  1. ¿Qué ganas al apilar capas densas con activaciones no lineales?
  2. ¿Qué riesgo aparece si la red es *demasiado* grande para tus datos?
  3. Relaciona “capa oculta” con la idea de aprender representaciones
     intermedias (en palabras simples).


5.0.1 Respuesta
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌

1. El poder aprender relaciones mas complejas que con una separacion lineal no seria suficiente.
2. Va a tardar un montón entrenando, además de un posible sobreajuste, no tendría la capacidad de detectar lo que realmente es.
3. Transforma las entradas en representaciones intermedias que van a ayudar a las capas siguientes a poder resolver el problema, construyendo la respuesta por etapas.


6 Pregunta 5 — Entrenamiento sin magia
══════════════════════════════════════

  1. Explica qué es una *época* a nivel intuitivo.
  2. ¿Hacia qué “objetivo” se mueven los pesos cuando hablamos de
     minimizar el error?
  3. Si el learning rate es absurdo (muy grande o muy chico), ¿qué
     síntomas conceptuales esperarías?


6.0.1 Respuesta
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌
1. Una epoca es una pasada completa por todos los ejemplos o elementos del conjunto que se le proporciona para el entrenamiento.
2. Se ajusta, despues de cada época comparando con lo que quiere acercarse.
3. Puede hacer que se salte demasiado y terminar mas arriba, o que aumente la perdida, y si es muy pequeño pues que el aprendizaje sea lento y tardar demasiado si avanzamos con pasos muy pequeños.

7 Pregunta 6 — Bloques de una CNN
═════════════════════════════════

  Sin matemáticas, di el papel de cada pieza:
  1. Kernel / filtro convolucional
  2. Función de activación después de conv
  3. Max pooling
  4. Capa densa + softmax al final de un clasificador


7.0.1 Respuesta
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌

1. Recorrer regiones de la imagen y responder a los patrones que encuentre, pistas visuales.
2. Transformar las respuestas.
3. Reducir el tamaño de las imagenes para conservar un tamaño maximo.
4. Combinar las caracteristicas para obtener puntuaciones por clase y obtener probabilidades.


8 Pregunta 7 — XOR y la historia del aprendizaje
════════════════════════════════════════════════

  1. ¿Por qué XOR se volvió un “contraejemplo” famoso?
  2. ¿Qué elemento de un MLP permite patrones que no son una sola línea?
  3. ¿En qué tipo de tabla/problema del mundo real seguirías usando algo
     tan simple como un perceptrón o un modelo lineal?


8.0.1 Respuesta
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌

1. Porque cuando comenzamos a trabajar con los perceptrones no fue capaz de representar directamente esta compuerta al no ser linealmente separables.
2. Las capas ocultas con activaciones no lineales sirven para estos patrones mas complejos.
3. Cuando hay relaciones sencillas pero las clases son facilmente diferenciables, entre lo que se requiere distinguir, una comparacion o diferencia que para nosotros pudiera ser más obvia, ejemplo de una computadora contra un celular.


9 Pregunta 8 — De lo simple a lo abstracto en visión
════════════════════════════════════════════════════

  1. Contrasta lo que buscan capas tempranas vs capas profundas en una
     CNN.
  2. ¿Qué intuición da eso para *transfer learning* (aunque no lo hayas
     programado)?
  3. ¿Cuándo sospecharías que tu CNN es “profunda de más” para el
     dataset del curso?


9.0.1 Respuesta
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌
1. En capas tempranas detecta patrones facilmente diferenciables o simples, como texturas en general, y las profundas comparan patrones mas complejos con partes especificas de la imagen y objetos.
2. No me acuerdo que es transfer learning, pero creo era de aprovechar lo que se aprendió y puede ser para capas profundas aplicar lo que aprendió o se entrenó antes.
3. Si se aprende bien el entrenamiento pero en validación o trabajo real no cumple con su trabajo, comparandola con alguna más pequeña que si lo haga.

10 Pregunta 9 — Análisis: matching problema → modelo
════════════════════════════════════════════════════

  Elige *Perceptrón/lineal* / *MLP* / *CNN* (una opción) y justifica en
  una frase:

  1. Predecir “compra / no compra” con edad, ingreso y visitas a la web
     (columnas numéricas).
  2. Detectar si hay un gato en una foto.
  3. Aproximar una frontera “curva” o a trozos en datos 2D tabulares.
  4. ¿CNN es la herramienta natural para predecir la siguiente palabra
     de un párrafo? Justifica y nombra una familia del curso más
     adecuada (RNN/Transformer a nivel nombre).


10.0.1 Respuesta
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌

Pendiente


11 Pregunta 10 — Análisis: auditoría de un equipo
═════════════════════════════════════════════════

  Un equipo entrega un modelo “muy preciso” en su laptop y falla con
  datos nuevos.

  1. ¿Qué diagnósticos conceptuales harías *antes* de tocar
     hiperparámetros al azar?
  2. Pide *dos* evidencias que deberían mostrar en la defensa (curvas,
     separación train/val, ejemplos de error, etc.).
  3. Redacta *tres* preguntas tuyas para comprobar si saben cuándo usar
     perceptrón, MLP o CNN (distintas a las del Grupo A si las conoces).


11.0.1 Respuesta
╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌

1. Checar si hay un sobreajuste, si se entrenó correctamente o si hay otros datos que no se han considerado.
2. 
