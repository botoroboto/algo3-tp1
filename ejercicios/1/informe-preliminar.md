# Ejercicio 1

## 2)
Si ordenamos la lista de actores por influencia **decreciente**, de modo que nuestro algoritmo empiece a armar cliques que contengan a los actores con más influencia, esto va a ***favorecer*** a la poda de optimalidad de modo que sea más efectiva, ya que es más probable que estos generen cliques de más influencia (y por ende, posiblemente podando más ramas del árbol). Por supuesto esta es una intuición (TODO - Tendríamos que demostrarlo? Referenciar a X parte del informe?). 

No obstante, depende en parte de la *distribución de influencias de los distintos actores*. Por ejemplo, si todos tuvieran niveles de influencia muy similares, ordenarlos **no** aportaría mucho a la eficiencia del algoritmo. El algoritmo va a performar mejor en los casos donde los actores que tengan mucha influencia estén *muy por encima* en cuanto a nivel de influencia de los actores cuyo nivel de influencia sea inferior.

En cambio, si nosotros ordenáramos la lista de actores por influencia creciente, esto sería muy perjudicial para nuestro algoritmo ya que tardaría más en llegar a explorar nodos que contengan actores con nivel de influencia grande, lo cual afectaría directamente a la poda de optimalidad. Nuevamente, al igual que para el ejemplo anterior, esto depende de la muestra (especificamente, de la variación entre las distintas influencias de los actores). 

