# TP: malloc

## Malloc

Nuestra función malloc hace un manejo simple de regiones. Se llama a la función `find_free_region` que, dependiendo de si estamos ejecutando una búsqueda first fit o best fit, busca la región de memoria correspondiente que vamos a utilizar. Si no devuelve null simplemente actualizamos las estadísticas y llamamos al split para evitar la fragmentación interna del bloque, pues podemos aprovechar la memoria interna restante del bloque que puede quedar, para generar uno nuevo. Si no se encuentra una región disponible, la función devuelve null y se procede a llamar a la función `grow_heap` con el tamaño correspondiente. La misma crea un bloque, con una única región de memoria, la inserta en la lista enlazada de bloques, y devuelve esta primera región del bloque como el que será utilizado por el usuario. El primer malloc siempre llama a `grow_heap` pues no hay bloques creados aún. Luego devolvemos `REGION2PTR`, la cual es una macro que toma la posición de memoria al header de la región y sumándole uno nos devuelve la posición de memoria que está justo después de que termina el header, es decir, donde empieza el espacio de memoria con permisos de escritura que devuelve el malloc.

### Split

Definimos así la función de `split`:

```C
void split(struct region* curr, size_t size) {
	if (curr->size - size > sizeof(struct region)) {
		struct region* new_region = (struct region*) ((char*) curr + sizeof(struct region) + size);
		new_region->size = curr->size - size - sizeof(struct region);
		new_region->next = curr->next;
		new_region->prev = curr;
		new_region->free = true;
		curr->size = size;
		curr->next = new_region;
	}
}
```

Donde tomamos la región libre con la memoria que vamos a devolver en el malloc. Si la región tiene más espacio que lo que ocupa el dato nuevo y lo que pesaría un nuevo header para la nueva región splitteada, realizamos el split. En caso de realizarlo, posicionamos el nuevo header al final de la memoria de la región anterior y actualizamos los punteros de la región anterior, la siguiente y los pesos correspondientes.

## Free

Nuestra función free primero valida que el puntero ingresado corresponda a alguna región de memoria pedida por nuestro malloc. Este chequeo se realiza en la función `is_invalid_or_already_free_region`, la cual, como su nombre indica, busca que una región ocupada cuya sección donde empiece la data tenga la misma dirección de memoria que el puntero ingresado al free. Esto evita hacer un double free pues verificamos que el header del puntero ingresado no esté marcado como libre. Tampoco realizaremos frees inválidos pues siempre verificamos que la dirección de memoria corresponda a alguna de las regiones pedidas por nuestro malloc. Finalmente realizamos un coalesce del bloque para evitar la fragmentación externa.

### Coalesce

Definimos así la función de `coalesce`:

```C
void coalesce(struct region* curr) {
	if ((curr->prev == NULL || !curr->prev->free) && (curr->next == NULL || !curr->next->free)) {
		return;
	} else if (curr->next != NULL && curr->next->free) {
		curr->size += curr->next->size + sizeof(struct region);
		curr->next = curr->next->next;
		if (curr->next != NULL) {
			curr->next->prev = curr;
		}
	} else if (curr->prev != NULL && curr->prev->free) {
		curr->prev->size += curr->size + sizeof(struct region);
		curr->prev->next = curr->next;
		if (curr->next != NULL) {
			curr->next->prev = curr->prev;
		}
	} else if (curr->prev != NULL && curr->prev->free && curr->next != NULL && curr->next->free) {
		curr->prev->size += curr->size + curr->next->size + 2 * sizeof(struct region);
		curr->prev->next = curr->next->next;
		if (curr->next->next != NULL) {
			curr->next->next->prev = curr->prev;
		}
	}
}
```

Donde tenemos cuatro casos posibles para juntar las regiones. El primer caso ocurre cuando las dos regiones vecinas están ocupadas, por lo que no hay nada que hacer. El segundo caso ocurre cuando la región siguiente está libre, por lo que podemos juntarlos; sumamos el tamaño de la región junto con el tamaño del header y ajustamos los punteros. El tercer caso ocurre cuando la región anterior está libre, realizamos el mismo proceso; agregamos la cantidad de bytes nuevos que adquirimos y reajustamos los punteros. El cuarto caso ocurre cuando ambas regiones vecinas están libres; mergeamos la región anterior con nuestra región actual y la próxima.

## Calloc

Nuestro calloc se implementa llamando a malloc. Si este nos devuelve un puntero válido, inicializamos la data de forma explicita en cero con `explicit_bzero` y devolvemos el puntero correspondiente a la data.

## Realloc

Finalmente está la cuarta función principal, el realloc. Si el puntero ingresado es nulo se devuelve el malloc del tamaño. Si el tamaño es cero y el puntero es válido, hacemos un free del mismo y devolvemos null. De lo contrario, si el puntero a la data ingresado es válido (realizando la misma verificación que hacemos en el free), tenemos 3 casos para analizar. Si el tamaño del nuevo bloque es el mismo que el actual, simplemente devolvemos el puntero que ingresó por parámetro pues no tiene sentido buscar otro bloque cuando el disponible cumple con lo pedido. Si el tamaño es que ingresa por parámetro es menor al tamaño del bloque actual (y nos queda memoria para insertar un header) realizamos un split del bloque para no tener que hacer una búsqueda de una región nueva. Partimos nuestra región en dos y a la nueva próxima región le hacemos un coalesce para que se junte con la vieja próxima región en caso de que estuviera libre. De lo contrario simplemente hacemos un malloc del tamaño pedido, setteamos la primera parte de la memoria con los datos viejos y la memoria nueva no queda inicializada, hacemos un free de la región vieja y devolvemos el puntero nuevo.

## End

Al final de la ejecución del proceso llamamos a la función `end`. La misma, además de imprimir las estadísticas si se ejecuta el programa en modo debug, imprime una representación en texto del estado del heap. Finalmente se llama a la función `unmap` que libera todos los bloques pedidos por nuestro malloc recorriendo la lisat enlazada y llamando a `munmap` para liberar la misma.

## Modo Debug

Algo que nos resultó muy útil a la hora de realizar este TP, fue la utilización de una función `print_heap` y `check_heap`. Las mismas aseguraron la integridad de los datos y facilitaron la visualización del estado del mismo. Las llamadas a estas funciones se ejecutan si y solo si se compila el programa definiendo la macro `HARD_DEBUG`.