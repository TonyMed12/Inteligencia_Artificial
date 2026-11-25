"""
Genera ComparacionCNN_deportes.md a partir de los reportes de cada corrida.

Uso (en una celda al final del notebook, después de evaluar cada modelo):

    from sklearn.metrics import classification_report
    import json
    rep = classification_report(y_true, y_pred, target_names=clases, output_dict=True)
    json.dump(rep, open(f"reportes/{nombre_corrida}.json", "w"), indent=2)

y luego:

    python generar_reporte_cnn.py   (desde la carpeta del repositorio)

Espera esta estructura:
    reportes/base.json, reportes/40_epocas.json, ...
    images/cnn/<corrida>_grafica.png y images/cnn/<corrida>_matriz.png
"""
import json
from pathlib import Path

UMBRAL = 0.80  # alto = 80 % o más

CORRIDAS = [
    ("base", "Base", "Nada, es la referencia"),
    ("40_epocas", "40 épocas", "40 épocas en vez de 20"),
    ("64_filtros", "64 filtros", "64 filtros en vez de 32"),
    ("dropout_02", "Dropout 0.2", "Dropout de 0.2 en vez de 0.5"),
    ("dos_bloques", "Dos bloques de convolución", "Se agregó un segundo bloque Conv2D + MaxPooling"),
    ("40_epocas_dropout_02", "40 épocas + Dropout 0.2", "Las dos mejoras juntas"),
]

REPORTES = Path("reportes")
IMG = "images/cnn"
NO_CLASES = {"accuracy", "macro avg", "weighted avg"}


def pct(x):
    return f"{x * 100:.1f} %"


def tabla_reporte(rep):
    filas = ["| Deporte | Precisión | Recall | F1 | Fotos |", "|---|---|---|---|---|"]
    for clase, m in rep.items():
        if clase in NO_CLASES:
            continue
        filas.append(f"| {clase} | {m['precision']:.2f} | {m['recall']:.2f} | "
                     f"{m['f1-score']:.2f} | {int(m['support']):,} |")
    total = int(rep["macro avg"]["support"])
    filas.append(f"| **Exactitud total** | | | {rep['accuracy']:.2f} | {total:,} |")
    for k, nombre in (("macro avg", "Promedio simple"), ("weighted avg", "Promedio ponderado")):
        m = rep[k]
        filas.append(f"| {nombre} | {m['precision']:.2f} | {m['recall']:.2f} | "
                     f"{m['f1-score']:.2f} | {total:,} |")
    return "\n".join(filas)


def clasificacion(rep):
    grupos = {"Alta precisión y alto recall": [], "Alta precisión y bajo recall": [],
              "Baja precisión y alto recall": [], "Baja precisión y bajo recall": []}
    for clase, m in rep.items():
        if clase in NO_CLASES:
            continue
        p = "Alta" if m["precision"] >= UMBRAL else "Baja"
        r = "alto" if m["recall"] >= UMBRAL else "bajo"
        grupos[f"{p} precisión y {r} recall"].append(clase)
    return "\n".join(f"- **{g}:** {', '.join(c) if c else 'ninguno'}" for g, c in grupos.items())


def main():
    reps = {}
    for clave, _, _ in CORRIDAS:
        f = REPORTES / f"{clave}.json"
        if f.exists():
            reps[clave] = json.loads(f.read_text(encoding="utf-8"))
        else:
            print(f"Falta {f}, se omite esa corrida")

    out = ["# Comparación de corridas: CNN de clasificación de deportes", "",
           "**Nombre:** José Antonio Medina Ayala", "", "---", "",
           "## Resumen", "",
           "| Corrida | Qué se cambió | Exactitud | F1 promedio | Deporte más débil (recall) |",
           "|---|---|---|---|---|"]
    for clave, nombre, cambio in CORRIDAS:
        if clave not in reps:
            continue
        rep = reps[clave]
        clases = {c: m for c, m in rep.items() if c not in NO_CLASES}
        debil = min(clases, key=lambda c: clases[c]["recall"])
        out.append(f"| {nombre} | {cambio} | {pct(rep['accuracy'])} | "
                   f"{rep['macro avg']['f1-score']:.2f} | {debil} ({pct(clases[debil]['recall'])}) |")

    out += ["", "**Cómo leer las tablas:**", "",
            "- **Precisión:** de las fotos que la red etiquetó como ese deporte, cuántas sí lo eran.",
            "- **Recall:** de todas las fotos reales de ese deporte, cuántas encontró.",
            "- **F1:** combina precisión y recall en un solo número.",
            f"- Se considera **alto** un valor de {int(UMBRAL * 100)} % o más.", "",
            "---", "", "## Corrida por corrida", ""]

    for clave, nombre, _ in CORRIDAS:
        if clave not in reps:
            continue
        out += [f"### {nombre}", "",
                f"![Gráfica de entrenamiento – {nombre}]({IMG}/{clave}_grafica.png)", "",
                f"![Matriz de confusión – {nombre}]({IMG}/{clave}_matriz.png)", "",
                "**Observaciones:**", "",
                "- <!-- ¿Las curvas de entrenamiento y validación se separan (sobreajuste) o siguen subiendo (le faltó tiempo)? -->",
                "- <!-- ¿Qué deportes confunde más según la matriz? (ej. americano ↔ golf) -->",
                "- <!-- ¿Qué cambió respecto a la corrida base? -->", "",
                "**Reporte completo:**", "", tabla_reporte(reps[clave]), "",
                "**Clasificación por deporte:**", "", clasificacion(reps[clave]), ""]

    out += ["---", "", "## Comparación: qué mejoró y qué empeoró", "",
            "<!-- Escribe aquí con tus palabras, un punto por cada cambio (épocas, filtros, dropout, bloques, combinación). -->",
            "", "## Conclusión y qué se podría mejorar", "",
            "<!-- Cuál fue la mejor corrida y por qué; qué probarías después. -->", ""]

    Path("ComparacionCNN_deportes.md").write_text("\n".join(out), encoding="utf-8")
    print("Listo: ComparacionCNN_deportes.md")


if __name__ == "__main__":
    main()
