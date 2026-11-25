# Modelos que generan imágenes a partir de texto y su dataset para LoRA

**Nombre:** José Antonio Medina Ayala

---

## 1. Qué buscamos

Actividad: **investigar modelos que puedan generar imágenes con texto**, es decir, modelos que generan imágenes a partir de una descripción y que además pueden escribir **texto legible dentro de la imagen** (letreros, carteles, portadas). También se define **cómo debe ser el dataset** para ajustar uno de esos modelos con **LoRA**.

Este archivo complementa [`dataset_lora_medina.md`](dataset_lora_medina.md). Ese archivo trata el fine-tuning de un modelo de **lenguaje**, y este trata un modelo de **imágenes**.

---

## 2. Modelos de texto a imagen

| Modelo | Empresa | ¿Pesos descargables? | ¿Acepta LoRA? | Texto dentro de la imagen |
|---|---|---|---|---|
| GPT Image 2 | OpenAI | No, solo API | No | Encabeza los rankings de texto en imagen, también con alfabetos asiáticos |
| Nano Banana Pro / 2 | Google | No, solo API | No | Entre los mejores en texto de varias líneas y fotorrealismo |
| Ideogram | Ideogram | No, solo API | No | Muy bueno para controlar el diseño del texto, pero falla con acentos y alfabetos no latinos |
| Qwen-Image / Qwen-Image-2512 | Alibaba (Qwen) | Sí, Apache 2.0 | Sí | Diseñado para texto complejo, sobre todo en chino |
| FLUX.2 [dev] | Black Forest Labs | Sí, licencia no comercial | Sí | Mejoró mucho respecto a FLUX.1 en tipografía e infografías |
| FLUX.2 [klein] 4B | Black Forest Labs | Sí, Apache 2.0 | Sí | Versión pequeña y rápida de FLUX.2 |
| Stable Diffusion 3 / 3.5 | Stability AI | Sí, licencia comunitaria | Sí | Su arquitectura MMDiT y sus tres codificadores de texto (CLIP ×2 + T5) mejoraron la tipografía |

### Observaciones

- **Los mejores en texto son cerrados.** GPT Image 2, Nano Banana e Ideogram solo se usan por API, así que no se les puede hacer LoRA.
- **Para hacer LoRA hay que usar un modelo abierto.** Qwen-Image es la mejor opción si se va a usar comercialmente, por su licencia Apache 2.0. FLUX.2 [dev] da más calidad, pero su licencia no permite usarlo comercialmente sin pagar.
- **El hardware importa.** Según su documentación, incluso FLUX.2 [klein] 4B, el más pequeño, pide unos 13 GB de VRAM. Con una GPU de 4 GB, como la RTX 3050 de mi equipo, no alcanza para entrenar ninguno de estos modelos en local. Habría que usar Google Colab o una GPU rentada, o bien Stable Diffusion 1.5, que es más ligero.
- **Los rankings cambian cada mes.** Los que consulté son de 2026 y algunos los publican empresas que venden acceso a los modelos, así que los tomé solo como referencia.

---

## 3. Estructura del dataset

Un modelo de texto a imagen aprende de **pares imagen + descripción**. Si queremos que aprenda a escribir texto, la descripción debe incluir **el texto exacto que aparece en la imagen, entre comillas**.

### 3.1 Organización de carpetas

Uso el formato `imagefolder` de Hugging Face, que es el que leen los scripts de `diffusers`:

```
dataset_letreros/
└── train/
    ├── metadata.jsonl
    ├── img_001.png
    ├── img_002.png
    └── img_003.png
```

### 3.2 Archivo `metadata.jsonl`

Cada línea describe una imagen:

| Campo | Contenido |
|---|---|
| `file_name` | Nombre de la imagen, relativo a la carpeta del `metadata.jsonl` |
| `text` | Descripción de la escena, con el texto exacto entre comillas |

Ejemplo con letreros de comercios de Morelia:

```json
{"file_name": "img_001.png", "text": "fachada de una tortillería con un letrero pintado a mano que dice \"Tortillería La Michoacana\" en letras rojas sobre fondo amarillo"}
{"file_name": "img_002.png", "text": "cartel de papel pegado en una pared que dice \"SE RENTA\" en letras negras y un número de teléfono abajo"}
{"file_name": "img_003.png", "text": "pizarrón de una cafetería que dice \"Café de olla $25\" escrito con gis blanco"}
```

### 3.3 Cargarlo en Python

```python
from datasets import load_dataset

ds = load_dataset("imagefolder", data_dir="dataset_letreros", split="train")
print(ds)          # columnas: image, text
print(ds[0]["text"])
```

### 3.4 Criterios de calidad

1. **Resolución uniforme.** Todas las imágenes deben estar a la resolución de entrenamiento (512 px para SD 1.5, 1024 px para SDXL o Qwen-Image), recortadas o escaladas.
2. **Texto legible en la foto.** Si una persona no puede leer el letrero, el modelo tampoco aprende a escribirlo.
3. **Descripción fiel.** Lo que va entre comillas debe coincidir letra por letra con la imagen, incluidos los acentos y la ñ, porque ahí es donde más fallan los modelos.
4. **Variedad.** Conviene mezclar tipos de letra, colores, ángulos e iluminación.
5. **Cantidad.** Para un estilo o un concepto, unas decenas o cientos de imágenes bien descritas suelen bastar con LoRA.

---

## 4. Cómo entra LoRA

LoRA **no cambia el dataset**. Lo que cambia es qué se entrena. El modelo original se congela y en las capas de atención se agregan dos matrices pequeñas, **A** y **B**:

$$h = W_0x + BAx$$

- `W₀`: pesos originales, que no se tocan.
- `B` y `A`: matrices de rango `r`, las únicas que se entrenan.

Parámetros principales:

| Parámetro | Qué controla |
|---|---|
| `rank` (`r`) | Tamaño de las matrices A y B. Un valor mayor da más capacidad, pero también más memoria |
| `lora_alpha` | Escala del ajuste. El efecto real es `alpha / r` |
| `target_modules` | Capas donde se aplica. En `diffusers` suelen ser `to_q`, `to_k`, `to_v` y `to_out.0` |

Ejemplo de entrenamiento con el script de `diffusers`:

```bash
accelerate launch train_text_to_image_lora.py \
  --pretrained_model_name_or_path="stable-diffusion-v1-5/stable-diffusion-v1-5" \
  --train_data_dir="dataset_letreros" \
  --caption_column="text" \
  --resolution=512 \
  --train_batch_size=1 \
  --gradient_accumulation_steps=4 \
  --rank=8 \
  --learning_rate=1e-4 \
  --max_train_steps=3000 \
  --mixed_precision="fp16" \
  --output_dir="lora_letreros"
```

El resultado es un archivo pequeño, `pytorch_lora_weights.safetensors`, que se carga sobre el modelo base:

```python
from diffusers import StableDiffusionPipeline
import torch

pipe = StableDiffusionPipeline.from_pretrained(
    "stable-diffusion-v1-5/stable-diffusion-v1-5", torch_dtype=torch.float16
).to("cuda")
pipe.load_lora_weights("lora_letreros")

img = pipe("letrero de una paletería que dice \"La Flor\" en letras azules").images[0]
img.save("prueba.png")
```

---

## 5. Diferencia con el dataset de un modelo de lenguaje

| | Modelo de lenguaje (mi `dataset_lora_medina.md`) | Modelo de imágenes (este archivo) |
|---|---|---|
| Ejemplo | Pregunta → respuesta, en texto | Descripción → imagen |
| Archivo | Un solo `.jsonl` con `messages` | Carpeta de imágenes + `metadata.jsonl` |
| Qué aprende | Cómo responder | Cómo se ve lo que se describe |
| Dónde se aplica LoRA | Capas de atención del transformer | Capas de atención del UNet o transformer de difusión |

---

## 6. Fuentes

- Qwen-Image, repositorio oficial y reporte técnico (arXiv 2508.02324)
- Black Forest Labs, anuncio de FLUX.2 y documentación de FLUX.2 [klein]
- Hugging Face `diffusers`, guía de entrenamiento LoRA para text-to-image
- Comparativas de 2026 de texto en imagen (fal.ai, invideo), usadas solo como referencia
