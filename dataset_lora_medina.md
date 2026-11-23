# ¿Qué estructura y formato debe tener un dataset para hacer fine-tuning de un modelo con LoRA?

**Nombre:** José Antonio Medina Ayala

---

## 1. Definiciones

El **fine-tuning** (ajuste fino) consiste en tomar un modelo de lenguaje ya entrenado y seguir entrenándolo con datos propios. Así se adapta a una tarea, un dominio o un estilo de respuesta en particular. El modelo no aprende el lenguaje desde cero. Lo que se ajusta es su comportamiento.

**LoRA** (*Low-Rank Adaptation*) es una técnica para hacer este ajuste de forma eficiente. Los pesos originales del modelo se dejan congelados y se entrenan unas matrices pequeñas adicionales, llamadas adaptadores, que se suman a ciertas capas. Así se entrena solo una fracción mínima de los parámetros, y eso reduce mucho el uso de memoria y el tiempo de entrenamiento. Su variante **QLoRA** carga además el modelo base cuantizado a 4 bits, lo que permite entrenar en GPUs con poca VRAM.

LoRA no cambia el formato del dataset. Es una técnica de entrenamiento, y los datos se preparan igual que para cualquier fine-tuning supervisado. Lo que define la estructura es **el tipo de tarea** y **el tipo de modelo** (base o de chat).

---

## 2. Formato de archivo: JSONL

El formato más usado es **JSONL** (*JSON Lines*):

- Cada línea es un objeto JSON independiente que representa **un ejemplo de entrenamiento**.
- Las líneas no se separan con comas ni se encierran en un arreglo `[ ]`.
- Esto permite leer el archivo línea por línea, sin cargarlo completo en memoria, y es el formato que esperan bibliotecas como Hugging Face `datasets`, TRL o Unsloth.

---

## 3. Estructuras de dataset según la tarea

### 3.1 Formato conversacional (modelos de chat)

Es el más recomendable cuando el modelo base es de tipo *instruct* o *chat* (por ejemplo Qwen, Gemma o Llama en sus versiones *Instruct*). Cada ejemplo es una lista de mensajes con un rol y un contenido:

```json
{"messages": [{"role": "system", "content": "Eres un asistente que clasifica facturas."}, {"role": "user", "content": "Proveedor: CFE. Concepto: consumo de energía eléctrica."}, {"role": "assistant", "content": "Categoría: Servicios - Electricidad"}]}
```

| Rol | Función |
|---|---|
| `system` | Instrucción general sobre el comportamiento del modelo (opcional). |
| `user` | Entrada del usuario: la pregunta o petición. |
| `assistant` | Respuesta esperada. Es lo que el modelo aprende a generar. |

### 3.2 Formato de instrucciones (estilo Alpaca)

Es útil para tareas de un solo paso, como clasificación, extracción de información, resumen o traducción:

```json
{"instruction": "Clasifica la siguiente factura en una categoría contable.", "input": "Proveedor: Telmex. Concepto: servicio de internet.", "output": "Servicios - Telecomunicaciones"}
```

- `instruction`: la tarea que debe realizar el modelo.
- `input`: los datos sobre los que trabaja (puede ir vacío).
- `output`: la respuesta correcta.

Antes de entrenar, estos campos se convierten a una plantilla de texto o al formato conversacional.

### 3.3 Formato de texto plano (*completion*)

```json
{"text": "Texto continuo del dominio que se desea que el modelo aprenda..."}
```

Sirve para adaptar el modelo a un vocabulario o estilo de un dominio específico (documentos técnicos, legales, médicos). **No** le enseña a seguir instrucciones. Por eso se usa más con modelos base que con modelos de chat.

---

## 4. La plantilla de chat (*chat template*)

Cada familia de modelos convierte la lista de mensajes a texto usando **tokens especiales** que marcan dónde empieza y termina cada turno. Por ejemplo:

- Qwen usa `<|im_start|>` y `<|im_end|>`.
- Gemma usa `<start_of_turn>` y `<end_of_turn>`.

---

## 5. Criterios de calidad del dataset

1. **Calidad antes que cantidad.** Entre unos cientos y un par de miles de ejemplos bien hechos suelen bastar para notar cambios con LoRA. Los ejemplos con errores o contradicciones se aprenden igual que los buenos.
2. **Consistencia.** Todas las respuestas deben seguir el mismo estilo, longitud y formato de salida. El modelo imita el patrón dominante.
3. **Variedad en las entradas.** Hay que incluir distintas formas de pedir lo mismo (sinónimos, errores de escritura, preguntas cortas y largas) para que el modelo generalice.
4. **Cobertura de casos límite.** Conviene incluir ejemplos de lo que el modelo **no** debe hacer o de cuándo debe decir que no sabe.
5. **Limpieza.** Hay que eliminar duplicados y datos personales o sensibles, y revisar que el JSON sea válido en cada línea.
6. **Longitud controlada.** Cada ejemplo debe caber dentro de la longitud máxima de secuencia configurada (`max_seq_length`). Si no, se trunca.

---

## 6. Flujo general de trabajo

1. **Reunir los datos:** juntar ejemplos de preguntas y las respuestas que se esperan del modelo.
2. **Darles formato:** pasar esos ejemplos a un archivo JSONL con la estructura `messages`.
3. **Revisar y limpiar:** quitar duplicados y errores y verificar que todos los ejemplos sigan el mismo estilo.
4. **Entrenar con LoRA:** usar una herramienta como Unsloth para entrenar los adaptadores sobre el modelo base.
5. **Probar el modelo:** hacerle preguntas nuevas y comprobar que responda como se espera.
6. **Usar el modelo:** exportarlo y cargarlo en una herramienta como Ollama para utilizarlo.
