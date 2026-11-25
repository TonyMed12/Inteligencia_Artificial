# Comparación de IAs: La elección imposible

**Nombre:** José Antonio Medina Ayala

---

## 1. Qué se hizo

Se tomó el caso **"La elección imposible"** (las 36 tarjetas de los tres sobres y las 6 preguntas del dictamen), se transcribió completo en un solo prompt y se le dio **el mismo prompt** a cuatro IAs. Después se compararon sus respuestas contra la solución del caso.

| IA | Dónde corrió | Tamaño | Tiempo de respuesta |
|---|---|---|---|
| qwen3.5:4b | Local, con Ollama | 3.3 GB | 295 s |
| gemma3:4b | Local, con Ollama | 3.4 GB | 112 s |
| smollm2:1.7b | Local, con Ollama | 1.8 GB | 42 s |
| Claude (Opus 5.5) | En línea (claude.ai) | — | — |

Condiciones:

- Los modelos locales corrieron en mi equipo (RTX 3050 de 4 GB y 16 GB de RAM) con un contexto de 8,192 tokens, para que pudieran leer el expediente completo.
- En qwen3.5 se desactivó el modo de razonamiento ("thinking"), porque estos modelos pueden quedarse pensando hasta el límite de tokens y no llegar a contestar.
- Cada IA se corrió una sola vez.

**Calificación:** 1 punto si la respuesta es correcta, 0.5 si está incompleta o tiene errores parciales y 0 si es incorrecta. Total posible: 6 puntos.

---

## 2. Respuestas por pregunta

### Pregunta 1: ¿Cuántos votos físicos hay y cuánto suman las cinco actas?

**Solución:** 256 votos físicos. Las actas suman 53 + 50 + 58 + 46 + 49 = 256, con Lucía 123, Mateo 119, blancos 7 y nulos 7 (E02–E06, E09).

- **qwen3.5:4b (1):** dio 256, sumó los totales de las cinco actas y lo confirmó con las firmas y boletas de E09 y con E35.
- **gemma3:4b (1):** dio 256 y listó el total de cada acta, apoyándose en E35. No desglosó por candidato, pero eso no lo pedía la pregunta de forma directa.
- **smollm2:1.7b (0):** dijo que los votos válidos eran 53, mezcló números de distintas tarjetas (por ejemplo, le atribuyó 18 boletas a E01, que son las reglas de validez y no un acta) y sumó los 22 votos de A17 como si fueran reales.
- **Claude (1):** 256, con una tabla de las cinco actas por candidato, y lo cruzó con E09, E13, E14 y E35.

### Pregunta 2: ¿Qué candidato gana con el recuento verificado y por cuántos votos?

**Solución:** Lucía, 123 contra 119, por 4 votos.

- **qwen3.5:4b (1):** Lucía por 4. Lo sacó restando los 22 votos de A17 al resultado del tablero (141 − 22 = 119), no sumando las actas, pero el resultado es correcto.
- **gemma3:4b (0):** escribió "Lucía gana con 123 votos y Mateo con 141, separándolos por 18 votos". Es contradictorio: usó los números del tablero, donde Mateo gana, y la diferencia de 18 tampoco cuadra con ninguna lectura del caso.
- **smollm2:1.7b (0):** dio como ganador a Mateo con 141, es decir, aceptó el resultado alterado del tablero.
- **Claude (1):** Lucía por 4, y explicó que 141 = 119 + 22 y que 278 = 256 + 22.

### Pregunta 3: ¿Qué efecto tiene A17? ¿Qué folios están duplicados?

**Solución:** A17 le suma 22 votos a Mateo, pero son los folios M1-029 a M1-050, que ya estaban contados en el acta de la Mesa 1 (E13, E15, E29, E30, E35).

- **qwen3.5:4b (1):** identificó los folios M1-029 a M1-050, los relacionó con los votos de Mateo en la Mesa 1 (E13, E02) y concluyó que se contaron dos veces (E35).
- **gemma3:4b (1):** identificó el rango M1-029 a M1-050 y citó a Elisa (E29), que confirma que esos 22 votos ya estaban dentro de las 53 boletas. Es curioso que aquí sí detectó la duplicación, pero en la pregunta 2 no la usó para corregir el resultado.
- **smollm2:1.7b (0):** dijo que A17 contiene "22 folios nuevos", que es justo lo contrario de lo que prueban las tarjetas.
- **Claude (1):** folios M1-029 a M1-050, ya contados (E13, E15, E29, E30, E32). Además señaló que A17 no cumple ninguna de las condiciones de validez de E01.

### Pregunta 4: Orden por hora oficial

**Solución** (relojes de E19: impresora P va 4 min atrasada, así que se le suman 4; terminal T va 7 min adelantada y cámara C 4 min adelantada, así que se les restan):

| Evento | Hora registrada | Hora oficial |
|---|---|---|
| Impresión P-43 | 11:05 (P) | 11:09 |
| Impresión P-44 | 11:10 (P) | 11:14 |
| Llegada de A17 a la bandeja | 11:18 | 11:18 |
| Captura en T | 11:27 (T) | 11:20 |

- **qwen3.5:4b (0.5):** convirtió bien P-44 (11:14) y la captura en T (11:20), y puso la llegada a las 11:18. Pero confundió P-43 con P-44: a las dos les asignó la hora 11:10 de la impresora (E17), cuando P-43 viene de E31 (11:05). Además la respuesta se contradice y se corrige a sí misma a mitad del texto.
- **gemma3:4b (0.5):** el orden le quedó bien, pero no convirtió ninguna hora: dejó 11:05, 11:10, 11:18 y 11:27, que son las horas de cada reloj y no la oficial. Dice que usó los atrasos de los relojes, pero no se nota en los resultados.
- **smollm2:1.7b (0):** no contestó la pregunta. Ordenó "Imagen 1" a "Imagen 5", que no tiene relación con el caso.
- **Claude (1):** las cuatro horas convertidas correctamente, y notó que la cámara (11:18 − 4 = 11:14) coincide con la impresión de P-44.

### Pregunta 5: ¿Quién envió/preparó A17? ¿Quién lo transcribió? ¿Qué NO se puede afirmar del beneficiario?

**Solución:**
- **Envió e imprimió:** la credencial R7, de Rebeca (E17, E24, E28). Una credencial sola no basta (E24), pero la cámara la muestra frente a la impresora con A17 a las 11:14 oficial (E25), estaba en el pasillo de 11:12 a 11:17 (E20) y Martín la vio dejarla en la bandeja (E33). Lo que no se puede probar es que ella redactó el contenido.
- **Transcribió:** Julia (E22, E26), sin verificar los folios.
- **Beneficiario:** Mateo. No se puede afirmar que conociera o autorizara A17 (E34, E36).

- **qwen3.5:4b (0.5):** acertó con Julia y con Mateo. Pero afirmó que Rebeca "fue la responsable de la generación del documento" basándose sobre todo en la credencial, justo lo que E24 dice que no es suficiente. Tampoco usó la cámara (E25) ni el testigo (E33) como prueba principal. Mezcló un hecho comprobado con una inferencia, que es lo que pedían las reglas evitar.
- **gemma3:4b (1):** Rebeca con tres evidencias distintas (cámara E25, credencial E28 y testigo E33), Julia con E22 y Mateo sin evidencia de conocer A17 (E34). Es una respuesta corta, pero bien sustentada.
- **smollm2:1.7b (0):** solo dijo que el usuario de envío es R7. No identificó a nadie, no mencionó a Julia y no respondió sobre el beneficiario.
- **Claude (1):** separó qué está comprobado (Julia transcribió), qué es una inferencia fuerte (Rebeca imprimió, con cámara, credencial y horario) y qué no se puede afirmar (que Rebeca redactara el contenido y que Mateo supiera de A17).

### Pregunta 6: Dos medidas de control

**Solución esperada:** cualquier par de controles que hubiera frenado A17. Por ejemplo, no capturar ajustes sin las dos firmas y la comprobación física (E01, E16), validar folios contra los ya registrados, eliminar cuentas compartidas como C1 (E18) o impedir el préstamo de credenciales (E24).

- **qwen3.5:4b (1):** propuso autenticación más fuerte para imprimir y usar la terminal, y validar los folios de cualquier ajuste contra las actas antes de publicarlo. La segunda hubiera detectado A17 directamente.
- **gemma3:4b (1):** propuso validar físicamente cada hoja antes de incluirla, como exige E01, y controlar mejor el acceso a la terminal y el uso de credenciales.
- **smollm2:1.7b (0):** solo repitió que el uso de la credencial R7 y de la impresora estaban prohibidos. Son reglas que ya existían, no medidas nuevas.
- **Claude (1):** bloquear la captura de ajustes sin firmas ni comprobación física, y validar folios automáticamente contra los ya registrados y contra el total de votantes.

---

## 3. Resultados

| Pregunta | qwen3.5:4b | gemma3:4b | smollm2:1.7b | Claude* |
|---|---|---|---|---|
| 1. Votos y actas | 1 | 1 | 0 | 1 |
| 2. Ganador y margen | 1 | 0 | 0 | 1 |
| 3. Efecto de A17 y folios | 1 | 1 | 0 | 1 |
| 4. Cronología | 0.5 | 0.5 | 0 | 1 |
| 5. Responsables | 0.5 | 1 | 0 | 1 |
| 6. Medidas de control | 1 | 1 | 0 | 1 |
| **Total** | **5 / 6** | **4.5 / 6** | **0 / 6** | **6 / 6** |
| Tiempo | 295 s | 112 s | 42 s | — |

\* Claude no fue una prueba a ciegas (ver la nota de la sección 1).

---

## 4. Análisis

- **qwen3.5:4b fue el mejor de los modelos locales**, pero también el más lento (casi 5 minutos). Hizo bien las cuentas. Sus fallas fueron de razonamiento: confundió dos impresiones distintas y dio por hecho algo que solo era una inferencia (que Rebeca creó el documento). Además, se nota que "piensa en voz alta": en la pregunta 4 se corrige a sí mismo varias veces dentro de la misma respuesta.
- **gemma3:4b tuvo el error más grave de los dos buenos:** en la pregunta 2 dio por buenos los números del tablero, que es justo lo que había que cuestionar. Aun así, en la pregunta 3 sí detectó que los votos estaban duplicados. Es decir, tenía la información correcta, pero no la conectó entre preguntas. A cambio, fue la que mejor sustentó a los responsables con varias tarjetas.
- **smollm2:1.7b no pudo con el caso.** Con 1.7 mil millones de parámetros, no logró seguir 36 tarjetas y 6 preguntas a la vez: inventó tarjetas, aceptó el resultado alterado y en la cronología contestó algo sin relación. Fue la más rápida, pero sin ningún punto.
- **La pregunta más difícil fue la cronología.** Ningún modelo local convirtió bien las cuatro horas. Es la que exige más pasos: buscar cada evento en su tarjeta, saber de qué reloj viene, recordar si ese reloj va atrasado o adelantado y aplicar la corrección.
- **El tamaño sí importó**, pero no por igual: qwen3.5 y gemma3 tienen casi el mismo tamaño (≈ 3.3 GB) y aun así cometieron errores muy distintos.

## 5. Conclusión

Para un caso que pide cruzar mucha información, los modelos locales de 4B parámetros sacan las cuentas directas (sumas, folios), pero fallan cuando hay que encadenar varios pasos o distinguir lo que está probado de lo que solo se sospecha. Ninguna de las IAs locales hubiera bastado por sí sola para escribir el dictamen sin que alguien revisara sus respuestas, y la más pequeña directamente aceptó el resultado alterado.
