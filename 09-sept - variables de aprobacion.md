# Análisis de variables para aprobar Cálculo Diferencial

## Objetivo

Predecir si un alumno aprobará o reprobará la materia de Cálculo Diferencial.

La variable que queremos predecir será:

- `1`: El alumno aprueba.
- `0`: El alumno reprueba.

## Variables principales

| Variable | Razón para utilizarla |
|---|---|
| Calificación de Álgebra | Cálculo necesita conocimientos de álgebra. |
| Examen diagnóstico | Muestra los conocimientos iniciales del alumno. |
| Porcentaje de asistencia | Faltar a clases puede afectar el aprendizaje. |
| Tareas entregadas | Indica práctica y responsabilidad. |
| Promedio de tareas | Refleja qué tanto comprende los ejercicios. |
| Calificación del primer parcial | Permite detectar alumnos en riesgo. |
| Horas de estudio | La práctica puede mejorar el desempeño. |
| Asistencia a asesorías | Indica si busca apoyo para resolver dudas. |

## Variable de resultado

Se considera que un alumno aprueba cuando obtiene una calificación final igual o mayor a 70:

- Si la calificación final es mayor o igual a 70, `aprobado = 1`.
- Si la calificación final es menor a 70, `aprobado = 0`.

## Posibles relaciones

- Una mayor asistencia puede aumentar la posibilidad de aprobar.
- Tener buenas bases de Álgebra facilita el aprendizaje de Cálculo.
- Entregar tareas demuestra práctica y constancia.
- Una buena calificación en el primer parcial aumenta la posibilidad de aprobar.
- Estudiar más horas puede ayudar, aunque también importa la calidad del estudio.

## Conclusión

Las variables más importantes serían el examen diagnóstico, la calificación de Álgebra, la asistencia, las tareas y el primer parcial.

Con estos datos se puede crear un modelo para estimar si un alumno probablemente aprobará o reprobará. Esta predicción puede utilizarse para identificar a los estudiantes que necesitan apoyo adicional.