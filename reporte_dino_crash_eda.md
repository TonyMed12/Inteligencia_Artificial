## Operación Dino Crash

### Bloque 1 — ¿Qué dataset necesitamos?

### Misión 1: Definir el problema y el dataset ideal
--- 
## Escenario P1: ¿Morirá en el siguiente frame?

#### 1. Variable objetivo (Y)

- **Nombre de la columna:** moriraSigFrame
- **Qué representa:** Esta columna representa el posible resultado de forma binaria, usando 0s y 1s para saber si moririá en el siguiente paso.
- **Tipo de variable:** binaria
- **Valores posibles:** 0 y 1

### 2. Variables de entrada (X)

| Variable | Tipo de dato | ¿Qué representa? | ¿Por qué es necesaria? |
|----------|--------------|-------------------|-------------------------|
| obstSigFrame       |  binario         |   Si el frame a continuación representa un obstaculo en el camino                | Porque nos indica si hay un riesgo potencial para evitar morir en el frame inmediato.                         |
| salto |  binario            |  Si el dinosaurio ha decidido saltar                 |   Si ha detectado un obstaculo cercano y salta para poder evitarlo, o decide mantenerse.                       |
| velocidad | numerico            | La velocidad a la que va el dinosaurio.                  |       Influye en la distancia del salto del mismo y la posible evasion de obstaculos.                  |
| altura |  numerico             |  En que posición sobre el eje Y se encuentra el dinosaurio.                 |   Para analizar la posición que ha mantenido cuando encuentra un obstáculo.                      |
| tipoObstaculo | numerico | Si el juego tiene diferentes obstaculos, que tipo es el que se está esquivando | Analizar si el dinosaurio decide mantenerse o brincar |


### 3. Granularidad

- **Una fila representa:** Una fila representa el estado del juego en un frame: posición del dinosaurio, velocidad, acción realizada y características del obstáculo más próximo.
- **Frecuencia de captura:** Lo más que sea posible, yo lo haría cada 16 ms.
- **Justificación:** Es el principal dataset para saber si ha seguido vivo después de realizar una acción o no realizarla, y cuando lo ha hecho de acuerdo a lo que ha detectado.

### 4. Tamaño mínimo razonable

- **Cantidad mínima de filas o partidas:** Con pocas partidas puede ser suficiente, pondría unas 50 a 100, para tener varios ejemplos de cuando ha colisionado y cual es el objetivo del juego, asi como aumentar la frecuencia con que van apareciendo nuevos obstaculos a medida que se avanza.
- **Justificación:** Al buscar clasificar pocas acciones, puede ser suficiente con pocas repeticiones de partidas, ya que los datos se están tomando cada 16 ms, por lo que una gran cantidad de datos pueden ser obtenidos y analizados en el por qué de las situaciones.

### 5. Riesgo si el dataset está mal definido

- **Error de diseño:** Identificar un obstaculo pero no saber donde se encuentra, si lejos o cerca.
- **Consecuencia:** El dinosaurio aprenderá a saltar cuando no debe hacerlo, y no lo hará cuando deba, o a tiempo, por lo que puede morir.

---

## Escenario P2: ¿Cuántos puntos alcanzará esta partida al morir?

### 1. Variable objetivo (Y)

- **Nombre de la columna:** maxPuntos
- **Qué representa:** La cantidad de puntos finales de la partida
- **Tipo de variable:** numérica
- **Valores posibles:** numéricos a partir de 0

### 2. Variables de entrada (X)

| Variable | Tipo de dato | ¿Qué representa? | ¿Por qué es necesaria? |
|----------|--------------|-------------------|-------------------------|
| cantidadPuntos       |   numerica            |   La cantidad de puntos que se han alcanzado                |    Es la metrica principal ligada a la variable objetivo                     |
| distRecorrida       |  numerica             |  La distancia que ha recorrido el dinosaurio                 |   Saber hasta donde ha llegado el dinosaurio para alcanzar esa puntuacion                      |
| tiempo       |  numerica            |  Cuanto tiempo ha estado en juego el dinosaurio                 |   Saber cuanto tiempo ha tardado en morir                      |
| velocidadJuego       | numerica              | La velocidad a la que se movia el dinosaurio o el juego durante esa partida                   |   Para saber si los puntos son linealmente proporcionales o no a la velocidad y al tiempo                      |
|        |              |                   |                         |

### 3. Granularidad

- **Una fila representa:** Cuanto tiempo ha durado vivo hasta alcanzar cierta puntuación
- **Frecuencia de captura:** Una vez por partida
- **Justificación:** Analiza la cantidad de puntos para saber de acuerdo al tiempo un aproximado de puntos que puede alcanzar durante la partida.

### 4. Tamaño mínimo razonable

- **Cantidad mínima de filas o partidas:** Una cantidad bastante grande
- **Justificación:** Puede variar entre partidas de acuerdo a la velocidad del juego si es mayor o menor.

### 5. Riesgo si el dataset está mal definido

- **Error de diseño:** Calcular de forma erronea la cantidad de puntos que puede alcanzar el dinosaurio.
- **Consecuencia:** Esperar una mayor o menor cantidad de puntos

---
## Escenario P3: ¿Qué tipo de obstáculo viene próximo?

### 1. Variable objetivo (Y)

- **Nombre de la columna:** `tipoObstaculoSiguiente`
- **Qué representa:** El tipo de obstáculo que aparecerá después del obstáculo actual.
- **Tipo de variable:** Categórica.
- **Valores posibles:** Cactus pequeño, cactus grande, grupo de cactus, pájaro bajo, pájaro medio y pájaro alto.

### 2. Variables de entrada (X)

| Variable | Tipo de dato | ¿Qué representa? | ¿Por qué es necesaria? |
|----------|--------------|-------------------|-------------------------|
| tipoObstaculoActual | Categórica | El tipo de obstáculo que se encuentra actualmente frente al dinosaurio. | Permite analizar si existe alguna relación entre el obstáculo actual y el que aparece después. |
| tipoObstaculoAnterior | Categórica | El tipo de obstáculo que apareció antes del actual. | Ayuda a identificar posibles secuencias o patrones en la aparición de obstáculos. |
| velocidadJuego | Numérica | La velocidad actual a la que avanza el juego. | Algunos tipos de obstáculos podrían aparecer con mayor frecuencia cuando aumenta la velocidad. |
| puntuacionActual | Numérica | Los puntos acumulados por el jugador en ese momento. | Representa el avance de la partida y permite relacionarlo con la dificultad de los obstáculos. |
| tiempoDesdeUltimoObstaculo | Numérica | El tiempo transcurrido desde que apareció el último obstáculo. | Puede ayudar a identificar la frecuencia y separación con la que se generan los obstáculos. |

### 3. Granularidad

- **Una fila representa:** Un evento de aparición de un obstáculo e incluye información sobre los obstáculos anteriores y el tipo de obstáculo que apareció después.
- **Frecuencia de captura:** Cada vez que aparezca o se genere un nuevo obstáculo.
- **Justificación:** No es necesario registrar información cada 16 ms, porque durante muchos frames el obstáculo será el mismo. Registrar cada aparición para que no repita tanto los datos y nos permite analizar directamente la secuencia de obstáculos.

### 4. Tamaño mínimo razonable

- **Cantidad mínima de filas o partidas:** Al menos 1,000 eventos de aparición de obstáculos, sigo con la idea de entre 50 a 100 partidas.
- **Justificación:** Se necesitan suficientes ejemplos de cada categoría para identificar posibles patrones. También es importante utilizar varias partidas con diferentes puntuaciones y velocidades para evitar que los datos representen una sola forma de jugar.

### 5. Riesgo si el dataset está mal definido

- **Error de diseño:** Registrar una gran cantidad de obstáculos comunes, como los cactus pequeños del juego de Google, pero muy pocos ejemplos de categorías menos frecuentes, como los pájaros.
- **Consecuencia:** El modelo podría aprender a predecir siempre el obstáculo más frecuente y tendría dificultades para identificar correctamente los obstáculos menos comunes.
